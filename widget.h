/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef WIDGET_H
#define WIDGET_H

#include <QtWidgets/QWidget>
#include <QtCharts/QChartGlobal>
//#include <QSerialPort>
#include "connectionhandler.h"
#include "devicehandler.h"
#include "devicefinder.h"
#include <QLabel>

QT_CHARTS_BEGIN_NAMESPACE
class QLineSeries;
class QChart;
class QChartView;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class XYSeriesIODevice;

QT_BEGIN_NAMESPACE
class QAudioInput;
class QAudioDeviceInfo;
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget( QWidget *parent = nullptr);
    ~Widget();

public slots:
    void addPoint();
    void playSound();


private:
    XYSeriesIODevice *m_device = nullptr;
    QLineSeries *expirationFlowSeries ;
    QLineSeries *pressureFlowSeries ;
    QLineSeries *volumeFlowSeries;


    QAudioInput *m_audioInput = nullptr;

   // QSerialPort *m_serial = nullptr;
    QVector<QPointF> flowBuffer;
    QVector<QPointF> volumeBuffer;
    const float sampleRate = 33.333333333333;


    ConnectionHandler* connectionHandler;
    DeviceHandler* deviceHandler;
    DeviceFinder* deviceFinder;

    static const int sampleCount = 500;

    qint64 addFlow(float flow_LPM);
    qint64 addVolume(float volume_mL);

    QLabel* tidalVolumeLabel;
    QLabel* respiratoryRateLabel;
    QLabel* minuteVolumeLabel;
    QLabel* respiratoryRateMinMax;

    void openSerialPort();
    void closeSerialPort();
    void writeData(const QByteArray &data);

    QChartView* createChart(QtCharts::QLineSeries* lineSeries, QString axisTitle, int rangeLow, int rangeHigh);
    void setTidalVolume(float tidalVolume);
    void setRespiratoryRate(float respiratoryRate);
    void setMinuteVolume(float minuteVolume);

    float minuteVolumeInferiorLimit = 5;
    float minuteVolumesuperiorLimit = 8;
    bool minuteVolumeAlarm = false;

    QLabel* minuteVolumeMinMax;
private slots:
    void newFlowMeasurement(float flow_LPM);
    void verifyAlarms();
    void configureAlarms();

  //  void handleError(QSerialPort::SerialPortError error);
};

#endif // WIDGET_H
