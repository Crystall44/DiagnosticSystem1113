#include "Appointment.h"
#include <algorithm> // нужен дл€ std::find

//  онструктор
Appointment::Appointment(Patient* patient)
    : patient(patient) {
    date = std::time(nullptr); // устанавливаем текущее врем€
}

void Appointment::setDate(std::time_t newDate) {
    date = newDate;
}

std::time_t Appointment::getDate() const {
    return date;
}

Patient* Appointment::getPatient() const {
    return patient;
}

void Appointment::setPatient(Patient* newPatient) {
    patient = newPatient;
}

void Appointment::addSymptom(const std::string& symptom) {
    symptoms.push_back(symptom);
}

void Appointment::removeSymptom(const std::string& symptom) {
    auto it = std::find(symptoms.begin(), symptoms.end(), symptom);
    if (it != symptoms.end()) {
        symptoms.erase(it);
    }
}

std::vector<std::string> Appointment::getSymptoms() const {
    return symptoms;
}