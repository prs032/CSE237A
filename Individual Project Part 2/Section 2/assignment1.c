#include "assignment1.h"
#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <stdint.h>

void init_shared_variable(SharedVariable* sv) {
	// You can initialize the shared variable if needed.
	sv->bProgramExit = 0;
	sv->state = REST;
    sv->temp = LOW;
    sv->touch = LOW;
	sv->track = LOW;
}

void init_sensors(SharedVariable* sv) {
	
	pinMode(PIN_YELLOW, OUTPUT);
	pinMode(PIN_RED, OUTPUT);
	pinMode(PIN_GREEN, OUTPUT);
	pinMode(PIN_BLUE, OUTPUT);
	pinMode(PIN_ALED, OUTPUT);
	pinMode(PIN_BUZZER, OUTPUT);
	
	pinMode(PIN_BUTTON, INPUT);
	pinMode(PIN_TEMP, INPUT);
	pinMode(PIN_TRACK, INPUT);
	pinMode(PIN_TOUCH, INPUT);
	
	ledInit();
}

void ledInit(void)
{
	softPwmCreate(PIN_RED, 0, 0xff);
	softPwmCreate(PIN_GREEN,0, 0xff);
	softPwmCreate(PIN_BLUE, 0, 0xff);
}

void body_button(SharedVariable* sv) {
	int button = READ(PIN_BUTTON);
    if (button == 0){
        if (sv->state == DRIVING){
            init_shared_variable(sv);
        }else{
            sv->state = DRIVING;
        }
	}
}

void body_twocolor(SharedVariable* sv) {
	
	if(sv->state == DRIVING){
        TURN_ON(PIN_YELLOW);
    }else{
        TURN_OFF(PIN_YELLOW);
	}
}

void body_temp(SharedVariable* sv) {
	
	int temp = READ(PIN_TEMP);
    printf("TEMP: %d\n",temp);
    if (sv->state == DRIVING){
        if(temp && sv->touch == LOW && sv->track == LOW){
            sv->temp = HIGH;
        }else{
            sv->temp = LOW;
        }
	}
}

void body_track(SharedVariable* sv) {
	
	int track = READ(PIN_TRACK);
    printf("TRACK: %d\n",track);
    if (sv->state == DRIVING){
        if(track == 0 && sv->touch == LOW && sv->temp == LOW){
            sv->track = HIGH;
        }
	}
}

void body_touch(SharedVariable* sv) {
	
	int touch = READ(PIN_TOUCH);
    printf("TOUCH: %d\n",touch);
    if (sv->state == DRIVING){
        if(touch == 0 && sv->track == LOW && sv->temp == LOW){
            sv->touch = HIGH;
        }else{
            sv->touch = LOW;
        }
	}
}

void body_rgbcolor(SharedVariable* sv) {
	
	if(sv->state == REST){
        LED_BLUE();
        return;
    }
    if(sv->track || sv->touch){
        if(sv->track)
            LED_RED();
        if(sv->touch)
            LED_PURPLE();
        return;
    }    
	LED_YELLOW();
}

void body_aled(SharedVariable* sv) {
	
	if(sv->temp == HIGH){
        TURN_ON(PIN_ALED);
    }else{
        TURN_OFF(PIN_ALED);
	}
}

void body_buzzer(SharedVariable* sv) {
	
	if(sv->touch == HIGH){
        TURN_ON(PIN_BUZZER);        
    }else{
        TURN_OFF(PIN_BUZZER);
	}
}
