#include "PatientMedicalRecord.h"
#include <iostream>
#include <iomanip>
#include <sstream>

PatientMedicalRecord::PatientMedicalRecord(const std::string& id, int maxSize)
    : patientId(id),
    maxHistorySize(maxSize) {
}

void PatientMedicalRecord::addSnapshot(std::unique_ptr<DiagnosisMemento> memento) {
    if (!memento) return;

    // Если достигнут максимальный размер, удаляем самый старый снимок
    if (history.size() >= static_cast<size_t>(maxHistorySize)) {
        std::cout << "История переполнена. Удален самый старый снимок.\n";
        history.erase(history.begin());
    }

    history.push_back(std::move(memento));
}

DiagnosisMemento* PatientMedicalRecord::getSnapshot(int index) const {
    if (index >= 0 && index < static_cast<int>(history.size())) {
        return history[index].get();
    }
    return nullptr;
}

DiagnosisMemento* PatientMedicalRecord::getLastSnapshot() const {
    if (history.empty()) {
        return nullptr;
    }
    return history.back().get();
}

DiagnosisMemento* PatientMedicalRecord::getFirstSnapshot() const {
    if (history.empty()) {
        return nullptr;
    }
    return history.front().get();
}

int PatientMedicalRecord::getSnapshotCount() const {
    return static_cast<int>(history.size());
}

std::string PatientMedicalRecord::getPatientId() const {
    return patientId;
}

void PatientMedicalRecord::printHistory() const {
    std::cout << "=== История диагностики пациента " << patientId << " ===\n";
    std::cout << "Всего снимков: " << history.size() << "\n";

    for (size_t i = 0; i < history.size(); ++i) {
        history[i]->printSnapshot();
    }

    std::cout << "========================================\n";
}

void PatientMedicalRecord::printBriefHistory() const {
    std::cout << "\n=== Краткая история диагностики ===\n";
    std::cout << "Пациент: " << patientId << "\n";
    std::cout << "Всего обращений: " << history.size() << "\n\n";

    for (size_t i = 0; i < history.size(); ++i) {
        std::cout << "\nОбращение #" << (i + 1) << ": "
            << history[i]->getShortInfo();
    }
    std::cout << "\n===================================\n";
}

void PatientMedicalRecord::clearHistory() {
    history.clear();
}

std::vector<std::string> PatientMedicalRecord::getDiagnosisTimeline() const {
    std::vector<std::string> timeline;

    for (size_t i = 0; i < history.size(); ++i) {
        std::stringstream ss;
        ss << "Визит " << (i + 1) << ": " << history[i]->getPrimaryDiagnosis() << " (" << std::fixed << std::setprecision(1) << (history[i]->getDiagnosisProbability() * 100) << "%)";
        timeline.push_back(ss.str());
    }

    return timeline;
}