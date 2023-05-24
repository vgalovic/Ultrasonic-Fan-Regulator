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

    int is_reversed(bool b);

    int mode_en(bool en);

    void get_manual_value(int value);

    int working_mode();

    int set_distance();

private:

    void get_distance();

    void fan_controle(int value);

    void lcd_diplay();

    int reversed_value(int value);

    int ultrasound_procent();
};

#endif // GPIO_H
