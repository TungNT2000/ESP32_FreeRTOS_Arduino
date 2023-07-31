#include <Arduino.h>
SemaphoreHandle_t xMutex;
void task1(void *parameter);
void task2(void *parameter);

void setup()
{
  Serial.begin(115200);
  /* create Mutex */
  xMutex = xSemaphoreCreateMutex();
  if (xMutex != NULL)
  {
    xTaskCreate(
        task1,   /* Task function. */
        "task1", /* name of task. */
        1000,    /* Stack size of task */
        NULL,    /* parameter of the task */
        1,       /* priority of the task */
        NULL);   /* Task handle to keep track of created task */
    delay(500);
    /* let lowPriorityTask run first then create highPriorityTask */
    xTaskCreate(
        task2,   /* Task function. */
        "task2", /* name of task. */
        1000,    /* Stack size of task */
        NULL,    /* parameter of the task */
        4,       /* priority of the task */
        NULL);   /* Task handle to keep track of created task */
  }
  // vTaskStartScheduler();
}

void loop()
{
  vTaskSuspend(NULL);
}
void task1(void *parameter)
{
  // Serial.println((char *)parameter);
  for (;;)
  {
    Serial.println("task1");
    xSemaphoreTake(xMutex, portMAX_DELAY);
    /* even low priority task delay high priority
    still in Block state */
    Serial.println("task1 is running");
    xSemaphoreGive(xMutex);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
  vTaskDelete(NULL);
}

void task2(void *parameter)
{
  // Serial.println((char *)parameter);
  for (;;)
  {
    Serial.println("task2");
    /* highPriorityTask wait until lowPriorityTask release key */
    xSemaphoreTake(xMutex, portMAX_DELAY);
    Serial.println("task2 is running");
    xSemaphoreGive(xMutex);
    /* delay so that lowPriorityTask has chance to run */
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
  vTaskDelete(NULL);
}