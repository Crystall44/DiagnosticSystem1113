#include "AlertSystem.h"
#include <iostream>

AlertSystem::AlertSystem()
    : emergencyMode(false),
    alertCount(0) {
    std::cout << "[AlertSystem] Система тревог инициализирована\n";
}

void AlertSystem::onDiagnosisChanged(
    const std::string& patientName,
    const std::string& oldDiagnosis,
    const std::string& newDiagnosis,
    float newProbability
) {
    std::cout << "[AlertSystem] Анализ изменения диагноза пациента "
        << patientName << "\n";

    // Если диагноз изменился на более серьезный — тревога
    if (newDiagnosis == "Пневмония" || newDiagnosis == "Инфаркт") {
        std::string alert = "ТРЕВОГА! Пациент " + patientName + ": серьезный диагноз — " + newDiagnosis;
        alertHistory.push_back(alert);
        alertCount++;

        std::cout << "[AlertSystem] " << alert << "\n";
        activateEmergencyMode();
    }
}

void AlertSystem::onCriticalCondition(
    const std::string& patientName,
    const std::string& condition
) {
    std::string alert = "КРИТИЧЕСКОЕ СОСТОЯНИЕ! Пациент " + patientName + ": " + condition;
    alertHistory.push_back(alert);
    alertCount++;

    std::cout << "[AlertSystem] " << alert << "\n";
    std::cout << "[AlertSystem] Немедленно оповестить дежурного врача!\n";
    activateEmergencyMode();
}

void AlertSystem::onAppointmentCompleted(
    const std::string& patientName,
    const std::string& finalDiagnosis
) {
    std::cout << "[AlertSystem] Прием пациента " << patientName << " завершен. Деактивация наблюдения.\n";
    deactivateEmergencyMode();
}

std::string AlertSystem::getObserverName() const {
    return "AlertSystem";
}

void AlertSystem::activateEmergencyMode() {
    emergencyMode = true;
    std::cout << "[AlertSystem] Активирован режим ЧС!\n";
}

void AlertSystem::deactivateEmergencyMode() {
    emergencyMode = false;
    std::cout << "[AlertSystem] Режим ЧС деактивирован\n";
}

bool AlertSystem::isEmergencyMode() const {
    return emergencyMode;
}

int AlertSystem::getAlertCount() const {
    return alertCount;
}

void AlertSystem::printAlertHistory() const {
    std::cout << "\n=== История тревог ===\n";
    std::cout << "Всего тревог: " << alertCount << "\n";
    for (size_t i = 0; i < alertHistory.size(); ++i) {
        std::cout << "  " << (i + 1) << ". " << alertHistory[i] << "\n";
    }
    std::cout << "=======================\n";
}