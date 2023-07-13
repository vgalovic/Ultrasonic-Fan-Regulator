/*!
* \file dialog.h
* \brief This is header of Dialog class.The main GUI layout and the application's user interface are implemented in this class.
* \author Vladimir Galovć
* \date 21 - 05 - 2023
* \version 1.0
*/

#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include

#include <QTimer>

#include "gpio.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog, public gpio
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void on_button_clicked(bool checked);

    void on_slider_sliderMoved(int value);

    void on_hcsr04_clicked();

    void on_manual_clicked();

    void on_reverse_stateChanged(int arg1);

    void on_chart_bt_clicked();

    void gpio_control();

private:
    //! Ui::ChartDialog *ui: This variable represents a pointer to the Ui::ChartDialog object,
    //! which manages the user interface elements of the Dialog class.
    Ui::Dialog *ui;

    //! QTimer *timer: This variable represents a pointer to a QTimer object, which is used for
    //! triggering timed events or actions. It allows for the scheduling and execution of code at regular intervals.
    QTimer *timer;
};
#endif // DIALOG_H
