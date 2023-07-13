/*!
* \file chartdialog.h
* \brief This is header of ChartDialog class. A class called ChartDialog generates dialog boxes that include charts.
* The graph illustrates how, in real time, the distance between the object and the distance sensor varies.
* \author Vladimir Galovć
* \date 21 - 05 - 2023
* \version 1.0
*/

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
    //! Ui::ChartDialog *ui: This variable represents a pointer to the Ui::ChartDialog object,
    //! which manages the user interface elements of the ChartDialog class.
    Ui::ChartDialog *ui;

    //! QLineSeries *series: This variable represents a pointer to a QLineSeries object, which is
    //! used to store and display data points in a line chart.
    QLineSeries *series;

    //! QChart *chart: This variable represents a pointer to a QChart object,
    //! which is the main chart component that holds series, axes, and other chart elements.
    QChart *chart;

    //! QValueAxis *axisX, *axisY: These variables represent pointers to QValueAxis objects, which
    //! define the X-axis and Y-axis of the chart. They provide functionality for configuring the axis
    //! ranges, labels, and titles.
    QValueAxis *axisX;
    QValueAxis *axisY;

    //! QTimer *timer: This variable represents a pointer to a QTimer object, which is used for triggering periodic updates of the chart.
    QTimer *timer;

    //! float t: This variable represents a floating-point value used for tracking the time in seconds for chart updates.
    float t;

    //! float time_limit: This variable represents a floating-point value indicating the time limit in seconds for the chart's X-axis.
    float time_limit;

    //! int distance_limit: This variable represents an integer value indicating the maximum distance limit for the chart's Y-axis.
    int distance_limit;
};

#endif // CHARTDIALOG_H


/*=====================================
 * sudo apt install libqt5charts5-dev
 =====================================*/
