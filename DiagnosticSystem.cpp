#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <iomanip>
#include <ctime>

// Интерфейсы
#include "IDiagnosticAlgorithm.h"
#include "ISymptomChecker.h"

// Алгоритмы
#include "SimpleDiagnostic.h"
#include "WeightedDiagnostic.h"
#include "BayesianDiagnostic.h"

// Проверки
#include "CriticalSymptomChecker.h"
#include "EpidemiologicalChecker.h"

// Пациенты
#include "RegularPatient.h"
#include "ChronicPatient.h"

// Справочник
#include "DiseaseDirectory.h"
#include "Disease.h"

// Прием
#include "Appointment.h"

// Результаты
#include "DiagnosisResult.h"
#include "DiagnosisCandidate.h"

// Движок
#include "DiagnosticEngine.h"

// Прокси
#include "DiagnosticCacheProxy.h"

//Строитель
#include "BasicFAPBuilder.h"
#include "ExperiencedFAPBuilder.h"
#include "CRBBuilder.h"
#include "DiagnosticEngineDirector.h"
#include "IDiagnosticEngineBuilder.h"

// State - состояние
#include "AppointmentContext.h"
#include "AppointmentState.h"
#include "RegistrationState.h"
#include "ExaminationState.h"
#include "DiagnosisState.h"
#include "RecommendationState.h"
#include "CompletedState.h"

// Memento
#include "DiagnosisHistory.h"
#include "DiagnosisMemento.h"
#include "PatientMedicalRecord.h"

// Observer
#include "AlertSystem.h"
#include "IObserver.h"
#include "ISubject.h"
#include "LoggingService.h"
#include "PatientMonitor.h"
#include "StatisticService.h"


// Вспомогательная функция для вывода результатов
void printDiagnosisResult(const DiagnosisResult* result, const std::string& scenarioName) {
    std::cout << "Результат диагностики: " << scenarioName << std::endl;

    // Получаем список кандидатов (возможных диагнозов) из результата
    auto candidates = result->getCandidates();

    // Если список пуст - значит ничего не нашли
    if (candidates.empty()) {
        std::cout << "Диагноз не найден.\n";
    }
    else {
        std::cout << "Наиболее вероятные диагнозы:\n";
        int i = 1;

        // Перебираем всех кандидатов (они уже отсортированы по убыванию вероятности)
        for (const auto& candidate : candidates) {
            // Выводим номер, название болезни и вероятность в процентах
            std::cout << i++ << ". " << candidate.getDisease()->getName() << " - вероятность: " << std::fixed << std::setprecision(2) << candidate.getProbability() * 100 << "%\n";
            
            // Выводим список симптомов, которые совпали
            std::cout << "Совпавшие симптомы: ";
            // Получаем вектор совпавших симптомов
            auto matched = candidate.getMatchedSymptoms();

            // Перебираем все совпавшие симптомы
            for (size_t j = 0; j < matched.size(); ++j) {
                std::cout << matched[j];
                if (j < matched.size() - 1) std::cout << ", ";
            }
            std::cout << "\n";
        }
    }

    auto warnings = result->getWarnings();
    if (!warnings.empty()) {
        std::cout << "\nПРЕДУПРЕЖДЕНИЯ:\n";

        // Перебираем и выводим каждое предупреждение
        for (const auto& warning : warnings) {
            std::cout << warning << "\n";
        }
    }

    // Выводим время, затраченное на диагностику (в миллисекундах)
    std::cout << "Время обработки: " << result->getProcessingTime() << " мс\n";
}

