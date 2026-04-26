#pragma once
#include <memory>
#include <string>
#include <vector>
#include <map>
#include "AppointmentState.h"
#include "RegistrationState.h"
#include "ExaminationState.h"
#include "DiagnosisState.h"
#include "RecommendationState.h"
#include "CompletedState.h"
#include "DiagnosisResult.h"

class DiagnosticCacheProxy;
class Appointment;

// Контекст приема пациента
class AppointmentContext {
private:
    AppointmentState* currentState;     // текущее состояние
    std::string patientName;            // имя пациента
    int patientAge;                     // возраст
    std::vector<std::string> complaints; // жалобы
    std::map<std::string, std::string> examinationData; // данные осмотра
    std::unique_ptr<DiagnosisResult> diagnosis; // результат диагностики
    bool diagnosisComplete;             // флаг завершения диагностики
    DiagnosticCacheProxy* diagnosticProxy;   // диагностический движок
    Appointment* currentAppointment;          // текущий приём
    bool diagnosticReady;                     // флаг готовности диагностики

public:
    AppointmentContext(const std::string& name, int age);

    // Управление состоянием
    void setCurrentState(AppointmentState* state);
    AppointmentState* getCurrentState() const;
    void run();  // выполнить текущее состояние

    // Данные пациента
    std::string getPatientName() const;
    int getPatientAge() const;

    // Жалобы
    void addComplaint(const std::string& complaint);
    std::vector<std::string> getComplaints() const;
    void clearComplaints();

    // Данные осмотра
    void addExaminationData(const std::string& key, const std::string& value);
    std::string getExaminationData(const std::string& key) const;
    void printExaminationData() const;

    // Диагноз
    void setDiagnosis(std::unique_ptr<DiagnosisResult> result);
    DiagnosisResult* getDiagnosis() const;
    bool isDiagnosisComplete() const;
    void setDiagnosisComplete(bool complete);

    // Печать информации о приеме
    void printAppointmentInfo() const;

    // Создать стандартную цепочку состояний внутри контекста
    void setupDiagnostics(DiagnosticCacheProxy* proxy, Appointment* appointment);
    void createStandardWorkflow();

    // Освободить память состояний
    void cleanupWorkflow();

    DiagnosticCacheProxy* getDiagnosticProxy() const;
    Appointment* getCurrentAppointment() const;
    bool isDiagnosticReady() const;

private:
    // Владеющие указатели на состояния (чтобы автоматически удалялись)
    std::unique_ptr<AppointmentState> registrationState;
    std::unique_ptr<AppointmentState> examinationState;
    std::unique_ptr<AppointmentState> diagnosisState;
    std::unique_ptr<AppointmentState> recommendationState;
    std::unique_ptr<AppointmentState> completedState;
};