// vim: ts=4 sw=4 expandtab:

#ifndef DEF_GENERIC_MEDIA_OBJECT
#define DEF_GENERIC_MEDIA_OBJECT

#include <Phonon/AudioOutput>
#include <Phonon/MediaObject>
#include <QList>
#include <QObject>

#include <QDebug>

using namespace Phonon;

class GenericMediaObject : public QObject
{
  Q_OBJECT

  public:
    GenericMediaObject(int minInstances = 1, int maxInstances = 3);
    ~GenericMediaObject();

    void setCurrentSource(const MediaSource & source);
    void setAudioOutput(AudioOutput * output);

  public slots:
    void play();
    void finished();

  signals:
    void mediaInstanced();
    void mediaDeleted();

  protected:
    AudioOutput *audioOutput;

    MediaObject* newInstance();
    bool deleteInstance();

    int minInstances;
    int maxInstances;
    int currentPlaying;

    QList<MediaObject*> * medias;
    const MediaSource *source;
};

#endif
