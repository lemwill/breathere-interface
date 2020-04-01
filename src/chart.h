#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QObject>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>

QT_CHARTS_BEGIN_NAMESPACE
class QLineSeries;
class QChart;
class QChartView;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class Chart : public QChart
{
public:
    Chart(QString axisTitle,  int sampleQty, int sampleRate, int yRangeLow, int yRangeHigh);
    QLineSeries* lineSeries;
    void addPoint(float yValue);
    QChartView* getChartView();

private:
    QChartView* chartView = new QChartView(this);
    QVector<QPointF> pointBuffer;
    int sampleQty;
    float sampleRate;

};

#endif // CHARTWIDGET_H
