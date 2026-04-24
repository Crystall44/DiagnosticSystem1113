#include "DiagnosisState.h"

DiagnosisState::DiagnosisState()
    : diagnosticProxy(nullptr),
    currentAppointment(nullptr),
    diagnosesMade(0) {
}

DiagnosisState::~DiagnosisState() {
}

void DiagnosisState::setDiagnosticProxy(DiagnosticCacheProxy* proxy) {
    diagnosticProxy = proxy;
}

void DiagnosisState::setCurrentAppointment(Appointment* appointment) {
    currentAppointment = appointment;
}

void DiagnosisState::execute(AppointmentContext* context) {
    std::cout << "ПОСТАНОВКА ДИАГНОЗА\n";
    std::cout << "Анализ симптомов...\n";

    if (diagnosticProxy && currentAppointment) {
        // Используем диагностическую систему
        std::unique_ptr<DiagnosisResult> result =
            diagnosticProxy->diagnose(currentAppointment);

        if (result) {
            std::cout << "Диагностика выполнена успешно\n";

            // Сохраняем результат в контексте
            context->setDiagnosis(std::move(result));
            context->setDiagnosisComplete(true);

            // Выводим результаты
            DiagnosisResult* diagResult = context->getDiagnosis();
            if (diagResult && !diagResult->getCandidates().empty()) {
                std::vector<DiagnosisCandidate> topCandidates =
                    diagResult->getTopCandidates(3);

                std::cout << "Наиболее вероятные диагнозы:\n";
                for (size_t i = 0; i < topCandidates.size(); ++i) {
                    std::cout << (i + 1) << ". " << topCandidates[i].getDisease()->getName() << " (" << (topCandidates[i].getProbability() * 100) << "%)\n";
                }
            }
        }
        else {
            std::cout << "Ошибка: диагностика не выполнена\n";
        }
    }
    else {
        std::cout << "Ошибка: диагностическая система не настроена\n";
    }

    diagnosesMade++;

    // Переходим к рекомендациям
    if (getNextState()) {
        std::cout << "-> Переход к рекомендациям\n";
        context->setCurrentState(getNextState());
    }
}

std::string DiagnosisState::getStateName() const {
    return "Диагностика";
}