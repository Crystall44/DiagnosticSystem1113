#pragma once
#include <memory>
#include <vector>
#include <string>
#include "DiagnosisMemento.h"

// Caretaker — хранит историю снимков, но не изменяет их содержимое
class PatientMedicalRecord {
private:
    std::string patientId;                                  // ID пациента
    std::vector<std::unique_ptr<DiagnosisMemento>> history; // история снимков
    int maxHistorySize;                                     // максимальный размер истории

public:
    explicit PatientMedicalRecord(const std::string& patientId, int maxSize = 10);

    // Добавить снимок в историю
    void addSnapshot(std::unique_ptr<DiagnosisMemento> memento);

    // Получить снимок по индексу (0 — самый старый)
    DiagnosisMemento* getSnapshot(int index) const;

    // Получить последний снимок
    DiagnosisMemento* getLastSnapshot() const;

    // Получить первый снимок
    DiagnosisMemento* getFirstSnapshot() const;

    // Получить количество снимков
    int getSnapshotCount() const;

    // Получить ID пациента
    std::string getPatientId() const;

    // Вывести всю историю
    void printHistory() const;

    // Вывести краткую историю (только основные диагнозы)
    void printBriefHistory() const;

    // Очистить историю
    void clearHistory();

    // Получить историю изменений диагнозов
    std::vector<std::string> getDiagnosisTimeline() const;
};