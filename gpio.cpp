#include "gpio.h"

//***************************************************************************************************************************//

//! The purpose of namespace global is to make variables accessible to other classes that derive from the gpio class.
namespace global {
    const int  TIME = 500;  //! Time for timer in miliseconds

    const int MAX_DISTANCE = 22;
    //! The greatest distance from HCSR04 that is permitted, anything beyond that will be calculated as a percentage of 100%.

    int distance = 0;   //! Calculates the distance in centimeters between HCSR04 and the object in front of him

    bool chart_en = false;  //! Stops multiple chart dialogs from opening
}

//***************************************************************************************************************************//

//! The wiringPi.h library manages the ultrasound sensor (HC-SR04) through the pins TRIG and ECHO.
const int TRIG = 4; //! Physical pin 16 or on DVK512 P4
const int ECHO = 5; //! Physical pin 18 or on DVK512 P5
//! 5v pin  4 and GND pin 6

//! The softPwm.h library uses PWM to regulate the speed of the cooling through a pin PWM.
const int PWM = 1; //! Physical pin 12 or on DVK512 P1
//! 5v piin 2 and GND pin 14

//! The pins that are used by the lcd.h library to initialize the LCD display are RS, EN, D0, D1, D2, D3, and D4.
const int RS = 3;
const int EN = 14;
const int D0 = 4;
const int D1 = 12;
const int D2 = 13;
const int D3 = 6;

//!  fd serves as a special identifier that enables interaction between other processes or functions and the
//! LCD display using the assigned file descriptor.
int fd;

int controle_value = 0; //! Stores the final value that controls the speed of the cooler.

bool hcsr04_en = true; //! Determine whether the PWM input comes from the slider or the HCSR04.
bool reverse_en = false; //! Determine whether or not controls are reversed.
bool manual_value_changed = false; //! Determines if the manual_value has changed.

/*========================================================================================================

                                         Public

========================================================================================================*/


/*!
 * \brief gpio::gpio
 * This code represents a destructor for the gpio class that clears the content of the LCD display initialized with the
 *  fd (file descriptor) variable and sets the value of the PWM pin, named PWM, to 0.
 */

    gpio::gpio(){
        wiringPiSetup();
        pinMode(TRIG, OUTPUT);
        pinMode(ECHO, INPUT);

        softPwmCreate(PWM, 0, 100);

        fd = lcdInit(2, 16, 4, RS, EN, D0, D1, D2, D3, D0, D1, D2, D3);

        //! TRIG pin must start LOW
        digitalWrite(TRIG, LOW);
        delay(30);
    }

//-------------------------------------------------------------------------------------------------------//

/*!
 * \brief gpio::~gpio
 * This code represents a destructor for the gpio class that clears the content of the LCD display initialized with the
 *  fd (file descriptor) variable and sets the value of the PWM pin, named PWM, to 0.
 */

gpio::~gpio(){
    lcdClear(fd);
    softPwmWrite(PWM, 0);
}

//-------------------------------------------------------------------------------------------------------//

/*!
 * \brief gpio::working_mode
 * The provided code is a function called working_mode that retrieves distance from an HC-SR04 sensor, updates an LCD display,
 *  calculates a percentage value based on the sensor data, handles manual value changes, and controls the fan speed using a PWM pin.
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


/* SET methodes */

/*!
 * \brief gpio::set_manual_value
 * \param value
 * The set_manual_value() function in the gpio class sets the controle_value to the provided value and marks manual_value_changed as true.
 */

void gpio::set_manual_value(int value){
    manual_value_changed = true;
    controle_value = value;
}

/*!
 * \brief gpio::set_hcsr04_en
 * \param check
 * The set_hcsr04_en() function in the gpio class sets the hcsr04_en variable to the provided check value.
 */

void gpio::set_hcsr04_en(bool check){hcsr04_en = check;}

/*!
 * \brief gpio::set_reverse_en
 * \param check
 * The set_reverse_en() function in the gpio class sets the reverse_en variable to the provided check value.
 */

void gpio::set_reverse_en(bool check){reverse_en = check;}

//-------------------------------------------------------------------------------------------------------//

/* Get methodes */

/*!
 * \brief gpio::get_controle_value
 * \return
 * The get_controle_value() function in the gpio class returns the value stored in the controle_value variable.
 */

int gpio::get_controle_value(){return controle_value; }

/*!
 * \brief gpio::get_hcsr04_en
 * \return
 * The get_hcsr04_en() function in the gpio class returns the value stored in the hcsr04_en variable.
 */

bool gpio::get_hcsr04_en(){return hcsr04_en;}

/*!
 * \brief gpio::get_reverse_en
 * \return
 * The get_hcsr04_en() function in the gpio class returns the value stored in the hcsr04_en variable.
 */

bool gpio::get_reverse_en(){return reverse_en;}


/*========================================================================================================

                                         Private

========================================================================================================*/

/*!
 * \brief gpio::get_distance_from_hcsr04
 * The get_distance_from_hcsr04() function in the gpio class measures the distance using an HC-SR04 sensor by sending trigger pulses,
 *  measuring the echo duration, and calculating the distance in centimeters. It also limits the distance to a maximum of 400 centimeters,
 *  which is the operating range of the sensor.
 */

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

        /*!
         * Get distance in centimeters.
         * The speed of sound is 340 m/s or 29 microseconds per centimeter.
         * Taking half the total distance (there and back) we can use the divisor 58.
         */

        global::distance = travelTime / 58;

        //! Operating Range & Accuraty:  2cm ~ 400cm (1in ~ 13ft) +(-) 3mm
        if(global::distance > 400)
            global::distance = 400;
}

//-------------------------------------------------------------------------------------------------------//

/*!
 * \brief gpio::lcd_diplay
 * The lcd_diplay() function in the gpio class updates the LCD display by clearing its content and printing the distance measurement
 *  in centimeters at the desired positions.
 */

void gpio::lcd_diplay(){
     lcdClear(fd);

     lcdPosition(fd, 0, 0);
     lcdPrintf(fd,"Udaljenost:");

     lcdPosition(fd, 0, 1);
     lcdPrintf(fd,"%d cm", global::distance);
}

//-------------------------------------------------------------------------------------------------------//

/*!
 * \brief gpio::hcsr04_procent
 * The hcsr04_procent() function in the gpio class calculates the control value based on the distance measurement.
 * It sets the control value to 100 if the distance is very close, to 0 if the distance exceeds the maximum,
 * and calculates a proportional value within the range of 0 to 100 based on the distance measurement.
 */

void gpio::hcsr04_procent(){
    if(global::distance <= 2)
        controle_value = 100;
    else if (global::distance >= global::MAX_DISTANCE)
        controle_value = 0;
    else
        controle_value = 100 - (int)(((float)global::distance / MAX_DISTANCE) * 100);
}

//-------------------------------------------------------------------------------------------------------//
