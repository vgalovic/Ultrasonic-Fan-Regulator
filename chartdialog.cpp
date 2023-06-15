#include "chartdialog.h"
#include "ui_chartdialog.h"

float counter = 0;

ChartDialog::ChartDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ChartDialog)
    , series(new QLineSeries)
    , chart(new QChart)
    , axisX(new QValueAxis)
    , axisY(new QValueAxis)
{
    ui->setupUi(this);
    timer = new QTimer(this);

    QChartView *chartView = new QChartView(chart);
     chartView->setMinimumSize(800, 600);

     chart->addSeries(series);
     axisX->setRange(0, 1);
     axisX->setLabelFormat("%g");
     axisX->setTitleText("Vreme [s]");

     axisY->setRange(2, 400);
     axisY->setLabelFormat("%g");
     axisY->setTitleText("Distanca [cm]");

     chart->addAxis(axisX, Qt::AlignBottom);
     series->attachAxis(axisX);

     chart->addAxis(axisY, Qt::AlignLeft);
     series->attachAxis(axisY);

     chart->legend()->hide();
     chart->setTitle("Izmerena distanca izmedju objekta i senzora HCSR04");
     ui->verticalLayout->addWidget(chartView);

    connect(timer,SIGNAL(timeout()),this,SLOT(create_chart()));

    timer->start(global::TIME);
}

ChartDialog::~ChartDialog()
{
    global::chart_en = false;
    delete ui;
}

void ChartDialog::update_chart(){
    series->append(counter, global::distance);

    counter += global::TIME/1000;

    axisX->setRange(0, counter+ 1.5);
}
