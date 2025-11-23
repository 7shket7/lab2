#include <iostream>
#include <locale>
#include <codecvt>
#include "RouteCipher.h"
using namespace std;

bool isValid(const wstring& s)
{
    wstring russianAlphabet = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ ";
    for (auto c : s) {
        if (russianAlphabet.find(c) == wstring::npos) {
            return false;
        }
    }
    return true;
}

wstring toUpperRussian(const wstring& s)
{
    wstring result = s;
    wstring lower = L"абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
    wstring upper = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    
    for (auto& c : result) {
        size_t pos = lower.find(c);
        if (pos != wstring::npos) {
            c = upper[pos];
        }
    }
    return result;
}

wstring stringToWstring(const string& str)
{
    wstring_convert<codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(str);
}

string wstringToString(const wstring& wstr)
{
    wstring_convert<codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(wstr);
}

void testScenarios() {
    cout << "=== Тестирование обработки исключений ===" << endl;
    // Тест 1: Неверный ключ (отрицательное число)
    try {
        RouteCipher cipher2(-5);
        cout << "Тест 1 - Ошибка: конструктор принял отрицательный ключ" << endl;
    } catch (const cipher_error& e) {
        cout << "Тест 1 - Отрицательный ключ: " << e.what() << endl << endl;
    }
    
    // Тест 2: Неверный ключ (ноль)
    try {
        RouteCipher cipher3(0);
        cout << "Тест 2 - Ошибка: конструктор принял нулевой ключ" << endl;
    } catch (const cipher_error& e) {
        cout << "Тест 2 - Нулевой ключ: " << e.what() << endl << endl;
    }
    
    // Тест 3: Пустой текст при шифровании
    try {
        RouteCipher cipher4(3);
        wstring empty = cipher4.encrypt(L"");
        cout << "Тест 3 - Ошибка: encrypt принял пустой текст" << endl;
    } catch (const cipher_error& e) {
        cout << "Тест 3 - Пустой текст: " << e.what() << endl << endl;
    }
    
    // Тест 4: Текст с недопустимыми символами
    try {
        RouteCipher cipher5(3);
        wstring bad_text = cipher5.encrypt(L"ПРИВЕТ123"); // цифры
        cout << "Тест 4 - Ошибка: encrypt принял текст с цифрами" << endl;
    } catch (const cipher_error& e) {
        cout << "Тест 4 - Текст с цифрами: " << e.what() << endl << endl;
    }
    
    // Тест 5: Текст только из пробелов
    try {
        RouteCipher cipher6(3);
        wstring spaces = cipher6.encrypt(L"   ");
        cout << "Тест 5 - Ошибка: encrypt принял текст только из пробелов" << endl;
    } catch (const cipher_error& e) {
        cout << "Тест 5 - Текст только из пробелов: " << e.what() << endl << endl;
    }
    
    // Тест 6: Слишком большой ключ
    try {
        RouteCipher cipher7(1000);
        cout << "Тест 6 - Ошибка: конструктор принял слишком большой ключ" << endl;
    } catch (const cipher_error& e) {
        cout << "Тест 6 - Слишком большой ключ: " << e.what() << endl << endl;
    }
}

int main()
{
    setlocale(LC_ALL, "ru_RU.UTF-8");
    
    // Запуск тестов
    testScenarios();
    
    // Интерактивный режим
    cout << "=== Интерактивный режим ===" << endl;
    
    int key_input;
    string text_input;
    unsigned op;
    
    cout << "Шифр готов. Введите ключ (количество столбцов): ";
    cin >> key_input;
    cin.ignore();
    
    try {
        RouteCipher cipher(key_input);
        cout << "Ключ загружен" << endl;
        
        do {
            cout << "Шифр готов. Выберите операцию (0-выход, 1-шифрование, 2-расшифрование): ";
            cin >> op;
            cin.ignore();
            
            if (op > 2) {
                cout << "Неверная операция" << endl;
            } else if (op > 0) {
                cout << "Введите текст: ";
                getline(cin, text_input);
                wstring text = stringToWstring(text_input);
                text = toUpperRussian(text);
                
                try {
                    if (op == 1) {
                        wstring encrypted = cipher.encrypt(text);
                        cout << "Зашифрованный текст: " << wstringToString(encrypted) << endl;
                    } else {
                        wstring decrypted = cipher.decrypt(text);
                        cout << "Расшифрованный текст: " << wstringToString(decrypted) << endl;
                    }
                } catch (const cipher_error& e) {
                    cout << "Ошибка операции: " << e.what() << endl;
                }
            }
        } while (op != 0);
        
    } catch (const cipher_error& e) {
        cout << "Ошибка инициализации: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}
