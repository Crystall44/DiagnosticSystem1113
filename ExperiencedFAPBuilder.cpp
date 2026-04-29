#include "ExperiencedFAPBuilder.h"
#include "DiagnosticEngine.h"
#include "DiagnosticCacheProxy.h"
#include "WeightedDiagnostic.h"
#include "CriticalSymptomChecker.h"
#include "EpidemiologicalChecker.h"
#include "DiseaseDirectory.h"

ExperiencedFAPBuilder::ExperiencedFAPBuilder(
    DiseaseDirectory* dir,
    const std::string& season,
    size_t cacheSize,
    bool normalize)
    : IDiagnosticEngineBuilder(dir),
    useProxy(true),
    normalize(normalize),  // из параметра конструктора
    season(season),
    maxCacheSize(cacheSize) {
}

void ExperiencedFAPBuilder::buildAlgorithm() {
    // Взвешенный алгоритм с нормализацией
    auto algorithm = std::make_unique<WeightedDiagnostic>(normalize);

    engine = std::make_unique<DiagnosticEngine>(
        std::move(algorithm),
        directory
        );
}

void ExperiencedFAPBuilder::buildCheckers() {
    if (!engine) return;

    // Проверка критических симптомов
    engine->addChecker(std::make_unique<CriticalSymptomChecker>());

    // Эпидемиологическая проверка с настройкой сезона
    auto epiChecker = std::make_unique<EpidemiologicalChecker>();
    epiChecker->setSeason(season);
    engine->addChecker(std::move(epiChecker));
}

void ExperiencedFAPBuilder::buildProxy() {
    if (!engine || !useProxy) {
        useProxy = false;
        proxy = nullptr;
        return;
    }

    // Оборачиваем в кэширующий прокси
    proxy = std::make_unique<DiagnosticCacheProxy>(
        std::move(engine),
        maxCacheSize
        );
    engine = nullptr;  // движок теперь внутри прокси
}

std::unique_ptr<DiagnosticEngine> ExperiencedFAPBuilder::getEngine() {
    return std::move(engine);
}

std::unique_ptr<DiagnosticCacheProxy> ExperiencedFAPBuilder::getProxy() {
    return std::move(proxy);
}

bool ExperiencedFAPBuilder::hasProxy() const {
    return useProxy;
}

void ExperiencedFAPBuilder::setSeason(const std::string& s) {
    season = s;
}

void ExperiencedFAPBuilder::setMaxCacheSize(size_t size) {
    maxCacheSize = size;
}