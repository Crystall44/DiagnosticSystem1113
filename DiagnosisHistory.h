#pragma once
#include <memory>
#include <vector>
#include <string>
#include "DiagnosisMemento.h"
#include "DiagnosisResult.h"
#include "Appointment.h"

// Originator — создает и восстанавливает снимки диагностики
class DiagnosisHistory {
private:
    std::string patientName; // текущий пациент
    std::vector<std::string> currentSymptoms; // текущие симптомы
    std::string currentAlgorithm; // текущий алгоритм
    int snapshotCounter; // счетчик версий снимков

public:
    DiagnosisHistory(const std::string& patientName);

    // Создать снимок текущего состояния диагностики
    std::unique_ptr<DiagnosisMemento> createSnapshot(
        const Appointment* appointment,
        const DiagnosisResult* result,
        const std::string& algorithmName
    );

    // Восстановить данные из снимка (для отображения)
    void restoreFromSnapshot(const DiagnosisMemento* memento);

    // Обновить текущие данные
    void setCurrentSymptoms(const std::vector<std::string>& symptoms);
    void setCurrentAlgorithm(const std::string& algorithm);

    // Получить текущие данные
    std::string getPatientName() const;
    std::vector<std::string> getCurrentSymptoms() const;
    std::string getCurrentAlgorithm() const;
    int getSnapshotCount() const;

    // Сравнить два снимка
    static void compareSnapshots(
        const DiagnosisMemento* older,
        const DiagnosisMemento* newer
    );
};