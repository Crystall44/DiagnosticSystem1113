#include "AppointmentContext.h"
#include <iostream>

AppointmentContext::AppointmentContext(const std::string& name, int age)
    : currentState(nullptr),
    patientName(name),
    patientAge(age),
    diagnosticProxy(nullptr),
    currentAppointment(nullptr),
    diagnosticReady(false),
    diagnosisComplete(false) {
}

void AppointmentContext::setCurrentState(AppointmentState* state) {
    currentState = state;
}

AppointmentState* AppointmentContext::getCurrentState() const {
    return currentState;
}

void AppointmentContext::run() {
    if (currentState) {
        std::cout << "\n[Состояние: " << currentState->getStateName() << "]\n";
        currentState->execute(this);
    }
}

std::string AppointmentContext::getPatientName() const {
    return patientName;
}

int AppointmentContext::getPatientAge() const {
    return patientAge;
}

void AppointmentContext::addComplaint(const std::string& complaint) {
    complaints.push_back(complaint);
}

std::vector<std::string> AppointmentContext::getComplaints() const {
    return complaints;
}

void AppointmentContext::clearComplaints() {
    complaints.clear();
}

void AppointmentContext::addExaminationData(const std::string& key, const std::string& value) {
    examinationData[key] = value;
}

std::string AppointmentContext::getExaminationData(const std::string& key) const {
    std::map<std::string, std::string>::const_iterator it = examinationData.find(key);
    if (it != examinationData.end()) {
        return it->second;
    }
    return "";
}

void AppointmentContext::printExaminationData() const {
    std::cout << "Данные осмотра:\n";
    for (std::map<std::string, std::string>::const_iterator it = examinationData.begin();
        it != examinationData.end(); ++it) {
        std::cout << it->first << ": " << it->second << "\n";
    }
}

void AppointmentContext::setDiagnosis(std::unique_ptr<DiagnosisResult> result) {
    diagnosis = std::move(result);
}

DiagnosisResult* AppointmentContext::getDiagnosis() const {
    return diagnosis.get();
}

bool AppointmentContext::isDiagnosisComplete() const {
    return diagnosisComplete;
}

void AppointmentContext::setDiagnosisComplete(bool complete) {
    diagnosisComplete = complete;
}

void AppointmentContext::printAppointmentInfo() const {
    std::cout << "\n=== Информация о приеме ===\n";
    std::cout << "Пациент: " << patientName << ", " << patientAge << " лет\n";

    std::cout << "Жалобы: ";
    for (size_t i = 0; i < complaints.size(); ++i) {
        std::cout << complaints[i];
        if (i < complaints.size() - 1) std::cout << ", ";
    }
    std::cout << "\n";

    printExaminationData();

    if (diagnosis && !diagnosis->getCandidates().empty()) {
        std::cout << "Диагноз: " << diagnosis->getBestCandidate().getDisease()->getName() << " (вероятность: " << (diagnosis->getBestCandidate().getProbability() * 100) << "%)\n";
    }

    std::cout << "Текущее состояние: " << (currentState ? currentState->getStateName() : "не установлено") << "\n";
    std::cout << "===========================\n";
}

void AppointmentContext::createStandardWorkflow() {
    // Создаем все состояния внутри контекста
    registrationState = std::make_unique<RegistrationState>();
    examinationState = std::make_unique<ExaminationState>();
    diagnosisState = std::make_unique<DiagnosisState>();
    recommendationState = std::make_unique<RecommendationState>();
    completedState = std::make_unique<CompletedState>();

    // Связываем в цепочку
    registrationState->setNextState(examinationState.get());
    examinationState->setNextState(diagnosisState.get());
    diagnosisState->setNextState(recommendationState.get());
    recommendationState->setNextState(completedState.get());
    completedState->setNextState(nullptr);

    // Устанавливаем начальное состояние
    currentState = registrationState.get();

    std::cout << "[AppointmentContext] Цепочка состояний создана: ";
    std::cout << "Регистрация -> Осмотр -> Диагностика -> Рекомендации -> Завершение\n";
}

void AppointmentContext::cleanupWorkflow() {
    // unique_ptr автоматически удалят состояния при разрушении контекста
    // Обнуляем текущее состояние
    currentState = nullptr;

    // Обнуляем владеющие указатели в порядке, обратном созданию
    completedState.reset();
    recommendationState.reset();
    diagnosisState.reset();
    examinationState.reset();
    registrationState.reset();

    std::cout << "[AppointmentContext] Цепочка состояний удалена\n";
}

void AppointmentContext::setupDiagnostics(DiagnosticCacheProxy* proxy, Appointment* appointment) {
    diagnosticProxy = proxy;
    currentAppointment = appointment;
    diagnosticReady = true;
    std::cout << "[AppointmentContext] Диагностика настроена\n";
}

DiagnosticCacheProxy* AppointmentContext::getDiagnosticProxy() const {
    return diagnosticProxy;
}

Appointment* AppointmentContext::getCurrentAppointment() const {
    return currentAppointment;
}

bool AppointmentContext::isDiagnosticReady() const {
    return diagnosticReady;
}
