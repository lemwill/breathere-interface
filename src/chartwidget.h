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

class ChartWidget : public QtCharts::QChartView
{
public:
    ChartWidget();
};

#endif // CHARTWIDGET_H
