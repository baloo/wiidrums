// vim: ts=4 sw=4 expandtab:

#ifndef DEF_AUDIO_OBJECT
#define DEF_AUDIO_OBJECT

#include <Phonon/AudioOutput>
#include <Phonon/MediaObject>
#include <QList>
#include <QObject>
#include <QMutex>

#include <QDebug>

using namespace Phonon;

class AudioObject:public QObject {
  Q_OBJECT public:
     AudioObject(Phonon::Category cat = Phonon::MusicCategory,
                 MediaSource * source = NULL);
    ~AudioObject();


    void setCurrentSource(const MediaSource * source);

     Phonon::State state();

    public slots:void play();
    void mediaStateChanged(Phonon::State newState, Phonon::State oldState);

     signals:void played();
    void finished();

  protected:
    void newSource();

     Phonon::Category cat;

    MediaObject *media;
    AudioOutput *audio;

    bool ready;

    //Mutex de d'attente du mediaSource
    QMutex *mutex;

    const MediaSource *source;
};

#endif
