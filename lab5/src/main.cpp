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

void printCurrentTime(DateTime currentTime) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Time");
  lcd.setCursor(6, 0);
  lcd.print(currentTime.hour(), DEC);
  lcd.print(":");
  lcd.print(currentTime.minute(), DEC);
  lcd.print(":");
  // TODO: if the current time is < 10, also display a zero
  lcd.print(currentTime.second(), DEC);
  printf("Current counter: %d\n", counter);
}

void vTaskUpdateTime(void *pvParameters) {
  DateTime initialTime = rtc.now();
  int driftCheck = 30;

  while (1) {
    // TODO: Implement semaphore handling, print time, check time drift
  }
}

void IRAM_ATTR onTimerISR() {
  // TODO: Implement the ISR to increment the counter
}

void setup() {
  rtc.begin();
  lcd.begin();
  lcd.backlight();

  xSemaphore = xSemaphoreCreateBinary();
  if (xSemaphore == NULL) {
    lcd.print("Semaphore Fail");
    while (1);
  }

  xTaskCreate(vTaskUpdateTime, "UpdateTimeTask", 2048, NULL, 1, NULL);

  hw_timer_t *timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimerISR, true);
  timerAlarmWrite(timer, 1000000, true);
  timerAlarmEnable(timer);
}

void loop() {
  // Empty loop
}