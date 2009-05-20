#ifndef PLOTLIST_H
#define PLOTLIST_H

#include <QList>


template <class T>
class plotList: public QList<T>
{
private:
    int size;
public:
    plotList(int size);

    void push(const T & item);
};

#endif // PLOTLIST_H
