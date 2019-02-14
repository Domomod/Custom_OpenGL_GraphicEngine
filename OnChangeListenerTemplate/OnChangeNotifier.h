//
// Created by dominik on 09.02.19.
//

#ifndef OBSERVER_ONCHANGENOTIFIER_H
#define OBSERVER_ONCHANGENOTIFIER_H

#include <vector>
#include <algorithm>

#include "OnChangeListener.h"

template <class T>
class OnChangeListener;

template <class T>
class OnChangeNotifier {
private:
    std::vector<OnChangeListener<T>*> listenersList;
    OnChangeNotifier(const OnChangeNotifier<T>&) = delete;
    OnChangeNotifier& operator=(const OnChangeNotifier<T>& onChangeNotifier);

public:
    OnChangeNotifier(){}
    virtual ~OnChangeNotifier(){}

    void addListener(OnChangeListener<T>* listener){
        if(std::find(listenersList.begin(), listenersList.end(), listener) == listenersList.end()){
            listenersList.push_back(listener);
        }
        else {
            //listener already added in the past
            return;
        }
    }

    void removeListener(OnChangeListener<T>* listener) {
        auto toBeErased = std::find(listenersList.begin(), listenersList.end(), listener);
        if( toBeErased != listenersList.end()){
            listenersList.erase(toBeErased, toBeErased+1);
        } else {
            //This listener is not listening to us.
            return;
        }
    }

    void notifyListeners(T x){
        for(auto listener : listenersList){
            listener->React(x);
        }
    }
};


#endif //OBSERVER_ONCHANGENOTIFIER_H
