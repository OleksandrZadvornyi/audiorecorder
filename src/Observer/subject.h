#ifndef SUBJECT_H
#define SUBJECT_H

#include "observer.h"
#include <QList>

class Subject
{
public:
    virtual ~Subject() = default;

    virtual void attach(Observer* observer) {
        observers.append(observer);
    }

    virtual void detach(Observer* observer) {
        observers.removeOne(observer);
    }

    virtual void notify() {
        for (Observer* observer : observers) {
            observer->update();
        }
    }

protected:
    QList<Observer*> observers;
};

#endif // SUBJECT_H
