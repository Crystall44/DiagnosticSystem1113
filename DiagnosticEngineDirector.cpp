#include "DiagnosticEngineDirector.h"

void DiagnosticEngineDirector::setBuilder(IDiagnosticEngineBuilder* b) {
    builder = b;
}

void DiagnosticEngineDirector::constructFull() {
    if (!builder) return;
    builder->buildAlgorithm();
    builder->buildCheckers();
    builder->buildProxy();
}

void DiagnosticEngineDirector::constructWithoutProxy() {
    if (!builder) return;
    builder->buildAlgorithm();
    builder->buildCheckers();
}

void DiagnosticEngineDirector::constructMinimal() {
    if (!builder) return;
    builder->buildAlgorithm();
}