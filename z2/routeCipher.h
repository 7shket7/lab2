#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <locale>
#include <codecvt>
#include <stdexcept>

// Класс исключений для шифра
class cipher_error : public std::invalid_argument {
public:
    explicit cipher_error(const std::string& what_arg) : std::invalid_argument(what_arg) {}
    explicit cipher_error(const char* what_arg) : std::invalid_argument(what_arg) {}
};

class RouteCipher
{
private:
    int key; // количество столбцов
    
    std::wstring getValidKey(const std::wstring& s);
    std::wstring getValidOpenText(const std::wstring& s);
    std::wstring getValidCipherText(const std::wstring& s);

public:
    RouteCipher() = delete;
    RouteCipher(int k);
    RouteCipher(const std::wstring& skey);
    
    std::wstring encrypt(const std::wstring& open_text);
    std::wstring decrypt(const std::wstring& cipher_text);
};

// Объявления функций конвертации
std::string wstring_to_string(const std::wstring& wstr);
std::wstring string_to_wstring(const std::string& str);
