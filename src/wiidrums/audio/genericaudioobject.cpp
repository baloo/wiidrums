// vim: ts=4 sw=4 expandtab:
#include "audio/genericaudioobject.h"

GenericAudioObject::GenericAudioObject(Phonon::Category cat,
                                       int minInstances,
                                       int maxInstances):cat(cat),
source(NULL), currentPlaying(0)
{
    this->minInstances = minInstances;
    this->maxInstances = maxInstances;

    this->audios = new QList < AudioObject * >;

    for (int i = 0; i < maxInstances; i++) {
        newInstance();
    }
}

GenericAudioObject::~GenericAudioObject()
{
    delete audios;
    this->audios = NULL;
}

//Définition de la source
void GenericAudioObject::setCurrentSource(const MediaSource & source)
{
    this->source = &source;

    //On définit le même source pour tous les médias
    AudioObject *item;
    foreach(item, *audios)
        item->setCurrentSource(this->source);
}

//Création d'une nouvelle instance de MediaObject
AudioObject *GenericAudioObject::newInstance()
{
    AudioObject *aud = new AudioObject;
    qDebug() << "GenericAudioObject::newInstance() : newInstance !";


    if (this->source != NULL) {
        qDebug() << "GenericAudioObject::newInstance() : source setting ... !";
        //aud->setCurrentSource(audios->first()->currentSource());
        qDebug() << "GenericAudioObject::newInstance() : source setted !";
    }

    connect(aud, SIGNAL(finished()), this, SLOT(finished()));


    audios->append(aud);
    emit audioInstanced();

    return aud;
}

//On supprime une instance de MediaObject
bool GenericAudioObject::deleteInstance()
{
    AudioObject *item;
    qDebug() << "GenericAudioObject::deleteInstance() : " << "called";
    foreach(item, *(this->audios)) {
        if (item->state() == Phonon::StoppedState) {
            emit audioDeleted();
            qDebug() << "GenericAudioObject::deleteInstance() : " <<
                "One available instance found ... deleting";
            this->audios->removeOne(item);
            return true;
        }
    }
    qDebug() << "GenericAudioObject::deleteInstance() : " << "none found";
    return false;
}

//Lorsqu'un MediaObject à terminé sa lecture
void GenericAudioObject::finished()
{
    this->currentPlaying--;
    qDebug() << "GenericAudioObject::finished() : " <<
        "currentPlaying = " << this->currentPlaying <<
        "instances = " << this->audios->size();

    if (this->currentPlaying < this->audios->size() - maxInstances) {
        qDebug() << "GenericAudioObject::finished() : " <<
            "calling deleteInstance()";
        this->deleteInstance();
    }
}

//On lance la lecture du premier MediaObject disponible
void GenericAudioObject::play()
{

    AudioObject *aud;
    bool hasPlayed = false;

    qDebug() << "GenericAudioObject::play() : Let's Play !!";

    //On vérifie l'état des MediaObject et on lance le premier MediaObject en StoppedState
    AudioObject *item;
    foreach(item, *(this->audios)) {
        if (item->state() == Phonon::StoppedState) {
            qDebug() <<
                "GenericAudioObject::play() : Available instance found !";
            item->play();
            qDebug() << "GenericAudioObject::play() : Played !!";
            currentPlaying++;
            qDebug() << "GenericAudioObject::play() : " <<
                "currentPlaying = " << currentPlaying <<
                "instances = " << this->audios->size();
            if (currentPlaying > this->audios->size() - minInstances) {
                qDebug() << "GenericAudioObject::play() : newInstance !!";
                newInstance();
            }
            hasPlayed = true;
            break;
        }
    }

    //Si on a pas pu lancer de lecture
    if (!hasPlayed) {
        aud = newInstance();
        aud->play();
        currentPlaying++;
        if (this->currentPlaying > this->audios->size() - minInstances)
            newInstance();
    }

}
