#ifndef CONFIGUREALARMDIALOG_H
#define CONFIGUREALARMDIALOG_H

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class configureAlarmDialog : public QDialog
{
    Q_OBJECT
public:
    configureAlarmDialog();

    QLabel* volumeMinuteMaxLabel;
    QLabel* volumeMinuteMinLabel;
    QLabel* volumeMinuteLabel;
    QLineEdit* volumeMinuteMinLineEdit;
    QLineEdit* volumeMinuteMaxLineEdit;
    QPushButton* okButton;
    QPushButton* cancelButton;

    QLabel* respiratoryRateLabel;
    QLabel* respiratoryRateMaxLabel;
    QLabel* respiratoryRateMinLabel;
    QLineEdit* respiratoryRateMinLineEdit;
    QLineEdit* respiratoryRateMaxLineEdit;


    float getVolumeMinuteMax();
    float getVolumeMinuteMin();
    void setVolumeMinuteMin(float volumeMinuteMax);
    void setVolumeMinuteMax(float volumeMinuteMin);

    float getRespiratoryRateMax();
    float getRespiratoryRateMin();
    void setRespiratoryRateMin(float respiratoryRateMax);
    void setRespiratoryRateMax(float respiratoryRateMin);

};

#endif // CONFIGUREALARMDIALOG_H
