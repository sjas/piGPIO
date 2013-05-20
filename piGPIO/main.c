#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <wiringPi.h>
#include <pthread.h>

void *threadFunc(void *arg)
{
    int outpin=0;
    for(;;)
    {
        printf("LED on\n");
        digitalWrite(outpin, 1);
        delay(500);
        printf("LED off \n");
        digitalWrite(outpin,0);
        delay(500);
    }
    return NULL;
}

bool checkRunning(int inpin)
{
    if(digitalRead(inpin)==0)
        return false;
    else
        return true;
}



void init(void)
{
    if(geteuid() != 0)
    {
        fprintf(stderr, "Need to be root to run!\n");
        exit(0);
    }
    if(wiringPiSetup() == -1)
        exit(1);
}


int main()
{
    init();

    printf("Setup...");
    fflush(stdout);

    int pin=0;
    int inpin =8;

    pinMode(pin, OUTPUT);
    pinMode(inpin, INPUT);

    printf("Creating blink thread\n");

    pthread_t pth;
    pthread_create(&pth,NULL,threadFunc,"blink");

    for(;;)
    {
        if(checkRunning(inpin)==false)
        {
            pthread_cancel(pth);
            break;
        }
    }
    //pthread_join(pth, NULL);

    return 0;
}
