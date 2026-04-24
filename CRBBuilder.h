#pragma once
#include "IDiagnosticEngineBuilder.h"
#include <memory>
#include <string>
#include <map>

// Строитель для консультации в ЦРБ
class CRBBuilder : public IDiagnosticEngineBuilder {
private:
    std::unique_ptr<DiagnosticEngine> engine;
    std::unique_ptr<DiagnosticCacheProxy> proxy;
    bool useProxy;
    std::string season;
    size_t maxCacheSize;
    float defaultPrior;  // априорная вероятность по умолчанию
    std::map<std::string, float> priorProbabilities;  // априорные вероятности

public:
    explicit CRBBuilder(DiseaseDirectory* dir,
        const std::string& season = "winter",
        size_t cacheSize = 100);

    void buildAlgorithm() override;
    void buildCheckers() override;
    void buildProxy() override;

    std::unique_ptr<DiagnosticEngine> getEngine() override;
    std::unique_ptr<DiagnosticCacheProxy> getProxy() override;
    bool hasProxy() const override;

    // Методы настройки байесовского алгоритма
    void setDefaultPrior(float prior);
    void setPriorProbability(const std::string& diseaseName, float prob);
    void setSeason(const std::string& s);
    void setMaxCacheSize(size_t size);
};