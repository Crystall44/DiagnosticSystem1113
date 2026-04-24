#pragma once
#include <memory>
#include "IDiagnosticEngineBuilder.h"

// Директор — управляет процессом пошагового построения
class DiagnosticEngineDirector {
private:
    IDiagnosticEngineBuilder* builder;  // текущий строитель (не владеем)

public:
    // Установить строителя
    void setBuilder(IDiagnosticEngineBuilder* b);

    // Построить полную конфигурацию (все шаги)
    void constructFull();

    // Построить без прокси (только алгоритм и чекеры)
    void constructWithoutProxy();

    // Построить только с алгоритмом (без чекеров и прокси)
    void constructMinimal();
};