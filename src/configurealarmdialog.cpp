#include "configurealarmdialog.h"
#include <QGridLayout>
#include <QSpacerItem>
#include <QDebug>
#include <QDialogButtonBox>

configureAlarmDialog::configureAlarmDialog()
{
   QGridLayout* gridLayout = new QGridLayout();
   QSpacerItem* spacer1 = new QSpacerItem(1,1,QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
   QFont labelFontBold( "Arial", 32, QFont::Bold);
   QFont labelFont( "Arial", 24, QFont::Thin);

   volumeMinuteLabel = new QLabel("Minute Volume alarm (L/Min)");
   volumeMinuteLabel->setFont(labelFontBold);
   volumeMinuteMaxLabel = new QLabel("Max:");
   volumeMinuteMaxLabel->setFont(labelFont);
   volumeMinuteMinLabel = new QLabel("Min:");
   volumeMinuteMinLabel->setFont(labelFont);
   volumeMinuteMinLineEdit = new QLineEdit();
   volumeMinuteMinLineEdit->setFont(labelFont);
   volumeMinuteMinLineEdit->setInputMethodHints(Qt::ImhFormattedNumbersOnly);

   volumeMinuteMaxLineEdit = new QLineEdit();
   volumeMinuteMaxLineEdit->setFont(labelFont);
   volumeMinuteMaxLineEdit->setInputMethodHints(Qt::ImhFormattedNumbersOnly);

   respiratoryRateLabel = new QLabel("Respiratory Rate (Breaths/minute)");
   respiratoryRateLabel->setFont(labelFontBold);
   respiratoryRateLabel->setInputMethodHints(Qt::ImhFormattedNumbersOnly);

   respiratoryRateMaxLabel = new QLabel("Max:");
   respiratoryRateMaxLabel->setFont(labelFont);
   respiratoryRateMaxLabel->setInputMethodHints(Qt::ImhFormattedNumbersOnly);

   respiratoryRateMinLabel = new QLabel("Min:");
   respiratoryRateMinLabel->setFont(labelFont);
   respiratoryRateMaxLabel->setInputMethodHints(Qt::ImhFormattedNumbersOnly);

   respiratoryRateMinLineEdit = new QLineEdit();
   respiratoryRateMinLineEdit->setFont(labelFont);
   respiratoryRateMinLineEdit->setInputMethodHints(Qt::ImhFormattedNumbersOnly);

   respiratoryRateMaxLineEdit = new QLineEdit();
   respiratoryRateMaxLineEdit->setFont(labelFont);
   respiratoryRateMaxLineEdit->setInputMethodHints(Qt::ImhFormattedNumbersOnly);

   QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
   buttonBox->buttons()[0]->setFont(labelFont);
   buttonBox->buttons()[1]->setFont(labelFont);
   this->setLayout(gridLayout);

   gridLayout->addWidget(volumeMinuteLabel,          0, 0, 1, 2);
   gridLayout->addWidget(volumeMinuteMinLabel,       1, 0);
   gridLayout->addWidget(volumeMinuteMinLineEdit,    1, 1);
   gridLayout->addWidget(volumeMinuteMaxLabel,       1, 2);
   gridLayout->addWidget(volumeMinuteMaxLineEdit,    1, 3);
   gridLayout->addWidget(respiratoryRateLabel   ,    2, 0, 1, 2);
   gridLayout->addWidget(respiratoryRateMinLabel,    3, 0);
   gridLayout->addWidget(respiratoryRateMinLineEdit, 3, 1);
   gridLayout->addWidget(respiratoryRateMaxLabel,    3, 2);
   gridLayout->addWidget(respiratoryRateMaxLineEdit, 3, 3);
   gridLayout->addWidget(buttonBox,                  4, 3, 1, 3);

   gridLayout->addItem(spacer1, 5,4);
   spacer1->changeSize(0,0, QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

   connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
   connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

void configureAlarmDialog::setVolumeMinuteMax(float volumeMinuteMax) {

    QString text;
    text.setNum(volumeMinuteMax, 'f', 1);
    volumeMinuteMaxLineEdit->setText(text);
}

void configureAlarmDialog::setVolumeMinuteMin(float volumeMinuteMin) {

    QString text;
    text.setNum(volumeMinuteMin, 'f', 1);
    volumeMinuteMinLineEdit->setText(text);
}

float configureAlarmDialog::getVolumeMinuteMax() {

    qDebug() << volumeMinuteMaxLineEdit->text();
    return volumeMinuteMaxLineEdit->text().toFloat();
}

float configureAlarmDialog::getVolumeMinuteMin() {

    return volumeMinuteMinLineEdit->text().toFloat();
}

void configureAlarmDialog::setRespiratoryRateMax(float respiratoryRateMax) {

    QString text;
    text.setNum(respiratoryRateMax, 'f', 1);
    respiratoryRateMaxLineEdit->setText(text);
}

void configureAlarmDialog::setRespiratoryRateMin(float respiratoryRateMin) {

    QString text;
    text.setNum(respiratoryRateMin, 'f', 1);
    respiratoryRateMinLineEdit->setText(text);
}

float configureAlarmDialog::getRespiratoryRateMax() {

    qDebug() << respiratoryRateMaxLineEdit->text();
    return respiratoryRateMaxLineEdit->text().toFloat();
}

float configureAlarmDialog::getRespiratoryRateMin() {

    return respiratoryRateMinLineEdit->text().toFloat();
}
