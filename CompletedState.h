#pragma once
#include "AppointmentState.h"
#include "AppointmentContext.h"

// Состояние: Прием завершен
class CompletedState : public AppointmentState {
public:
    CompletedState();
    virtual ~CompletedState();

    virtual void execute(AppointmentContext* context) override;
    virtual std::string getStateName() const override;
};