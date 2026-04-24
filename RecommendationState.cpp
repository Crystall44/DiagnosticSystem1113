#include "RecommendationState.h"

RecommendationState::RecommendationState()
    : recommendationsGiven(0) {
}

RecommendationState::~RecommendationState() {
}

void RecommendationState::execute(AppointmentContext* context) {
    std::cout << "ВЫДАЧА РЕКОМЕНДАЦИЙ\n";
    std::cout << "Формирование рекомендаций на основе диагноза...\n";

    DiagnosisResult* diagnosis = context->getDiagnosis();

    if (diagnosis && !diagnosis->getCandidates().empty()) {
        DiagnosisCandidate bestDiagnosis = diagnosis->getBestCandidate();
        std::string diseaseName = bestDiagnosis.getDisease()->getName();

        std::cout << "Диагноз: " << diseaseName << "\n";
        std::cout << "Рекомендации:\n";

        // Общие рекомендации
        std::cout << "1. Постельный режим\n";
        std::cout << "2. Обильное питье\n";

        // Специфические рекомендации в зависимости от диагноза
        if (diseaseName == "Грипп" || diseaseName == "ОРВИ") {
            std::cout << "3. Противовирусные препараты\n";
            std::cout << "4. Жаропонижающие при температуре >38°C\n";
            std::cout << "5. Повторный прием через 3 дня\n";
        }
        else if (diseaseName == "Ангина") {
            std::cout << "3. Антибиотики по назначению врача\n";
            std::cout << "4. Полоскание горла\n";
        }
        else if (diseaseName == "Пневмония") {
            std::cout << "3. СРОЧНАЯ ГОСПИТАЛИЗАЦИЯ!\n";
            std::cout << "4. Рентгенография грудной клетки\n";
            std::cout << "5. Антибиотикотерапия в стационаре\n";
        }
        else {
            std::cout << "3. Консультация узкого специалиста\n";
            std::cout << "4. Дополнительные исследования\n";
        }
    }
    else {
        std::cout << "Диагноз не установлен. Требуется дополнительное обследование.\n";
    }

    recommendationsGiven++;

    // Переходим к завершению
    if (getNextState()) {
        std::cout << "-> Завершение приема\n";
        context->setCurrentState(getNextState());
    }
}

std::string RecommendationState::getStateName() const {
    return "Рекомендации";
}