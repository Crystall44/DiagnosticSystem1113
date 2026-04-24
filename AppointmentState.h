#pragma once
#include <iostream>
#include <string>
class AppointmentContext;

// Ѕазовый класс состо€ни€ приема
class AppointmentState {
protected:
    AppointmentState* nextState;  // следующее состо€ние

public:
    AppointmentState();
    virtual ~AppointmentState();

    // ”становить следующее состо€ние
    void setNextState(AppointmentState* state);

    // ѕолучить следующее состо€ние
    AppointmentState* getNextState() const;

    // ¬ыполнить действие, соответствующее состо€нию
    virtual void execute(AppointmentContext* context) = 0;

    // ѕолучить название состо€ни€
    virtual std::string getStateName() const = 0;
};