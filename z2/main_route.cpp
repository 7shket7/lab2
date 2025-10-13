#include <iostream>
#include <locale>
#include <codecvt>
#include <stdexcept>
#include "routeCipher.h"

using namespace std;

// Функции для преобразования string/wstring (единственное определение)
std::wstring string_to_wstring(const std::string& str) {
    try {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        return converter.from_bytes(str);
    } catch (const std::exception& e) {
        wcerr << L"Ошибка преобразования string в wstring: " << e.what() << endl;
        return L"";
    }
}

std::string wstring_to_string(const std::wstring& wstr) {
    try {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        return converter.to_bytes(wstr);
    } catch (const std::exception& e) {
        cerr << "Ошибка преобразования wstring в string: " << e.what() << endl;
        return "";
    }
}

void testRouteCipher()
{
    try {
        wcout << L"=== Тестирование шифра маршрутной перестановки ===" << endl;
        
        // Нормальные тесты
        wcout << L"\n--- Нормальные тесты ---" << endl;
        
        vector<pair<wstring, int>> normalTests = {
            {L"ПРИВЕТМИР", 4},
            {L"ПРОГРАММИРОВАНИЕ", 3},
            {L"КОМПЬЮТЕР", 5},
            {L"ШАШЛЫК", 2},
            {L"ТЕСТ", 1}
        };
        
        for (const auto& test : normalTests) {
            try {
                RouteCipher cipher(test.second);
                wstring encrypted = cipher.encrypt(test.first);
                wstring decrypted = cipher.decrypt(encrypted);
                
                wcout << L"Ключ: " << test.second << L", Текст: " << test.first << endl;
                wcout << L"Зашифровано: " << encrypted << L", Расшифровано: " << decrypted << endl;
                wcout << L"Результат: " << (test.first == decrypted ? L"✓ Успех" : L"✗ Ошибка") << endl << endl;
            } catch (const cipher_error& e) {
                wcout << L"✗ Ошибка: " << string_to_wstring(e.what()) << endl << endl;
            }
        }
        
        // Тесты с исключениями
        wcout << L"--- Тесты с исключениями ---" << endl;
        
        vector<pair<wstring, wstring>> exceptionTests = {
            {L"", L"4"}, // Пустой текст
            {L"ПРИВЕТ", L""}, // Пустой ключ
            {L"ПРИВЕТ", L"0"}, // Нулевой ключ
            {L"ПРИВЕТ", L"-5"}, // Отрицательный ключ
            {L"ПРИВЕТ", L"ABC"}, // Нечисловой ключ
            {L"123", L"3"}, // Текст без букв
            {L"ПРИВЕТ", L"1000"} // Слишком большой ключ
        };
        
        for (const auto& test : exceptionTests) {
            wcout << L"Тест: текст='" << test.first << L"', ключ='" << test.second << L"'" << endl;
            try {
                if (test.second.empty()) {
                    RouteCipher cipher(1); // Создаем с любым ключом
                    cipher.encrypt(test.first);
                } else {
                    RouteCipher cipher(test.second);
                    wstring encrypted = cipher.encrypt(test.first);
                    wcout << L"✗ Ошибка: исключение не сгенерировано!" << endl;
                }
            } catch (const cipher_error& e) {
                wcout << L"✓ Поймано исключение: " << string_to_wstring(e.what()) << endl;
            } catch (const std::exception& e) {
                wcout << L"✓ Поймано std исключение: " << e.what() << endl;
            }
            wcout << endl;
        }
        
    } catch (const exception& e) {
        wcout << L"Неожиданная ошибка в тестах: " << string_to_wstring(e.what()) << endl;
    }
}

