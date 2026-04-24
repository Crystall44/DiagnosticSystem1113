#include "CRBBuilder.h"
#include "DiagnosticEngine.h"
#include "DiagnosticCacheProxy.h"
#include "BayesianDiagnostic.h"
#include "CriticalSymptomChecker.h"
#include "EpidemiologicalChecker.h"
#include "DiseaseDirectory.h"

CRBBuilder::CRBBuilder(
    DiseaseDirectory* dir,
    const std::string& season,
    size_t cacheSize)
    : IDiagnosticEngineBuilder(dir),
    useProxy(true),
    season(season),
    maxCacheSize(cacheSize),
    defaultPrior(0.01f) {

    // Предустановленные априорные вероятности
    priorProbabilities["ОРВИ"] = 0.3f;
    priorProbabilities["Грипп"] = 0.2f;
    priorProbabilities["Ангина"] = 0.15f;
    priorProbabilities["Пневмония"] = 0.05f;
}

void CRBBuilder::buildAlgorithm() {
    // Создаем байесовский алгоритм
    auto algorithm = std::make_unique<BayesianDiagnostic>(defaultPrior);

    // Настраиваем априорные вероятности для каждой болезни
    for (const auto& pair : priorProbabilities) {
        algorithm->setPriorProbability(pair.first, pair.second);
    }

    engine = std::make_unique<DiagnosticEngine>(
        std::move(algorithm),
        directory
        );
}

void CRBBuilder::buildCheckers() {
    if (!engine) return;

    // Полный набор проверок
    engine->addChecker(std::make_unique<CriticalSymptomChecker>());

    auto epiChecker = std::make_unique<EpidemiologicalChecker>();
    epiChecker->setSeason(season);
    engine->addChecker(std::move(epiChecker));
}

void CRBBuilder::buildProxy() {
    if (!engine || !useProxy) {
        useProxy = false;
        proxy = nullptr;
        return;
    }

    proxy = std::make_unique<DiagnosticCacheProxy>(
        std::move(engine),
        maxCacheSize
        );
    engine = nullptr;
}

std::unique_ptr<DiagnosticEngine> CRBBuilder::getEngine() {
    return std::move(engine);
}

std::unique_ptr<DiagnosticCacheProxy> CRBBuilder::getProxy() {
    return std::move(proxy);
}

bool CRBBuilder::hasProxy() const {
    return useProxy;
}

void CRBBuilder::setDefaultPrior(float prior) {
    defaultPrior = prior;
}

void CRBBuilder::setPriorProbability(const std::string& diseaseName, float prob) {
    priorProbabilities[diseaseName] = prob;
}

void CRBBuilder::setSeason(const std::string& s) {
    season = s;
}

void CRBBuilder::setMaxCacheSize(size_t size) {
    maxCacheSize = size;
}