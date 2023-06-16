#include "dialog.h"
#include "ui_dialog.h"

#include "chartdialog.h"

gpio g;

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{

    ui->setupUi(this);

    timer = new QTimer(this);

    connect(timer,SIGNAL(timeout()),this,SLOT(gpio_control()));

    timer->start(global::TIME);
}

Dialog::~Dialog()
{
    delete ui;
}

//-----------------------------------Radio box-------------------------------------------------------------

//+++++++++++++++++++++++++++++++++++Ultrasound+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void Dialog::on_hcsr04_clicked()
{
    g.set_hcsr04_en(true);

    if(g.get_reverse_en())
        ui->slider->setValue(100);
    else
       ui->slider->setValue(0);

    ui->slider->setEnabled(false);

    ui->button->setEnabled(true);
    ui->button->setStyleSheet("border-image:url(:/Icons/Icons/unlock.png);");
}

//++++++++++++++++++++++++++++++++Manual+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

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

void Dialog::on_slider_valueChanged(int value)
{
    g.set_manual_value(value);
}

//-----------------------------------Check box------------------------------------------------------------------------

void Dialog::on_reverse_stateChanged(int arg1)
{
    if(arg1 == 0){
        g.set_reverse_en(false);

        if(g.get_hcsr04_en())
             ui->slider->setValue(0);
    }

    else if(arg1 == 2){
        g.set_reverse_en(true);

        if(g.get_hcsr04_en())
             ui->slider->setValue(100);
    }

    if(!g.get_hcsr04_en()){
        reverse();

        ui->slider->setValue(g.get_controle_value());
        ui->bar->setValue(g.get_controle_value());
    }
}

//-----------------------------------button opens a Dialog with chart-------------------------------------------------

void Dialog::on_chart_bt_clicked()
{
    if(!global::chart_en){
        global::chart_en = true;
        ChartDialog *cd = new ChartDialog(this);

        cd->setAttribute(Qt::WA_DeleteOnClose);

        cd->show(); 
    }
}

//----------Slot that executes working_mode() from gpio.c every 30 milliseconds and sets the value of the bar---------------

void Dialog::gpio_control(){
    g.working_mode();
    ui->bar->setValue(g.get_controle_value());
}
