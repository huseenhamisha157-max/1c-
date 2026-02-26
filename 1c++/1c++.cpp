#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <ctime>

// Структура с 4 полями
struct Student {
    int id;                 // Поле 1: идентификатор студента
    char name[50];          // Поле 2: имя (массив символов для бинарной записи)
    int age;                // Поле 3: возраст
    double grade;           // Поле 4: оценка
};

// Функция для генерации случайных данных
Student generateRandomStudent(int index) {
    Student s;
    s.id = index + 1;  // Устанавливаем ID

    // Массив имен для случайного выбора
    std::string names[] = { "Ahmed", "Mohamed", "Sara", "Fatima", "Omar",
                           "Aisha", "Ali", "Noor", "Khaled", "Layla" };
    std::string nameStr = names[index % 10];  // Выбираем имя по индексу

    // Копируем строку в массив символов
    for (size_t i = 0; i < nameStr.length() && i < 49; i++) {
        s.name[i] = nameStr[i];
    }
    s.name[nameStr.length()] = '\0';  // Добавляем завершающий нуль

    // Генерируем случайные данные
    s.age = 18 + (rand() % 10);        // Возраст от 18 до 27
    s.grade = 60.0 + (rand() % 400) / 10.0;  // Оценка от 60.0 до 100.0

    return s;
}

// Функция для вывода заголовка таблицы
void printTableHeader() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << std::left
        << std::setw(10) << "ID"
        << std::setw(15) << "Имя"
        << std::setw(10) << "Возраст"
        << std::setw(15) << "Оценка"
        << std::endl;
    std::cout << std::string(60, '-') << std::endl;
}

// Функция для вывода одной записи
void printStudent(const Student& s) {
    std::cout << std::left
        << std::setw(10) << s.id
        << std::setw(15) << s.name
        << std::setw(10) << s.age
        << std::setw(15) << std::fixed << std::setprecision(2) << s.grade
        << std::endl;
}

int main() {
    // Инициализация генератора случайных чисел
    srand(time(nullptr));

    const std::string filename = "students.bin";  // Имя бинарного файла
    const int RECORDS_COUNT = 10;                  // Количество записей

    std::cout << "Программа управления записями студентов (Бинарный файл)" << std::endl;
    std::cout << "======================================================" << std::endl;

    // 1. СОЗДАНИЕ И ГЕНЕРАЦИЯ ДАННЫХ
    std::vector<Student> students;  // Вектор для хранения студентов

    // Цикл для создания 10 записей
    for (int i = 0; i < RECORDS_COUNT; i++) {
        students.push_back(generateRandomStudent(i));  // Добавляем случайного студента
    }

    // 2. ЗАПИСЬ В БИНАРНЫЙ ФАЙЛ
    std::cout << "\n1. Запись " << RECORDS_COUNT << " записей в файл..." << std::endl;

    // Открываем файл для бинарной записи
    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile) {
        std::cerr << "Ошибка открытия файла для записи!" << std::endl;
        return 1;
    }

    // Записываем каждую структуру в файл
    for (const auto& student : students) {
        outFile.write(reinterpret_cast<const char*>(&student), sizeof(Student));
    }

    outFile.close();  // Закрываем файл
    std::cout << "Запись успешно завершена!" << std::endl;

    // 3. ПОЛУЧЕНИЕ РАЗМЕРА ФАЙЛА
    std::ifstream fileSizeCheck(filename, std::ios::binary | std::ios::ate);
    if (fileSizeCheck) {
        int fileSize = fileSizeCheck.tellg();  // Получаем размер файла
        std::cout << "Размер файла: " << fileSize << " байт" << std::endl;
        std::cout << "Размер одной записи: " << sizeof(Student) << " байт" << std::endl;
        fileSizeCheck.close();  // Закрываем файл
    }

    // 4. ЧТЕНИЕ ИЗ БИНАРНОГО ФАЙЛА
    std::cout << "\n2. Чтение данных из файла..." << std::endl;

    // Открываем файл для бинарного чтения
    std::ifstream inFile(filename, std::ios::binary);
    if (!inFile) {
        std::cerr << "Ошибка открытия файла для чтения!" << std::endl;
        return 1;
    }

    std::vector<Student> readStudents;  // Вектор для прочитанных студентов
    Student temp;  // Временная переменная для чтения

    // Читаем записи пока не достигнем конца файла
    while (inFile.read(reinterpret_cast<char*>(&temp), sizeof(Student))) {
        readStudents.push_back(temp);  // Добавляем прочитанную запись
    }

    inFile.close();  // Закрываем файл

    // 5. ВЫВОД ДАННЫХ В КОНСОЛЬ
    std::cout << "\n3. Данные, прочитанные из файла:" << std::endl;
    printTableHeader();  // Выводим заголовок таблицы

    // Выводим каждую запись
    for (const auto& student : readStudents) {
        printStudent(student);
    }

    std::cout << std::string(60, '=') << std::endl;
    std::cout << "Количество прочитанных записей: " << readStudents.size() << std::endl;

    return 0;  // Успешное завершение программы
}