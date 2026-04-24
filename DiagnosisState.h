#pragma once
#include "AppointmentState.h"
#include "AppointmentContext.h"
#include "DiagnosticCacheProxy.h"

// —осто€ние: ѕостановка диагноза
class DiagnosisState : public AppointmentState {
private:
    DiagnosticCacheProxy* diagnosticProxy;  // указатель на прокси (не владеем)
    Appointment* currentAppointment;        // текущий прием дл€ диагностики
    int diagnosesMade;                      // счетчик поставленных диагнозов

public:
    DiagnosisState();
    virtual ~DiagnosisState();

    // ”становить диагностическую систему
    void setDiagnosticProxy(DiagnosticCacheProxy* proxy);

    // ”становить текущий прием
    void setCurrentAppointment(Appointment* appointment);

    virtual void execute(AppointmentContext* context) override;
    virtual std::string getStateName() const override;
};