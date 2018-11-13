#include "mainwindow.h"
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QWidget>
#include <QtCharts/QtCharts>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>
#include <QtCharts/QChart>
#include <QtMultimedia/QAudioDeviceInfo>
#include <QtMultimedia/QAudioInput>
#include "xyseriesiodevice.h"
#include "wave.h"
#include <QtCharts/QLineSeries>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QGridLayout>
#include <QComboBox>
#include <QFont>
#include <QPalette>
#include <QDateTime>
#include <QString>
#include <QMessageBox>
#include <QDebug>
#include <qdebug.h>
#include <QtDebug>

MainWindow::MainWindow(QWidget *parent):
        QWidget(parent),
        m_chart(new QChart),
        m_series(new QLineSeries)
{
    this->setFixedSize(900, 600);
    this->setWindowTitle(tr("录音APP (Created by Evin 2018-11-13)"));

    QChartView *chartView = new QChartView(m_chart);
    chartView->setMinimumSize(800, 600);
    m_chart->addSeries(m_series);
    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(0, XYSeriesIODevice::sampleCount);
    axisX->setLabelFormat("%g");
//    axisX->setTitleText("Samples");
    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(-1, 1);
//    axisY->setTitleText("Audio level");
    m_chart->setAxisX(axisX, m_series);
    m_chart->setAxisY(axisY, m_series);
    m_chart->legend()->hide();
//    m_chart->setTitle("Data from the microphone (" + deviceInfo.deviceName() + ')');

    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->addWidget(chartView, 0, 0, 1, 10);

    QLabel *sample_rate = new QLabel(tr("采样率"));
    mainLayout->addWidget(sample_rate, 1, 0, 1, 1);
    combo_samplerate = new QComboBox;
    combo_samplerate->setEditable(false);
    combo_samplerate->insertItem(0, "8 KHz");
    combo_samplerate->insertItem(1, "11.025 KHz");
    combo_samplerate->insertItem(2, "16 KHz");
    combo_samplerate->insertItem(3, "22.05 KHz");
    combo_samplerate->insertItem(4, "44.1 KHz");
    combo_samplerate->insertItem(5, "48 KHz");
    mainLayout->addWidget(combo_samplerate, 1, 1, 1, 1);
    combo_samplerate->setCurrentIndex(4);
    connect(combo_samplerate, SIGNAL(currentTextChanged(QString)), this, SLOT(set_samplerate(QString)));

    QLabel *audio_channel = new QLabel(tr("通道"));
    mainLayout->addWidget(audio_channel, 2, 0, 1, 1);
    combo_channel = new QComboBox;
    combo_channel->setEditable(false);
    combo_channel->insertItem(0, "1");
    combo_channel->insertItem(1, "2");
    mainLayout->addWidget(combo_channel, 2, 1, 1, 1);
    combo_channel->setCurrentIndex(0);
    connect(combo_channel, SIGNAL(currentTextChanged(QString)), this, SLOT(set_channel(QString)));

    QLabel *samplesize_text = new QLabel(tr("位深"));
    mainLayout->addWidget(samplesize_text, 3, 0, 1, 1);
    combo_samplesize = new QComboBox;
    combo_samplesize->setEditable(false);
    combo_samplesize->insertItem(0, "8");
    combo_samplesize->insertItem(1, "16");
    mainLayout->addWidget(combo_samplesize, 3, 1, 1, 1);
    combo_samplesize->setCurrentIndex(1);
    connect(combo_samplesize, SIGNAL(currentTextChanged(QString)), this, SLOT(set_samplesize(QString)));

    QFont ft;
    ft.setPointSize(15);
    QPushButton *button_start = new QPushButton(tr("开始录音"));
    button_start->setFont(ft);
//    button_start->setStyleSheet("border: 1px solid black; color: black; background: green");
    mainLayout->addWidget(button_start, 1, 4, 3, 2);
    connect(button_start, SIGNAL(clicked()), this, SLOT(start_record()));

    QPushButton *button_stop = new QPushButton(tr("停止录音"));
    button_stop->setFont(ft);
//    button_stop->setStyleSheet("border: 1px solid black; color: black; background: red");
    mainLayout->addWidget(button_stop, 1, 7, 3, 2);
    connect(button_stop, SIGNAL(clicked()), this, SLOT(stop_record()));

    formatAudio.setCodec("audio/pcm");
    formatAudio.setByteOrder(QAudioFormat::LittleEndian);
    formatAudio.setSampleType(QAudioFormat::UnSignedInt);
}

void MainWindow::set_samplerate(QString data)
{
    if (data.compare("8 KHz") == 0){
        formatAudio.setSampleRate(int(8 * 1000));
    }else if(data.compare("11.025 KHz") == 0){
        formatAudio.setSampleRate(int(11.025 * 1000));
    }else if(data.compare("16 KHz") == 0){
        formatAudio.setSampleRate(int(16 * 1000));
    }else if(data.compare("22.05 KHz") == 0){
        formatAudio.setSampleRate(int(22.05 * 1000));
    }else if(data.compare("44.1 KHz") == 0){
        formatAudio.setSampleRate(int(44.1 * 1000));
    }else if(data.compare("48 KHz") == 0){
        formatAudio.setSampleRate(int(48 * 1000));
    }else{
        formatAudio.setSampleRate(int(44.1 * 1000));
    }
    qDebug() << formatAudio;
}

void MainWindow::set_channel(QString data)
{
    if (data.contains("1", Qt::CaseSensitive)){
        formatAudio.setChannelCount(1);
    }else if(data.contains("2", Qt::CaseSensitive)){
        formatAudio.setChannelCount(2);
    }
    qDebug() << formatAudio;
}

void MainWindow::set_samplesize(QString data)
{
    QString samplesize = data;
    if (samplesize.contains("8", Qt::CaseSensitive)){
        formatAudio.setSampleSize(8);
    }else if(samplesize.contains("16", Qt::CaseSensitive)){
        formatAudio.setSampleSize(16);
    }
    qDebug() << formatAudio;
}

void MainWindow::start_record()
{
    if (recording == true){
        QMessageBox::warning(this, "Audio", " Audio Recording is running! ");
        return;
    }

    recording = true;
    formatAudio.setSampleRate(8000);
    formatAudio.setChannelCount(1);
    formatAudio.setSampleSize(8);

    const QAudioDeviceInfo deviceInfo = QAudioDeviceInfo::defaultInputDevice();
    m_audioInput = new QAudioInput(deviceInfo, formatAudio, this);

    m_device = new XYSeriesIODevice(m_series, this);
    m_device->open(QIODevice::WriteOnly);

    m_audioInput->start(m_device);
}

void MainWindow::stop_record()
{
    if (recording == false){
        QMessageBox::warning(this, "Audio", " Audio Recording is stoped! ");
        return;
    }

    recording = false;
    m_audioInput->stop();

    QDateTime local(QDateTime::currentDateTime());
    QString localTime = local.toString("yyyy-MM-dd_hhmmss");
    QString filename = localTime + ".wav";
    qDebug() << filename;

    char *data = nullptr;

//    wave *w = new wave();
//    w->saveWave(filename, data, )

}

MainWindow::~MainWindow()
{
    m_audioInput->stop();
    m_device->close();
}
