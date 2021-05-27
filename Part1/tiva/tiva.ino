
int button5 = PE_4;
int button4 = PC_4;
int button3 = PC_5;
int button2 = PC_6;
int button1 = PE_5;
int redLed = PP_0;
int greenLed = PP_1;
int buttons[] =  {0, button1, button2, button3, button4, button5};
int correctPass[] = { -1, -1, -1, -1, -1};
int passToCheck[] = {0, 0, 0, 0, 0};

int numOfCorrectButtons = 0;

void setup() {
  pinMode(button1, OUTPUT);
  pinMode(button2, OUTPUT);
  pinMode(button3, OUTPUT);
  pinMode(button4, OUTPUT);
  pinMode(button5, OUTPUT);
  pinMode(redLed, INPUT);
  pinMode(greenLed, INPUT);
  Serial.begin(9600);
  delay(5000);
 

}

void generatePass(int buttonToCheck)
{  
  for (int i  = 0 ; i < 5; i++)
  {
    if (correctPass[i] == -1)
      passToCheck[i] = buttons[buttonToCheck];
    else
      passToCheck[i] = correctPass[i];
  } 


}

void loop() {
  if (digitalRead(greenLed) != HIGH)
  {
    int maxTime = 0;
    int button = 0;
    for (int i = 1; i <= 5; i++)
    {
      generatePass(i);    
      int t1 = millis();
      enterPass(passToCheck);      
      while (digitalRead(redLed) != HIGH && digitalRead(greenLed) != HIGH);
      int t2 = millis();
      int delta = (t2 - t1);
      Serial.print(i);
      Serial.print(" : ");
      Serial.print(delta);
      Serial.print(", ");
  
      if (maxTime < delta)
      {
        maxTime = delta;
        button = i;        
      }
      if(digitalRead(greenLed) == HIGH)
      {
        printPass();
        break;
      }
    }
    if(digitalRead(greenLed) != HIGH)
    {
      Serial.println();
      Serial.print(button);
      Serial.println();
      if(numOfCorrectButtons < 5)
      {
        correctPass[numOfCorrectButtons] = buttons[button];
        numOfCorrectButtons++;
      }
    }
  }
}

void printPass()
{
  Serial.println();
  Serial.println("Password found!!");
  Serial.println("The password is: ");
  for(int i = 0; i < 5; i++)
  {
    Serial.print(numButton(passToCheck[i]));
    if(i != 4)
      Serial.print(" ,");
  }
}
int numButton(int x)
{
  for(int i = 0; i <= 5; i++)
  {
    if(buttons[i] == x)
      return i;
  }
  return -1;
}
void enterPass(int* pass)
{
  delay(100);
  for (int i = 0 ; i < 5 ; i++)
  {
    pressButton(*(pass + i));
  }
}


void pressButton(int button)
{
  digitalWrite(button, HIGH);
  delay(10);
  digitalWrite(button, LOW);
  delay(50);
}
