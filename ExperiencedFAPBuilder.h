#pragma once
#include "IDiagnosticEngineBuilder.h"
#include <memory>
#include <string>

// Строитель для опытного фельдшера
class ExperiencedFAPBuilder : public IDiagnosticEngineBuilder {
private:
    std::unique_ptr<DiagnosticEngine> engine;
    std::unique_ptr<DiagnosticCacheProxy> proxy;
    bool useProxy;
    bool normalize;        // флаг нормализации в WeightedDiagnostic
    std::string season;    // текущий сезон для EpidemiologicalChecker
    size_t maxCacheSize;   // максимальный размер кэша

public:
    explicit ExperiencedFAPBuilder(DiseaseDirectory* dir,
        const std::string& season = "winter",
        size_t cacheSize = 100,
        bool normalize = true);

    void buildAlgorithm() override;
    void buildCheckers() override;
    void buildProxy() override;

    std::unique_ptr<DiagnosticEngine> getEngine() override;
    std::unique_ptr<DiagnosticCacheProxy> getProxy() override;
    bool hasProxy() const override;

    // Методы настройки
    void setSeason(const std::string& s);
    void setMaxCacheSize(size_t size);
};