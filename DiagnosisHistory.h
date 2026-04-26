#pragma once
#include <memory>
#include <vector>
#include <string>
#include "DiagnosisMemento.h"
#include "DiagnosisResult.h"
#include "Appointment.h"

// DiagnosisHistory — создает снимки и сам их хранит
class DiagnosisHistory {
private:
    std::string patientName; // имя пациента
    std::vector<std::unique_ptr<DiagnosisMemento>> history; // история снимков
    int snapshotCounter; // счетчик версий
    int maxHistorySize; // максимальный размер истории

    // Текущие данные
    std::vector<std::string> currentSymptoms;
    std::string currentAlgorithm;

public:
    DiagnosisHistory(const std::string& patientName, int maxSize = 10);

    // Создать снимок и сразу сохранить в историю
    DiagnosisMemento* createAndStoreSnapshot(
        const Appointment* appointment,
        const DiagnosisResult* result,
        const std::string& algorithmName
    );

    // Восстановить данные из снимка
    void restoreFromSnapshot(int index);
    void restoreFromSnapshot(const DiagnosisMemento* memento);

    // Доступ к истории
    DiagnosisMemento* getSnapshot(int index) const;
    DiagnosisMemento* getLastSnapshot() const;
    DiagnosisMemento* getFirstSnapshot() const;
    int getSnapshotCount() const;

    // Просмотр истории
    void printHistory() const;
    void printBriefHistory() const;
    std::vector<std::string> getDiagnosisTimeline() const;

    // Сравнение двух снимков
    void compareSnapshots(int index1, int index2) const;
    static void compareSnapshots(const DiagnosisMemento* older, const DiagnosisMemento* newer);

    // Очистить историю
    void clearHistory();

    // Геттеры
    std::string getPatientName() const;
    std::vector<std::string> getCurrentSymptoms() const;
    std::string getCurrentAlgorithm() const;
};