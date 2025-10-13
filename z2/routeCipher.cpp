#include "routeCipher.h"
#include <iostream>
#include <stdexcept>

// Используем внешние функции конвертации (определены в main_route.cpp)
extern std::string wstring_to_string(const std::wstring& wstr);
extern std::wstring string_to_wstring(const std::string& str);

RouteCipher::RouteCipher(int k)
{
    try {
        if (k <= 0) {
            throw cipher_error("Ключ должен быть положительным числом");
        }
        key = k;
    } catch (const cipher_error& e) {
        throw;
    } catch (const std::exception& e) {
        throw cipher_error("Ошибка при создании шифра с числовым ключом");
    }
}

RouteCipher::RouteCipher(const std::wstring& skey)
{
    try {
        std::wstring validKey = getValidKey(skey);
        key = std::stoi(wstring_to_string(validKey));
    } catch (const cipher_error& e) {
        throw;
    } catch (const std::invalid_argument& e) {
        throw cipher_error("Недопустимый формат ключа");
    } catch (const std::out_of_range& e) {
        throw cipher_error("Ключ слишком большой");
    } catch (const std::exception& e) {
        throw cipher_error("Ошибка при создании шифра со строковым ключом");
    }
}

std::wstring RouteCipher::encrypt(const std::wstring& open_text)
{
    try {
        std::wstring text = getValidOpenText(open_text);
        
        // Определяем количество строк
        int textLength = text.length();
        int rows = (textLength + key - 1) / key;
        
        // Проверка на минимальный размер таблицы
        if (rows == 0 || key == 0) {
            throw cipher_error("Недопустимый размер таблицы для шифрования");
        }
        
        // Создаем таблицу
        std::vector<std::vector<wchar_t>> table(rows, std::vector<wchar_t>(key, L' '));
        
        // Записываем текст по горизонтали слева направо, сверху вниз
        int index = 0;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < key; j++) {
                if (index < textLength) {
                    table[i][j] = text[index++];
                }
            }
        }
        
        // Считываем по вертикали сверху вниз, справа налево
        std::wstring result;
        for (int j = key - 1; j >= 0; j--) {
            for (int i = 0; i < rows; i++) {
                if (table[i][j] != L' ') {
                    result += table[i][j];
                }
            }
        }
        
        if (result.empty()) {
            throw cipher_error("Результат шифрования пуст");
        }
        
        return result;
        
    } catch (const cipher_error& e) {
        throw;
    } catch (const std::bad_alloc& e) {
        throw cipher_error("Недостаточно памяти для создания таблицы");
    } catch (const std::exception& e) {
        throw cipher_error("Ошибка при шифровании");
    }
}

std::wstring RouteCipher::decrypt(const std::wstring& cipher_text)
{
    try {
        std::wstring text = getValidCipherText(cipher_text);
        
        int textLength = text.length();
        int rows = (textLength + key - 1) / key;
        
        // Проверка на минимальный размер таблицы
        if (rows == 0 || key == 0) {
            throw cipher_error("Недопустимый размер таблицы для расшифрования");
        }
        
        // Проверка корректности данных для таблицы
        if (rows * key < textLength) {
            throw cipher_error("Некорректные данные для расшифрования");
        }
        
        // Создаем пустую таблицу
        std::vector<std::vector<wchar_t>> table(rows, std::vector<wchar_t>(key, L' '));
        
        // Заполняем таблицу по вертикали сверху вниз, справа налево
        int index = 0;
        for (int j = key - 1; j >= 0; j--) {
            for (int i = 0; i < rows; i++) {
                if (index < textLength) {
                    table[i][j] = text[index++];
                }
            }
        }
        
        // Считываем по горизонтали слева направо, сверху вниз
        std::wstring result;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < key; j++) {
                if (table[i][j] != L' ') {
                    result += table[i][j];
                }
            }
        }
        
        if (result.empty()) {
            throw cipher_error("Результат расшифрования пуст");
        }
        
        return result;
        
    } catch (const cipher_error& e) {
        throw;
    } catch (const std::bad_alloc& e) {
        throw cipher_error("Недостаточно памяти для создания таблицы");
    } catch (const std::exception& e) {
        throw cipher_error("Ошибка при расшифровании");
    }
}

