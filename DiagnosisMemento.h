#pragma once
#include <string>
#include <vector>
#include <ctime>
#include <memory>

#include "DiagnosisResult.h";

// Memento — хранит снимок состояния диагностики
class DiagnosisMemento {
private:
    // Данные снимка
    std::string patientName; // имя пациента
    std::vector<std::string> symptoms; // симптомы на момент диагностики
    std::string algorithmName; // название использованного алгоритма
    std::time_t diagnosisDate; // дата диагностики

    // Результаты диагностики
    std::string primaryDiagnosis; // основной диагноз
    float diagnosisProbability; // вероятность
    std::string confidenceLevel; // уровень уверенности

    // Список всех кандидатов (сериализованные)
    std::vector<std::string> candidatesInfo; // информация о кандидатах

    // Предупреждения
    std::vector<std::string> warnings; // предупреждения от чекеров

    // Время выполнения
    int processingTimeMs; // время в миллисекундах

    // Версия снимка
    int snapshotVersion; // номер версии снимка

public:
    // Конструктор (доступен только Originator)
    DiagnosisMemento();

    // Сеттеры для заполнения снимка (используются Originator)
    void setPatientName(const std::string& name);
    void setSymptoms(const std::vector<std::string>& sym);
    void setAlgorithmName(const std::string& algo);
    void setDiagnosisDate(std::time_t date);
    void setPrimaryDiagnosis(const std::string& diagnosis);
    void setDiagnosisProbability(float prob);
    void setConfidenceLevel(const std::string& level);
    void addCandidateInfo(const std::string& info);
    void addWarning(const std::string& warning);
    void setProcessingTime(int ms);
    void setSnapshotVersion(int version);

    // Геттеры для чтения снимка
    std::string getPatientName() const;
    std::vector<std::string> getSymptoms() const;
    std::string getAlgorithmName() const;
    std::time_t getDiagnosisDate() const;
    std::string getPrimaryDiagnosis() const;
    float getDiagnosisProbability() const;
    std::string getConfidenceLevel() const;
    std::vector<std::string> getCandidatesInfo() const;
    std::vector<std::string> getWarnings() const;
    int getProcessingTime() const;
    int getSnapshotVersion() const;

    // Вывод информации о снимке
    void printSnapshot() const;

    // Получить краткую информацию
    std::string getShortInfo() const;
};