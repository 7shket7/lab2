#pragma once
#include <vector>
#include <string>
#include <locale>
#include <codecvt>
#include <stdexcept>
using namespace std;

class cipher_error : public invalid_argument {
public:
    explicit cipher_error(const string& what_arg):
        invalid_argument(what_arg) {}
    explicit cipher_error(const char* what_arg):
        invalid_argument(what_arg) {}
};

class RouteCipher
{
private:
    int columns; // количество столбцов таблицы
    void validateKey(int key);
    wstring validateText(const wstring& text);

public:
    RouteCipher() = delete; // запретим конструктор без параметров
    RouteCipher(int cols);
    wstring encrypt(const wstring& open_text); // зашифрование
    wstring decrypt(const wstring& cipher_text); // расшифрование
};
