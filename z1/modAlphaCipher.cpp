#include "modAlphaCipher.h"
#include <iostream>
#include <cwctype>

modAlphaCipher::modAlphaCipher(const std::wstring& skey) {
    // Инициализируем алфавит
    for (size_t i = 0; i < numAlpha.size(); i++) {
        alphaNum[numAlpha[i]] = i;
    }
    key = convert(getValidKey(skey));
}

std::wstring modAlphaCipher::getValidKey(const std::wstring& s) {
    if (s.empty()) {
        throw cipher_error("Пустой ключ");
    }
    
    std::wstring tmp;
    for (wchar_t c : s) {
        if (std::iswalpha(c)) {
            tmp += std::towupper(c);
        } else {
            throw cipher_error("Ключ содержит недопустимые символы");
        }
    }
    return tmp;
}

std::wstring modAlphaCipher::getValidText(const std::wstring& s, bool isEncrypt) {
    if (s.empty()) {
        throw cipher_error("Пустой текст");
    }
    
    std::wstring tmp;
    for (wchar_t c : s) {
        if (std::iswalpha(c)) {
            tmp += std::towupper(c);
        } else if (!isEncrypt) {
            // При расшифровании не допускаем не-буквы
            throw cipher_error("Текст содержит недопустимые символы");
        }
        // При шифровании просто пропускаем не-буквы
    }
    
    if (tmp.empty()) {
        throw cipher_error("Нет букв в тексте");
    }
    
    return tmp;
}

std::vector<int> modAlphaCipher::convert(const std::wstring& s) {
    std::vector<int> result;
    for (wchar_t c : s) {
        result.push_back(alphaNum.at(c));
    }
    return result;
}

std::wstring modAlphaCipher::convert(const std::vector<int>& v) {
    std::wstring result;
    for (int i : v) {
        result += numAlpha[i];
    }
    return result;
}

std::wstring modAlphaCipher::encrypt(const std::wstring& open_text) {
    std::wstring valid_text = getValidText(open_text, true);
    std::vector<int> work = convert(valid_text);
    
    for (size_t i = 0; i < work.size(); i++) {
        work[i] = (work[i] + key[i % key.size()]) % numAlpha.size();
    }
    
    return convert(work);
}

std::wstring modAlphaCipher::decrypt(const std::wstring& cipher_text) {
    std::wstring valid_text = getValidText(cipher_text, false);
    std::vector<int> work = convert(valid_text);
    
    for (size_t i = 0; i < work.size(); i++) {
        work[i] = (work[i] + numAlpha.size() - key[i % key.size()]) % numAlpha.size();
    }
    
    return convert(work);
}
