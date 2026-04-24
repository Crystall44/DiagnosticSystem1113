#pragma once
#include "IObserver.h"
#include <string>
#include <map>
#include <vector>

// Наблюдатель: Сбор статистики по диагнозам
class StatisticService : public IObserver {
private:
    // Ключ: название болезни, значение: количество случаев
    std::map<std::string, int> diagnosisStatistics;

    // История всех диагнозов
    std::vector<std::string> diagnosisHistory;

    int totalDiagnoses;  // общее количество диагнозов

public:
    StatisticService();

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

    // Статистические методы
    void printStatistics() const;
    int getDiagnosisCount(const std::string& diagnosis) const;
    std::string getMostCommonDiagnosis() const;
    int getTotalDiagnoses() const;
};