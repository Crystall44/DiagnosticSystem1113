#include "ExaminationState.h"

ExaminationState::ExaminationState()
    : examinationsDone(0) {
}

ExaminationState::~ExaminationState() {
}

void ExaminationState::execute(AppointmentContext* context) {
    std::cout << "ОСМОТР ПАЦИЕНТА\n";
    std::cout << "Проведение объективного осмотра...\n";

    // Собираем данные осмотра
    context->addExaminationData("Температура", "38.5°C");
    context->addExaminationData("Давление", "130/85");
    context->addExaminationData("Пульс", "92 уд/мин");
    context->addExaminationData("Горло", "гиперемировано");
    context->addExaminationData("Лимфоузлы", "увеличены");

    context->printExaminationData();

    examinationsDone++;
    std::cout << "Осмотр завершен\n";

    // Переходим к диагностике
    if (getNextState()) {
        std::cout << "-> Переход к диагностике\n";
        context->setCurrentState(getNextState());
    }
}

std::string ExaminationState::getStateName() const {
    return "Осмотр";
}