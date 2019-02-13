//
// Created by dominik on 09.02.19.
//

#ifndef OBSERVER_ONCHANGELISTENER_H
#define OBSERVER_ONCHANGELISTENER_H


//#include "OnChangeNotifier.h"

template <class T>
class OnChangeNotifier;

template <class T>
class OnChangeListener {
public:
    OnChangeListener(){}
    virtual ~OnChangeListener(){}

    virtual void react(OnChangeNotifier<T>* Notify, T obj) = 0;

private:
    OnChangeListener(const OnChangeListener<T>&) = delete;
    OnChangeListener& operator=(const OnChangeListener<T>& onChangeListener);
};


#endif //OBSERVER_ONCHANGELISTENER_H
