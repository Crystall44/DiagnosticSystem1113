#pragma once
#include <string>
#include <vector>
#include <memory>
#include "ISubject.h"
#include "DiagnosisResult.h"

// Субъект наблюдения — мониторит состояние пациента
class PatientMonitor : public ISubject {
private:
    std::string patientName; // имя пациента
    std::string currentDiagnosis; // текущий диагноз
    float currentProbability; // текущая вероятность
    std::vector<std::string> criticalConditions; // критические состояния
    int appointmentCount; // счетчик приемов
    bool isActive; // активен ли монитор

public:
    PatientMonitor(const std::string& patientName);

    // Установить новый диагноз
    void setDiagnosis(const std::string& diagnosis, float probability);

    // Получить текущие данные
    std::string getPatientName() const;
    std::string getCurrentDiagnosis() const;
    float getCurrentProbability() const;
    int getAppointmentCount() const;

    // Обнаружить критическое состояние
    void detectCriticalCondition(const std::string& condition);

    // Завершить прием
    void completeAppointment(const std::string& finalDiagnosis);

    // Добавить критическое состояние в историю
    void addCriticalCondition(const std::string& condition);

    // Вывести информацию о мониторе
    void printMonitorInfo() const;

    // Оповещения (реализация ISubject)
    void notifyDiagnosisChanged(
        const std::string& patientName,
        const std::string& oldDiagnosis,
        const std::string& newDiagnosis,
        float newProbability
    ) override;

    void notifyCriticalCondition(
        const std::string& patientName,
        const std::string& condition
    ) override;

    void notifyAppointmentCompleted(
        const std::string& patientName,
        const std::string& finalDiagnosis
    ) override;
};