// Вспомогательная функция для создания справочника заболеваний
void setupDiseaseDirectory(DiseaseDirectory& directory) {
    // ОРВИ
    auto orvi = std::make_unique<Disease>("ОРВИ", "J00");
    orvi->addSymptom("кашель", 0.5f);
    orvi->addSymptom("насморк", 0.9f);
    orvi->addSymptom("температура 38", 0.6f);
    orvi->addSymptom("головная боль", 0.4f);
    orvi->addSymptom("слабость", 0.5f);
    directory.addDisease(std::move(orvi));

    // Ангина
    auto angina = std::make_unique<Disease>("Ангина", "J03");
    angina->addSymptom("боль в горле", 0.9f);
    angina->addSymptom("температура 39", 0.9f);
    angina->addSymptom("головная боль", 0.4f);
    angina->addSymptom("слабость", 0.6f);
    angina->addSymptom("увеличение лимфоузлов", 0.8f);
    directory.addDisease(std::move(angina));

    // Грипп
    auto flu = std::make_unique<Disease>("Грипп", "J11");
    flu->addSymptom("температура 39", 0.9f);
    flu->addSymptom("озноб", 0.8f);
    flu->addSymptom("головная боль", 0.7f);
    flu->addSymptom("ломота в теле", 0.8f);
    flu->addSymptom("кашель", 0.6f);
    flu->addSymptom("слабость", 0.8f);
    directory.addDisease(std::move(flu));

    // Пневмония
    auto pneumonia = std::make_unique<Disease>("Пневмония", "J18");
    pneumonia->addSymptom("кашель", 0.9f);
    pneumonia->addSymptom("температура 39", 0.8f);
    pneumonia->addSymptom("одышка", 0.9f);
    pneumonia->addSymptom("боль в груди", 0.5f);
    pneumonia->addSymptom("слабость", 0.6f);
    directory.addDisease(std::move(pneumonia));

    // Аллергия
    auto allergy = std::make_unique<Disease>("Аллергия", "T78");
    allergy->addSymptom("насморк", 0.7f);
    allergy->addSymptom("чихание", 0.8f);
    allergy->addSymptom("зуд в глазах", 0.9f);
    allergy->addSymptom("слезотечение", 0.8f);
    allergy->addSymptom("сыпь", 0.5f);
    directory.addDisease(std::move(allergy));

    // Ковид
    auto covid = std::make_unique<Disease>("COVID-19", "U07.1");
    covid->addSymptom("кашель", 0.8f);
    covid->addSymptom("лихорадка", 0.8f);
    covid->addSymptom("потеря обоняния", 0.6f);       
    covid->addSymptom("потеря вкуса", 0.5f);
    covid->addSymptom("слабость", 0.9f);
    covid->addSymptom("одышка", 0.3f);                 
    covid->addSymptom("головная боль", 0.5f);
    directory.addDisease(std::move(covid));
}

