#include "mainWindow.h"
#include "configurealarmdialog.h"
#include "chart.h"

#include <QGraphicsLayout>
#include <QtWidgets/QVBoxLayout>
#include <QDebug>
#include <QLabel>
#include <QTimer>
#include <QTime>
#include <QtMath>
#include <QPushButton>
#include <QtMath>
#include <QLineEdit>
#include <QInputDialog>

#include <QBuffer>
#include <QAudioOutput>


#define SAMPLE_RATE 44000
#define FREQ_CONST ((2.0 * M_PI) / SAMPLE_RATE)

QT_CHARTS_USE_NAMESPACE

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
    if (minuteVolumeAlarm == true || respiratoryRateAlarm == true){
        playSound();
    }

}


MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent)
{
    // Start bluetooth connexion
    connectionHandler = new ConnectionHandler();
    deviceHandler = new DeviceHandler();
    deviceFinder = new DeviceFinder(deviceHandler);

    // Create plots
    pressureFlowChart = new Chart("Pressure (cmH2O)", sampleCount, sampleRate, -50, 50);
    expirationFlowChart = new Chart("Exp Flow (L/min)", sampleCount, sampleRate, 0, 100);
    volumeChart = new Chart("Exp Volume (mL)", sampleCount, sampleRate, 0, 1000);

    // Connect the bluetooth messages to the flow measurement calculation
    connect(deviceHandler, SIGNAL(newFlowMeasurement(float)), this, SLOT(newFlowMeasurement(float)));

    // Create the graph layout
    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->setContentsMargins(30,0,0,0);
    mainLayout->setVerticalSpacing(0);
    mainLayout->setHorizontalSpacing(30);

    // Create the parameter layout
    QGridLayout *parameterLayout = new QGridLayout();
    mainLayout->addLayout(parameterLayout,0,0,3,1);

    // Create the fonts for the labels
    QFont numberFont( "Arial", 100, QFont::Thin);
    QFont labelFont( "Arial", 18, QFont::Thin);
    QFont alarmLimitsFont( "Arial", 12, QFont::Bold);

    // Create the peak pressure display
    QLabel* peakPressureLabel = new QLabel("0");
    peakPressureLabel->setFont(numberFont);
    peakPressureLabel->setAlignment(Qt::AlignRight);
    QLabel* peakPressureTitleLabel = new QLabel("Ppeak (cmH20)");
    peakPressureTitleLabel->setAlignment(Qt::AlignRight);
    peakPressureTitleLabel->setFont(labelFont);
    peakPressureMinMax = new QLabel("Min: 5.0 Max: 8.0");
    peakPressureMinMax->setAlignment(Qt::AlignRight| Qt::AlignTop);
    peakPressureMinMax->setFont(labelFont);

    // Create the respiratory rate label
    respiratoryRateLabel = new QLabel("0");
    respiratoryRateLabel->setFont(numberFont);
    respiratoryRateLabel->setAlignment(Qt::AlignRight);
    QLabel* respiratoryRateLabelTitle = new QLabel("f<sub>R</sub> (b/min)");
    respiratoryRateLabelTitle->setFont(labelFont);
    respiratoryRateLabelTitle->setAlignment(Qt::AlignRight);

    // Create the respiratory rate min max
    respiratoryRateMinMax = new QLabel("Min: 17 Max: 22");
    respiratoryRateMinMax->setAlignment(Qt::AlignRight| Qt::AlignTop);
    respiratoryRateMinMax->setFont(labelFont);

    // Create the minute volume label
    minuteVolumeLabel = new QLabel("0.0");
    minuteVolumeLabel->setFont(numberFont);
    minuteVolumeLabel->setAlignment(Qt::AlignRight);
    QLabel* minuteVolumeTitleLabel = new QLabel("V<sub>E</sub> (L/min)");
    minuteVolumeTitleLabel->setAlignment(Qt::AlignRight);
    minuteVolumeTitleLabel->setFont(labelFont);
    minuteVolumeMinMax = new QLabel("Min: 5.0 Max: 8.0");
    minuteVolumeMinMax->setAlignment(Qt::AlignRight| Qt::AlignTop);
    minuteVolumeMinMax->setFont(labelFont);

    // Create the tidal volume label
    tidalVolumeLabel = new QLabel("0");
    tidalVolumeLabel->setFont(numberFont);
    tidalVolumeLabel->setAlignment(Qt::AlignRight);
    QLabel* tidalVolumeTitleLabel = new QLabel("V<sub>Te</sub> (mL)");
    tidalVolumeTitleLabel->setAlignment(Qt::AlignRight);
    tidalVolumeTitleLabel->setFont(labelFont);
    tidalVolumeMinMax = new QLabel("Min: 5.0 Max: 8.0");
    tidalVolumeMinMax->setAlignment(Qt::AlignRight| Qt::AlignTop);
    tidalVolumeMinMax->setFont(labelFont);

    // Create the tidal volume label
    mainLayout->addWidget(pressureFlowChart->getChartView(),        0, 1);
    mainLayout->addWidget(expirationFlowChart->getChartView(),  1, 1);
    mainLayout->addWidget(volumeChart->getChartView(),      2, 1);

    // Create the alarm config button
    QPushButton* alarmConfigButton = new QPushButton("Configure Alarms");

    // Place the parameters in the parameter layout
    // Peak pressure
    parameterLayout->addWidget(peakPressureLabel,        0, 0);
    parameterLayout->addWidget(peakPressureTitleLabel,   1, 0);

    // Respiratory rate
    parameterLayout->addWidget(respiratoryRateLabel,        3, 0);
    parameterLayout->addWidget(respiratoryRateLabelTitle,   4, 0);

    // Minute volume
    parameterLayout->addWidget(minuteVolumeLabel,6,0);
    parameterLayout->addWidget(minuteVolumeTitleLabel,7,0);

    // Tidal volume
    parameterLayout->addWidget(tidalVolumeLabel,9,0);
    parameterLayout->addWidget(tidalVolumeTitleLabel,10,0);

    // Alarm config button
    parameterLayout->addWidget(alarmConfigButton, 12, 0);
    connect(alarmConfigButton, SIGNAL(clicked()), this, SLOT(configureAlarms()));

    // UNCOMMENT TO START SIMULATED WAVEFORMS
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(addPoint()));
    timer->start(30);

    // Alarm timer
    QTimer *alarmTimer = new QTimer(this);
    connect(alarmTimer, SIGNAL(timeout()), this, SLOT(verifyAlarms()));
    alarmTimer->start(2000);
}

