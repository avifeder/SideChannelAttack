
#include <math.h>


uint64_t result[] = {0,0};
uint64_t n = 3839256683;
uint64_t e = 548447537;
uint64_t d = 3839132753;
uint64_t m;
uint64_t c;
uint64_t r;
uint64_t k;


String longToBytes(uint64_t x)
{
    //This function convert long to string of bits
    String bits = "";
    if (x == 0)
        return "0";
    while (x > 0)
    {
        if (x % 2 == 0)
            bits += '0';
        else
            bits += '1';
        x = x / 2;
    }
    return bits;
}

uint64_t ModInverse(uint64_t a, uint64_t n)
{
    //This function calc the inverse modulo of number a
    uint64_t n_orginal = n;
    int64_t t = 0;
    int64_t new_t = 1;
    uint64_t quotient;
    int64_t t1;
    uint64_t n1;

    while (a != 0) {
        quotient = n / a;
        t1 = t;
        t = new_t;
        new_t = t1 - quotient * new_t;
        n1 = n;
        n = a;
        a = n1 - quotient * a;
    }
    if (t < 0)
        t = t + n_orginal;
    return t;
}

uint64_t MongomeryProduct(uint64_t a, uint64_t b, uint64_t n, uint64_t nprime, uint64_t r)
{
    //This function calc the montgomery product of numbers a and b
    uint64_t t = a * b;
    uint64_t t1 = t % r;
    uint64_t m = t1 * nprime % r;
 

    uint64_t t_div_r = t / r;
    uint64_t mn_div_r = (m * n) / r;

    uint64_t t_apr_r = t % r;
    uint64_t mn_apr_r = (m * n) % r;


    uint64_t u = t_div_r + mn_div_r  +1;

    if (u < n)
        return u;
    delay(100);
    return u - n;
}

uint64_t modexp(uint64_t a, String exp, uint64_t n, uint64_t r, uint64_t k) {
    //This function encrypt/decrypt message m by rsa protocol
    uint64_t a_ = (a * r) % n;
    uint64_t x_ = (1 * r) % n;

    for (int i = exp.length() - 1; i >= 0; i--)
    {
        x_ = MongomeryProduct(x_, x_, n, k, r);
        if (exp[i] == '1')
            x_ = MongomeryProduct(a_, x_, n, k, r);
            
    }
    
    return MongomeryProduct(1, x_, n, k, r);
}

void nPrime(uint64_t n)
{
    //This function calc the r and the k for the montgomery product
    uint64_t k = (log(n)/log(2)) + 1;
    uint64_t r = pow(2, k);
    uint64_t rInverse = ModInverse(r, n);
    uint64_t nPrime = (r * rInverse - 1) / n;
    result[0] = nPrime;
    result[1] = r;
}

uint64_t stringToInt(String number)
{
  int n = number.length();
  uint64_t returnNum = 0;
  uint64_t  temp = 1;
  for(int i = n-1 ; i >=0 ; i--)
  {
    if(number[i] == '\r' || number[i] == '\n')
      continue;
    if(number[i] == '1')
      returnNum += temp;
    temp *= 2;
  }

  return returnNum;
}


String reverseStr(String str)
{
    int n = str.length();
    String returnStr = "";
    for (int i = n-1; i >= 0 ; i--)
        returnStr += str[i];
    return returnStr;
}

void setup() {
  Serial.begin(9600);
  nPrime(n);
  k = result[0];
  r = result[1];
}

void loop() {
    while(Serial.available() <= 0);
    String mess = Serial.readString();
    
    m = stringToInt(mess);
    c = modexp(m, longToBytes(d), n, r, k);
   
    String temp = reverseStr(longToBytes(c));
    Serial.print(temp);

}
