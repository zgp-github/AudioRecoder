#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QAudioFormat>
#include <QtWidgets/QWidget>
#include <QtCharts/QChartGlobal>
#include <QMainWindow>
#include <QComboBox>
#include <QFile>


QT_CHARTS_BEGIN_NAMESPACE
class QLineSeries;
class QChart;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class XYSeriesIODevice;

QT_BEGIN_NAMESPACE
class QAudioInput;
class QAudioDeviceInfo;
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void init_ui();
    void init_data();
    void init_signal();
    QChart *m_chart;
    XYSeriesIODevice *m_device = nullptr;
    QLineSeries *m_series ;
    QAudioInput *m_audioInput = nullptr;
    QAudioInput *m_audioInput2 = nullptr;

private:
    QAudioFormat formatAudio;
    QComboBox *combo_samplerate;
    QComboBox *combo_channel;
    QComboBox *combo_samplesize;

private:
    bool recording = false;
    QByteArray *rawdata;
    QFile *filename;

private slots:
    void start_record();
    void stop_record();
    void set_samplerate(QString data);
    void set_channel(QString data);
    void set_samplesize(QString data);
};

#endif // MAINWINDOW_H
