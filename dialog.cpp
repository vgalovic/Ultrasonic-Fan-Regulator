/*!
* \file dialog.cpp
* \brief  This is source of Dialog class.The main GUI layout and the application's user interface are implemented in this class.
* \author Vladimir Galovć
* \date 21 - 05 - 2023
* \version 1.0
*/

#include "dialog.h"
#include "ui_dialog.h"

#include "chartdialog.h"

//! Declares an object g of type gpio. It creates an instance of the gpio class.
//! The object can be used to access and utilize the features and methods provided by the gpio class.
gpio g;

//-------------------------------------------------------------------------------------------------------//
/*!
 * \brief Dialog::Dialog
 * \param parent
 * The constructor for the Dialog class sets up the user interface, creates a timer for timed events,
 * establishes a connection between the timer and a function slot, and starts the timer.
 */
Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{

    ui->setupUi(this);

    timer = new QTimer(this);

    connect(timer,SIGNAL(timeout()),this,SLOT(gpio_control()));

    timer->start(global::TIME);
}
//-------------------------------------------------------------------------------------------------------//

/*!
 * \brief Dialog::~Dialog
 * The destructor for the Dialog class deletes the ui object, freeing the memory.
 */
Dialog::~Dialog()
{
    delete ui;
}

//-----------------------------------Radio box-------------------------------------------------------------

//+++++++++++++++++++++++++++++++++++Ultrasound+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


/*!
 * \brief Dialog::on_hcsr04_clicked
 * The on_hcsr04_clicked() function sets the hcsr04_en flag to true, adjusts the slider and button states
 * and appearances in the user interface accordingly.
 */
void Dialog::on_hcsr04_clicked()
{
    g.set_hcsr04_en(true);

    ui->slider->setValue(0);

    ui->slider->setEnabled(false);

    ui->button->setEnabled(true);
    ui->button->setStyleSheet("border-image:url(:/Icons/Icons/unlock.png);");
}

//++++++++++++++++++++++++++++++++Manual+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/*!
 * \brief Dialog::on_manual_clicked
 * The on_manual_clicked() function enables the slider, updates the button states and appearances,
 * disables the HC-SR04 functionality, and sets the slider value.
 */
void Dialog::on_manual_clicked()
{
    ui->slider->setEnabled(true);

    ui->button->setEnabled(false);
    ui->button->setChecked(false);
    ui->button->setStyleSheet("border-image:url(:/Icons/Icons/disabled.png);");

    g.set_hcsr04_en(false);

    ui->slider->setValue(g.get_controle_value());
}

//------------------------------------Image Button-----------------------------------------------------------------------
/*!
 * \brief Dialog::on_button_clicked
 * \param checked
 * The on_button_clicked() function updates the button's appearance and toggles the HC-SR04 functionality based on whether the
 * button is checked or unchecked.
 */
void Dialog::on_button_clicked(bool checked)
{
    //=================================Locked=========================================================
    if(checked){
        ui->button->setStyleSheet("border-image:url(:/Icons/Icons/lock.png);");
        g.set_hcsr04_en(false);
    }
    //=================================Unlocked=========================================================
    else{
       ui->button->setStyleSheet("border-image:url(:/Icons/Icons/unlock.png);");
       g.set_hcsr04_en(true);
    }
}

//-------------------------------------Slider-------------------------------------------------------------------------

/*!
 * \brief Dialog::on_slider_sliderMoved
 * \param value
 * The on_slider_sliderMoved() function updates the manual value in the g object based on the slider position.
 * If the reverse flag is true, it adjusts the value for a progress bar in reverse. Otherwise, it sets the value normally.
 */
void Dialog::on_slider_sliderMoved(int value)
{
    g.set_manual_value(value);

    if(g.get_reverse_en())
        ui->bar->setValue(100 - value);
    else
        ui->bar->setValue(value);
}

//-----------------------------------Check box------------------------------------------------------------------------

/*!
 * \brief Dialog::on_reverse_stateChanged
 * \param arg1
 * The on_reverse_stateChanged() function updates the reverse flag in the g object based on the state change.
 * If the HC-SR04 functionality is disabled, it adjusts the manual value, invokes the working mode, and updates
 * the slider and bar values in the user interface.
 */
void Dialog::on_reverse_stateChanged(int arg1)
{
    if(arg1 == 0){
        g.set_reverse_en(false);
    }

    else if(arg1 == 2){
        g.set_reverse_en(true);
    }

    if(!g.get_hcsr04_en()){
        if(arg1 == 2)
            g.set_manual_value(g.get_controle_value());
        else
            g.set_manual_value(100 - g.get_controle_value());

        g.working_mode();

        ui->slider->setValue(g.get_controle_value());
        ui->bar->setValue(g.get_controle_value());
    }
}

//-----------------------------------button opens a Dialog with chart-------------------------------------------------

/*!
 * \brief Dialog::on_chart_bt_clicked
 * The on_chart_bt_clicked() function enables the chart functionality if it is currently disabled, creates a new
 * ChartDialog instance, and displays the chart window.
 */
void Dialog::on_chart_bt_clicked()
{
    if(!global::chart_en){
        global::chart_en = true;
        ChartDialog *cd = new ChartDialog(this);

        cd->setAttribute(Qt::WA_DeleteOnClose);

        cd->show(); 
    }
}
//-------------------------------------------------------------------------------------------------------//

/*!
 * \brief Dialog::gpio_control
 * The gpio_control() function updates the working mode and progress bar value.
 */
void Dialog::gpio_control(){
    g.working_mode();

    ui->bar->setValue(g.get_controle_value());
}
//-------------------------------------------------------------------------------------------------------//
