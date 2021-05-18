int redLedPin = 8;    // choose the pin for the LED
int greenLedPin = 9;    

int input5Pin = 7;       // define push button input pins
int input4Pin = 6;
int input3Pin = 5;
int input2Pin = 4;
int input1Pin = 3;
int code[] = {1,2,3,4,5};
int pressHistory[] = {0,0,0,0,0};
int counter = 0;


void setup()
{
  pinMode(redLedPin, OUTPUT);   // declare LED as outputs
  pinMode(greenLedPin, OUTPUT);   

  pinMode(input5Pin, INPUT); // declare push button inputs
  pinMode(input4Pin, INPUT);
  pinMode(input3Pin, INPUT);
  pinMode(input2Pin, INPUT);
  pinMode(input1Pin, INPUT);
}

void loop()
{
  if(counter == 5)
    checkPass();
  for( int i = 3; i<= 7; i++)
    checkPush(i);
}

void checkPush(int pinNumber)
{
  int pushed = digitalRead(pinNumber);  // read input value
  if (pushed == HIGH) // check if the input is HIGH (button released)
  {
      pressHistory[counter] = pinNumber-2;
      counter++;
      delay(50);
      digitalWrite(redLedPin, LOW);
      digitalWrite(greenLedPin, LOW);
  }
  
}

void checkPass()
{
   for( int i = 0; i<5; i++)
   {
    delay(random(0, 100));
    if(pressHistory[i]!=code[i])
    {
       counter = 0;
       digitalWrite(redLedPin, HIGH);
       return;
    }
    delay(30);
   }
   digitalWrite(greenLedPin, HIGH);
   counter = 0;
   
}
