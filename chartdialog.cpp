#include "chartdialog.h"
#include "ui_chartdialog.h"

int const TIME = 500;

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

    timer->start(TIME);

}

ChartDialog::~ChartDialog()
{
    gpio::chart_en = false;
    delete ui;
}

void ChartDialog::create_chart(){
    series->append(gpio::distance, counter);

     counter += TIME;
}
