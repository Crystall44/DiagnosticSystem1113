#include "CompletedState.h"

CompletedState::CompletedState() {
}

CompletedState::~CompletedState() {
}

void CompletedState::execute(AppointmentContext* context) {
    std::cout << "ПРИЕМ ЗАВЕРШЕН\n";
    std::cout << "Печать заключения...\n";
    context->printAppointmentInfo();
    std::cout << "\nДокументы сформированы и сохранены в ЭМК.\n";
    std::cout << "Пациент направлен на выполнение рекомендаций.\n";
}

std::string CompletedState::getStateName() const {
    return "Завершен";
}