int main() {
    setlocale(LC_ALL, "Russian");
    std::cout << "МЕДИЦИНСКАЯ СИСТЕМА ДИАГНОСТИКИ\n\n";

    // 1. Создаем справочник заболеваний
    std::cout << "1. Инициализация справочника заболеваний...\n";
    DiseaseDirectory directory;
    setupDiseaseDirectory(directory);
    std::cout << "   Загружено заболеваний: " << directory.getDiseaseCount() << "\n\n";

    // 2. Создаем пациентов
    std::cout << "2. Регистрация пациентов...\n";
    auto patient1 = std::make_unique<RegularPatient>("Иванов И.И.", 35, "муж");
    patient1->addAllergy("пенициллин");

    auto patient2 = std::make_unique<ChronicPatient>("Петрова А.С.", 42, "жен");
    patient2->addChronicDisease("бронхиальная астма", "2018");
    patient2->addAllergy("аспирин");

    auto patient3 = std::make_unique<RegularPatient>("Сидоров П.Р.", 28, "муж");

    std::cout << "Зарегистрировано пациентов: 3\n\n";

    // 3. Создаем приемы с разными симптомами
    std::cout << "3. Создание приемов...\n";

    // Прием 1 - симптомы ОРВИ
    Appointment apt1(patient1.get());
    apt1.addSymptom("кашель");
    apt1.addSymptom("насморк");
    apt1.addSymptom("температура 38");
    apt1.addSymptom("слабость");

    // Прием 2 - симптомы ангины
    Appointment apt2(patient2.get());
    apt2.addSymptom("боль в горле");
    apt2.addSymptom("температура 39");
    apt2.addSymptom("головная боль");
    apt2.addSymptom("слабость");

    // Прием 3 - симптомы гриппа
    Appointment apt3(patient3.get());
    apt3.addSymptom("температура 39");
    apt3.addSymptom("головная боль");
    apt3.addSymptom("ломота в теле");
    apt3.addSymptom("кашель");
    apt3.addSymptom("слабость");

    // Прием 4 - для демонстрации кэша (повтор симптомов приема 1)
    Appointment apt4(patient1.get());
    apt4.addSymptom("кашель");
    apt4.addSymptom("насморк");
    apt4.addSymptom("температура 38");
    apt4.addSymptom("слабость");

    std::cout << "Создано приемов: 4\n\n";

    // Сценарий 1: Базовый ФАП (SimpleDiagnostic)
    std::cout << "Сценарий 1: Базовый ФАП (фельдшер-стажёр)\n";

    // Создаем движок с простым алгоритмом
    auto simpleEngine = std::make_unique<DiagnosticEngine>(
        std::make_unique<SimpleDiagnostic>(0.3f),
        &directory
        );

    // Добавляем проверку критических симптомов
    simpleEngine->addChecker(std::make_unique<CriticalSymptomChecker>());

    // Оборачиваем в прокси для кэширования
    DiagnosticCacheProxy proxy1(std::move(simpleEngine));

    // Диагностика первого пациента
    auto result1 = proxy1.diagnose(&apt1);
    printDiagnosisResult(result1.get(), "\nПациент Иванов (симптомы ОРВИ)");

    // Диагностика второго пациента
    auto result2 = proxy1.diagnose(&apt2);
    printDiagnosisResult(result2.get(), "\nПациент Петрова (симптомы ангины)");

    // Сценарий 2: Опытный фельдшер 
    std::cout << "\nСценарий 2: Опытный фельдшер\n";

    // Создаем движок с взвешенным алгоритмом
    auto weightedEngine = std::make_unique<DiagnosticEngine>(
        std::make_unique<WeightedDiagnostic>(true),
        &directory
        );

    // Добавляем проверки
    weightedEngine->addChecker(std::make_unique<CriticalSymptomChecker>());
    auto epiChecker = std::make_unique<EpidemiologicalChecker>();
    epiChecker->setSeason("winter");
    weightedEngine->addChecker(std::move(epiChecker));

    // Оборачиваем в прокси
    DiagnosticCacheProxy proxy2(std::move(weightedEngine));

    // Диагностика третьего пациента
    auto result3 = proxy2.diagnose(&apt3);
    printDiagnosisResult(result3.get(), "\nПациент Сидоров (симптомы гриппа)");

    // Демонстрация работы прокси (кэша)
    // Повторная диагностика с теми же симптомами (должно быть попадание в кэш)
    std::cout << "\nПовторная диагностика пациента Иванов (те же симптомы)...\n";
    auto resultCached = proxy1.diagnose(&apt4);
    printDiagnosisResult(resultCached.get(), "Повторно (из кэша)");

    std::cout << "\nСтатистика кэша после повторного обращения:\n";
    std::cout << "Попаданий (hits): " << proxy1.getHits() << "\n";
    std::cout << "Промахов (misses): " << proxy1.getMisses() << "\n";

    // Сценарий 3: Консультация в ЦРБ 
    std::cout << "\nСценарий 3: Консультация в ЦРБ\n";

    // Создаем байесовский алгоритм с настройкой априорных вероятностей
    auto bayesian = std::make_unique<BayesianDiagnostic>(0.01f);
    bayesian->setPriorProbability("ОРВИ", 0.3f);
    bayesian->setPriorProbability("Грипп", 0.2f);
    bayesian->setPriorProbability("Ангина", 0.15f);
    bayesian->setPriorProbability("Пневмония", 0.05f);

    auto bayesianEngine = std::make_unique<DiagnosticEngine>(
        std::move(bayesian),
        &directory
        );

    // Добавляем все проверки
    bayesianEngine->addChecker(std::make_unique<CriticalSymptomChecker>());
    auto epiChecker2 = std::make_unique<EpidemiologicalChecker>();
    epiChecker2->setSeason("winter");
    bayesianEngine->addChecker(std::move(epiChecker2));

    DiagnosticCacheProxy proxy3(std::move(bayesianEngine));

    // Диагностика сложного случая
    auto resultBayes = proxy3.diagnose(&apt3);
    printDiagnosisResult(resultBayes.get(), "\nПациент Сидоров (байесовский алгоритм)");

    std::cout << "\n\n===== Демонстрация новых паттернов =====\n\n";
    std::cout << "1. Builder\n\n";

    // Создаем директора (управляет процессом построения)
    DiagnosticEngineDirector director;

    // Создаем одного пациента для всех трёх сценариев
    std::unique_ptr<RegularPatient> demoPatient =
        std::make_unique<RegularPatient>("Сидоров П.Р.", 28, "муж");

    // Создаем прием с симптомами гриппа
    Appointment demoAppointment(demoPatient.get());
    demoAppointment.addSymptom("температура 39");
    demoAppointment.addSymptom("головная боль");
    demoAppointment.addSymptom("ломота в теле");
    demoAppointment.addSymptom("кашель");
    demoAppointment.addSymptom("слабость");

    std::cout << "Пациент: " << demoPatient->getName() << "\n";
    std::cout << "Симптомы: температура 39, головная боль, ломота в теле, кашель, слабость\n\n";

    std::cout << "\n=== Сценарий 1: Базовый ФАП ===\n";
    
    // Создаем строитель для базового ФАП
    BasicFAPBuilder basicBuilder(&directory, 0.3f);

    // Директор управляет процессом сборки
    director.setBuilder(&basicBuilder);
    director.constructFull();

    // Получаем готовый движок
    std::unique_ptr<DiagnosticEngine> basicEngine = basicBuilder.getEngine();

    // Выполняем диагностику
    std::unique_ptr<DiagnosisResult> resultBasic = basicEngine->diagnose(&demoAppointment);
    printDiagnosisResult(resultBasic.get(), "Базовый ФАП (SimpleDiagnostic)");

    std::cout << "\n=== Сценарий 2: Опытный фельдшер ===\n";

    // Создаем строитель для опытного фельдшера
    ExperiencedFAPBuilder experiencedBuilder(&directory, "winter", 100);
    experiencedBuilder.setNormalization(true);

    // Директор собирает полную конфигурацию
    director.setBuilder(&experiencedBuilder);
    director.constructFull();

    // Получаем прокси с кэшированием
    std::unique_ptr<DiagnosticCacheProxy> experiencedProxy = experiencedBuilder.getProxy();
    std::unique_ptr<DiagnosisResult> resultExperienced1 = experiencedProxy->diagnose(&demoAppointment);
    printDiagnosisResult(resultExperienced1.get(), "Опытный фельдшер");

    std::cout << "\n=== Сценарий 3: Консультация в ЦРБ ===\n";

    // Создаем строитель для ЦРБ
    CRBBuilder crbBuilder(&directory, "winter", 100);

    // Донастраиваем априорные вероятности
    crbBuilder.setPriorProbability("ОРВИ", 0.3f);
    crbBuilder.setPriorProbability("Грипп", 0.2f);
    crbBuilder.setPriorProbability("Ангина", 0.15f);
    crbBuilder.setPriorProbability("Пневмония", 0.05f);
    crbBuilder.setPriorProbability("COVID-19", 0.1f);

    // Собираем систему
    director.setBuilder(&crbBuilder);
    director.constructFull();

    // Получаем прокси с байесовским движком
    std::unique_ptr<DiagnosticCacheProxy> crbProxy = crbBuilder.getProxy();

    // Выполняем диагностику
    std::unique_ptr<DiagnosisResult> resultCRB = crbProxy->diagnose(&demoAppointment);
    printDiagnosisResult(resultCRB.get(), "ЦРБ (BayesianDiagnostic)");

    std::cout << "\n\n2. State\n\n";

    // Создаем состояния
    std::cout << "Создание цепочки состояний приема...\n";
    RegistrationState* registrationState = new RegistrationState();
    ExaminationState* examinationState = new ExaminationState();
    DiagnosisState* diagnosisState = new DiagnosisState();
    RecommendationState* recommendationState = new RecommendationState();
    CompletedState* completedState = new CompletedState();

    // Связываем состояния в цепочку
    registrationState->setNextState(examinationState);
    examinationState->setNextState(diagnosisState);
    diagnosisState->setNextState(recommendationState);
    recommendationState->setNextState(completedState);
    completedState->setNextState(nullptr);  // конечное состояние

    std::cout << "Цепочка создана: Регистрация -> Осмотр -> Диагностика -> Рекомендации -> Завершение\n\n";

    // Создаем контекст приема (пациент)
    std::cout << "Создание приема пациента...\n";
    AppointmentContext* patientContext = new AppointmentContext("Петрова А.С.", 42);

    // Устанавливаем начальное состояние
    patientContext->setCurrentState(registrationState);

    // Выполняем все состояния по очереди
    std::cout << "\n=== Выполнение приема ===\n";

    // Шаг 1: Регистрация
    patientContext->run();

    // Шаг 2: Осмотр
    patientContext->run();

    // Шаг 3: Диагностика (нужно подключить прокси)
    // Создаем простой движок для диагностики
    std::unique_ptr<DiagnosticEngine> stateEngine =
        std::make_unique<DiagnosticEngine>(
            std::make_unique<WeightedDiagnostic>(true),
            &directory
            );
    stateEngine->addChecker(std::make_unique<CriticalSymptomChecker>());

    // Оборачиваем в прокси
    DiagnosticCacheProxy stateProxy(std::move(stateEngine), 10);

    // Создаем временный Appointment для передачи в состояние диагностики
    std::unique_ptr<RegularPatient> statePatient = std::make_unique<RegularPatient>("Петрова А.С.", 42, "жен");
    Appointment stateAppointment(statePatient.get());
    stateAppointment.addSymptom("температура 39");
    stateAppointment.addSymptom("боль в горле");
    stateAppointment.addSymptom("головная боль");
    stateAppointment.addSymptom("слабость");

    // Настраиваем состояние диагностики
    diagnosisState->setDiagnosticProxy(&stateProxy);
    diagnosisState->setCurrentAppointment(&stateAppointment);

    // Выполняем диагностику
    patientContext->run();

    // Шаг 4: Рекомендации
    patientContext->run();

    // Шаг 5: Завершение
    patientContext->run();

    // Показываем финальную информацию
    std::cout << "\n=== Итоговая информация ===\n";
    patientContext->printAppointmentInfo();

    // Очистка памяти
    delete completedState;
    delete recommendationState;
    delete diagnosisState;
    delete examinationState;
    delete registrationState;
    delete patientContext;

    std::cout << "\n\n3. Momento\n\n";
    // Создаем Originator (историю диагнозов для пациента)
    DiagnosisHistory patientHistory("Петрова А.С.");
    std::cout << "Создана история диагностики для пациента: " << patientHistory.getPatientName() << "\n";

    // Создаем Caretaker (медицинскую карту)
    PatientMedicalRecord medicalRecord("P-001", 10);
    std::cout << "Создана медицинская карта ID: " << medicalRecord.getPatientId() << "\n\n";

    // Создаем пациента и приемы с разными симптомами
    std::unique_ptr<ChronicPatient> mementoPatient = std::make_unique<ChronicPatient>("Петрова А.С.", 42, "жен");
    mementoPatient->addChronicDisease("бронхиальная астма", "2018");

    // Визит 1: Первый визит с симптомами ОРВИ
    std::cout << "--- Визит 1: Симптомы ОРВИ ---\n";
    Appointment visit1(mementoPatient.get());
    visit1.addSymptom("кашель");
    visit1.addSymptom("насморк");
    visit1.addSymptom("температура 38");
    visit1.addSymptom("слабость");

    // Создаем простой диагностический движок
    std::unique_ptr<DiagnosticEngine> simpleEngine5 = std::make_unique<DiagnosticEngine>(
            std::make_unique<SimpleDiagnostic>(0.3f),
            &directory
            );
    simpleEngine5->addChecker(std::make_unique<CriticalSymptomChecker>());

    // Выполняем диагностику
    std::unique_ptr<DiagnosisResult> resultVisit1 = simpleEngine5->diagnose(&visit1);

    // Создаем снимок
    std::unique_ptr<DiagnosisMemento> snapshot1 = patientHistory.createSnapshot(&visit1, resultVisit1.get(), "SimpleDiagnostic");

    // Сохраняем снимок в медицинскую карту
    medicalRecord.addSnapshot(std::move(snapshot1));
    std::cout << "Снимок #1 сохранен\n\n";

    // Визит 2: Состояние ухудшилось (симптомы гриппа)
    std::cout << "--- Визит 2: Симптомы ухудшились (грипп) ---\n";
    Appointment visit2(mementoPatient.get());
    visit2.addSymptom("температура 39");
    visit2.addSymptom("озноб");
    visit2.addSymptom("головная боль");
    visit2.addSymptom("ломота в теле");
    visit2.addSymptom("кашель");
    visit2.addSymptom("слабость");

    // Создаем взвешенный движок
    std::unique_ptr<DiagnosticEngine> weightedEngine5 = std::make_unique<DiagnosticEngine>(
            std::make_unique<WeightedDiagnostic>(true),
            &directory
            );
    weightedEngine5->addChecker(std::make_unique<CriticalSymptomChecker>());

    std::unique_ptr<EpidemiologicalChecker> epiChecker5 = std::make_unique<EpidemiologicalChecker>();
    epiChecker5->setSeason("winter");
    weightedEngine5->addChecker(std::move(epiChecker5));

    // Выполняем диагностику
    std::unique_ptr<DiagnosisResult> resultVisit2 = weightedEngine5->diagnose(&visit2);

    // Создаем снимок
    std::unique_ptr<DiagnosisMemento> snapshot2 = patientHistory.createSnapshot(&visit2, resultVisit2.get(), "WeightedDiagnostic");

    // Сохраняем снимок
    medicalRecord.addSnapshot(std::move(snapshot2));
    std::cout << "Снимок #2 сохранен\n\n";

    // Визит 3: Контрольный осмотр (симптомы ослабли)
    std::cout << "--- Визит 3: Контрольный осмотр ---\n";
    Appointment visit3(mementoPatient.get());
    visit3.addSymptom("кашель");
    visit3.addSymptom("слабость");

    // Создаем байесовский движок
    std::unique_ptr<BayesianDiagnostic> bayesian5 = std::make_unique<BayesianDiagnostic>(0.01f);
    bayesian5->setPriorProbability("ОРВИ", 0.3f);
    bayesian5->setPriorProbability("Грипп", 0.2f);

    std::unique_ptr<DiagnosticEngine> bayesianEngine5 = std::make_unique<DiagnosticEngine>(
            std::move(bayesian5),
            &directory
            );

    // Выполняем диагностику
    std::unique_ptr<DiagnosisResult> resultVisit3 =
        bayesianEngine5->diagnose(&visit3);

    // Создаем снимок
    std::unique_ptr<DiagnosisMemento> snapshot3 =
        patientHistory.createSnapshot(&visit3, resultVisit3.get(), "BayesianDiagnostic");

    // Сохраняем снимок
    medicalRecord.addSnapshot(std::move(snapshot3));
    std::cout << "Снимок #3 сохранен\n\n";

    // 1. Просмотр истории
    std::cout << "\n=== 1. Просмотр полной истории ===\n";
    medicalRecord.printHistory();

    // 2. Просмотр краткой истории
    std::cout << "\n=== 2. Краткая история ===\n";
    medicalRecord.printBriefHistory();

    // 3. Сравнение первого и последнего снимков
    std::cout << "\n=== 3. Сравнение первого и последнего снимков ===\n";
    DiagnosisMemento* firstSnapshot = medicalRecord.getFirstSnapshot();
    DiagnosisMemento* lastSnapshot = medicalRecord.getLastSnapshot();
    DiagnosisHistory::compareSnapshots(firstSnapshot, lastSnapshot);

    // 4. Восстановление из снимка (просмотр старого диагноза)
    std::cout << "\n=== 4. Восстановление данных из снимка #1 ===\n";
    DiagnosisMemento* snapshotToRestore = medicalRecord.getSnapshot(0);
    if (snapshotToRestore) {
        patientHistory.restoreFromSnapshot(snapshotToRestore);
    }

    // 5. Хронология диагнозов
    std::cout << "\n=== 5. Хронология диагнозов ===\n";
    std::vector<std::string> timeline = medicalRecord.getDiagnosisTimeline();
    for (size_t i = 0; i < timeline.size(); ++i) {
        std::cout << timeline[i] << "\n";
    }


    std::cout << "\n\n4. Observer\n\n";

    // 1. Создаем субъект наблюдения — монитор пациента
    PatientMonitor patientMonitor("Петрова А.С.");
    std::cout << "\n";
    // 2. Создаем наблюдателей
    AlertSystem alertSystem;
    StatisticService statisticService;
    LoggingService loggingService;

    // 3. Подписываем наблюдателей на монитор
    std::cout << "\n=== Подписка наблюдателей ===\n";
    patientMonitor.attach(&alertSystem);
    patientMonitor.attach(&statisticService);
    patientMonitor.attach(&loggingService);
    std::cout << "Подписано наблюдателей: " << patientMonitor.getObserverCount() << "\n\n";

    // 4. Создаем пациента и диагностические системы
    std::unique_ptr<RegularPatient> observerPatient = std::make_unique<RegularPatient>("Петрова А.С.", 42, "жен");

    DiseaseDirectory observerDirectory;
    setupDiseaseDirectory(observerDirectory);

    // Создаем разные диагностические движки
    std::unique_ptr<DiagnosticEngine> simpleDiagEngine = std::make_unique<DiagnosticEngine>(
            std::make_unique<SimpleDiagnostic>(0.3f),
            &observerDirectory
            );
    simpleDiagEngine->addChecker(std::make_unique<CriticalSymptomChecker>());

    std::unique_ptr<DiagnosticEngine> weightedDiagEngine = std::make_unique<DiagnosticEngine>(
            std::make_unique<WeightedDiagnostic>(true),
            &observerDirectory
            );
    weightedDiagEngine->addChecker(std::make_unique<CriticalSymptomChecker>());

    std::cout << "=== Симуляция приемов пациента ===\n\n";

    std::cout << "--- Прием 1: Симптомы ОРВИ ---\n";
    Appointment obsVisit1(observerPatient.get());
    obsVisit1.addSymptom("кашель");
    obsVisit1.addSymptom("насморк");
    obsVisit1.addSymptom("слабость");

    std::unique_ptr<DiagnosisResult> obsResult1 = simpleDiagEngine->diagnose(&obsVisit1);

    if (obsResult1 && !obsResult1->getCandidates().empty()) {
        DiagnosisCandidate best1 = obsResult1->getBestCandidate();
        patientMonitor.setDiagnosis(
            best1.getDisease()->getName(),
            best1.getProbability()
        );
    }

    // Проверяем предупреждения
    if (obsResult1 && obsResult1->hasWarnings()) {
        std::vector<std::string> warnings = obsResult1->getWarnings();
        for (size_t i = 0; i < warnings.size(); ++i) {
            patientMonitor.detectCriticalCondition(warnings[i]);
        }
    }

    patientMonitor.completeAppointment(patientMonitor.getCurrentDiagnosis());
    std::cout << "\n";

    std::cout << "--- Прием 2: Ухудшение (грипп) ---\n";
    Appointment obsVisit2(observerPatient.get());
    obsVisit2.addSymptom("температура 39");
    obsVisit2.addSymptom("озноб");
    obsVisit2.addSymptom("головная боль");
    obsVisit2.addSymptom("ломота в теле");
    obsVisit2.addSymptom("кашель");
    obsVisit2.addSymptom("слабость");

    std::unique_ptr<DiagnosisResult> obsResult2 = weightedDiagEngine->diagnose(&obsVisit2);

    if (obsResult2 && !obsResult2->getCandidates().empty()) {
        DiagnosisCandidate best2 = obsResult2->getBestCandidate();
        patientMonitor.setDiagnosis(
            best2.getDisease()->getName(),
            best2.getProbability()
        );
    }

    if (obsResult2 && obsResult2->hasWarnings()) {
        std::vector<std::string> warnings = obsResult2->getWarnings();
        for (size_t i = 0; i < warnings.size(); ++i) {
            patientMonitor.detectCriticalCondition(warnings[i]);
        }
    }

    patientMonitor.completeAppointment(patientMonitor.getCurrentDiagnosis());
    std::cout << "\n";

    std::cout << "--- Прием 3: Критическое состояние ---\n";
    Appointment obsVisit3(observerPatient.get());
    obsVisit3.addSymptom("кашель");
    obsVisit3.addSymptom("температура 39");
    obsVisit3.addSymptom("одышка");
    obsVisit3.addSymptom("боль в груди");
    obsVisit3.addSymptom("слабость");

    std::unique_ptr<DiagnosisResult> obsResult3 = weightedDiagEngine->diagnose(&obsVisit3);

    if (obsResult3 && !obsResult3->getCandidates().empty()) {
        DiagnosisCandidate best3 = obsResult3->getBestCandidate();
        patientMonitor.setDiagnosis(
            best3.getDisease()->getName(),
            best3.getProbability()
        );
    }

    // Симулируем критическое состояние
    patientMonitor.detectCriticalCondition("высокая температура >39");
    patientMonitor.detectCriticalCondition("острая боль в груди");

    patientMonitor.completeAppointment(patientMonitor.getCurrentDiagnosis());
    std::cout << "\n";

    // 1. Информация монитора
    std::cout << "\n=== 1. Информация монитора ===\n";
    patientMonitor.printMonitorInfo();

    // 2. Статистика диагнозов
    std::cout << "\n=== 2. Статистика ===\n";
    statisticService.printStatistics();

    std::cout << "Самый частый диагноз: " << statisticService.getMostCommonDiagnosis() << "\n";
    std::cout << "Всего диагнозов: " << statisticService.getTotalDiagnoses() << "\n";

    // 3. История тревог
    std::cout << "\n=== 3. История тревог ===\n";
    alertSystem.printAlertHistory();

    // 4. Журнал событий
    std::cout << "\n=== 4. Журнал событий ===\n";
    loggingService.printLastNLogs(5);

    // 5. Отписка наблюдателя
    std::cout << "\n=== 5. Отписка наблюдателя ===\n";
    patientMonitor.detach(&alertSystem);
    std::cout << "AlertSystem отписан. Осталось наблюдателей: " << patientMonitor.getObserverCount() << "\n";

    // Проверяем, что AlertSystem больше не получает оповещения
    std::cout << "\nПроверка: изменение диагноза после отписки AlertSystem\n";
    patientMonitor.setDiagnosis("ОРВИ", 0.85f);

    std::cout << "\n\nРабота системы завершена успешно\n";

    std::cin.get();

    return 0;
}