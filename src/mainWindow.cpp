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

#include "mainWindow.h"

#include <QMediaPlayer>


#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>
#include <QGraphicsLayout>

#include <QtWidgets/QVBoxLayout>
#include <QDebug>
#include <QLabel>
#include <QFontDatabase>
#include <QTimer>
#include <QTime>
#include <QtMath>
#include <QPushButton>
#include "configurealarmdialog.h"


#include "QBuffer"
#include <QAudioOutput>
#include <QtMath>
#include <QLineEdit>
#include <QInputDialog>
#include <QSizePolicy>

#define SAMPLE_RATE 44000
#define FREQ_CONST ((2.0 * M_PI) / SAMPLE_RATE)

QT_CHARTS_USE_NAMESPACE

QChartView* MainWindow::createChart(QtCharts::QLineSeries* lineSeries, QString axisTitle, int rangeLow, int rangeHigh){

    QChart* chart = new QChart();
    QChartView *chartView = new QChartView(chart);

    chart->addSeries(lineSeries);
    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(0, sampleCount/sampleRate);
    axisX->setLabelFormat("%g");
    QValueAxis *axisY = new QValueAxis;
    axisY->setLinePenColor(QColor(0,0,0));

    axisY->setRange(rangeLow, rangeHigh);
    axisY->setTitleText(axisTitle);
    chart->addAxis(axisX, Qt::AlignBottom);
    lineSeries->attachAxis(axisX);
    chart->addAxis(axisY, Qt::AlignLeft);
    axisX->setTickCount(16);
    axisX->setGridLineVisible(false);

    lineSeries->attachAxis(axisY);
    chart->legend()->hide();
    chart->setContentsMargins(0,-15,0,-15);

    chart->layout()->setContentsMargins(0, 0, 0, 0);
    chart->setBackgroundRoundness(0);

    chart->setTheme(QChart::ChartThemeDark);
    QBrush backgroundBrush(QColor(44,46,56));
    chart->setBackgroundBrush(backgroundBrush);



    return chartView;
}


void MainWindow::playSound(){

    QAudioFormat format;
    // Set up the format, eg.
    format.setSampleRate(8000);
    format.setChannelCount(1);
    format.setSampleSize(8);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    float seconds = 0.08;
    int freq = 4000;
    int TG_MAX_VAL = 32;
    QByteArray* buf = new QByteArray();
    buf->resize(seconds * SAMPLE_RATE);

    for (int i=0; i<(seconds * SAMPLE_RATE); i++) {
        qreal t = (qreal)(freq * i);
        t = t * FREQ_CONST;
        t = qSin(t);
        // now we normalize t
        t *= TG_MAX_VAL;
        (*buf)[i] = (quint8)t;
    }

    // Make a QBuffer from our QByteArray
    QBuffer* input = new QBuffer(buf);
    input->open(QIODevice::ReadOnly);

    // Create an output with our premade QAudioFormat (See example in QAudioOutput)
    QAudioOutput* audio = new QAudioOutput(format, this);
    audio->start(input);
}

void MainWindow::verifyAlarms(){
    if (minuteVolumeAlarm == true){
        playSound();
    }
}


MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    expirationFlowSeries(new QLineSeries),
    pressureFlowSeries(new QLineSeries),
    volumeFlowSeries(new QLineSeries)
{
    connectionHandler = new ConnectionHandler();
    deviceHandler = new DeviceHandler();
    deviceFinder = new DeviceFinder(deviceHandler);

    QMediaPlayer* player = new QMediaPlayer;
    player->setMedia(QUrl::fromLocalFile("/Users/William/dev/ventilatorInterface/sounds/beep.mp3"));
    player->setVolume(50);

    QChartView *pressureFlowChartView = createChart(pressureFlowSeries, "Pressure (cmH2O)", -50, 50);
    QChartView *expirationFlowChartView = createChart(expirationFlowSeries, "Exp Flow (L/min)", 0, 100);
    QChartView *volumeFlowChartView = createChart(volumeFlowSeries, "Exp Volume (mL)", 0 , 1000);
    connect(deviceHandler, SIGNAL(newFlowMeasurement(float)), this, SLOT(newFlowMeasurement(float)));

    QGridLayout *mainLayout = new QGridLayout(this);
    
    QGridLayout *parameterLayout = new QGridLayout();

    mainLayout->addLayout(parameterLayout,0,0,3,1);
    
    QFont numberFont( "Arial", 100, QFont::Thin);
    QFont labelFont( "Arial", 18, QFont::Thin);
    QFont alarmLimitsFont( "Arial", 12, QFont::Bold);

    QLabel* peakPressureLabel = new QLabel("0");
    peakPressureLabel->setFont(numberFont);
    peakPressureLabel->setAlignment(Qt::AlignRight);
    QLabel* peakPressureTitleLabel = new QLabel("Ppeak (cmH20)");
    peakPressureTitleLabel->setAlignment(Qt::AlignRight);
    peakPressureTitleLabel->setFont(labelFont);
    peakPressureMinMax = new QLabel("Min: 5.0 Max: 8.0");
    peakPressureMinMax->setAlignment(Qt::AlignRight| Qt::AlignTop);
    peakPressureMinMax->setFont(labelFont);

    respiratoryRateLabel = new QLabel("0");
    respiratoryRateLabel->setFont(numberFont);
    respiratoryRateLabel->setAlignment(Qt::AlignRight);
    QLabel* respiratoryRateLabelTitle = new QLabel("f<sub>R</sub> (b/min)");
    respiratoryRateLabelTitle->setFont(labelFont);
    respiratoryRateLabelTitle->setAlignment(Qt::AlignRight);

    respiratoryRateMinMax = new QLabel("Min: 17 Max: 22");
    respiratoryRateMinMax->setAlignment(Qt::AlignRight| Qt::AlignTop);
    respiratoryRateMinMax->setFont(labelFont);

    minuteVolumeLabel = new QLabel("0.0");
    minuteVolumeLabel->setFont(numberFont);
    minuteVolumeLabel->setAlignment(Qt::AlignRight);
    QLabel* minuteVolumeTitleLabel = new QLabel("V<sub>E</sub> (L/min)");
    minuteVolumeTitleLabel->setAlignment(Qt::AlignRight);
    minuteVolumeTitleLabel->setFont(labelFont);
    minuteVolumeMinMax = new QLabel("Min: 5.0 Max: 8.0");
    minuteVolumeMinMax->setAlignment(Qt::AlignRight| Qt::AlignTop);
    minuteVolumeMinMax->setFont(labelFont);

    tidalVolumeLabel = new QLabel("0");
    tidalVolumeLabel->setFont(numberFont);
    tidalVolumeLabel->setAlignment(Qt::AlignRight);
    QLabel* tidalVolumeTitleLabel = new QLabel("V<sub>Te</sub> (mL)");
    tidalVolumeTitleLabel->setAlignment(Qt::AlignRight);
    tidalVolumeTitleLabel->setFont(labelFont);
    tidalVolumeMinMax = new QLabel("Min: 5.0 Max: 8.0");
    tidalVolumeMinMax->setAlignment(Qt::AlignRight| Qt::AlignTop);
    tidalVolumeMinMax->setFont(labelFont);

    mainLayout->setContentsMargins(30,0,0,0);
    mainLayout->setVerticalSpacing(0);
    mainLayout->setHorizontalSpacing(30);
    mainLayout->addWidget(pressureFlowChartView,    0, 1);
    mainLayout->addWidget(expirationFlowChartView,  1, 1);
    mainLayout->addWidget(volumeFlowChartView,      2, 1);

    QFrame* line = new QFrame();

    line->setStyleSheet("QFrame {color: rgb(100,100,100);}");
    line->setFrameShape(QFrame::HLine);
    parameterLayout->addWidget(peakPressureLabel,        0, 0);
    parameterLayout->addWidget(peakPressureTitleLabel,   1, 0);
    parameterLayout->addWidget(line,                     2, 0);

    parameterLayout->addWidget(respiratoryRateLabel,        3, 0);
    parameterLayout->addWidget(respiratoryRateLabelTitle,   4, 0);
   // parameterLayout->addWidget(respiratoryRateMinMax,       5, 0);

    parameterLayout->addWidget(minuteVolumeLabel,6,0);
    parameterLayout->addWidget(minuteVolumeTitleLabel,7,0);
   // parameterLayout->addWidget(minuteVolumeMinMax,8,0);

    parameterLayout->addWidget(tidalVolumeLabel,9,0);
    parameterLayout->addWidget(tidalVolumeTitleLabel,10,0);
   // parameterLayout->addWidget(tidalVolumeMinMax,11,0);

    QPushButton* alarmConfigButton = new QPushButton("Configure Alarms");

    parameterLayout->addWidget(alarmConfigButton, 12, 0);
    connect(alarmConfigButton, SIGNAL(clicked()), this, SLOT(configureAlarms()));

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(addPoint()));
    timer->start(30);


    QTimer *alarmTimer = new QTimer(this);
    connect(alarmTimer, SIGNAL(timeout()), this, SLOT(verifyAlarms()));


    alarmTimer->start(2000);

}
void MainWindow::configureAlarms(){

    configureAlarmDialog configAlarmDialog;

    if(configAlarmDialog.exec()){

        minuteVolumesuperiorLimit = configAlarmDialog.getVolumeMinuteMax();

        qDebug() << minuteVolumesuperiorLimit;

        minuteVolumeInferiorLimit = configAlarmDialog.getVolumeMinuteMin();
        qDebug() << minuteVolumeInferiorLimit;

        QString minuteVolumeInferiorLimitText;
        minuteVolumeInferiorLimitText.setNum(minuteVolumeInferiorLimit, 'f', 1);

        QString minuteVolumesuperiorLimitText;
        minuteVolumesuperiorLimitText.setNum(minuteVolumesuperiorLimit, 'f', 1);

        minuteVolumeMinMax->setText(QString("Min: %1 Max: %2").arg(minuteVolumeInferiorLimit).arg(minuteVolumesuperiorLimit));
    }
}


