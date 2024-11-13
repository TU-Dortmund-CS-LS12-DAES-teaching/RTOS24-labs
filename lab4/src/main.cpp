#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

// Configurable parameters for task periods and upload simulation
uint32_t periodTask1 = 3000;  // Period in ms for Task 1
uint32_t periodTask2 = 3000;  // Period in ms for Task 2
uint32_t periodTask3 = 1000; // Period in ms for Task 3
uint32_t busyTime = 2000; // Busy loop time in ms for simulated upload
uint8_t queueSize = 10;        // Configurable queue size

// Shared parameter for queue send timeout
const TickType_t xTicksToWait = pdMS_TO_TICKS(100); // Timeout of 100 ms for xQueueSend

// Maximum ticks to run the loop
const TickType_t maxTicksToRun = pdMS_TO_TICKS(20000); // Run until 20,000 ms (20 seconds)


// Queue handle
QueueHandle_t sensorQueue;

// Function to simulate an upload by performing a busy loop
void simulateUpload(const char* sensorName, int sensorValue) {
  // TODO: use uxQueueMessagesWaiting to print the number of items in the queue before sending

  printf("Sending Sensor: %s, Value: %d\n", sensorName, sensorValue);

  // TODO: Simulate busy work with a configurable delay loop

  printf("Finished sending Sensor: %s, Value: %d\n", sensorName, sensorValue);
}

// Task 1: Temperature Sensor Task
void vTask1(void *pvParameters) {
  // TODO
  // Generate random pressure in range 0-30
  // TODO: Add 1000 as offset to distinguish from other sensor readings
}

// Task 2: Humidity Sensor Task
void vTask2(void *pvParameters) {
  // TODO
  // Generate random pressure in range 0-100
  // TODO: Add 2000 as offset to distinguish from other sensor readings
}

// Task 3: Pressure Sensor Task
void vTask3(void *pvParameters) {
  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t xFrequency = periodTask3 / portTICK_PERIOD_MS;

  // Variables to accumulate queue count and calculate average
  UBaseType_t queueCountSum = 0;
  int sampleCount = 0, lossCount = 0;

  // Run until maxTicksToRun has been reached
  while ((xTaskGetTickCount() - xLastWakeTime) < maxTicksToRun) {
    int pressure = random(0, 21);  // Generate random pressure in range 0-20
    int encodedValue = pressure + 3000;  // Add 3000 as offset to distinguish from other sensor readings
    
    // Get the current queue count and add it to the total
    queueCountSum += uxQueueMessagesWaiting(sensorQueue);
    sampleCount++;

    // TODO: Enqueue the encoded pressure value sensorQueue with xTicksToWait
    // If the queue is full and timeout, record with lossCount

    // TODO: Delay using vTaskDelayUntil.  Parameters: xLastWakeTime, xFrequency
  }

  if (sampleCount > 0) {
    printf("Average queue count observed by vTask3: %.2f\n", (float)queueCountSum / sampleCount);
    printf("Average loss count observed by vTask3: %.2f\n", (float)lossCount / sampleCount);
  }

}

// Task 4: Upload Task (Dedicated to handling simulated "uploads")
// This task continuously waits to receive data from the queue. Since FreeRTOS queues can only store single values (e.g., integers),
// we encode both the sensor type and value into a single integer. Each sensor type (temperature, humidity, and pressure) is assigned 
// a unique range by adding an offset to the raw sensor value. For example, temperature values are encoded by adding 1000, humidity by 
// adding 2000, and pressure by adding 3000. In this task, we decode the received value to identify the sensor type and retrieve the 
// original sensor reading by subtracting the corresponding offset. Once decoded, the data is used to simulate an upload process 
// (represented by a busy loop) with the function `simulateUpload`. If the received value does not fall within any recognized range, 
// it is considered unknown, and an error message is printed.
void vTaskUpload(void *pvParameters) {
  while (1) {
    int receivedValue;
    // TODO use xQueueReceivexQueueReceive to get a value from `sensorQueue` and store it in receivedValue
    const char* sensorName;
    int sensorValue;

    // Decode the sensor type and value
    if (receivedValue >= 1000 && receivedValue < 2000) {
      sensorName = "temperature";
      sensorValue = receivedValue - 1000;
    } else if (receivedValue >= 2000 && receivedValue < 3000) {
      sensorName = "humidity";
      sensorValue = receivedValue - 2000;
    } else if (receivedValue >= 3000 && receivedValue < 4000) {
      sensorName = "pressure";
      sensorValue = receivedValue - 3000;
    } else {
      printf("Unknown sensor value received: %d\n", receivedValue);
      continue;
    }

    // Simulate an upload with a busy loop
    simulateUpload(sensorName, sensorValue);
  }
}

void setup() {
  randomSeed(analogRead(0));  // Initialize random seed

  // Create a queue to hold sensor data with configurable size
  // TODO: xQueueCreate with a size of `queueSize`
  
  // Use xTaskCreatePinnedToCore to create three sender tasks and one receiver task
}

void loop() {
  // Empty, as FreeRTOS is managing tasks
}