#ifndef WAVE_H
#define WAVE_H

#include <QString>
#include <QAudioFormat>
#include <QByteArray>

typedef unsigned short WORD;
typedef unsigned long DWORD;

class wave
{
public:
    wave();
    ~wave();

public:
    void saveWave(const QString &fileName, const QByteArray &raw, const QAudioFormat &format);
};

#endif // WAVE_H
