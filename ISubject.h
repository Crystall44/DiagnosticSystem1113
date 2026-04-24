#pragma once
#include <vector>
#include <algorithm>
#include "IObserver.h"

// Интерфейс субъекта наблюдения
class ISubject {
protected:
    std::vector<IObserver*> observers;  // список наблюдателей

public:
    virtual ~ISubject() = default;

    // Прикрепить наблюдателя
    void attach(IObserver* observer) {
        if (observer) {
            observers.push_back(observer);
        }
    }

    // Открепить наблюдателя
    void detach(IObserver* observer) {
        std::vector<IObserver*>::iterator it =
            std::find(observers.begin(), observers.end(), observer);
        if (it != observers.end()) {
            observers.erase(it);
        }
    }

    // Оповестить всех о смене диагноза
    virtual void notifyDiagnosisChanged( const std::string& patientName, const std::string& oldDiagnosis, const std::string& newDiagnosis, float newProbability
    ) = 0;

    // Оповестить всех о критическом состоянии
    virtual void notifyCriticalCondition(const std::string& patientName, const std::string& condition
    ) = 0;

    // Оповестить всех о завершении приема
    virtual void notifyAppointmentCompleted( const std::string& patientName, const std::string& finalDiagnosis
    ) = 0;

    // Получить количество наблюдателей
    int getObserverCount() const {
        return static_cast<int>(observers.size());
    }
};