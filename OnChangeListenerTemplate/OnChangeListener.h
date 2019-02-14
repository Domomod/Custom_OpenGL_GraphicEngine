//
// Created by dominik on 09.02.19.
//

#ifndef OBSERVER_ONCHANGELISTENER_H
#define OBSERVER_ONCHANGELISTENER_H

#include <functional>

//#include "OnChangeNotifier.h"

template <class T>
class OnChangeNotifier;

template <class T>
class OnChangeListener {
    friend class OnChangeNotifier<T>;
public:
    OnChangeListener(){}
    virtual ~OnChangeListener(){}

    void setReactionFuncPtr(std::function<void(T obj)> _React){
        React = _React;
    }

private:
    std::function<void(T obj)> React;

    OnChangeListener(const OnChangeListener<T>&) = delete;
    OnChangeListener& operator=(const OnChangeListener<T>& onChangeListener);
};


#endif //OBSERVER_ONCHANGELISTENER_H
