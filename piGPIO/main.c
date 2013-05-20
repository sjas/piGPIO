#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <wiringPi.h>

bool checkRunning(int inpin)
{
    if(digitalRead(inpin)==0)
        return false;
    else
        return true;
}

void blink(int outpin)
{
    printf("LED on\n");
    digitalWrite(outpin, 1);
    delay(500);
    printf("LED off \n");
    digitalWrite(outpin,0);
    delay(500);
}

int main()
{

    int pin=0;
    int inpin =8;
    printf("Raspberry Pi wiringPi blink test\n");

    if(wiringPiSetup() == -1)
        exit(1);

    pinMode(pin, OUTPUT);
    pinMode(inpin, INPUT);

    for(;;)
    {
        if(checkRunning(inpin))
            blink(pin);
        else
            delay(500);
    }

    return 0;
}
