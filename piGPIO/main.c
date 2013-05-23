#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <wiringPi.h>
#include <pthread.h>
#include <signal.h>

static int inpin=8;
static int outpin=0;

static volatile bool blinking = true, running=true;

void my_handler (int sig)
{
  printf("Exiting");
  running=false;
}

void *threadFunc(void *arg)
{
    for(;;)
    {
        if(blinking)
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
            blinking = !blinking;
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
    signal (SIGQUIT, my_handler);
    signal (SIGINT, my_handler);

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

    while(running)
    {
        printf("Running: %d\n", blinking);
        delay(500);
    }
    digitalWrite(outpin,0);
    return 0;
}
