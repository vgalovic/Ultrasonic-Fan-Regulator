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

    timer->start(30);
}

Dialog::~Dialog()
{
    delete ui;
}

//-----------------------------------Radio box-------------------------------------------------------------

//+++++++++++++++++++++++++++++++++++Ultrasound+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void Dialog::on_hcsr04_clicked()
{
    ui ->slider->setEnabled(false);
    g.mode_en(true);

    ui->button->setEnabled(true);
    ui->button->setStyleSheet("border-image:url(:/Icons/unlock.png);");
}

//++++++++++++++++++++++++++++++++Manual+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void Dialog::on_manual_clicked()
{
    ui ->slider->setEnabled(true);

    ui->button->setEnabled(false);
    ui->button->setChecked(false);
    ui->button->setStyleSheet("border-image:url(:/Icons/disabled.png);");

    ui->slider->setValue(g.mode_en(false));
}

//------------------------------------Image Button-----------------------------------------------------------------------

void Dialog::on_button_clicked(bool checked)
{
    //=================================Locked=========================================================
    if(checked){
        ui->button->setStyleSheet("border-image:url(:/Icons/lock.png);");
        g.mode_en(false);
    }
    //=================================Unlocked=========================================================
    else{
        ui->button->setStyleSheet("border-image:url(:/Icons/unlock.png);");
        g.mode_en(true);
    }
}

//-------------------------------------Slider-------------------------------------------------------------------------

void Dialog::on_slider_valueChanged(int value)
{
   g.get_manual_value(value);
}

//-----------------------------------Check box------------------------------------------------------------------------

void Dialog::on_reverce_stateChanged(int arg1)
{
    if(arg1 == 0){
        ui->slider->setInvertedAppearance(false);
        ui->slider->setValue(g.is_reversed(false));
    }
    else if(arg1 == 2){
        ui->slider->setInvertedAppearance(true);
        ui->slider->setValue(g.is_reversed(true));
    }
}

//-----------------------------------button that opens Dialog with chart-------------------------------------------------

void Dialog::on_chart_bt_clicked()
{
    ChartDialog *cd =new ChartDialog(this);
    cd->show();
}

//------------------------------Slot that calls working_mode() from gpio.c evrey 30ms and sets value on bar---------------

void Dialog::gpio_control(){
    ui->bar->setValue(g.working_mode());

}

