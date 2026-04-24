#include "PatientMonitor.h"
#include <iostream>

PatientMonitor::PatientMonitor(const std::string& name)
    : patientName(name),
    currentDiagnosis("не установлен"),
    currentProbability(0.0f),
    appointmentCount(0),
    isActive(true) {
    std::cout << "[Монитор] Создан монитор пациента: " << patientName << "\n";
}

void PatientMonitor::setDiagnosis(const std::string& diagnosis, float probability) {
    std::string oldDiagnosis = currentDiagnosis;
    float oldProbability = currentProbability;

    currentDiagnosis = diagnosis;
    currentProbability = probability;
    appointmentCount++;

    // Если диагноз изменился — оповещаем
    if (oldDiagnosis != diagnosis || oldProbability != probability) {
        std::cout << "[Монитор] Диагноз изменен: "
            << oldDiagnosis << " -> " << diagnosis
            << " (" << (probability * 100) << "%)\n";

        notifyDiagnosisChanged(patientName, oldDiagnosis, diagnosis, probability);
    }
}

std::string PatientMonitor::getPatientName() const {
    return patientName;
}

std::string PatientMonitor::getCurrentDiagnosis() const {
    return currentDiagnosis;
}

float PatientMonitor::getCurrentProbability() const {
    return currentProbability;
}

int PatientMonitor::getAppointmentCount() const {
    return appointmentCount;
}

void PatientMonitor::detectCriticalCondition(const std::string& condition) {
    std::cout << "[Монитор] ОБНАРУЖЕНО КРИТИЧЕСКОЕ СОСТОЯНИЕ: " << condition << "\n";
    addCriticalCondition(condition);
    notifyCriticalCondition(patientName, condition);
}

void PatientMonitor::completeAppointment(const std::string& finalDiagnosis) {
    std::cout << "[Монитор] Прием завершен. Итоговый диагноз: " << finalDiagnosis << "\n";
    isActive = false;
    notifyAppointmentCompleted(patientName, finalDiagnosis);
}

void PatientMonitor::addCriticalCondition(const std::string& condition) {
    criticalConditions.push_back(condition);
}

void PatientMonitor::printMonitorInfo() const {
    std::cout << "\n=== Информация монитора ===\n";
    std::cout << "Пациент: " << patientName << "\n";
    std::cout << "Текущий диагноз: " << currentDiagnosis
        << " (" << (currentProbability * 100) << "%)\n";
    std::cout << "Количество приемов: " << appointmentCount << "\n";
    std::cout << "Наблюдателей: " << getObserverCount() << "\n";

    if (!criticalConditions.empty()) {
        std::cout << "Критические состояния:\n";
        for (size_t i = 0; i < criticalConditions.size(); ++i) {
            std::cout << "  - " << criticalConditions[i] << "\n";
        }
    }
    std::cout << "===========================\n";
}

// Реализация оповещений

void PatientMonitor::notifyDiagnosisChanged(
    const std::string& patientName,
    const std::string& oldDiagnosis,
    const std::string& newDiagnosis,
    float newProbability
) {
    std::cout << "[Монитор] Оповещение " << observers.size() << " наблюдателей о смене диагноза\n";

    for (size_t i = 0; i < observers.size(); ++i) {
        observers[i]->onDiagnosisChanged(
            patientName, oldDiagnosis, newDiagnosis, newProbability
        );
    }
}

void PatientMonitor::notifyCriticalCondition(
    const std::string& patientName,
    const std::string& condition
) {
    std::cout << "[Монитор] Оповещение " << observers.size() << " наблюдателей о критическом состоянии\n";

    for (size_t i = 0; i < observers.size(); ++i) {
        observers[i]->onCriticalCondition(patientName, condition);
    }
}

void PatientMonitor::notifyAppointmentCompleted(
    const std::string& patientName,
    const std::string& finalDiagnosis
) {
    std::cout << "[Монитор] Оповещение " << observers.size() << " наблюдателей о завершении приема\n";

    for (size_t i = 0; i < observers.size(); ++i) {
        observers[i]->onAppointmentCompleted(patientName, finalDiagnosis);
    }
}