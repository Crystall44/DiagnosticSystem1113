#include "DiagnosisHistory.h"
#include <iostream>
#include <sstream>
#include <iomanip>

DiagnosisHistory::DiagnosisHistory(const std::string& name, int maxSize)
    : patientName(name),
    snapshotCounter(0),
    maxHistorySize(maxSize),
    currentAlgorithm("") {
    std::cout << "[DiagnosisHistory] История диагностики создана для пациента: " << patientName << "\n";
}

DiagnosisMemento* DiagnosisHistory::createAndStoreSnapshot(
    const Appointment* appointment,
    const DiagnosisResult* result,
    const std::string& algorithmName
) {
    // Проверяем переполнение
    if (history.size() >= static_cast<size_t>(maxHistorySize)) {
        std::cout << "[DiagnosisHistory] История переполнена. Удалён самый старый снимок.\n";
        history.erase(history.begin());
    }

    // Увеличиваем счетчик
    snapshotCounter++;

    // Создаем новый снимок
    std::unique_ptr<DiagnosisMemento> memento =
        std::make_unique<DiagnosisMemento>();

    // Заполняем снимок данными
    memento->setPatientName(patientName);

    if (appointment) {
        std::vector<std::string> symptoms = appointment->getSymptoms();
        memento->setSymptoms(symptoms);
        currentSymptoms = symptoms;
    }

    memento->setAlgorithmName(algorithmName);
    currentAlgorithm = algorithmName;
    memento->setDiagnosisDate(std::time(nullptr));

    if (result && !result->getCandidates().empty()) {
        DiagnosisCandidate bestCandidate = result->getBestCandidate();

        memento->setPrimaryDiagnosis(bestCandidate.getDisease()->getName());
        memento->setDiagnosisProbability(bestCandidate.getProbability());
        memento->setConfidenceLevel(bestCandidate.getConfidenceLevel());

        std::vector<DiagnosisCandidate> allCandidates = result->getCandidates();
        for (size_t i = 0; i < allCandidates.size(); ++i) {
            std::stringstream ss;
            ss << (i + 1) << ". " << allCandidates[i].getDisease()->getName() << " — " << std::fixed << std::setprecision(2)
                << (allCandidates[i].getProbability() * 100) << "%" << " [" << allCandidates[i].getConfidenceLevel() << "]";
            memento->addCandidateInfo(ss.str());
        }
    }

    if (result) {
        std::vector<std::string> warnings = result->getWarnings();
        for (size_t i = 0; i < warnings.size(); ++i) {
            memento->addWarning(warnings[i]);
        }
        memento->setProcessingTime(result->getProcessingTime());
    }

    memento->setSnapshotVersion(snapshotCounter);

    // Сохраняем сырой указатель перед перемещением
    DiagnosisMemento* rawPtr = memento.get();

    // Сохраняем в историю
    history.push_back(std::move(memento));

    std::cout << "[DiagnosisHistory] Снимок v" << snapshotCounter << " создан и сохранён. Всего снимков: " << history.size() << "\n";

    return rawPtr;
}

void DiagnosisHistory::restoreFromSnapshot(int index) {
    DiagnosisMemento* memento = getSnapshot(index);
    restoreFromSnapshot(memento);
}

void DiagnosisHistory::restoreFromSnapshot(const DiagnosisMemento* memento) {
    if (!memento) return;

    std::cout << "\n[DiagnosisHistory] ВОССТАНОВЛЕНИЕ ИЗ СНИМКА v" << memento->getSnapshotVersion() << "\n";
    std::cout << "Пациент: " << memento->getPatientName() << "\n";
    std::cout << "Алгоритм: " << memento->getAlgorithmName() << "\n";
    std::cout << "Основной диагноз: " << memento->getPrimaryDiagnosis() << " (" << (memento->getDiagnosisProbability() * 100) << "%)\n";

    currentSymptoms = memento->getSymptoms();
    currentAlgorithm = memento->getAlgorithmName();
}

DiagnosisMemento* DiagnosisHistory::getSnapshot(int index) const {
    if (index >= 0 && index < static_cast<int>(history.size())) {
        return history[index].get();
    }
    return nullptr;
}

DiagnosisMemento* DiagnosisHistory::getLastSnapshot() const {
    if (history.empty()) return nullptr;
    return history.back().get();
}

DiagnosisMemento* DiagnosisHistory::getFirstSnapshot() const {
    if (history.empty()) return nullptr;
    return history.front().get();
}

