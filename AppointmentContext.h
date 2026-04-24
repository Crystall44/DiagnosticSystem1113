#pragma once
#include <memory>
#include <string>
#include <vector>
#include <map>
#include "AppointmentState.h"
#include "DiagnosisResult.h"

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
};