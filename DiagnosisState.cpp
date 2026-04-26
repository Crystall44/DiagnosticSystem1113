#include "DiagnosisState.h"

DiagnosisState::DiagnosisState()
    : diagnosesMade(0) {
}

DiagnosisState::~DiagnosisState() {
}

void DiagnosisState::execute(AppointmentContext* context) {
    std::cout << "ПОСТАНОВКА ДИАГНОЗА\n";
    std::cout << "Анализ симптомов...\n";

    // Берём движок и приём из контекста (а не из своих полей)
    DiagnosticCacheProxy* proxy = context->getDiagnosticProxy();
    Appointment* appointment = context->getCurrentAppointment();

    if (proxy && appointment && context->isDiagnosticReady()) {
        // Используем диагностическую систему
        std::unique_ptr<DiagnosisResult> result = proxy->diagnose(appointment);

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
                    std::cout << (i + 1) << ". "
                        << topCandidates[i].getDisease()->getName()
                        << " (" << (topCandidates[i].getProbability() * 100)
                        << "%)\n";
                }
            }
        }
        else {
            std::cout << "Ошибка: диагностика не выполнена\n";
        }
    }
    else {
        std::cout << "Ошибка: диагностическая система не настроена\n";
        std::cout << "  Вызовите context->setupDiagnostics() перед запуском цепочки\n";
    }

    diagnosesMade++;

    if (getNextState()) {
        std::cout << "-> Переход к рекомендациям\n";
        context->setCurrentState(getNextState());
    }
}

std::string DiagnosisState::getStateName() const {
    return "Диагностика";
}