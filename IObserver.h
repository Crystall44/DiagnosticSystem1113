#pragma once
#include <string>

// Интерфейс наблюдателя
class IObserver {
public:
    virtual ~IObserver() = default;

    // Реакция на изменение диагноза
    virtual void onDiagnosisChanged(
        const std::string& patientName,
        const std::string& oldDiagnosis,
        const std::string& newDiagnosis,
        float newProbability
    ) = 0;

    // Реакция на критическое состояние
    virtual void onCriticalCondition(
        const std::string& patientName,
        const std::string& condition
    ) = 0;

    // Реакция на завершение приема
    virtual void onAppointmentCompleted(
        const std::string& patientName,
        const std::string& finalDiagnosis
    ) = 0;

    // Получить имя наблюдателя
    virtual std::string getObserverName() const = 0;
};