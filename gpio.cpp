#include "gpio.h"

//***************************************************************************************************************************//

namespace global {
    const int  TIME = 500; //Time for timer in miliseconds

    // The greatest distance from HCSR04 that is permitted, anything beyond that will be calculated as a percentage of 100%.
    const int MAX_DISTANCE = 22;

    int distance = 0; //calculates the distance in centimeters between HCSR04 and the object in front of him
    bool chart_en = false; //stops multiple chart dialogs from opening
}

//***************************************************************************************************************************//

//ultrasound sensor - HCSR04
const int TRIG = 4; //Physical: 16 ; DVK512: P4
const int ECHO = 5; //Physical: 18 ; DVK512: P5
//5v: 4 GND: 6

//Cooler
const int PWM = 1; //Physical: 12 ; DVK512: P1
//5v: 2 GND: 14

//LCD pins
const int RS = 3;
const int EN = 14;
const int D0 = 4;
const int D1 = 12;
const int D2 = 13;
const int D3 = 6;

int fd;

// The greatest distance from HCSR04 that is permitted, anything beyond that will be calculated as a percentage of 100%.
const int MAX_DISTANCE = 22;

int controle_value = 0; //obtains the final value to be utilized in fan_control();

bool hcsr04_en = true; //decide whether the PWM input comes from the slider or the HCSR04
bool reverse_en = false; //determine whether or not controls are reversed
bool manual_value_changed = false; //determines if the manual_value has changed.

/*========================================================================================================

                                         Public

========================================================================================================*/


/*declares a variables to that will be used in program.
 * Set up WiringPi
 * Set the TRIGER pin mode to OUTPUT and the ECHO pin mode to INPUT.
 * Set up SoftPWM
 * Set up LCD
 * TRIGER set to LOW and wait 30 milliseconds
 */

gpio::gpio(){
    wiringPiSetup();
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);

    softPwmCreate(PWM, 0, 100);

    fd = lcdInit(2, 16, 4, RS, EN, D0, D1, D2, D3, D0, D1, D2, D3);

    //TRIG pin must start LOW
    digitalWrite(TRIG, LOW);
    delay(30);
}

//-------------------------------------------------------------------------------------------------------//

/*Clear lcd and delete class*/

gpio::~gpio(){
    lcdClear(fd);
    softPwmWrite(PWM, 0);
}

//-------------------------------------------------------------------------------------------------------//

/* Use get_distance() to determine the distance
 * Using display_lcd(), show the distance on the LCD,
 * Verify whether the slider's manually entered value or the value from HCSR04 is being used
 * Identify whether reversing a value is necessary, and if so, do it
 * based on the outcome of the code beforehand, adjust the cooler's speed
 */

void gpio::working_mode(){
    get_distance_from_hcsr04();
    lcd_diplay();

    if(hcsr04_en)
        hcsr04_procent();

    else if(manual_value_changed)
        manual_value_changed = false;

    else
        goto FAN_CONTROLE;

    if(reverse_en)
        controle_value = 100 - controle_value;

    FAN_CONTROLE:
    softPwmWrite(PWM, controle_value);
}

//-------------------------------------------------------------------------------------------------------//

/*Set methodes*/

void gpio::set_manual_value(int value){
    manual_value_changed = true;
    controle_value = value;
}

void gpio::set_hcsr04_en(bool check){hcsr04_en = check;}
void gpio::set_reverse_en(bool check){reverse_en = check;}

//-------------------------------------------------------------------------------------------------------//

/*Get methodes*/

int gpio::get_controle_value(){return controle_value; }

bool gpio::get_hcsr04_en(){return hcsr04_en;}
bool gpio::get_reverse_en(){return reverse_en;}


/*========================================================================================================

                                         Private

========================================================================================================*/

/*gets the centimeters between the sensor HCSR04 and an object*/

void gpio::get_distance_from_hcsr04() {
        //Send trig pulse
        digitalWrite(TRIG, HIGH);
        delayMicroseconds(20);
        digitalWrite(TRIG, LOW);

        //Wait for echo start
        while(digitalRead(ECHO) == LOW);

        //Wait for echo end
        long startTime = micros();
        while(digitalRead(ECHO) == HIGH);
        long travelTime = micros() - startTime;

        //Get distance in cm
        global::distance = travelTime / 58;

        /* The speed of sound is 340 m/s or 29 microseconds per centimeter.
         * Taking half the total distance (there and back) we can use the divisor 58.
         */

        // Operating Range & Accuraty:  2cm ~ 400cm (1in ~ 13ft) +(-) 3mm
        if(global::distance > 400)
            global::distance = 400;
}

//-------------------------------------------------------------------------------------------------------//

/*Display distance on LCD*/

void gpio::lcd_diplay(){
     lcdClear(fd);

     lcdPosition(fd, 0, 0);
     lcdPrintf(fd,"Udaljenost:");

     lcdPosition(fd, 0, 1);
     lcdPrintf(fd,"%d cm", global::distance);
}

//-------------------------------------------------------------------------------------------------------//

/*returns the distance in percentage from get_distance()*/

void gpio::hcsr04_procent(){
    if(global::distance <= 2)
        controle_value = 100;
    else if (global::distance >= global::MAX_DISTANCE)
        controle_value = 0;
    else
        controle_value = 100 - (int)(((float)global::distance / MAX_DISTANCE) * 100);
}

//-------------------------------------------------------------------------------------------------------//
