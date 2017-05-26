#include <wiringPi.h>
#include <softPwm.h>

#ifndef _ASSIGNMENT_BODY_
#define _ASSIGNMENT_BODY_

#define DRIVING 10
#define REST 2

#define LOW 0
#define HIGH 1

#define TURN_ON(pin) digitalWrite(pin, 1)

#define TURN_OFF(pin) digitalWrite(pin, 0)

#define READ(pin) digitalRead(pin)

#define WRITE(pin, x) digitalWrite(pin, x)

// A. Pin number definitions (DO NOT MODIFY)
// Important! USE the pin numbers and WIRE.
// We use 8 sensors.
//
// 1. Button
#define PIN_BUTTON 0

// 2. 2-Color LED
#define PIN_YELLOW 1

// 3. Temperature
#define PIN_TEMP 4

// 4. Tracking Sensor
#define PIN_TRACK 5

// 5. Touch Sensor
#define PIN_TOUCH 6

// 6. RGB(3-Color) LED
#define PIN_RED 7
#define PIN_GREEN 8
#define PIN_BLUE 9

// 7. Auto-flash Red
#define PIN_ALED 12

// 8. Buzzer
#define PIN_BUZZER 13

// Blue PWM code
#define LED_BLUE() {                   \
        softPwmWrite(PIN_RED, 0x00);   \
        softPwmWrite(PIN_GREEN, 0x00); \
        softPwmWrite(PIN_BLUE, 0xff);  \
}

// Red PWM code
#define LED_RED() {                    \
        softPwmWrite(PIN_RED, 0xff);   \
        softPwmWrite(PIN_GREEN, 0x00); \
        softPwmWrite(PIN_BLUE, 0x00);  \
}

// Yellow PWM code
#define LED_YELLOW() {                 \
        softPwmWrite(PIN_RED, 0xff);   \
        softPwmWrite(PIN_GREEN, 0xff); \
        softPwmWrite(PIN_BLUE, 0x00);  \
}

// Purple PWM code
#define LED_PURPLE() {                 \
        softPwmWrite(PIN_RED, 0xc8);   \
        softPwmWrite(PIN_GREEN, 0x3b); \
        softPwmWrite(PIN_BLUE, 0xff);  \
}

// B. Shared structure
// All thread fuctions get a shared variable of the structure
// as the function parameter.
// If needed, you can add anything in this structure.
typedef struct shared_variable {
	int bProgramExit; // Once it is set to 1, the program will be terminated.
	int state;
    int temp;
    int touch;
	int track;
} SharedVariable;


// C. Functions
// You need to implement following functions.
// Do not change any function name here.
void init_shared_variable(SharedVariable* sv);
void init_sensors(SharedVariable* sv);

void body_button(SharedVariable* sv);
void body_twocolor(SharedVariable* sv);
void body_temp(SharedVariable* sv);
void body_track(SharedVariable* sv);
void body_touch(SharedVariable* sv);
void body_rgbcolor(SharedVariable* sv);
void body_aled(SharedVariable* sv);
void body_buzzer(SharedVariable* sv);

#endif
