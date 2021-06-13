// ConsoleApplication2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <thread>        
#include <chrono>   
#include <iostream>
#include <string>
#include <vector>
#include <math.h>   

using namespace std;
#include <fstream>
typedef unsigned long long myulong;


string longToBytes(myulong x)
{
    //This function convert long to string of bits
    string bits = "";
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



myulong ModInverse(myulong a, myulong n)
{
    //This function calc the inverse modulo of number a
    myulong n_orginal = n;
    long long t = 0;
    long long new_t = 1;
    myulong quotient;
    long long t1;
    myulong n1;

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

myulong MongomeryProduct(myulong a, myulong b, myulong n, myulong nprime, myulong r)
{
    //This function calc the montgomery product of numbers a and b
    myulong t = a * b;
    myulong t1 = t % r;
    myulong m = t1 * nprime % r;
    long double t_div_r = (long double)t / r;
    long double mn_div_r = (long double)(m * n) / r;
    myulong u = t_div_r + mn_div_r;
    if (u < n)
        return u;
    this_thread::sleep_for(chrono::milliseconds(100));
    return u - n;
}

myulong modexp(myulong a, string exp, myulong n, myulong r, myulong k) {
    //This function encrypt/decrypt message m by rsa protocol
    myulong a_ = (a * r) % n;
    myulong x_ = (1 * r) % n;

    for (int i = exp.size() - 1; i >= 0; i--)
    {
        x_ = MongomeryProduct(x_, x_, n, k, r);
        if (exp[i] == '1')
            x_ = MongomeryProduct(a_, x_, n, k, r);
    }
    return MongomeryProduct(1, x_, n, k, r);
}
vector<myulong> nPrime(myulong n)
{
    //This function calc the r and the k for the montgomery product
    myulong k = log2(n) + 1;
    myulong r = pow(2, k);
    myulong rInverse = ModInverse(r, n);
    myulong nPrime = (r * rInverse - 1) / n;
    vector<myulong> result;
    result.push_back(nPrime);
    result.push_back(r);
    return result;
}

int main()
{
    myulong n = 3733280207;
    myulong  e = 1599924857;
    myulong d = 3733157993;
    myulong m;
    myulong c;
    myulong t;
    srand(time(0));
    vector<myulong> result = nPrime(n);
    myulong r = result[1];
    myulong k = result[0];

    ofstream myfile;
    myfile.open("data.txt");
    myfile << "N,E\n3733280207,1599924857\nmessage,signature,duration\n";
    for (int i = 0; i < 10000; i++)
    {
        m = (((long long)rand() << 32) | rand()) % n + 1;
        chrono::steady_clock::time_point begin = chrono::steady_clock::now();
        c = modexp(m, longToBytes(d), n, r, k);
        chrono::steady_clock::time_point end = chrono::steady_clock::now();
        t = chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
        myfile << m << "," << c << "," << t << endl;
    }

    myfile.close();
    system("pause");
}
