#include <wiringPi.h>
#include <stdio.h>

#define PIN_BUTTON 0
#define PIN_YELLOW 1

int main(void)
{
if(wiringPiSetup() == -1){ //when initialize wiring failed,print messageto screen
printf("setup wiringPi failed !");
return 1;
}

pinMode(PIN_YELLOW, OUTPUT);
pinMode(PIN_BUTTON, INPUT);

pullUpDnControl(PIN_BUTTON, PUD_UP); //pull up to 3.3V,make GPIO1 a stable level
while(1){
digitalWrite(PIN_YELLOW, HIGH);
if(digitalRead(PIN_BUTTON) == 0){ //indicate that button has pressed down
digitalWrite(PIN_YELLOW, LOW); //led on
}
}

return 0;
}