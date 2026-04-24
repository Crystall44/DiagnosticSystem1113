#pragma once
#include "AppointmentState.h"
#include "AppointmentContext.h"

// Состояние: Осмотр пациента
class ExaminationState : public AppointmentState {
private:
    int examinationsDone;  // счетчик проведенных осмотров

public:
    ExaminationState();
    virtual ~ExaminationState();

    virtual void execute(AppointmentContext* context) override;
    virtual std::string getStateName() const override;
};