#ifndef GPIO_H
#define GPIO_H

#include <stdio.h>
#include <stdlib.h>

#include <wiringPi.h>
#include <softPwm.h>
#include <lcd.h>

namespace global {
    extern  const int TIME;
    extern  const int MAX_DISTANCE;
    extern int distance; //calculates the distance in centimeters between HCSR04 and the object in front of him
    extern bool chart_en; //stops multiple chart dialogs from opening
}

class gpio
{
public:
    gpio();
    ~gpio();

    void working_mode();
    void reverse();

    //-----set------------//

    void set_manual_value(int value);

    void set_hcsr04_en(bool check);
    void set_reverse_en(bool check);

    //-----get------------//

    int get_controle_value();

    bool get_hcsr04_en();
    bool get_reverse_en();

//-------------------------------------------------------------------------------------------------------//

private:
    void get_distance_from_hcsr04();
    void lcd_diplay();
    void hcsr04_procent();
    void fan_controle(int value);
};

#endif // GPIO_H
