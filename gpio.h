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
    gpio();

    ~gpio();

    void is_reversed(bool b);

    void mode_en(bool en);

    void get_manual_value(int value);

    int working_mode();

    int set_distance();

    int set_control_value();

    bool set_hcsr04_en();

private:

    void get_distance();

    void lcd_diplay();

    int reversed_value(int value);

    int hcsr04_procent();
};

#endif // GPIO_H
