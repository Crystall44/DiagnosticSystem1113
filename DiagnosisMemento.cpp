#include "DiagnosisMemento.h"
#include <iostream>
#include <sstream>
#include <iomanip>

DiagnosisMemento::DiagnosisMemento()
    : patientName(""),
    algorithmName(""),
    diagnosisDate(0),
    primaryDiagnosis(""),
    diagnosisProbability(0.0f),
    confidenceLevel(""),
    processingTimeMs(0),
    snapshotVersion(0) {
}

// Сеттеры 

void DiagnosisMemento::setPatientName(const std::string& name) {
    patientName = name;
}

void DiagnosisMemento::setSymptoms(const std::vector<std::string>& sym) {
    symptoms = sym;
}

void DiagnosisMemento::setAlgorithmName(const std::string& algo) {
    algorithmName = algo;
}

void DiagnosisMemento::setDiagnosisDate(std::time_t date) {
    diagnosisDate = date;
}

void DiagnosisMemento::setPrimaryDiagnosis(const std::string& diagnosis) {
    primaryDiagnosis = diagnosis;
}

void DiagnosisMemento::setDiagnosisProbability(float prob) {
    diagnosisProbability = prob;
}

void DiagnosisMemento::setConfidenceLevel(const std::string& level) {
    confidenceLevel = level;
}

void DiagnosisMemento::addCandidateInfo(const std::string& info) {
    candidatesInfo.push_back(info);
}

void DiagnosisMemento::addWarning(const std::string& warning) {
    warnings.push_back(warning);
}

void DiagnosisMemento::setProcessingTime(int ms) {
    processingTimeMs = ms;
}

void DiagnosisMemento::setSnapshotVersion(int version) {
    snapshotVersion = version;
}

// Геттеры

std::string DiagnosisMemento::getPatientName() const {
    return patientName;
}

std::vector<std::string> DiagnosisMemento::getSymptoms() const {
    return symptoms;
}

std::string DiagnosisMemento::getAlgorithmName() const {
    return algorithmName;
}

std::time_t DiagnosisMemento::getDiagnosisDate() const {
    return diagnosisDate;
}

std::string DiagnosisMemento::getPrimaryDiagnosis() const {
    return primaryDiagnosis;
}

float DiagnosisMemento::getDiagnosisProbability() const {
    return diagnosisProbability;
}

std::string DiagnosisMemento::getConfidenceLevel() const {
    return confidenceLevel;
}

std::vector<std::string> DiagnosisMemento::getCandidatesInfo() const {
    return candidatesInfo;
}

std::vector<std::string> DiagnosisMemento::getWarnings() const {
    return warnings;
}

int DiagnosisMemento::getProcessingTime() const {
    return processingTimeMs;
}

int DiagnosisMemento::getSnapshotVersion() const {
    return snapshotVersion;
}

// Вывод информации

void DiagnosisMemento::printSnapshot() const {
    std::cout << "\n=== Снимок диагностики (версия " << snapshotVersion << ") ===\n";
    std::cout << "Пациент: " << patientName << "\n";
    char timeBuffer[26];
    ctime_s(timeBuffer, sizeof(timeBuffer), &diagnosisDate);
    std::cout << "Дата: " << timeBuffer;

    std::cout << "Алгоритм: " << algorithmName << "\n";

    std::cout << "Симптомы: ";
    for (size_t i = 0; i < symptoms.size(); ++i) {
        std::cout << symptoms[i];
        if (i < symptoms.size() - 1) std::cout << ", ";
    }
    std::cout << "\n";

    std::cout << "Основной диагноз: " << primaryDiagnosis << " (вероятность: " << std::fixed << std::setprecision(2) << (diagnosisProbability * 100) << "%)\n";
    std::cout << "Уверенность: " << confidenceLevel << "\n";

    if (!candidatesInfo.empty()) {
        std::cout << "Все кандидаты:\n";
        for (size_t i = 0; i < candidatesInfo.size(); ++i) {
            std::cout << candidatesInfo[i] << "\n";
        }
    }

    if (!warnings.empty()) {
        std::cout << "Предупреждения:\n";
        for (size_t i = 0; i < warnings.size(); ++i) {
            std::cout << warnings[i] << "\n";
        }
    }

    std::cout << "Время выполнения: " << processingTimeMs << " мс\n";
    std::cout << "========================================\n";
}

std::string DiagnosisMemento::getShortInfo() const {
    std::stringstream ss;
    std::tm localTime;
    localtime_s(&localTime, &diagnosisDate);
    ss << "v" << snapshotVersion << " | "
        << std::put_time(&localTime, "%d.%m.%Y %H:%M") << " | "
        << primaryDiagnosis << " ("
        << std::fixed << std::setprecision(1) << (diagnosisProbability * 100) << "%)";
    return ss.str();
}