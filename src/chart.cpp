#include "chart.h"
#include <QGraphicsLayout>

QT_CHARTS_USE_NAMESPACE

Chart::Chart(QString axisTitle, int sampleQty, int sampleRate, int yRangeLow, int yRangeHigh){

    this->sampleQty = sampleQty;
    this->sampleRate = sampleRate;

    // Create chart view
    chartView = new QChartView(this);

    // Create line series
    lineSeries = new QLineSeries(chartView);

    // Add line series to the chart
    this->addSeries(lineSeries);

    // Configure the X Axis
    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(0, sampleQty/sampleRate);
    axisX->setLabelFormat("%g");
    axisX->setTickCount(16);
    axisX->setGridLineVisible(false);
    this->addAxis(axisX, Qt::AlignBottom);
    lineSeries->attachAxis(axisX);

    // Configure the Y axis
    QValueAxis *axisY = new QValueAxis;
    axisY->setLinePenColor(QColor(0,0,0));
    axisY->setRange(yRangeLow, yRangeHigh);
    axisY->setTitleText(axisTitle);
    this->addAxis(axisY, Qt::AlignLeft);
    lineSeries->attachAxis(axisY);

    // Hide the legend
    this->legend()->hide();

    // Configure the margins
    this->setContentsMargins(0,-15,0,-15);
    this->layout()->setContentsMargins(0, 0, 0, 0);
    this->setBackgroundRoundness(0);

    // Configure the graphic style
    this->setTheme(QChart::ChartThemeDark);
    QBrush backgroundBrush(QColor(44,46,56));
    this->setBackgroundBrush(backgroundBrush);
}

QChartView* Chart::getChartView() {

    return chartView;

}

void Chart::addPoint(float yValue)
{
    if (pointBuffer.isEmpty()) {

        pointBuffer.reserve(sampleQty);

        for (int i = 0; i < sampleQty; ++i){
            pointBuffer.append(QPointF(i/sampleRate, 0));
        }
    }

    int start = sampleQty-1;

    for (int s = 0; s < start; ++s){
        pointBuffer[s].setY(pointBuffer.at(s + 1).y());
    }

    for (int s = start; s < sampleQty; ++s){
        pointBuffer[s].setY(yValue);
    }

    lineSeries->replace(pointBuffer);

}
