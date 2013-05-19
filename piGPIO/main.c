#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>



int main()
{

    int pin=0;
    printf("Raspberry Pi wiringPi blink test\n");

    if(wiringPiSetup() == -1)
        exit(1);

    pinMode(pin, OUTPUT);

    for (;;){
        printf("LED on\n");
        digitalWrite(pin, 1);
        delay(500);
        printf("LED off \n");
        digitalWrite(pin,0);
        delay(500);
    }

    return 0;
}
