#include "RouteCipher.h"
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <locale>
#include <codecvt>
using namespace std;

RouteCipher::RouteCipher(int cols)
{
    validateKey(cols);
    columns = cols;
}

void RouteCipher::validateKey(int key)
{
    if (key <= 0)
        throw cipher_error("Количество столбцов должно быть положительным");
    if (key > 100) // разумное ограничение
        throw cipher_error("Слишком большое количество столбцов");
}

wstring RouteCipher::validateText(const wstring& text)
{
    if (text.empty())
        throw cipher_error("Пустой текст");
    
    // Проверяем, что текст состоит только из русских букв и пробелов
    wstring valid_chars = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ ";
    for (wchar_t c : text) {
        if (valid_chars.find(c) == wstring::npos) {
            throw cipher_error("Текст содержит недопустимые символы");
        }
    }
    
    // Проверяем, что есть хотя бы одна буква (не только пробелы)
    bool has_letters = false;
    for (wchar_t c : text) {
        if (c != L' ') {
            has_letters = true;
            break;
        }
    }
    
    if (!has_letters)
        throw cipher_error("Текст должен содержать хотя бы одну букву");
    
    return text;
}

wstring RouteCipher::encrypt(const wstring& open_text)
{
    wstring valid_text = validateText(open_text);
    
    int text_len = valid_text.length();
    int rows = (text_len + columns - 1) / columns;
    
    // Заполняем таблицу по горизонтали слева направо, сверху вниз
    vector<vector<wchar_t>> table(rows, vector<wchar_t>(columns, L' '));
    int index = 0;
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < columns; col++) {
            if (index < text_len) {
                table[row][col] = valid_text[index++];
            }
        }
    }
    
    // Читаем по вертикали сверху вниз, справа налево
    wstring result;
    for (int col = columns - 1; col >= 0; col--) {
        for (int row = 0; row < rows; row++) {
            result += table[row][col];
        }
    }
    
    return result;
}

wstring RouteCipher::decrypt(const wstring& cipher_text)
{
    wstring valid_text = validateText(cipher_text);
    
    int text_len = valid_text.length();
    int rows = (text_len + columns - 1) / columns;
    
    // Создаем таблицу
    vector<vector<wchar_t>> table(rows, vector<wchar_t>(columns, L' '));
    
    // Заполняем таблицу по вертикали сверху вниз, справа налево
    int index = 0;
    for (int col = columns - 1; col >= 0; col--) {
        for (int row = 0; row < rows; row++) {
            if (index < text_len) {
                table[row][col] = valid_text[index++];
            }
        }
    }
    
    // Читаем по горизонтали слева направо, сверху вниз
    wstring result;
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < columns; col++) {
            result += table[row][col];
        }
    }
    
    return result;
}
