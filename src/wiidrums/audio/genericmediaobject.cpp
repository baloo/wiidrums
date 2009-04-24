
#include "audio/genericmediaobject.h"

GenericMediaObject::GenericMediaObject(int minInstances, int maxInstances) {
  this->minInstances = minInstances;
  this->maxInstances = maxInstances;
  this->currentPlaying = 0;

  this->source = NULL;
  this->audioOutput = NULL;

  medias = new QList<MediaObject*>;

  for(int i=0; i<maxInstances; i++){
    newInstance();
  }
}

GenericMediaObject::~GenericMediaObject() {
   delete medias;
}

//Définition de la source
void GenericMediaObject::setCurrentSource(const MediaSource & source){
  
  this->source = &source;

  //On définit le même source pour tous les médias
  MediaObject *item;
  foreach(item, *medias)
    item->setCurrentSource(*(this->source));

}

void GenericMediaObject::setAudioOutput(AudioOutput *output){
  this->audioOutput = output;

  //On définit le même audioOutput pour tous les médias
  MediaObject *item;
  foreach(item, *medias)
    Phonon::createPath(item, audioOutput);
}

//Création d'une nouvelle instance de MediaObject
MediaObject* GenericMediaObject::newInstance(){
  MediaObject * med = new MediaObject;
  qDebug() << "GenericMediaObject::newInstance() : newInstance !";


  if(this->source != NULL)
  {
    qDebug() << "GenericMediaObject::newInstance() : source setting ... !";
    med->setCurrentSource(medias->first()->currentSource());
    qDebug() << "GenericMediaObject::newInstance() : source setted !";
  }
  if(this->audioOutput != NULL){
    qDebug() << "GenericMediaObject::newInstance() : audioOutput setting !";
    Phonon::createPath(med, audioOutput);
    qDebug() << "GenericMediaObject::newInstance() : audioOutput setted !";
  }

  connect(med, SIGNAL(finished()), this, SLOT(finished()));

  // Personne n'y touche avant que ca marche !
  // TODO: faire une boucle d'attente il faut que le status passe en stop
  // voir QWaitCondition, en gros on set un mutex à lock à la creation et on map le signal
  // sur un slot dans cette fonction c'est cette derniere qui va delock et on wait la mutex 
  // se delock
  //
  // Voir aussi http://lists.trolltech.com/qt-interest/2000-10/thread00270-0.html

  medias->append(med);
  emit mediaInstanced();

  return med;
}

//On supprime une instance de MediaObject
bool GenericMediaObject::deleteInstance(){
  MediaObject *item;
  qDebug() << "GenericMediaObject::deleteInstance() : "<<
    "called";
  foreach(item, *medias){
    if(item->state() == Phonon::StoppedState){
      qDebug() << "GenericMediaObject::deleteInstance() : "<<
        "One available instance found ... deleting";
      return medias->removeOne(item);
    }
  }
  qDebug() << "GenericMediaObject::deleteInstance() : "<<
    "none found";
  return false;
}

//Lorsqu'un MediaObject à terminé sa lecture
void GenericMediaObject::finished(){
  currentPlaying--;
  qDebug() << "GenericMediaObject::finished() : "<<
    "currentPlaying = "<< currentPlaying << 
    "instances = "<< medias->size();

  if(currentPlaying < medias->size()-maxInstances){
    qDebug() << "GenericMediaObject::finished() : "<<
      "calling deleteInstance()";
    deleteInstance();
  }
}

//On lance la lecture du premier MediaObject disponible
void GenericMediaObject::play(){

  MediaObject *med;
  bool hasPlayed = false;

  qDebug() << "GenericMediaObject::play() : Let's Play !!";

  //On vérifie l'état des MediaObject et on lance le premier MediaObject en StoppedState
  MediaObject *item;
  foreach(item, *medias){
    if(item->state() == Phonon::StoppedState){
      qDebug() << "GenericMediaObject::play() : Available instance found !";
      item->play();
      qDebug() << "GenericMediaObject::play() : Played !!";
      currentPlaying++;
      qDebug() << "GenericMediaObject::play() : "<<
        "currentPlaying = "<< currentPlaying << 
        "instances = "<< medias->size();
      if(currentPlaying > medias->size()-minInstances){
        qDebug() << "GenericMediaObject::play() : newInstance !!";
        newInstance();
      }
      hasPlayed = true;
      break;
    }
  }

  //Si on a pas pu lancer de lecture
  if(!hasPlayed){
    med = newInstance();
    med->play();
    currentPlaying++;
    if(currentPlaying > medias->size()-minInstances)
      newInstance();
  }

}

