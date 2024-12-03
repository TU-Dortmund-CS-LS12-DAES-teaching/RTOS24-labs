#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include <stdio.h>
#include <string.h>

// Communication hub: 5 slots, each 10-char array
#define NUM_SLOTS 5
#define SLOT_SIZE 10

char sendingStation[NUM_SLOTS][SLOT_SIZE];
bool slotUsed[NUM_SLOTS] = {false}; // Track usage of slots

SemaphoreHandle_t stationMutex;
SemaphoreHandle_t printMutex;

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Function to print to LCD
void writeToLCD(const char *msg) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(msg);
}

// Function to allocate a slot
int allocateSlot() {
    // TODO: Use stationMutex to protect access to slotUsed: xSemaphoreTake(stationMutex, portMAX_DELAY);
    // Find the first unused slot and mark it as used
    // Return the allocated slot ID or -1 if no slot is available
    // Release the mutex
}

// Function to release a slot
void releaseSlot(int slotID) {
    // TODO: Use stationMutex to protect access to slotUsed
    // Mark the slot as free
    // Clear the content of the slot in sendingStation mit memset
}

// Function to print the content of a slot
void sendToEarth(int slotID) {
    // TODO: Use printMutex to protect access to the LCD and console
    // Print the content of the slot mit writeToLCD
    // Simulate a 1 second delay to mimic transmission time
    vTaskDelay(pdMS_TO_TICKS(1000));

    // Release the mutex
}

// Task function for Rovers
void roverTask(void *pvParameters) {
    char *taskType = (char *)pvParameters;
    int counter = 0;

    while (1) {
        // Random delay for each task
        int waitTime;
        if (strcmp(taskType, "Temp") == 0) {
            waitTime = (rand() % 300) + 200; // 0.2-0.5 seconds
        } else if (strcmp(taskType, "Soil") == 0) {
            waitTime = (rand() % 400) + 300; // 0.3-0.7 seconds
        } else if (strcmp(taskType, "Image") == 0) {
            waitTime = (rand() % 500) + 400; // 0.4-0.9 seconds
        }
        vTaskDelay(pdMS_TO_TICKS(waitTime));

        // Try to allocate a slot
        int slotID = allocateSlot();
        if (slotID >= 0) {
            // Populate the slot with rover task type and counter
            snprintf(sendingStation[slotID], SLOT_SIZE, "%s:%d", taskType, counter++);
            printf("%s Rover allocated slot %d\n", taskType, slotID);

            // Print the content of the slot
            sendToEarth(slotID);

            // Release the slot after printing
            releaseSlot(slotID);
        } else {
            printf("%s Rover: No free slot available\n", taskType);
        }
    }
}

void setup() {
    lcd.begin();
    lcd.backlight();

    stationMutex = xSemaphoreCreateMutex();
    printMutex = xSemaphoreCreateMutex();

    if (stationMutex == NULL || printMutex == NULL) {
        printf("Mutex initialization failed!");
        while (1);
    }

    // Create rover tasks
    xTaskCreate(roverTask, "Temperature Rover", 2048, (void *)"Temp", 1, NULL);
    xTaskCreate(roverTask, "Soil Rover", 2048, (void *)"Soil", 1, NULL);
    xTaskCreate(roverTask, "Image Rover", 2048, (void *)"Image", 1, NULL);
}

void loop() {
    // Empty loop as FreeRTOS handles task execution
}