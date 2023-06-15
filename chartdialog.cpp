#include "chartdialog.h"
#include "ui_chartdialog.h"


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

    t = (float)global::TIME/1000;

    time_limit = 5;
    distance_limit = global::MAX_DISTANCE;



    QChartView *chartView = new QChartView(chart);
    chartView->setMinimumSize(800, 600);

    chart->addSeries(series);
    axisX->setRange(0, time_limit);
    axisX->setLabelFormat("%g");
    axisX->setTitleText("Vreme [s]");

    axisY->setRange(2, distance_limit);
    axisY->setLabelFormat("%g");
    axisY->setTitleText("Distanca [cm]");

    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->hide();
    chart->setTitle("Izmerena distanca između objekta i senzora HCSR04");
    ui->verticalLayout->addWidget(chartView);

    connect(timer,SIGNAL(timeout()),this,SLOT(update_chart()));

    timer->start(global::TIME);
}

ChartDialog::~ChartDialog()
{
    global::chart_en = false;
    delete ui;
}

void ChartDialog::update_chart(){
    series->append(t, global::distance);

    QString title = QString("Izmerena distanca između objekta i senzora HCSR04 [Distanca: %1 cm, Vreme: %2 s]").arg(global::distance).arg(t);
    chart->setTitle(title);

    if(t == time_limit){
        time_limit += 5;
        axisX->setRange(t, time_limit);
    }
    if(global::distance > distance_limit){
        distance_limit = global::distance;
        axisY->setRange(2, distance_limit);
    }

   t += (float)global::TIME/1000;
}
