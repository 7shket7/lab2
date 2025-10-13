#include "modAlphaCipher.h"
#include <iostream>
#include <stdexcept>

// Реализация вспомогательных функций
std::string wstring_to_string(const std::wstring& wstr) {
    try {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        return converter.to_bytes(wstr);
    } catch (const std::exception& e) {
        throw cipher_error("Ошибка преобразования wstring в string");
    }
}

std::wstring string_to_wstring(const std::string& str) {
    try {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        return converter.from_bytes(str);
    } catch (const std::exception& e) {
        throw cipher_error("Ошибка преобразования string в wstring");
    }
}

modAlphaCipher::modAlphaCipher(const std::wstring& skey)
{
    try {
        for (unsigned i = 0; i < numAlpha.size(); i++) {
            alphaNum[numAlpha[i]] = i;
        }
        key = convert(getValidKey(skey));
    } catch (const cipher_error& e) {
        throw;
    } catch (const std::exception& e) {
        throw cipher_error("Ошибка при создании шифра");
    }
}

std::wstring modAlphaCipher::encrypt(const std::wstring& open_text)
{
    try {
        std::vector<int> work = convert(getValidOpenText(open_text));
        for (unsigned i = 0; i < work.size(); i++) {
            work[i] = (work[i] + key[i % key.size()]) % numAlpha.size();
        }
        return convert(work);
    } catch (const cipher_error& e) {
        throw;
    } catch (const std::exception& e) {
        throw cipher_error("Ошибка при шифровании");
    }
}

std::wstring modAlphaCipher::decrypt(const std::wstring& cipher_text)
{
    try {
        std::vector<int> work = convert(getValidCipherText(cipher_text));
        for (unsigned i = 0; i < work.size(); i++) {
            work[i] = (work[i] + numAlpha.size() - key[i % key.size()]) % numAlpha.size();
        }
        return convert(work);
    } catch (const cipher_error& e) {
        throw;
    } catch (const std::exception& e) {
        throw cipher_error("Ошибка при расшифровании");
    }
}

std::vector<int> modAlphaCipher::convert(const std::wstring& s)
{
    std::vector<int> result;
    for (auto c : s) {
        if (alphaNum.find(c) == alphaNum.end()) {
            throw cipher_error("Недопустимый символ при конвертации: " + wstring_to_string(std::wstring(1, c)));
        }
        result.push_back(alphaNum[c]);
    }
    return result;
}

std::wstring modAlphaCipher::convert(const std::vector<int>& v)
{
    std::wstring result;
    for (auto i : v) {
        if (i < 0 || i >= static_cast<int>(numAlpha.size())) {
            throw cipher_error("Недопустимый индекс при конвертации: " + std::to_string(i));
        }
        result.push_back(numAlpha[i]);
    }
    return result;
}

std::wstring modAlphaCipher::getValidKey(const std::wstring& s)
{
    if (s.empty()) {
        throw cipher_error("Пустой ключ");
    }
    
    std::wstring tmp(s);
    for (auto& c : tmp) {
        if (!iswalpha(c)) {
            throw cipher_error("Ключ содержит недопустимые символы");
        }
        if (iswlower(c)) {
            c = towupper(c);
        }
    }
    
    // Проверка, что все символы ключа есть в алфавите
    for (auto c : tmp) {
        if (numAlpha.find(c) == std::wstring::npos) {
            throw cipher_error("Ключ содержит символы не из русского алфавита");
        }
    }
    
    return tmp;
}

std::wstring modAlphaCipher::getValidOpenText(const std::wstring& s)
{
    std::wstring tmp;
    for (auto c : s) {
        if (iswalpha(c)) {
            if (iswlower(c)) {
                tmp.push_back(towupper(c));
            } else {
                tmp.push_back(c);
            }
        }
    }
    
    if (tmp.empty()) {
        throw cipher_error("Пустой открытый текст");
    }
    
    // Проверка, что все символы есть в алфавите
    for (auto c : tmp) {
        if (numAlpha.find(c) == std::wstring::npos) {
            throw cipher_error("Открытый текст содержит символы не из русского алфавита");
        }
    }
    
    return tmp;
}

std::wstring modAlphaCipher::getValidCipherText(const std::wstring& s)
{
    if (s.empty()) {
        throw cipher_error("Пустой зашифрованный текст");
    }
    
    for (auto c : s) {
        if (!iswupper(c)) {
            throw cipher_error("Зашифрованный текст содержит строчные символы");
        }
        if (numAlpha.find(c) == std::wstring::npos) {
            throw cipher_error("Зашифрованный текст содержит символы не из русского алфавита");
        }
    }
    
    return s;
}
