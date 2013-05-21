#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <wiringPi.h>
#include <pthread.h>

static int inpin=8;
static int outpin=0;

static volatile bool running = true;

void *threadFunc(void *arg)
{
    for(;;)
    {
        if(running)
        {
            printf("LED on\n");
            digitalWrite(outpin, 1);
            delay(500);
            printf("LED off \n");
            digitalWrite(outpin,0);
            delay(500);
        }
        else
        {
            digitalWrite(outpin,0);
            delay(500);
        }
    }
    digitalWrite(outpin,0);
    return NULL;
}

void *checkRunning(void *arg)
{
    for(;;)
    {
        if(digitalRead(inpin)==0)
        {
            running = !running;
            delay(500);
        }
        delay(50);
    }
    return NULL;
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

    pinMode(outpin, OUTPUT);
    pinMode(inpin, INPUT);

    pthread_t blink, interrupt;

    printf("Creating blink thread\n");
    pthread_create(&blink,NULL,threadFunc,"blink");
    printf("Creating interrupt thread\n");
    pthread_create(&interrupt,NULL,checkRunning,"interrupt");

    for(;;)
    {
        printf("Running: %d\n", running);
        delay(500);
    }
    digitalWrite(outpin,0);
    return 0;
}