QDateTime previousLoopTime = QDateTime::currentDateTime();

void MainWindow::addPoint(){

    int time = QDateTime::currentDateTime().toMSecsSinceEpoch();

    time = time % 3200;
    float wave;

    // Expiration
    if (time < 1300 ) {
         wave =  70*pow(M_E, -time/280.0) + qrand() % 8;
    }
    // Inspiration
    else {
        wave = 0;
    }

    newFlowMeasurement(wave);

}


void MainWindow::setTidalVolume( float tidalVolume){

    QString text;

    text.setNum(tidalVolume, 'f',0);

    tidalVolumeLabel->setText(text);

}

void MainWindow::setRespiratoryRate(float respiratoryRate){

    QString text;

    text.setNum(respiratoryRate, 'f', 0);

    respiratoryRateLabel->setText(text);
}

void MainWindow::setMinuteVolume(float minuteVolume){

    QString text;

    text.setNum(minuteVolume, 'f', 1);

    if( (minuteVolume > minuteVolumeInferiorLimit)  and (minuteVolume < minuteVolumesuperiorLimit)){
        minuteVolumeLabel->setStyleSheet("QWidget{color: rgb(220,220,220);}");
        minuteVolumeAlarm = false;

    } else {

        minuteVolumeLabel->setStyleSheet("QWidget{color: rgb(255,0,0);}");
        minuteVolumeAlarm = true;
    }
    minuteVolumeLabel->setText(text);

}

void MainWindow::newFlowMeasurement( float flow_LPM)
{
        if (inspirationValid == true){

            // If flow is under the threshold
            if (flow_LPM < tidalThresholdLow){

               // Accumulate
               underThresholdAccumulation +=  1;
            }

            // If 2 flows are below 5 LPM
            if (underThresholdAccumulation > 10){

                    underThresholdAccumulation = 0;
                    tidalVolume = volume;

                    setTidalVolume(tidalVolume);
                    volume = 0;

                    inspirationValid = false;
                }
        }

        if (flow_LPM > tidalThresholdHigh ) {
            if(inspirationValid == false){

                qint64 millisecondsDiff = previousTime.msecsTo(QDateTime::currentDateTime());
                respiratoryRate = 60.0*1000.0/millisecondsDiff;
                qDebug() << millisecondsDiff;
                previousTime = QDateTime::currentDateTime();

                setMinuteVolume(respiratoryRate*tidalVolume/1000.0);
                setRespiratoryRate(respiratoryRate);
            }
            inspirationValid = true;
        }

        float second_per_minute = 60;
        volume = volume + 1000*flow_LPM/(second_per_minute*sampleRate);

        if (volume < 0 ) {
            volume = 0;
        }

        addFlow(flow_LPM);
        addVolume(volume);
}


qint64 MainWindow::addFlow(float flow_LPM)
{

    //qDebug("test2");

    if (flowBuffer.isEmpty()) {
        flowBuffer.reserve(sampleCount);
        for (int i = 0; i < sampleCount; ++i)
            flowBuffer.append(QPointF(i/sampleRate, 0));
    }

    int start = sampleCount-1;
    for (int s = 0; s < start; ++s){
        flowBuffer[s].setY(flowBuffer.at(s + 1).y());

    }

    for (int s = start; s < sampleCount; ++s){
        flowBuffer[s].setY(flow_LPM);
    }

    expirationFlowSeries->replace(flowBuffer);

    return (sampleCount);
}


qint64 MainWindow::addVolume(float volume_mL)
{
    if (volumeBuffer.isEmpty()) {
        volumeBuffer.reserve(sampleCount);
        for (int i = 0; i < sampleCount; ++i)
            volumeBuffer.append(QPointF(i/sampleRate, 0));
    }

    int start = sampleCount-1;
    for (int s = 0; s < start; ++s){
        volumeBuffer[s].setY(volumeBuffer.at(s + 1).y());
    }


    for (int s = start; s < sampleCount; ++s){
        volumeBuffer[s].setY(volume_mL);
    }

    volumeFlowSeries->replace(volumeBuffer);



    return (sampleCount);
}