void interactiveRouteCipher()
{
    try {
        wcout << L"\n=== Интерактивный режим ===" << endl;
        
        wstring keyStr;
        while (true) {
            wcout << L"Введите ключ (количество столбцов, положительное число): ";
            wcin >> keyStr;
            
            try {
                RouteCipher cipher(keyStr);
                wcout << L"Ключ установлен: " << keyStr << endl;
                break;
            } catch (const cipher_error& e) {
                wcout << L"Ошибка ключа: " << string_to_wstring(e.what()) << endl;
                wcout << L"Попробуйте снова." << endl;
            }
        }
        
        RouteCipher cipher(keyStr);
        int key = std::stoi(wstring_to_string(keyStr));
        
        int op;
        do {
            wcout << L"\nВыберите операцию:" << endl;
            wcout << L"0 - Выход" << endl;
            wcout << L"1 - Шифрование" << endl;
            wcout << L"2 - Расшифрование" << endl;
            wcout << L"Ваш выбор: ";
            
            if (!(wcin >> op)) {
                wcin.clear();
                wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
                wcout << L"Неверный ввод! Пожалуйста, введите число." << endl;
                continue;
            }
            
            if (op == 1 || op == 2) {
                wcout << L"Введите текст: ";
                wstring text;
                wcin.ignore(); // очищаем буфер
                getline(wcin, text);
                
                try {
                    if (op == 1) {
                        wstring result = cipher.encrypt(text);
                        wcout << L"Зашифрованный текст: " << result << endl;
                    } else {
                        wstring result = cipher.decrypt(text);
                        wcout << L"Расшифрованный текст: " << result << endl;
                    }
                } catch (const cipher_error& e) {
                    wcout << L"Ошибка при обработке: " << string_to_wstring(e.what()) << endl;
                } catch (const std::exception& e) {
                    wcout << L"Неожиданная ошибка: " << string_to_wstring(e.what()) << endl;
                }
            } else if (op != 0) {
                wcout << L"Неверная операция!" << endl;
            }
            
        } while (op != 0);
        
        wcout << L"Выход из программы." << endl;
        
    } catch (const exception& e) {
        wcout << L"Критическая ошибка: " << string_to_wstring(e.what()) << endl;
    }
}

void demonstrateAlgorithm()
{
    try {
        wcout << L"\n=== Демонстрация работы алгоритма ===" << endl;
        
        wstring text = L"ПРИВЕТМИР";
        int columns = 3;
        
        wcout << L"Исходный текст: " << text << endl;
        wcout << L"Количество столбцов: " << columns << endl;
        
        RouteCipher cipher(columns);
        wstring encrypted = cipher.encrypt(text);
        wstring decrypted = cipher.decrypt(encrypted);
        
        wcout << L"Зашифрованный текст: " << encrypted << endl;
        wcout << L"Расшифрованный текст: " << decrypted << endl;
        wcout << L"Совпадение: " << (text == decrypted ? L"✓ Да" : L"✗ Нет") << endl;
        
        // Демонстрация с ошибкой
        wcout << L"\n--- Демонстрация обработки ошибок ---" << endl;
        
        try {
            RouteCipher invalidCipher(0);
        } catch (const cipher_error& e) {
            wcout << L"Поймано исключение при создании с ключом 0: " << string_to_wstring(e.what()) << endl;
        }
        
        try {
            RouteCipher cipher2(2);
            cipher2.encrypt(L"123"); // Текст без букв
        } catch (const cipher_error& e) {
            wcout << L"Поймано исключение при шифровании чисел: " << string_to_wstring(e.what()) << endl;
        }
        
    } catch (const exception& e) {
        wcout << L"Ошибка в демонстрации: " << string_to_wstring(e.what()) << endl;
    }
}

int main()
{
    setlocale(LC_ALL, "ru_RU.UTF-8");
    
    int choice;
    do {
        wcout << L"\n=== Шифр маршрутной перестановки ===" << endl;
        wcout << L"1 - Автоматическое тестирование" << endl;
        wcout << L"2 - Интерактивный режим" << endl;
        wcout << L"3 - Демонстрация алгоритма" << endl;
        wcout << L"0 - Выход" << endl;
        wcout << L"Ваш выбор: ";
        
        if (!(wcin >> choice)) {
            wcin.clear();
            wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
            wcout << L"Неверный ввод! Пожалуйста, введите число от 0 до 3." << endl;
            continue;
        }
        
        switch (choice) {
            case 1:
                testRouteCipher();
                break;
            case 2:
                interactiveRouteCipher();
                break;
            case 3:
                demonstrateAlgorithm();
                break;
            case 0:
                wcout << L"До свидания!" << endl;
                break;
            default:
                wcout << L"Неверный выбор! Пожалуйста, введите число от 0 до 3." << endl;
        }
    } while (choice != 0);
    
    return 0;
}
