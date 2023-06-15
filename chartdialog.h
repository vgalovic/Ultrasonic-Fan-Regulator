#ifndef CHARTDIALOG_H
#define CHARTDIALOG_H

#include <QDialog>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QValueAxis>
#include <QTimer>

#include "gpio.h"

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class ChartDialog;
}

using namespace QtCharts;

class ChartDialog : public QDialog, public gpio
{
    Q_OBJECT

public:
    explicit ChartDialog(QWidget *parent = nullptr);
    ~ChartDialog();

private slots:
    void update_chart();

private:
    Ui::ChartDialog *ui;

    QLineSeries *series;

    QChart *chart;

    QValueAxis *axisX;
    QValueAxis *axisY;

    QTimer *timer;


    float t;

    float time_limit;
    int distance_limit;
};

#endif // CHARTDIALOG_H


/*=====================================
 * sudo apt install libqt5charts5-dev
 =====================================*/
