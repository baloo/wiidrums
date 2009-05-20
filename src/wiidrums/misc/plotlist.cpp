#include "plotlist.h"

template <class T>
plotList<T>::plotList(int size)
{
    this->size = size;
}

template <class T>
void plotList<T>::push(const T & item)
{
    this->pop_back();
    this->push_front(item);
}

