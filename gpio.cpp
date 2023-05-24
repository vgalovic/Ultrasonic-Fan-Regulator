#include "gpio.h"

//ultrasound sensor - HCSR04
const int TRIG = 5; //Physical: 18
const int ECHO =  6;//Physical: 22
//5v: 4 GND: 6

//Cooler
const int PWM = 0; //Physical: 11
//5v: 2 GND: 9

//LCD pins
const int RS = 3;
const int EN = 14;
const int D0 = 4;
const int D1 = 12;
const int D2 = 13;
const int D3 = 6;

int fd;

int distance = 0; //calculates the distance in centimeters between HCSR04 and the object in front of him
int controle_value = 0; //obtains the final value to be utilized in fan_control();
int manual_value = 0; //obtains value from slider

bool hcsr04_en = true; //decide whether the PWM input comes from the slider or the HCSR04
bool reverse_en = false; //determine whether or not controls are reversed

//-----------------------------------WiringPi setup,HCSR04 setup, PWM setup--------------------------------------------

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

//----------------------------Clear lcd and delete class----------------------------------------------------------------

gpio::~gpio(){
    lcdClear(fd);
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

//--------------------------------Fan controle-----------------------------------------------------------------------

void gpio::fan_controle(){
    softPwmWrite(PWM, controle_value);
}

//------------------------------Display distance on LCD---------------------------------------------------------------

void gpio::lcd_diplay(){
     lcdClear(fd);
     lcdHome(fd);
     lcdPrintf(fd,"Dist: %d cm", distance);
}


//-------------------------------------Returns whether the controls are reversed--------------------------------------

int gpio::is_reversed(bool b){
    if(b){
        reverse_en = true;
    } else
        reverse_en = false;

    return reversed_value(controle_value);
}

//---------------------------------------Returns the value of the reversed control------------------------------------------


int gpio::reversed_value(int value){
    if(reverse_en){
        return 100 - value;
    }else{
        return value;
    }
}

//------------------------------returns the distance from get_distance() in percentage-----------------------------------

int gpio::hcsr04_procent(){
    if(distance <= 2){
        if(reverse_en)
            return 0;
        else
            return 100;
   }else if (distance >= 102){
        if(reverse_en)
            return 100;
        else
            return 0;
    }else
        return reversed_value(distance) - 2;
}

/*Searches for the distance via get_distance(), displays the distance on the LCD, checks whether the ultrasonic sensor value
is used or the manually entered value from the slider, sets the speed of the cooler based on the check and returns the given value*/

int gpio::working_mode(){
    get_distance();
    lcd_diplay();

    if(hcsr04_en){
        controle_value = hcsr04_procent();

    } else{
        controle_value = reversed_value(manual_value);
    }

    fan_controle();
    return controle_value;
}

//------------------It returns confirmation in which mode the program works and returns the controle_value-----------------------

int gpio::mode_en(bool en){
    if(en){
        hcsr04_en = true;
    } else{
        hcsr04_en = false;
        fan_controle();
    }

    return controle_value;
}

//---------------------------------------------------Gets value from slider-----------------------------------------------------
void gpio::get_manual_value(int value){
    manual_value = value;
}

//------------------------------------------------------Set distance------------------------------------------------------------

int gpio::set_distance(){
    return distance;
}
