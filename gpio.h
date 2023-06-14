#ifndef GPIO_H
#define GPIO_H

#include <stdio.h>
#include <stdlib.h>

#include <wiringPi.h>
#include <softPwm.h>
#include <lcd.h>

class gpio
{
public:
    int const TIME = 500;

    gpio();
    ~gpio();

    void working_mode();

    //-----set------------//

    void set_manual_value(int value);

    void set_hcsr04_en(bool check);
    void set_reverse_en(bool check);

    void set_chart_en(bool check);

    //-----get------------//

    int get_distance();

    int get_controle_value();

    bool get_hcsr04_en();
    bool get_reverse_en();

    bool get_chart_en();

//-------------------------------------------------------------------------------------------------------//

private:
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

    int distance = 0; //calculates the distance in centimeters between HCSR04 and the object in front of him

    int controle_value = 0; //obtains the final value to be utilized in fan_control();

    bool hcsr04_en = true; //decide whether the PWM input comes from the slider or the HCSR04
    bool reverse_en = false; //determine whether or not controls are reversed
    bool manual_value_changed = false; //determines if the manual_value has changed.

   //-------------//
    bool chart_en = false; //stops multiple chart dialogs from opening
   //-------------//

    void get_distance_from_hcsr04();
    void lcd_diplay();
    int hcsr04_procent();
};

#endif // GPIO_H
