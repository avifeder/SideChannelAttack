int ledPin = 8;
unsigned long privateKey = 2147483648;
int publicKey = 7;
int mod = 55;
int m = 32;
bool finish = false;
void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  delay(2000);
}

void loop() {
  if(!finish)
  {
    repeatedSquaring(m, privateKey, mod);
    finish = true;
  }
}

int repeatedSquaring(int m, unsigned long e, int n)
{
  int res = 1;
  while (e > 0) {
    digitalWrite(ledPin, HIGH);
    delay(150);
    if (e % 2 != 0) {
      delay(200);
      res = (m*res) % n;
    }
    m = (m*m) % n;
    e /= 2;
    digitalWrite(ledPin, LOW);
    delay(150);
  }
  return res;
}
