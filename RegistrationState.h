#pragma once
#include "AppointmentState.h"
#include "AppointmentContext.h"

// Состояние: Регистрация пациента
class RegistrationState : public AppointmentState {
private:
    int registeredCount;  // счетчик зарегистрированных на этом шаге

public:
    RegistrationState();
    virtual ~RegistrationState();

    virtual void execute(AppointmentContext* context) override;
    virtual std::string getStateName() const override;
};