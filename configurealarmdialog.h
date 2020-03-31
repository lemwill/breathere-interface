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
    ~configureAlarmDialog();

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
};

#endif // CONFIGUREALARMDIALOG_H
