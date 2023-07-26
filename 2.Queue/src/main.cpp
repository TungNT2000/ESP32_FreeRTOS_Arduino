#include <Arduino.h>
// task sender
void task1(void *param);
// task reciever
void task2(void *param);
TaskHandle_t TaskHandle_1;
TaskHandle_t TaskHandle_2;
// init Queue
QueueHandle_t Queue;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  // 1 create task
  xTaskCreate(task1, "task1", 1024 * 2, NULL, 1, &TaskHandle_1);
  xTaskCreate(task2, "task2", 1024 * 2, NULL, 1, &TaskHandle_2);
}
void loop() {}

void task1(void *p)
{
  Serial.println("on task 1");
  int data = 1;
  // create Queue
  Queue = xQueueCreate(7, sizeof(int));
  // send data to Queue
  xQueueSend(Queue, (void *)&data, (TickType_t)0);
  data = 2;
  xQueueSend(Queue, (void *)&data, (TickType_t)0);
  Serial.print("data waiting to be read: ");
  // get queue state
  Serial.print(uxQueueMessagesWaiting(Queue));
  Serial.print("\n");
  Serial.print("available spaces: ");
  Serial.print(uxQueueSpacesAvailable(Queue));
  Serial.print("\n");
  while (true)
  {
  }
}
void task2(void *p)
{
  vTaskDelay(100 / portTICK_PERIOD_MS);
  Serial.println("on task 2");
  int dataRcv;
  while (true)
  {
    if (Queue != 0)
    {
      if (xQueueReceive(Queue, (void *)&dataRcv, (TickType_t)5))
      {
        Serial.print("data receive: ");
        Serial.print(dataRcv);
        Serial.print("\n");
      }
    }
  }
}