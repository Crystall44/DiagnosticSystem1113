#pragma once
#include "IObserver.h"
#include <string>
#include <vector>

// Наблюдатель: Система тревог для критических состояний
class AlertSystem : public IObserver {
private:
    std::vector<std::string> alertHistory; // история тревог
    bool emergencyMode; // режим ЧС
    int alertCount; // счетчик тревог

public:
    AlertSystem();

    void onDiagnosisChanged(
        const std::string& patientName,
        const std::string& oldDiagnosis,
        const std::string& newDiagnosis,
        float newProbability
    ) override;

    void onCriticalCondition(
        const std::string& patientName,
        const std::string& condition
    ) override;

    void onAppointmentCompleted(
        const std::string& patientName,
        const std::string& finalDiagnosis
    ) override;

    std::string getObserverName() const override;

    // Специфичные методы
    void activateEmergencyMode();
    void deactivateEmergencyMode();
    bool isEmergencyMode() const;
    int getAlertCount() const;
    void printAlertHistory() const;
};