#include <wiringPi.h>
#include <stdio.h>
 
#define  LEDPin    0
#define  PIN_TOUCH 6
#define  PIN_TRACK 5
#define  PIN_TEMP  4
#define  PIN_RED  7
#define  PIN_GREEN  8
#define  PIN_BLUE 9

void led(unsigned char r_val, unsigned char g_val, unsigned char b_val){
	softPwmWrite(PIN_RED, r_val);
	softPwmWrite(PIN_GREEN, g_val);
	softPwmWrite(PIN_BLUE, b_val);
}

int main(void)
{
  if(wiringPiSetup() == -1){
    //if the wiringPi initialization fails, print error message
    printf("setup wiringPi failed !");
    return 1;
  }
  
  pinMode(LEDPin, OUTPUT);
  softPwmCreate(PIN_RED, 0, 100);
  softPwmCreate(PIN_GREEN, 0, 100);
  softPwmCreate(PIN_BLUE, 0, 100);
  pinMode(PIN_TOUCH, INPUT);
  pinMode(PIN_TRACK, INPUT);
  pinMode(PIN_TEMP, INPUT);
  
  while(1){
    static prev_a = 0;
    int touch,track,temp,a,b,c,state;
    temp = digitalRead(PIN_TEMP);
    touch = digitalRead(PIN_TOUCH);
    track  = digitalRead(PIN_TRACK);
    FILE *f = fopen("current_status.txt","w");
    fprintf(f,"%d %d %d\n",touch,track,temp);
    fclose(f);
    FILE *g = fopen("output_WC.txt","r");
    fscanf(g, "%d", &a);
    fclose(g);
    //printf("%d",prev_a);
    if (a == 1)
    {
    if (prev_a == 0)
    {
    	FILE *h = fopen("output_ML.txt","r");
    	fscanf(h, "%d", &b);
        prev_a = 1;
        if (b == 1)
	{
	        
		{
		led(0x50, 0x00, 0x50);
                state =1;
		//delay(5000);
    		//FILE *j = fopen("input_ML.txt","a");
    		//fprintf(j,"%d %d %d %d\n",touch,track,temp,state);
                //fclose(j);
		}
	}
	else if(b == 2)
	{
		
		{
		led(0xc8, 0x00, 0x00);
                state = 2;
		
                //FILE *j = fopen("input_ML.txt","a");
    		//fprintf(j,"%d %d %d %d\n",touch,track,temp,state);
                //fclose(j);
		}
	}
	else if(b == 3)
	{
   		led(0xFF, 0xFF, 0x00);
                state = 3;
 
                //FILE *j = fopen("input_ML.txt","a");
    		//fprintf(j,"%d %d %d %d\n",touch,track,temp,state);
                //fclose(j);
	}
    }
    else
    {
	FILE *i = fopen("output_HT.txt","r");
    	fscanf(i, "%d", &c);
	fclose(i);
        if (c == 1)
        {
           if (state ==1)
           {
           led(0xFF, 0xFF, 0x00);
           state = 3;
           sleep(5);
 
           FILE *j = fopen("input_ML.txt","a");
           fprintf(j,"%d %d %d %d\n",touch,track,temp,state);
           fclose(j);    
           }
           if (state ==2)
           {
           led(0x50, 0x00, 0x50);
           state = 1;
           delay(5000);
           FILE *j = fopen("input_ML.txt","a");
           fprintf(j,"%d %d %d %d\n",touch,track,temp,state);
           fclose(j);     
           }
           if (state ==3)
           {
           led(0xc8, 0x00, 0x00);
           state = 2;
           delay(5000);
           FILE *j = fopen("input_ML.txt","a");
    	   fprintf(j,"%d %d %d %d\n",touch,track,temp,state);
           fclose(j);     
           }
        }
        else if (c == 2)
        {
           if (state ==1)
           {
           led(0xc8, 0x00, 0x00);
           state = 2;
           delay(5000);
           FILE *j = fopen("input_ML.txt","a");
    	   fprintf(j,"%d %d %d %d\n",touch,track,temp,state);
           fclose(j);     
           }
           if (state ==2)
           {
           led(0xFF, 0xFF, 0x00);
           state = 3;
           delay(5000);
           FILE *j = fopen("input_ML.txt","a");
    	   fprintf(j,"%d %d %d %d\n",touch,track,temp,state);
           fclose(j);     
           }
           if (state ==3)
           {
           led(0x50, 0x00, 0x50);
           state = 1;
           delay(5000);
           FILE *j = fopen("input_ML.txt","a");
    	   fprintf(j,"%d %d %d %d\n",touch,track,temp,state);
           fclose(j);     
           }
        }
    }
    
    
    digitalWrite(LEDPin, LOW);   //led off
    printf("led off...\n");
    delay(500);
    digitalWrite(LEDPin, HIGH);  //led on
    printf("...led on\n");
    delay(500);
    printf("%d",state);
  }
  }
  return 0;
}



