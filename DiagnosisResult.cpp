#include "DiagnosisResult.h"
#include <algorithm>

// ”станавливает текущее врем€ и обнул€ет врем€ обработки
DiagnosisResult::DiagnosisResult() : timestamp(std::time(nullptr)), processingTimeMs(0) {}

// ƒобавл€ет кандидата и сразу сортирует список
void DiagnosisResult::addCandidate(const DiagnosisCandidate& candidate) {
    candidates.push_back(candidate); // добавл€ем в конец
    sortCandidatesByProbability(); // сортируем (самый веро€тный - первый)
}

// ¬озвращает всех кандидатов (копию вектора)
std::vector<DiagnosisCandidate> DiagnosisResult::getCandidates() const {
    return candidates;
}

// ¬озвращает первых N кандидатов (самых веро€тных)
std::vector<DiagnosisCandidate> DiagnosisResult::getTopCandidates(int count) const {
    std::vector<DiagnosisCandidate> result;
    // Ѕерем минимум из запрошенного count и реального размера
    int limit = std::min(count, static_cast<int>(candidates.size()));

    //  опируем первые limit элементов
    for (int i = 0; i < limit; ++i) {
        result.push_back(candidates[i]);
    }
    return result;
}

// ¬озвращает самого веро€тного кандидата (первый в отсортированном списке)
DiagnosisCandidate DiagnosisResult::getBestCandidate() const {
    if (candidates.empty()) {
        return DiagnosisCandidate(nullptr, 0.0f);
    }
    return candidates[0]; // первый элемент - самый веро€тный (после сортировки)
}

// ƒобавл€ет предупреждение в список
void DiagnosisResult::addWarning(const std::string& warning) {
    warnings.push_back(warning);
}

// ¬озвращает все предупреждени€
std::vector<std::string> DiagnosisResult::getWarnings() const {
    return warnings;
}

// ѕровер€ет, есть ли предупреждени€
bool DiagnosisResult::hasWarnings() const {
    return !warnings.empty();
}

// ”станавливает врем€ диагностики
void DiagnosisResult::setTimestamp(std::time_t time) {
    timestamp = time;
}

// ¬озвращает врем€ диагностики
std::time_t DiagnosisResult::getTimestamp() const {
    return timestamp;
}

// ”станавливает врем€ выполнени€ диагностики (в миллисекундах)
void DiagnosisResult::setProcessingTime(int ms) {
    processingTimeMs = ms;
}

// ¬озвращает врем€ выполнени€ диагностики
int DiagnosisResult::getProcessingTime() const {
    return processingTimeMs;
}

// —ортирует кандидатов по убыванию веро€тности
void DiagnosisResult::sortCandidatesByProbability() {
    // std::sort с л€мбда-функцией дл€ сравнени€
    std::sort(candidates.begin(), candidates.end(),
        [](const DiagnosisCandidate& a, const DiagnosisCandidate& b) {
            // ¬озвращаем true, если a должно идти перед b
            return a.getProbability() > b.getProbability();
        });
}