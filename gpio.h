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

    void working_mode();

 protected:
    int distance;

    int controle_value;
    int manual_value;

    bool hcsr04_en;
    bool reverse_en;
    bool manual_value_changed;

   //-------------//
    bool chart_en;
   //-------------//

private:
    void get_distance();
    void lcd_diplay();
    int hcsr04_procent();
};

#endif // GPIO_H
