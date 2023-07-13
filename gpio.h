/*!
* \file gpio.h
* \brief This is header of gpio class. The class gpio contains methods that interact with connected components using GPIO.
* \author Vladimir Galovć
* \date 21 - 05 - 2023
* \version 1.0
*/

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
    extern int distance;
    extern bool chart_en;
}

class gpio
{
public:
    gpio();
    ~gpio();

    void working_mode();

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
};

#endif // GPIO_H
