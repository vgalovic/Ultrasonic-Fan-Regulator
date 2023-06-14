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

    connect(timer,SIGNAL(timeout()),this,SLOT(update_chart()));

    timer->start(TIME);

}

ChartDialog::~ChartDialog()
{
    gpio::set_chart_en(false);
    delete ui;
}

void ChartDialog::update_chart(){
    series->append(gpio::get_distance(), counter);

     counter += TIME;
}