std::wstring RouteCipher::getValidKey(const std::wstring& s)
{
    if (s.empty()) {
        throw cipher_error("Пустой ключ");
    }
    
    // Проверка на допустимые символы
    for (wchar_t c : s) {
        if (!iswdigit(c)) {
            throw cipher_error("Ключ должен содержать только цифры");
        }
    }
    
    // Проверка на наличие только нулей
    bool allZeros = true;
    for (wchar_t c : s) {
        if (c != L'0') {
            allZeros = false;
            break;
        }
    }
    if (allZeros) {
        throw cipher_error("Ключ не может состоять только из нулей");
    }
    
    try {
        // Преобразуем wstring в string для stoi
        std::string keyStr = wstring_to_string(s);
        int k = std::stoi(keyStr);
        
        if (k <= 0) {
            throw cipher_error("Ключ должен быть положительным числом");
        }
        
        // Проверка на слишком большой ключ
        if (k > 1000) {
            throw cipher_error("Слишком большой ключ");
        }
        
    } catch (const std::invalid_argument& e) {
        throw cipher_error("Недопустимый формат ключа");
    } catch (const std::out_of_range& e) {
        throw cipher_error("Ключ слишком большой");
    }
    
    return s;
}

std::wstring RouteCipher::getValidOpenText(const std::wstring& s)
{
    std::wstring result;
    
    // Проверка на пустую строку
    if (s.empty()) {
        throw cipher_error("Пустой открытый текст");
    }
    
    // Фильтрация и преобразование символов
    bool hasValidChars = false;
    for (wchar_t c : s) {
        if (iswalpha(c)) {
            hasValidChars = true;
            // Преобразуем в верхний регистр для русского языка
            if (iswlower(c)) {
                result += towupper(c);
            } else {
                result += c;
            }
        } else if (!iswspace(c)) {
            // Игнорируем пробелы, но сообщаем о других недопустимых символах
            throw cipher_error("Открытый текст содержит недопустимые символы");
        }
    }
    
    if (!hasValidChars) {
        throw cipher_error("Открытый текст не содержит буквенных символов");
    }
    
    if (result.empty()) {
        throw cipher_error("После фильтрации открытый текст стал пустым");
    }
    
    // Проверка на минимальную длину
    if (result.length() < 2) {
        throw cipher_error("Открытый текст слишком короткий для шифрования");
    }
    
    return result;
}

std::wstring RouteCipher::getValidCipherText(const std::wstring& s)
{
    if (s.empty()) {
        throw cipher_error("Пустой зашифрованный текст");
    }
    
    // Проверка на допустимые символы
    bool hasValidChars = false;
    for (wchar_t c : s) {
        if (iswalpha(c)) {
            hasValidChars = true;
        } else if (!iswspace(c)) {
            throw cipher_error("Зашифрованный текст содержит недопустимые символы");
        }
    }
    
    if (!hasValidChars) {
        throw cipher_error("Зашифрованный текст не содержит буквенных символов");
    }
    
    // Преобразуем в верхний регистр
    std::wstring result;
    for (wchar_t c : s) {
        if (iswalpha(c)) {
            if (iswlower(c)) {
                result += towupper(c);
            } else {
                result += c;
            }
        }
    }
    
    if (result.empty()) {
        throw cipher_error("После фильтрации зашифрованный текст стал пустым");
    }
    
    // Проверка на минимальную длину
    if (result.length() < 2) {
        throw cipher_error("Зашифрованный текст слишком короткий для расшифрования");
    }
    
    return result;
}
