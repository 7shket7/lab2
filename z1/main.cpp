#include <iostream>
#include <locale>
#include "modAlphaCipher.h"

// Простые функции для работы с русским текстом
std::wstring toUpperRussian(const std::wstring& s) {
    std::wstring result = s;
    for (wchar_t& c : result) {
        if (c >= L'а' && c <= L'я') {
            c = c - L'а' + L'А';
        } else if (c == L'ё') {
            c = L'Ё';
        }
    }
    return result;
}

bool isRussianText(const std::wstring& s) {
    for (wchar_t c : s) {
        if (!((c >= L'А' && c <= L'Я') || c == L'Ё' || 
              (c >= L'а' && c <= L'я') || c == L'ё' || c == L' ')) {
            return false;
        }
    }
    return true;
}

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    
    std::wcout << L"Шифр Гронсфельда для русского алфавита" << std::endl;
    std::wcout << L"======================================" << std::endl;
    
    // Тестирование с обработкой исключений
    try {
        // Тест 1: Нормальная работа
        modAlphaCipher cipher1(L"КЛЮЧ");
        std::wstring text1 = L"ПРИВЕТМИР";
        std::wstring encrypted1 = cipher1.encrypt(text1);
        std::wstring decrypted1 = cipher1.decrypt(encrypted1);
        
        std::wcout << L"Тест 1 - Нормальная работа:" << std::endl;
        std::wcout << L"Исходный: " << text1 << std::endl;
        std::wcout << L"Зашифрованный: " << encrypted1 << std::endl;
        std::wcout << L"Расшифрованный: " << decrypted1 << std::endl;
        std::wcout << (text1 == decrypted1 ? L"OK" : L"Ошибка") << std::endl << std::endl;
        
    } catch (const cipher_error& e) {
        std::wcout << L"Ошибка: " << e.what() << std::endl << std::endl;
    }
    
    // Тест 2: Пустой ключ
    try {
        modAlphaCipher cipher2(L"");
        std::wcout << L"Тест 2 - Ошибка: конструктор принял пустой ключ" << std::endl;
    } catch (const cipher_error& e) {
        std::wcout << L"Тест 2 - Пустой ключ: " << e.what() << std::endl << std::endl;
    }
    
    // Тест 3: Ключ с цифрами
    try {
        modAlphaCipher cipher3(L"КЛЮЧ123");
        std::wcout << L"Тест 3 - Ошибка: конструктор принял ключ с цифрами" << std::endl;
    } catch (const cipher_error& e) {
        std::wcout << L"Тест 3 - Ключ с цифрами: " << e.what() << std::endl << std::endl;
    }
    
    // Тест 4: Пустой текст при шифровании
    try {
        modAlphaCipher cipher4(L"КЛЮЧ");
        std::wstring empty = cipher4.encrypt(L"");
        std::wcout << L"Тест 4 - Ошибка: encrypt принял пустой текст" << std::endl;
    } catch (const cipher_error& e) {
        std::wcout << L"Тест 4 - Пустой текст: " << e.what() << std::endl << std::endl;
    }
    
    // Тест 5: Текст с пробелами и знаками (должен работать при шифровании)
    try {
        modAlphaCipher cipher5(L"КЛЮЧ");
        std::wstring text5 = L"привет, мир! 123";
        std::wstring encrypted5 = cipher5.encrypt(text5);
        std::wstring decrypted5 = cipher5.decrypt(encrypted5);
        
        std::wcout << L"Тест 5 - Текст с пробелами и знаками:" << std::endl;
        std::wcout << L"Исходный: " << text5 << std::endl;
        std::wcout << L"Зашифрованный: " << encrypted5 << std::endl;
        std::wcout << L"Расшифрованный: " << decrypted5 << std::endl;
        std::wcout << L"OK" << std::endl << std::endl;
        
    } catch (const cipher_error& e) {
        std::wcout << L"Ошибка: " << e.what() << std::endl << std::endl;
    }
    
    // Тест 6: Испорченный зашифрованный текст
    try {
        modAlphaCipher cipher6(L"КЛЮЧ");
        std::wstring bad_text = L"ПРИВЕТ123"; // цифры в зашифрованном тексте
        std::wstring decrypted6 = cipher6.decrypt(bad_text);
        std::wcout << L"Тест 6 - Ошибка: decrypt принял текст с цифрами" << std::endl;
    } catch (const cipher_error& e) {
        std::wcout << L"Тест 6 - Испорченный текст: " << e.what() << std::endl << std::endl;
    }
    
    // Интерактивный режим
    std::wcout << L"Интерактивный режим:" << std::endl;
    
    std::wstring key;
    std::wcout << L"Введите ключ (русские буквы): ";
    std::wcin >> key;
    key = toUpperRussian(key);
    
    if (!isRussianText(key) || key.empty()) {
        std::wcout << L"Неверный ключ!" << std::endl;
        return 1;
    }
    
    try {
        modAlphaCipher cipher(key);
        
        int op;
        do {
            std::wcout << L"\nВыберите операцию (0-выход, 1-шифрование, 2-расшифрование): ";
            std::wcin >> op;
            std::wcin.ignore();
            
            if (op == 1 || op == 2) {
                std::wstring text;
                std::wcout << L"Введите текст: ";
                std::getline(std::wcin, text);
                
                try {
                    if (op == 1) {
                        std::wstring encrypted = cipher.encrypt(text);
                        std::wcout << L"Зашифрованный текст: " << encrypted << std::endl;
                    } else {
                        std::wstring decrypted = cipher.decrypt(text);
                        std::wcout << L"Расшифрованный текст: " << decrypted << std::endl;
                    }
                } catch (const cipher_error& e) {
                    std::wcout << L"Ошибка: " << e.what() << std::endl;
                }
            }
        } while (op != 0);
        
    } catch (const cipher_error& e) {
        std::wcout << L"Ошибка создания шифра: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
