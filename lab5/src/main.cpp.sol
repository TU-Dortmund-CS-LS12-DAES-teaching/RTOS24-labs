#include <LiquidCrystal_I2C.h>
#include "RTClib.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

// RTC and LCD setup
LiquidCrystal_I2C lcd(0x27, 16, 2);
RTC_DS1307 rtc;

// Global counter and semaphore
volatile int counter = 0;
SemaphoreHandle_t xSemaphore = NULL;

// Task function for printing and drift checking
void vTaskPrintTime(void *pvParameters) {
  // Record the initial time from RTC
  DateTime initialTime = rtc.now();
  unsigned long lastDriftCheck = millis();
  int driftCheck = 30;

  while (1) {
    // Wait for the semaphore from the ISR
    if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE) {
      // Calculate the time using RTC + counter_value
      DateTime calculatedTime = initialTime + TimeSpan(counter);
      
      // Print the calculated time
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Time");
      lcd.setCursor(6, 0);
      lcd.print(calculatedTime.hour(), DEC);
      lcd.print(":");
      lcd.print(calculatedTime.minute(), DEC);
      lcd.print(":");
      lcd.print(calculatedTime.second(), DEC);
      printf("Current counter: %d\n", counter);
      if (driftCheck-- == 0) {
        DateTime currentRTC = rtc.now();
        int drift = (calculatedTime - currentRTC).totalseconds();
        driftCheck = 30;
        printf("Drift: %d\n", drift); 
      }
    }
  }
}

// ISR for the timer
void IRAM_ATTR onTimerISR() {
  taskENTER_CRITICAL_FROM_ISR();
  counter++;
  taskEXIT_CRITICAL_FROM_ISR();

  // Signal the semaphore
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  xSemaphoreGiveFromISR(xSemaphore, &xHigherPriorityTaskWoken);
  portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void setup() {
  // Initialize RTC and LCD
  rtc.begin();
  lcd.begin();
  lcd.backlight();

  // Create the binary semaphore
  xSemaphore = xSemaphoreCreateBinary();
  if (xSemaphore == NULL) {
    lcd.print("Semaphore Fail");
    while (1); // Stop if semaphore creation fails
  }

  // Create the print task
  xTaskCreate(vTaskPrintTime, "PrintTimeTask", 2048, NULL, 1, NULL);

  // Configure the timer interrupt
  hw_timer_t *timer = timerBegin(0, 80, true); // Timer 0, prescaler = 80 (1 MHz clock)
  timerAttachInterrupt(timer, &onTimerISR, true); // Attach ISR
  timerAlarmWrite(timer, 1000000, true); // 1-second interval
  timerAlarmEnable(timer); // Enable the timer
}

void loop() {
  // Empty loop since FreeRTOS tasks handle the functionality
}