void MainWindow::configureAlarms(){

    configureAlarmDialog configAlarmDialog;

    configAlarmDialog.setVolumeMinuteMax(minuteVolumeSuperiorLimit);
    configAlarmDialog.setVolumeMinuteMin(minuteVolumeInferiorLimit);

    configAlarmDialog.setRespiratoryRateMax(respiratoryRateSuperiorLimit);
    configAlarmDialog.setRespiratoryRateMin(respiratoryRateInferiorLimit);

    if(configAlarmDialog.exec()){

        // Get minute volume
        minuteVolumeSuperiorLimit = configAlarmDialog.getVolumeMinuteMax();
        minuteVolumeInferiorLimit = configAlarmDialog.getVolumeMinuteMin();
        QString minuteVolumeInferiorLimitText;
        minuteVolumeInferiorLimitText.setNum(minuteVolumeInferiorLimit, 'f', 1);
        QString minuteVolumesuperiorLimitText;
        minuteVolumesuperiorLimitText.setNum(minuteVolumeSuperiorLimit, 'f', 1);
        minuteVolumeMinMax->setText(QString("Min: %1 Max: %2").arg(minuteVolumeInferiorLimit).arg(minuteVolumeSuperiorLimit));

        // RespiratoryRate
        respiratoryRateSuperiorLimit = configAlarmDialog.getRespiratoryRateMax();
        respiratoryRateInferiorLimit = configAlarmDialog.getRespiratoryRateMin();
        QString respiratoryRateInferiorLimitText;
        respiratoryRateInferiorLimitText.setNum(respiratoryRateInferiorLimit, 'f', 1);
        QString respiratoryRateSuperiorLimitText;
        respiratoryRateSuperiorLimitText.setNum(respiratoryRateSuperiorLimit, 'f', 1);
        respiratoryRateMinMax->setText(QString("Min: %1 Max: %2").arg(respiratoryRateInferiorLimit).arg(respiratoryRateSuperiorLimit));
    }
}



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


    if( (respiratoryRate > respiratoryRateInferiorLimit)  and (respiratoryRate < respiratoryRateSuperiorLimit)){
        respiratoryRateLabel->setStyleSheet("QWidget{color: rgb(220,220,220);}");
        respiratoryRateAlarm = false;

    } else {

        respiratoryRateLabel->setStyleSheet("QWidget{color: rgb(255,0,0);}");
        respiratoryRateAlarm = true;
    }

    respiratoryRateLabel->setText(text);

}

void MainWindow::setMinuteVolume(float minuteVolume){

    QString text;

    text.setNum(minuteVolume, 'f', 1);

    if( (minuteVolume > minuteVolumeInferiorLimit)  and (minuteVolume < minuteVolumeSuperiorLimit)){
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

        expirationFlowChart->addPoint(flow_LPM);
        volumeChart->addPoint(volume);
}
