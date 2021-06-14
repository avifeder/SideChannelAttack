
#include <stdlib.h>
int publicKey = 23;
int mod = 55;
int ledPin = PP_0;
int timeFromLastOperation = 0;
int timeOfOperationArray[64];
int numOfOperations = 0;
unsigned long privateKey;
bool finish = false;
void setup()
{
  pinMode(ledPin, INPUT);

  for (int i = 0; i < 64; i++)
  {
    timeOfOperationArray[i] = 0;
  }
  Serial.begin(9600);

}

void loop()
{
  if(digitalRead(ledPin) == HIGH)
  {
    timeOfOperationArray[numOfOperations++] = timeOfOperation();
    timeFromLastOperation = millis();
  }
  else if((millis() - timeFromLastOperation) > 1000 && timeOfOperationArray[0] != 0 && !finish)
  {
    finish = true;
    Serial.println("finished");
    timeAnalysisToKey();
    Serial.println(privateKey);   
  }    
}

int timeOfOperation()
{
  int t1 = millis();
  while(digitalRead(ledPin) == HIGH);
  return (millis() - t1);
}

void timeAnalysisToKey()
{  
  int zeroOperationTime = 9999;
  int OneOperationTime = 0;
  int middleTime;
  privateKey = 1;
  bool firstOperation = true;
  for (int i = 0; i < 64; i++)
  {
    if(timeOfOperationArray[i] != 0)
    {
      if(timeOfOperationArray[i] < zeroOperationTime)
      {
        zeroOperationTime = timeOfOperationArray[i];
      }
      if(timeOfOperationArray[i] > OneOperationTime)
      {
        OneOperationTime = timeOfOperationArray[i];
      }
    }
  }
  middleTime = (zeroOperationTime + OneOperationTime) / 2;
  if(abs(zeroOperationTime - OneOperationTime) < 20) //its all one's
  { 
    middleTime = 0;
  }
  for (int i = 63; i >= 0; i--)
  {
    if(timeOfOperationArray[i] != 0)
    {
      if(firstOperation)
      {
        firstOperation = false;
      }
      else
      {
        privateKey *= 2;
        if(timeOfOperationArray[i] >= middleTime)
          privateKey += 1;   
      }
    }
  }
}
