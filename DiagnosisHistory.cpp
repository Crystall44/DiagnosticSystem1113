#include "DiagnosisHistory.h"
#include <iostream>
#include <sstream>
#include <iomanip>

DiagnosisHistory::DiagnosisHistory(const std::string& name)
    : patientName(name),
    currentAlgorithm(""),
    snapshotCounter(0) {
}

std::unique_ptr<DiagnosisMemento> DiagnosisHistory::createSnapshot(
    const Appointment* appointment,
    const DiagnosisResult* result,
    const std::string& algorithmName
) {
    // Увеличиваем счетчик версий
    snapshotCounter++;

    // Создаем новый снимок
    std::unique_ptr<DiagnosisMemento> memento =
        std::make_unique<DiagnosisMemento>();

    // ===== Заполняем снимок данными =====

    // 1. Информация о пациенте и симптомах
    memento->setPatientName(patientName);

    if (appointment) {
        std::vector<std::string> symptoms = appointment->getSymptoms();
        memento->setSymptoms(symptoms);
        currentSymptoms = symptoms;
    }

    // 2. Информация об алгоритме
    memento->setAlgorithmName(algorithmName);
    currentAlgorithm = algorithmName;

    // 3. Дата диагностики
    memento->setDiagnosisDate(std::time(nullptr));

    // 4. Результаты диагностики
    if (result && !result->getCandidates().empty()) {
        DiagnosisCandidate bestCandidate = result->getBestCandidate();

        memento->setPrimaryDiagnosis(bestCandidate.getDisease()->getName()
        );
        memento->setDiagnosisProbability(bestCandidate.getProbability()
        );
        memento->setConfidenceLevel(bestCandidate.getConfidenceLevel()
        );

        // 5. Все кандидаты
        std::vector<DiagnosisCandidate> allCandidates = result->getCandidates();

        for (size_t i = 0; i < allCandidates.size(); ++i) {
            std::stringstream ss;
            ss << (i + 1) << ". "
                << allCandidates[i].getDisease()->getName()
                << " — " << std::fixed << std::setprecision(2)
                << (allCandidates[i].getProbability() * 100) << "%"
                << " [" << allCandidates[i].getConfidenceLevel() << "]";
            memento->addCandidateInfo(ss.str());
        }
    }

    // 6. Предупреждения
    if (result) {
        std::vector<std::string> warnings = result->getWarnings();
        for (size_t i = 0; i < warnings.size(); ++i) {
            memento->addWarning(warnings[i]);
        }

        // 7. Время выполнения
        memento->setProcessingTime(result->getProcessingTime());
    }

    // 8. Версия снимка
    memento->setSnapshotVersion(snapshotCounter);

    return memento;
}

void DiagnosisHistory::restoreFromSnapshot(const DiagnosisMemento* memento) {
    if (!memento) return;

    std::cout << "\nВосстановление из снимка\n";
    std::cout << "Восстановлены данные версии "
        << memento->getSnapshotVersion() << ":\n";
    std::cout << "  Пациент: " << memento->getPatientName() << "\n";
    std::cout << "  Алгоритм: " << memento->getAlgorithmName() << "\n";
    std::cout << "  Основной диагноз: " << memento->getPrimaryDiagnosis()
        << " (" << (memento->getDiagnosisProbability() * 100) << "%)\n";

    // Восстанавливаем текущие данные
    currentSymptoms = memento->getSymptoms();
    currentAlgorithm = memento->getAlgorithmName();
}

void DiagnosisHistory::setCurrentSymptoms(const std::vector<std::string>& symptoms) {
    currentSymptoms = symptoms;
}

void DiagnosisHistory::setCurrentAlgorithm(const std::string& algorithm) {
    currentAlgorithm = algorithm;
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

int DiagnosisHistory::getSnapshotCount() const {
    return snapshotCounter;
}

// Статический метод сравнения двух снимков
void DiagnosisHistory::compareSnapshots(
    const DiagnosisMemento* older,
    const DiagnosisMemento* newer
) {
    if (!older || !newer) {
        std::cout << "Ошибка: снимки не предоставлены для сравнения\n";
        return;
    }

    std::cout << "\n=== Сравнение снимков ===\n";
    std::cout << "Снимок v" << older->getSnapshotVersion() << " vs Снимок v" << newer->getSnapshotVersion() << "\n\n";

    // Сравниваем основные диагнозы
    std::cout << "Основной диагноз:\n";
    std::cout << "Было: " << older->getPrimaryDiagnosis() << " (" << (older->getDiagnosisProbability() * 100) << "%)\n";
    std::cout << "Стало: " << newer->getPrimaryDiagnosis() << " (" << (newer->getDiagnosisProbability() * 100) << "%)\n";

    // Сравниваем алгоритмы
    std::cout << "\nАлгоритм:\n";
    std::cout << "Было: " << older->getAlgorithmName() << "\n";
    std::cout << "Стало: " << newer->getAlgorithmName() << "\n";

    // Сравниваем симптомы
    std::cout << "\nСимптомы:\n";
    std::vector<std::string> oldSymptoms = older->getSymptoms();
    std::vector<std::string> newSymptoms = newer->getSymptoms();

    std::cout << "Было (" << oldSymptoms.size() << "): ";
    for (size_t i = 0; i < oldSymptoms.size(); ++i) {
        std::cout << oldSymptoms[i];
        if (i < oldSymptoms.size() - 1) std::cout << ", ";
    }
    std::cout << "\n";

    std::cout << "Стало (" << newSymptoms.size() << "): ";
    for (size_t i = 0; i < newSymptoms.size(); ++i) {
        std::cout << newSymptoms[i];
        if (i < newSymptoms.size() - 1) std::cout << ", ";
    }
    std::cout << "\n";

    // Сравниваем предупреждения
    std::cout << "\nПредупреждения:\n";
    std::cout << "Было: " << older->getWarnings().size() << " предупреждений\n";
    std::cout << "Стало: " << newer->getWarnings().size() << " предупреждений\n";

    std::cout << "===========================\n";
}