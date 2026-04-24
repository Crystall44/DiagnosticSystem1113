#pragma once
#include "AppointmentState.h"
#include "AppointmentContext.h"

// Состояние: Выдача рекомендаций
class RecommendationState : public AppointmentState {
private:
    int recommendationsGiven;  // счетчик выданных рекомендаций

public:
    RecommendationState();
    virtual ~RecommendationState();

    virtual void execute(AppointmentContext* context) override;
    virtual std::string getStateName() const override;
};