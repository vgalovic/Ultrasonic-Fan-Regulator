#include "chartdialog.h"
#include "ui_chartdialog.h"

int counter = 0;

ChartDialog::ChartDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ChartDialog)
{
    ui->setupUi(this);

    series = new QLineSeries();
    chart = new QChart();

    timer = new QTimer(this);

    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    //postavljanje grafika u vertikal layout
     ui->verticalLayout->addWidget(chartView);

    connect(timer,SIGNAL(timeout()),this,SLOT(create_chart()));

    timer->start(30);

}

ChartDialog::~ChartDialog()
{
    delete ui;
}

void ChartDialog::create_chart(){
    series->append(gpio::distance, counter);

     counter += 30;
}
