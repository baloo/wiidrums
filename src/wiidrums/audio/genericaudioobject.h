// vim: ts=4 sw=4 expandtab:

#ifndef DEF_GENERIC_AUDIO_OBJECT
#define DEF_GENERIC_AUDIO_OBJECT

#include <Phonon/AudioOutput>
#include <Phonon/MediaObject>
#include <QList>
#include <QObject>

#include <QDebug>

#include "audio/audioobject.h"

using namespace Phonon;

class GenericAudioObject:public QObject {
  Q_OBJECT public:
     GenericAudioObject(Phonon::Category cat, int minInstances =
                        1, int maxInstances = 3);
    ~GenericAudioObject();

    void setCurrentSource(const MediaSource & source);

    public slots:void play();
    void finished();

     signals:void audioInstanced();
    void audioDeleted();

  protected:
     Phonon::Category cat;

    AudioObject *newInstance();
    bool deleteInstance();

    int minInstances;
    int maxInstances;
    int currentPlaying;

     QList < AudioObject * >*audios;
    const MediaSource *source;
};

#endif
