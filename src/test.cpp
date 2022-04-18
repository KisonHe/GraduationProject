#include <Arduino.h>
TaskHandle_t highPriorityHandle;
TaskHandle_t lowPriorityHandle;

static const UBaseType_t lowPriority = 2;
static const UBaseType_t highPriority = 10;

static const char lowWords[10] = "I am L";
static const char highWords[10] = "I am H";

char lowPriorityPrint[10];
char highPriorityPrint[10];

void consumeSomeCPUTime(){
  // 1 in 10000 chance to exit trap
  // waste basically 30 - 60000 us
  while ((rand()%10000) != 2){
    NOP();
  }
}

/**
 * @brief 
 * 
 * @param isHigh true if caller is high priority
 */
void tskTestInit(bool isHigh){
  if (isHigh){
    memset(highPriorityPrint,0,sizeof(highPriorityPrint));
    memcpy(highPriorityPrint,highWords,sizeof(highWords));
  }else{
    memset(lowPriorityPrint,0,sizeof(lowPriorityPrint));
    memcpy(lowPriorityPrint,lowWords,sizeof(lowWords));
  }
}

/**
 * @brief 
 * 
 * @param isHigh true if caller is high priority
 */
void tskTestBegin(bool isHigh){
  if (isHigh){
    memcpy(lowPriorityPrint,highWords,sizeof(highWords));
  }else{
    memcpy(highPriorityPrint,lowWords,sizeof(lowWords));
  }
}

/**
 * @brief 
 * 
 * @param isHigh true if caller is high priority 
 * @return true shit happend
 * @return false nothing 
 */
bool tskTestResult(bool isHigh){
  if (isHigh){
    return strcmp(highPriorityPrint,highWords);
  }else{
    return strcmp(lowPriorityPrint,lowWords);
  }
}


void high_priority_task(void *n){
  //High Task Try to overwrite low task during runtime, should happen some time
  while (1)
  {
    static long enterTime;
    static long exitTime;
    //init own value
    memset(highPriorityPrint,0,sizeof(highPriorityPrint));
    memcpy(highPriorityPrint,highWords,sizeof(highWords));

    //try to do bad things to high task
    memcpy(lowPriorityPrint,highWords,sizeof(highWords));
    enterTime = micros();
    consumeSomeCPUTime();
    exitTime = micros();
    log_d("Spent %dus wasting cpu time",exitTime - enterTime);
    memcpy(lowPriorityPrint,highWords,sizeof(highWords));

    //check if own word is wrong, which is expected
    if (strcmp(highPriorityPrint,highWords)!=0){
      log_e("UnExpected Scheduling happend!!!");
    }
    vTaskDelay(1);

  }
  
}

void low_priority_task(void *n){
  //Low Task Try to overwrite low task during runtime, which should never success
  while (1)
  {
    static long enterTime;
    static long exitTime;
    //init own value
    memset(lowPriorityPrint,0,sizeof(lowPriorityPrint));
    memcpy(lowPriorityPrint,lowWords,sizeof(lowWords));

    //try to do bad things to high task
    memcpy(highPriorityPrint,lowWords,sizeof(lowWords));
    enterTime = micros();
    consumeSomeCPUTime();
    exitTime = micros();
    log_d("Spent %dus wasting cpu time",exitTime - enterTime);
    memcpy(highPriorityPrint,lowWords,sizeof(lowWords));

    //check if own word is wrong, which is expected
    if (strcmp(lowPriorityPrint,lowWords)!=0){
      log_i("Expected Scheduling happend");
    }
    vTaskDelay(1);

  }
  
}

// void setup(void) {
//   //clear at start
//   memset(lowPriorityPrint,0,sizeof(lowPriorityPrint));
//   memset(highPriorityPrint,0,sizeof(highPriorityPrint));

//   //init with their should be values
//   memcpy(lowPriorityPrint,lowWords,sizeof(lowWords));
//   memcpy(highPriorityPrint,highWords,sizeof(highWords));

//   Serial.begin(115200);
//   log_i("Begin Test");

//   //Pin to Same Core so never run at same time
//   xTaskCreatePinnedToCore(low_priority_task, "low_priority_task", 4096, NULL, lowPriority, &lowPriorityHandle,1);
//   xTaskCreatePinnedToCore(high_priority_task, "high_priority_task", 4096, NULL, highPriority, &highPriorityHandle,1);
// }
