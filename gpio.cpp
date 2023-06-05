#include "gpio.h"

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

const int MAX_DISTANCE = 22; // The greatest distance from HCSR04 that is permitted; anything beyond that will be calculated as a percentage of 100%.

//-----------------------------------WiringPi setup,HCSR04 setup, PWM setup--------------------------------------------

gpio::gpio(){

    //===================================================================================================//
    distance = 0; //calculates the distance in centimeters between HCSR04 and the object in front of him

    controle_value = 0; //obtains the final value to be utilized in fan_control();
    manual_value = 0; //obtains value from slider

    hcsr04_en = true; //decide whether the PWM input comes from the slider or the HCSR04
    reverse_en = false; //determine whether or not controls are reversed
    //===================================================================================================//

    wiringPiSetup();
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);

    softPwmCreate(PWM, 0, 100);

    fd = lcdInit(2, 16, 4, RS, EN, D0, D1, D2, D3, D0, D1, D2, D3);

    //TRIG pin must start LOW
    digitalWrite(TRIG, LOW);
    delay(30);
}

//----------------------------Clear lcd and delete class----------------------------------------------------------------

gpio::~gpio(){
    lcdClear(fd);
    softPwmWrite(PWM, 0);
}

/* Get_distance() is used to find the distance, display the distance on the LCD, check whether the value from HCSR04 or the manually
 * input value from the slider is being utilized, set the cooler's speed based on the result of the check, and then return the value
 * that was supplied.
 */

void gpio::working_mode(){
    get_distance();
    lcd_diplay();

    if(hcsr04_en)
        controle_value = hcsr04_procent();

    else
        controle_value = manual_value;

    if(reverse_en)
        controle_value = 100 - controle_value;

    softPwmWrite(PWM, controle_value);
}

//--------------------------------HCSR04 distance-----------------------------------------------------------------------

void gpio::get_distance() {
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
        distance = travelTime / 58;
}

//------------------------------Display distance on LCD---------------------------------------------------------------

void gpio::lcd_diplay(){
     lcdClear(fd);
     lcdPosition(fd, 0 , 0);
     lcdPrintf(fd,"Dist: %d cm", distance);
}

//------------------------------returns the distance in percentage from get_distance()-----------------------------------

int gpio::hcsr04_procent(){
    if(distance <= 2)
        return 0;
    else if (distance >= MAX_DISTANCE)
        return 100;
    else
        return (distance / MAX_DISTANCE) * 100;
}
