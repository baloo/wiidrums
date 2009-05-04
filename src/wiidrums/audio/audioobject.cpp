// vim: ts=4 sw=4 expandtab:


#include "audioobject.h"




AudioObject::AudioObject(Phonon::Category cat, MediaSource * source):
media(NULL), audio(NULL), source(NULL), ready(false)
{
    this->cat = cat;
    this->audio = new Phonon::AudioOutput(cat, this);

    this->mutex = new QMutex;
    this->mutex->unlock();
    if (source != NULL) {
        this->source = source;
        // On spawn une source
        this->newSource();
    }
}

AudioObject::~AudioObject()
{
    if (this->media != NULL) {
        delete this->media;
        this->media = NULL;
    }
    delete this->audio;
    this->audio = NULL;
    delete this->mutex;
    this->mutex = NULL;
}

void AudioObject::newSource()
{
    this->media = new MediaObject;

    qDebug() << "AudioObject::newSource()" << "called";

    //On connect le changement d'etat pour bloquer l'objet jusqu'a sa
    //disponibilité totale
    QObject::connect(this->media,
                     SIGNAL(stateChanged(Phonon::State, Phonon::State)),
                     this,
                     SLOT(mediaStateChanged(Phonon::State, Phonon::State)));

    QObject::connect(this->media, SIGNAL(finished()), this, SIGNAL(finished()));
    //On va maintenant attendre que le media source soit pret
    this->mutex->lock();


    //On set le source
    this->media->setCurrentSource(*(this->source));

    createPath(this->media, this->audio);



    //On va maintenant attendre le signal du mediaSource qui
    //va nous dire qu'il est pret

    //Ceci va donc bloquer jusqu'a ce qu'il soit pret
    this->mutex->lock();

    //On est arrivé ici, le mediaSource est donc utilisable :)

    //On oublie pas de le débloquer :)
    this->mutex->unlock();


    this->ready = true;


}

void AudioObject::mediaStateChanged(Phonon::State newState,
                                    Phonon::State oldState)
{
    qDebug() << "AudioObject::mediaStateChanged()" << "called";
    //On debloque le mutex !
    qDebug() << "AudioObject::mediaStateChanged()" << "states " <<
        oldState << ":" << newState;
    if (oldState == Phonon::LoadingState && newState == Phonon::StoppedState) {
        if (this->mutex != NULL)
            this->mutex->unlock();
        qDebug() << "AudioObject::mediaStateChanged()" << "mutex unlocked";
    }
}

Phonon::State AudioObject::state()
{
    if (this->media != NULL)
        return this->media->state();
    else
        return Phonon::ErrorState;
}

void AudioObject::play()
{
    qDebug() << "AudioObject::play()" << "called";
    if (this->ready == false) {
        // TODO: throw new Exception
        // envoyer une exception heritant de QtConcurrent::Exception
    }
    if (this->media != NULL)
        this->media->play();
}

void AudioObject::setCurrentSource(const MediaSource * source)
{
    this->source = source;
    this->newSource();
}
