#include <Arduino.h>
// task sender
void task1(void *param);
// task reciever
void task2(void *param);
TaskHandle_t TaskHandle_1;
TaskHandle_t TaskHandle_2;
// init Queue
QueueHandle_t Queue;
QueueHandle_t Queue1;
struct Messenge
{
  int id;
  char data[20];
} xMess;
char TxBuff[30];
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  // 1 create task

  Queue1 = xQueueCreate(7, sizeof(TxBuff));
  xTaskCreatePinnedToCore(task1, "task1", 1024, NULL, 1, &TaskHandle_1, 1);
  xTaskCreatePinnedToCore(task2, "task2", 1024, NULL, 1, &TaskHandle_2, 1);
}
void loop()
{
  if (Serial.available() > 0)
  {
    String buff = Serial.readStringUntil('\n');
    Serial.print("\nReceived String: ");
    Serial.println(buff);
    sprintf(TxBuff, buff.c_str());
    xQueueSend(Queue, (void *)TxBuff, (TickType_t)0);
  }
}

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
  ////////////////////////////// Send custom data
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
  char RxBuff[30];
  while (true)
  {
    if (Queue != 0)
    {
      if (xQueueReceive(Queue, (void *)&dataRcv, (TickType_t)0))
      {
        Serial.print("data 1 receive: ");
        Serial.print(dataRcv);
        Serial.print("\n");
      }
    }
    if (Queue1 != 0)
    {
      if (xQueueReceive(Queue, (void *)RxBuff, (TickType_t)0))
      {
        Serial.print("data 2 receive: ");
        Serial.print(RxBuff);
        Serial.print("\n");
      }
    }
  }
}