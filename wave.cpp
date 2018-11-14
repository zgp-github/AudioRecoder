#include <windef.h>
#include "wave.h"
#include <QString>
#include <QAudioFormat>
#include <QByteArray>
#include <QFile>
#include <QDebug>
#include <qdebug.h>

#define WAVE_FORMAT_PCM WORD(1)

wave::wave()
{

}

wave::~wave()
{

}

void wave::saveWave(const QString &fileName, const QByteArray &raw, const QAudioFormat &format)
{
    typedef struct{
        char riff_fileid[4];//"RIFF" 4Byte
        DWORD riff_fileLen; // 4Byte
        char waveid[4];//"WAVE" 4Byte

        char fmt_chkid[4];//"fmt" 4Byte
        DWORD fmt_chkLen; // 4Byte

        WORD    wFormatTag;        /* format type 2Byte*/
        WORD    nChannels;         /* number of channels (i.e. mono, stereo, etc.) 2Byte*/
        DWORD   nSamplesPerSec;    /* sample rate 4Byte*/
        DWORD   nAvgBytesPerSec;   /* for buffer estimation 4Byte*/
        WORD    nBlockAlign;       /* block size of data 2Byte*/
        WORD    wBitsPerSample; // 2Byte

        char data_chkid[4];//"DATA" 4Byte
        DWORD data_chkLen; // 4Byte
    }WaveHeader;

    WaveHeader wh = {0};
    strcpy(wh.riff_fileid, "RIFF");
    wh.riff_fileLen = DWORD(raw.length() + 32);
    strcpy(wh.waveid, "WAVE");

    strcpy(wh.fmt_chkid, "fmt ");
    wh.fmt_chkLen = 16;

    wh.wFormatTag = WAVE_FORMAT_PCM;
    wh.nChannels = WORD(format.channelCount());
    wh.nSamplesPerSec = DWORD(format.sampleRate());
    wh.wBitsPerSample = WORD(format.sampleSize());
    wh.nBlockAlign = (wh.nChannels*wh.wBitsPerSample)/8;
    wh.nAvgBytesPerSec = wh.nBlockAlign*wh.nSamplesPerSec;

    strcpy(wh.data_chkid, "data");
    wh.data_chkLen = DWORD(raw.length());

    QFile f(fileName);
    f.open(QFile::WriteOnly);
    f.write((char *)&wh, sizeof(wh));
    f.write(raw);
    f.close();
}
