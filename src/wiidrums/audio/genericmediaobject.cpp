
#include "audio/genericmediaobject.h"

GenericMediaObject::GenericMediaObject(int minInstances, int maxInstances) {
  this->minInstances = minInstances;
  this->maxInstances = maxInstances;
  this->currentPlaying = 0;

  this->source = NULL;
  this->audioOutput = NULL;

  medias = new QList<MediaObject*>;

  for(int i=0; i<minInstances; i++){
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
    item->setCurrentSource(source);

}

void GenericMediaObject::setAudioOutput(AudioOutput &output){
  this->audioOutput = &output;

  //On définit le même audioOutput pour tous les médias
  MediaObject *item;
  foreach(item, *medias)
    Phonon::createPath(item, audioOutput);
}

//Création d'une nouvelle instance de MediaObject
MediaObject* GenericMediaObject::newInstance(){
  MediaObject * med = new MediaObject;
  if(source != NULL)
    med->setCurrentSource(*source);
  if(audioOutput != NULL)
    Phonon::createPath(med, audioOutput);

  connect(med, SIGNAL(finished()), this, SLOT(finished()));

  medias->append(med);
  emit mediaInstanced();

  return med;
}

//On supprime une instance de MediaObject
bool GenericMediaObject::deleteInstance(){
  MediaObject *item;
  foreach(item, *medias){
    if(item->state() == Phonon::StoppedState){
      return medias->removeOne(item);
    }
  }
}

//Lorsqu'un MediaObject à terminé sa lecture
void GenericMediaObject::finished(){
  currentPlaying--;

  if(currentPlaying < medias->size()-maxInstances)
    deleteInstance();
}

//On lance la lecture du premier MediaObject disponible
void GenericMediaObject::play(){

  MediaObject *med;
  bool hasPlayed = false;

  //On vérifie l'état des MediaObject et on lance le premier MediaObject en StoppedState
  MediaObject *item;
  foreach(item, *medias){
    if(item->state() == Phonon::StoppedState){
      item->play();
      currentPlaying++;
      if(currentPlaying > medias->size()-minInstances)
        newInstance();
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

