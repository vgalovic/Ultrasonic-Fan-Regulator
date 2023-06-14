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


 protected:
    int distance;

    int controle_value;

    bool hcsr04_en;
    bool reverse_en;
    bool manual_value_changed;

   //-------------//
    bool chart_en;
   //-------------//

private:
    void get_distance_from_hcsr04();
    void lcd_diplay();
    int hcsr04_procent();
};

#endif // GPIO_H
