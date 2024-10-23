#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Configurable parameters for task periods
uint32_t periodTask1 = 500;  // Period in ms for Task 1 (using vTaskDelay)
uint32_t periodTask2 = 500;  // Period in ms for Task 2 (using vTaskDelayUntil)

void burnCPU(uint32_t ticks) {
  // TODO: Add timestamp recording
  //
  for (volatile uint32_t i = 0; i < ticks; i++) {
    asm("nop"); // dummy loop
  }

  // TODO: Add timestamp recording
}

// Task 1: Uses vTaskDelay (relative delay) and prints the timestamp
void vTask1(void *pvParameters) {
  static unsigned long prevTimestamp = 0;
  while (1) {
    // Get the current timestamp (absolute time in milliseconds)
    unsigned long currentTimestamp = millis();

    // Calculate the difference from the previous invocation
    unsigned long timeDifference = currentTimestamp - prevTimestamp;
    prevTimestamp = currentTimestamp;

    // Print the absolute timestamp and the time difference
    printf("Task 1 using vTaskDelay, Timestamp: %lu ms, Time since last: %lu ms\n", currentTimestamp, timeDifference);

    // TODO: Call burnCPU function with appropriate ticks to simulate CPU load

    // TODO: Use vTaskDelay for relative delay between task executions
  }
}

// Task 2: Uses vTaskDelayUntil (absolute delay) and prints the timestamp
void vTask2(void *pvParameters) {
  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t xFrequency = periodTask2 / portTICK_PERIOD_MS;  // Configurable period

  static unsigned long prevTimestamp = 0;
  while (1) {
    // Get the current timestamp (absolute time in milliseconds)
    unsigned long currentTimestamp = millis();

    // Calculate the difference from the previous invocation
    unsigned long timeDifference = currentTimestamp - prevTimestamp;
    prevTimestamp = currentTimestamp;

    // Print the absolute timestamp and the time difference
    printf("Task 2 using vTaskDelayUntil, Timestamp: %lu ms, Time since last: %lu ms\n", currentTimestamp, timeDifference);

    // TODO: Call burnCPU function with appropriate ticks to simulate CPU load

    // TODO: Use vTaskDelayUntil for absolute delay between task executions
  }
}

// Sporadic Task: Runs at random intervals to introduce interference
void vSporadicTask(void *pvParameters) {
  while (1) {
    // Introduce a random delay 
    unsigned long delayTime = random(300, 500);
    // Use vTaskDelay to introduce the random delay

    // Print when the sporadic task starts
    printf("Sporadic task starting, running on core %d\n", xPortGetCoreID());

    // TODO: Call brunCPU to introduce interference

    // Print when the sporadic task finishes
    printf("Sporadic task finished, ran for %lu ms\n", delayTime);
  }
}

void setup() {
  // for sporadic tasks, seed the random number generator with a fixed value
  randomSeed(50);

  // Create Task 1 (using vTaskDelay)
  xTaskCreate(vTask1, "Task1", 2048, NULL, 1, NULL);

  // Create Task 2 (using vTaskDelayUntil)
  xTaskCreate(vTask2, "Task2", 2048, NULL, 1, NULL);

  // TODO: Create the sporadic task that causes interference
  
}

void loop() {
  // Empty, as FreeRTOS is managing tasks
}
