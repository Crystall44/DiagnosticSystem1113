#pragma once
#include "AppointmentState.h"
#include "AppointmentContext.h"
#include "DiagnosticCacheProxy.h"

// Состояние: Постановка диагноза
class DiagnosisState : public AppointmentState {
private:
    int diagnosesMade;                      // счетчик поставленных диагнозов

public:
    DiagnosisState();
    virtual ~DiagnosisState();

    virtual void execute(AppointmentContext* context) override;
    virtual std::string getStateName() const override;
};