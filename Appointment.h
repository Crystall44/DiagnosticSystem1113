#pragma once
#include <string>
#include <vector>
#include <ctime>
#include "Patient.h"

// Приём пациента - фиксирует дату, симптомы и ссылку на пациента
class Appointment {
private:
    std::time_t date; // время приема
    Patient* patient;
    std::vector<std::string> symptoms; // список симптомов

public:
    Appointment(Patient* patient);

    // Сеттер и геттер для даты
    void setDate(std::time_t newDate);
    std::time_t getDate() const;

    // Геттер и сеттер для пациента
    Patient* getPatient() const;
    void setPatient(Patient* newPatient);

    // Работа с симптомами
    void addSymptom(const std::string& symptom); // добавить симптом
    void removeSymptom(const std::string& symptom); // удалить симптом
    std::vector<std::string> getSymptoms() const; // получить все симптомы
};
