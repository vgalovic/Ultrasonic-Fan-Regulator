#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QApplication>
#include <QFile>
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

    void on_slider_valueChanged(int value);

    void on_hcsr04_clicked();

    void on_manual_clicked();

    void on_reverse_stateChanged(int arg1);

    void on_chart_bt_clicked();

    void gpio_control();

private:
    Ui::Dialog *ui;

    QTimer *timer;
};
#endif // DIALOG_H
