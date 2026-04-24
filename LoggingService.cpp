#include "LoggingService.h"
#include <iostream>
#include <sstream>
#include <iomanip>

LoggingService::LoggingService()
    : logEntryCount(0) {
    std::cout << "[LoggingService] Служба журналирования инициализирована\n";
    addLog("Служба журналирования запущена");
}

void LoggingService::onDiagnosisChanged(
    const std::string& patientName,
    const std::string& oldDiagnosis,
    const std::string& newDiagnosis,
    float newProbability
) {
    std::stringstream ss;
    ss << "ИЗМЕНЕНИЕ ДИАГНОЗА | Пациент: " << patientName
        << " | Было: " << oldDiagnosis
        << " | Стало: " << newDiagnosis
        << " | Вероятность: " << std::fixed << std::setprecision(1)
        << (newProbability * 100) << "%";

    addLog(ss.str());
    std::cout << "[LoggingService] Событие зарегистрировано в журнале\n";
}

void LoggingService::onCriticalCondition(
    const std::string& patientName,
    const std::string& condition
) {
    std::stringstream ss;
    ss << "КРИТИЧЕСКОЕ СОСТОЯНИЕ | Пациент: " << patientName << " | Состояние: " << condition;

    addLog(ss.str());
    std::cout << "[LoggingService] Критическое событие зарегистрировано\n";
}

void LoggingService::onAppointmentCompleted(
    const std::string& patientName,
    const std::string& finalDiagnosis
) {
    std::stringstream ss;
    ss << "ПРИЕМ ЗАВЕРШЕН | Пациент: " << patientName << " | Итоговый диагноз: " << finalDiagnosis;

    addLog(ss.str());
    std::cout << "[LoggingService] Завершение приема зарегистрировано\n";
}

std::string LoggingService::getObserverName() const {
    return "LoggingService";
}

void LoggingService::addLog(const std::string& message) {
    std::stringstream ss;
    ss << "[" << getCurrentTimeString() << "] " << message;
    logs.push_back(ss.str());
    logEntryCount++;
}

void LoggingService::printLogs() const {
    std::cout << "\n=== Журнал событий (всего: " << logEntryCount << ") ===\n";
    for (size_t i = 0; i < logs.size(); ++i) {
        std::cout << logs[i] << "\n";
    }
    std::cout << "========================================\n";
}

void LoggingService::printLastNLogs(int n) const {
    std::cout << "\n=== Последние " << n << " записей журнала ===\n";
    int start = logs.size() - n;
    if (start < 0) start = 0;

    for (size_t i = start; i < logs.size(); ++i) {
        std::cout << logs[i] << "\n";
    }
    std::cout << "========================================\n";
}

int LoggingService::getLogCount() const {
    return logEntryCount;
}

void LoggingService::clearLogs() {
    logs.clear();
    logEntryCount = 0;
}

std::string LoggingService::getCurrentTimeString() const {
    std::time_t now = std::time(nullptr);
    std::tm localTime;
    localtime_s(&localTime, &now);

    std::stringstream ss;
    ss << std::setfill('0')
        << std::setw(2) << localTime.tm_hour << ":"
        << std::setw(2) << localTime.tm_min << ":"
        << std::setw(2) << localTime.tm_sec;

    return ss.str();
}