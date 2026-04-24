#include "RegistrationState.h"

RegistrationState::RegistrationState()
    : registeredCount(0) {
}

RegistrationState::~RegistrationState() {
}

void RegistrationState::execute(AppointmentContext* context) {
    std::cout << "РЕГИСТРАЦИЯ ПАЦИЕНТА\n";
    std::cout << "Пациент: " << context->getPatientName() << "\n";
    std::cout << "Возраст: " << context->getPatientAge() << " лет\n";

    // Сбор жалоб
    std::cout << "Сбор жалоб пациента...\n";
    context->addComplaint("температура");
    context->addComplaint("кашель");
    context->addComplaint("слабость");

    std::cout << "Зарегистрировано жалоб: " << context->getComplaints().size() << "\n";

    registeredCount++;

    // Переходим к следующему состоянию
    if (getNextState()) {
        std::cout << "-> Переход к осмотру\n";
        context->setCurrentState(getNextState());
    }
}

std::string RegistrationState::getStateName() const {
    return "Регистрация";
}