#include "BasicFAPBuilder.h"
#include "DiagnosticEngine.h"
#include "DiagnosticCacheProxy.h"
#include "SimpleDiagnostic.h"
#include "CriticalSymptomChecker.h"
#include "DiseaseDirectory.h"

BasicFAPBuilder::BasicFAPBuilder(DiseaseDirectory* dir, float threshold)
    : IDiagnosticEngineBuilder(dir),
    useProxy(false),
    threshold(threshold) {
}

void BasicFAPBuilder::buildAlgorithm() {
    // Создаем простой алгоритм с низким порогом
    auto algorithm = std::make_unique<SimpleDiagnostic>(threshold);

    // Создаем движок с этим алгоритмом
    engine = std::make_unique<DiagnosticEngine>(
        std::move(algorithm),
        directory
        );
}

void BasicFAPBuilder::buildCheckers() {
    if (!engine) return;

    // Только проверка критических симптомов
    engine->addChecker(std::make_unique<CriticalSymptomChecker>());
}

void BasicFAPBuilder::buildProxy() {
    if (!engine) return;

    // Для стажера кэширование не критично
    useProxy = false;
    proxy = nullptr;
}

std::unique_ptr<DiagnosticEngine> BasicFAPBuilder::getEngine() {
    return std::move(engine);
}

std::unique_ptr<DiagnosticCacheProxy> BasicFAPBuilder::getProxy() {
    if (!proxy && engine) {
        // Если прокси нет, но он нужен — создаем на лету
        useProxy = true;
        proxy = std::make_unique<DiagnosticCacheProxy>(
            std::move(engine)
            );
    }
    return std::move(proxy);
}

bool BasicFAPBuilder::hasProxy() const {
    return useProxy;
}

void BasicFAPBuilder::setThreshold(float t) {
    threshold = t;
}