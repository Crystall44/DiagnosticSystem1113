#pragma once
#include "IDiagnosticEngineBuilder.h"
#include <memory>

#include "DiagnosticEngine.h";
#include "DiagnosticCacheProxy.h";
#include "DiseaseDirectory.h";

// Строитель для базового ФАП (фельдшер-стажёр)
class BasicFAPBuilder : public IDiagnosticEngineBuilder {
private:
    std::unique_ptr<DiagnosticEngine> engine;   // собираемый движок
    std::unique_ptr<DiagnosticCacheProxy> proxy; // собираемый прокси
    bool useProxy;                               // флаг использования прокси
    float threshold;                             // порог для SimpleDiagnostic

public:
    explicit BasicFAPBuilder(DiseaseDirectory* dir,
        float threshold = 0.3f);

    void buildAlgorithm() override;
    void buildCheckers() override;
    void buildProxy() override;

    std::unique_ptr<DiagnosticEngine> getEngine() override;
    std::unique_ptr<DiagnosticCacheProxy> getProxy() override;
    bool hasProxy() const override;

    // Дополнительные методы настройки
    void setThreshold(float t);
};