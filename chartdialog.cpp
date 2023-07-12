#include "chartdialog.h"
#include "ui_chartdialog.h"

//-------------------------------------------------------------------------------------------------------//
/*!
 * \brief ChartDialog::ChartDialog
 * \param parent
 * This is the constructor for the ChartDialog class, which sets up a chart dialog window. It initializes various components such as the chart,
 * axes, and timer, sets the limits for the chart, adds the series to the chart, and connects the timer signal to update the chart. Finally,
 * it starts the timer for periodic updates.
 */
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
//-------------------------------------------------------------------------------------------------------//

/*!
 * \brief ChartDialog::~ChartDialog
 * The destructor for the ChartDialog class sets the chart_en flag to false and deletes the ui object, freeing the memory.
 */
ChartDialog::~ChartDialog()
{
    global::chart_en = false;
    delete ui;
}
//-------------------------------------------------------------------------------------------------------//

/*!
 * \brief ChartDialog::update_chart
 * The update_chart() function in the ChartDialog class appends a new data point to the chart series, updates the chart's title with the current
 * distance and time values, adjusts the time and distance limits on the chart if needed, and increments the time value for the next data point.
 */

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
//-------------------------------------------------------------------------------------------------------//
