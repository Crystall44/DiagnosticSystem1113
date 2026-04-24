#pragma once
#include <memory>

#include "DiagnosticEngine.h";
#include "DiagnosticCacheProxy.h";
#include "DiseaseDirectory.h";

// Интерфейс строителя диагностического движка
class IDiagnosticEngineBuilder {
protected:
    DiseaseDirectory* directory;  // справочник (не владеем)

public:
    explicit IDiagnosticEngineBuilder(DiseaseDirectory* dir)
        : directory(dir) {}

    virtual ~IDiagnosticEngineBuilder() = default;

    // Шаг 1: Создать и настроить алгоритм диагностики
    virtual void buildAlgorithm() = 0;

    // Шаг 2: Создать и добавить проверки (чекера)
    virtual void buildCheckers() = 0;

    // Шаг 3: Оборачиваем в прокси (опционально)
    virtual void buildProxy() = 0;

    // Получить результат сборки
    // Возвращаем либо DiagnosticEngine, либо DiagnosticCacheProxy
    virtual std::unique_ptr<DiagnosticEngine> getEngine() = 0;
    virtual std::unique_ptr<DiagnosticCacheProxy> getProxy() = 0;

    // Проверяем, используется ли прокси
    virtual bool hasProxy() const = 0;
};