int DiagnosisHistory::getSnapshotCount() const {
    return static_cast<int>(history.size());
}

void DiagnosisHistory::printHistory() const {
    std::cout << "\n========================================\n";
    std::cout << "История диагностики пациента: " << patientName << "\n";
    std::cout << "========================================\n";
    std::cout << "Всего снимков: " << history.size() << "\n";

    for (size_t i = 0; i < history.size(); ++i) {
        history[i]->printSnapshot();
    }

    std::cout << "========================================\n";
}

void DiagnosisHistory::printBriefHistory() const {
    std::cout << "\n=== Краткая история ===\n";
    std::cout << "Пациент: " << patientName << "\n";
    std::cout << "Всего обращений: " << history.size() << "\n\n";

    for (size_t i = 0; i < history.size(); ++i) {
        std::cout << "Обращение #" << (i + 1) << ": " << history[i]->getShortInfo() << "\n";
    }
    std::cout << "===========================\n";
}

std::vector<std::string> DiagnosisHistory::getDiagnosisTimeline() const {
    std::vector<std::string> timeline;

    for (size_t i = 0; i < history.size(); ++i) {
        std::stringstream ss;
        ss << "Визит " << (i + 1) << ": " << history[i]->getPrimaryDiagnosis() << " (" << std::fixed << std::setprecision(1) << (history[i]->getDiagnosisProbability() * 100) << "%)";
        timeline.push_back(ss.str());
    }

    return timeline;
}

void DiagnosisHistory::compareSnapshots(int index1, int index2) const {
    DiagnosisMemento* older = getSnapshot(index1);
    DiagnosisMemento* newer = getSnapshot(index2);

    if (!older || !newer) {
        std::cout << "[DiagnosisHistory] Ошибка: снимки не найдены\n";
        return;
    }

    compareSnapshots(older, newer);
}

void DiagnosisHistory::compareSnapshots(const DiagnosisMemento* older, const DiagnosisMemento* newer) {
    if (!older || !newer) {
        std::cout << "[DiagnosisHistory] Ошибка: снимки не предоставлены\n";
        return;
    }

    std::cout << "\n=== СРАВНЕНИЕ СНИМКОВ ===\n";
    std::cout << "Снимок v" << older->getSnapshotVersion() << " vs Снимок v" << newer->getSnapshotVersion() << "\n\n";

    std::cout << "Основной диагноз:\n";
    std::cout << "Было: " << older->getPrimaryDiagnosis() << " (" << (older->getDiagnosisProbability() * 100) << "%)\n";
    std::cout << "Стало: " << newer->getPrimaryDiagnosis() << " (" << (newer->getDiagnosisProbability() * 100) << "%)\n";

    std::cout << "\nАлгоритм:\n";
    std::cout << "Было: " << older->getAlgorithmName() << "\n";
    std::cout << "Стало: " << newer->getAlgorithmName() << "\n";

    std::vector<std::string> oldSymptoms = older->getSymptoms();
    std::vector<std::string> newSymptoms = newer->getSymptoms();

    std::cout << "\nСимптомы:\n";
    std::cout << "Было (" << oldSymptoms.size() << "): ";
    for (size_t i = 0; i < oldSymptoms.size(); ++i) {
        std::cout << oldSymptoms[i];
        if (i < oldSymptoms.size() - 1) std::cout << ", ";
    }
    std::cout << "\nСтало (" << newSymptoms.size() << "): ";
    for (size_t i = 0; i < newSymptoms.size(); ++i) {
        std::cout << newSymptoms[i];
        if (i < newSymptoms.size() - 1) std::cout << ", ";
    }
    std::cout << "\n";

    std::cout << "\nПредупреждения:\n";
    std::cout << "Было: " << older->getWarnings().size() << "\n";
    std::cout << "Стало: " << newer->getWarnings().size() << "\n";

    std::cout << "\nВремя выполнения:\n";
    std::cout << "Было: " << older->getProcessingTime() << " мс\n";
    std::cout << "Стало: " << newer->getProcessingTime() << " мс\n";
    std::cout << "===========================\n";
}

void DiagnosisHistory::clearHistory() {
    history.clear();
    snapshotCounter = 0;
    std::cout << "[DiagnosisHistory] История очищена\n";
}

std::string DiagnosisHistory::getPatientName() const {
    return patientName;
}

std::vector<std::string> DiagnosisHistory::getCurrentSymptoms() const {
    return currentSymptoms;
}

std::string DiagnosisHistory::getCurrentAlgorithm() const {
    return currentAlgorithm;
}