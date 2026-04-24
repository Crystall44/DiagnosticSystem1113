#include "AppointmentState.h"

AppointmentState::AppointmentState()
    : nextState(nullptr) {
}

AppointmentState::~AppointmentState() {
    // nextState не удаляем, так как не владеем им
}

void AppointmentState::setNextState(AppointmentState* state) {
    nextState = state;
}

AppointmentState* AppointmentState::getNextState() const {
    return nextState;
}