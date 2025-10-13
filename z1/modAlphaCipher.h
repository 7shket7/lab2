#pragma once
#include <vector>
#include <string>
#include <map>
#include <locale>
#include <codecvt>
#include <stdexcept>

// Вспомогательные функции для конвертации
std::string wstring_to_string(const std::wstring& wstr);
std::wstring string_to_wstring(const std::string& str);

// Классы исключений
class cipher_error : public std::invalid_argument {
public:
    explicit cipher_error (const std::string& what_arg) :
        std::invalid_argument(what_arg) {}
    explicit cipher_error (const char* what_arg) :
        std::invalid_argument(what_arg) {}
};

class modAlphaCipher
{
private:
    std::wstring numAlpha = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    std::map<wchar_t, int> alphaNum;
    std::vector<int> key;
    
    std::vector<int> convert(const std::wstring& s);
    std::wstring convert(const std::vector<int>& v);
    std::wstring getValidKey(const std::wstring& s);
    std::wstring getValidOpenText(const std::wstring& s);
    std::wstring getValidCipherText(const std::wstring& s);

public:
    modAlphaCipher() = delete;
    modAlphaCipher(const std::wstring& skey);
    
    std::wstring encrypt(const std::wstring& open_text);
    std::wstring decrypt(const std::wstring& cipher_text);
};
