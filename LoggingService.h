#pragma once
#include "IObserver.h"
#include <string>
#include <vector>
#include <ctime>

// Наблюдатель: Служба журналирования всех событий
class LoggingService : public IObserver {
private:
    std::vector<std::string> logs;  // журнал событий
    int logEntryCount;              // количество записей

public:
    LoggingService();

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

    // Методы для работы с логами
    void addLog(const std::string& message);
    void printLogs() const;
    void printLastNLogs(int n) const;
    int getLogCount() const;
    void clearLogs();

private:
    std::string getCurrentTimeString() const;
};