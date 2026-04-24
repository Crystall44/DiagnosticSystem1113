#include "StatisticService.h"
#include <iostream>
#include <algorithm>

StatisticService::StatisticService()
    : totalDiagnoses(0) {
    std::cout << "[StatisticService] Служба статистики инициализирована\n";
}

void StatisticService::onDiagnosisChanged(
    const std::string& patientName,
    const std::string& oldDiagnosis,
    const std::string& newDiagnosis,
    float newProbability
) {
    std::cout << "[StatisticService] Регистрация диагноза: "<< newDiagnosis << " (" << patientName << ")\n";

    // Обновляем статистику
    diagnosisStatistics[newDiagnosis]++;
    totalDiagnoses++;

    // Добавляем в историю
    std::string record = patientName + ": " + newDiagnosis + " (" + std::to_string(static_cast<int>(newProbability * 100)) + "%)";
    diagnosisHistory.push_back(record);
}

void StatisticService::onCriticalCondition(
    const std::string& patientName,
    const std::string& condition
) {
    std::cout << "[StatisticService] Зафиксировано критическое состояние: " << condition << " (пациент: " << patientName << ")\n";
}

void StatisticService::onAppointmentCompleted(
    const std::string& patientName,
    const std::string& finalDiagnosis
) {
    std::cout << "[StatisticService] Прием завершен. Статистика обновлена.\n";
}

std::string StatisticService::getObserverName() const {
    return "StatisticService";
}

void StatisticService::printStatistics() const {
    std::cout << "\n=== Статистика диагнозов ===\n";
    std::cout << "Всего диагнозов: " << totalDiagnoses << "\n\n";

    // Сортируем по количеству (для вывода топа)
    std::vector<std::pair<std::string, int>> sortedStats;
    for (std::map<std::string, int>::const_iterator it = diagnosisStatistics.begin();
        it != diagnosisStatistics.end(); ++it) {
        sortedStats.push_back(std::make_pair(it->first, it->second));
    }

    // Сортировка по убыванию
    for (size_t i = 0; i < sortedStats.size(); ++i) {
        for (size_t j = i + 1; j < sortedStats.size(); ++j) {
            if (sortedStats[j].second > sortedStats[i].second) {
                std::swap(sortedStats[i], sortedStats[j]);
            }
        }
    }

    std::cout << "Распределение диагнозов:\n";
    for (size_t i = 0; i < sortedStats.size(); ++i) {
        std::cout << "  " << sortedStats[i].first << ": "
            << sortedStats[i].second << " случаев\n";
    }
    std::cout << "===========================\n";
}

int StatisticService::getDiagnosisCount(const std::string& diagnosis) const {
    std::map<std::string, int>::const_iterator it = diagnosisStatistics.find(diagnosis);
    if (it != diagnosisStatistics.end()) {
        return it->second;
    }
    return 0;
}

std::string StatisticService::getMostCommonDiagnosis() const {
    std::string mostCommon;
    int maxCount = 0;

    for (std::map<std::string, int>::const_iterator it = diagnosisStatistics.begin();
        it != diagnosisStatistics.end(); ++it) {
        if (it->second > maxCount) {
            maxCount = it->second;
            mostCommon = it->first;
        }
    }

    return mostCommon;
}

int StatisticService::getTotalDiagnoses() const {
    return totalDiagnoses;
}