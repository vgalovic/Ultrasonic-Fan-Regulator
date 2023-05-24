#ifndef CHARTDIALOG_H
#define CHARTDIALOG_H

#include <QDialog>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QTimer>

#include "gpio.h"

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class ChartDialog;
}

class ChartDialog : public QDialog, public gpio
{
    Q_OBJECT

public:
    explicit ChartDialog(QWidget *parent = nullptr);
    ~ChartDialog();

private slots:
    void create_chart();

private:
    Ui::ChartDialog *ui;

    QLineSeries *series;

    QChart *chart;

    QTimer *timer;
};

#endif // CHARTDIALOG_H
