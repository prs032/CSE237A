#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>

#define uchar unsigned char

#define PIN_RED 7
#define PIN_GREEN 8
#define PIN_BLUE 9

void ledInit(void)
{
softPwmCreate(PIN_RED, 0, 100);
softPwmCreate(PIN_GREEN,0, 100);
softPwmCreate(PIN_BLUE, 0, 100);
}

void ledColorSet(uchar r_val, uchar g_val, uchar b_val)
{
softPwmWrite(PIN_RED, r_val);
softPwmWrite(PIN_GREEN, g_val);
softPwmWrite(PIN_BLUE, b_val);
}

int main(void)
{
int i;

if(wiringPiSetup() == -1){ //when initialize wiring failed,print messageto screen
printf("setup wiringPi failed !");
return 1;
}
//printf("linker LedPin : GPIO %d(wiringPi pin)\n",LedPin); //when initialize wiring successfully,print message to screen

ledInit();

while(1){
ledColorSet(0xff,0x00,0x00); //red
delay(500);
ledColorSet(0x00,0xff,0x00); //green
delay(500);
ledColorSet(0x00,0x00,0xff); //blue
delay(500);

ledColorSet(0xff,0xff,0x00); //yellow
delay(500);
ledColorSet(0xff,0x00,0xff); //pick
delay(500);
ledColorSet(0xc0,0xff,0x3e);
delay(500);

ledColorSet(0x94,0x00,0xd3);
delay(500);
ledColorSet(0x76,0xee,0x00);
delay(500);
ledColorSet(0x00,0xc5,0xcd);
delay(500);

}

return 0;
}