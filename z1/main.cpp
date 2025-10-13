#include <iostream>
#include <locale>
#include <codecvt>
#include <stdexcept>
#include "modAlphaCipher.h"

using namespace std;

void check(const wstring& text, const wstring& key)
{
    try {
        wstring cipherText;
        wstring decryptedText;
        modAlphaCipher cipher(key);
        
        cipherText = cipher.encrypt(text);
        decryptedText = cipher.decrypt(cipherText);
        
        wcout << L"Ключ: " << key << endl;
        wcout << L"Исходный текст: " << text << endl;
        wcout << L"Зашифрованный текст: " << cipherText << endl;
        wcout << L"Расшифрованный текст: " << decryptedText << endl;
        
        if (text == decryptedText) {
            wcout << L"Успех: текст совпадает!" << endl;
        } else {
            wcout << L"Ошибка: текст не совпадает!" << endl;
        }
        wcout << endl;
    } catch (const cipher_error& e) {
        wcout << L"Ошибка шифрования: " << string_to_wstring(e.what()) << endl << endl;
    } catch (const std::exception& e) {
        wcout << L"Неизвестная ошибка: " << e.what() << endl << endl;
    }
}

void test_exceptions()
{
    wcout << L"=== Тестирование обработки исключений ===" << endl;
    
    // Тест с пустым ключом
    wcout << L"1. Тест с пустым ключом:" << endl;
    check(L"ТЕСТ", L"");
    
    // Тест с ключом, содержащим недопустимые символы
    wcout << L"2. Тест с ключом, содержащим цифры:" << endl;
    check(L"ТЕСТ", L"КЛЮЧ123");
    
    // Тест с пустым открытым текстом
    wcout << L"3. Тест с пустым открытым текстом:" << endl;
    check(L"", L"КЛЮЧ");
    
    // Тест с открытым текстом на латинице
    wcout << L"4. Тест с открытым текстом на латинице:" << endl;
    check(L"HELLO", L"КЛЮЧ");
    
    // Тест с недопустимыми символами в зашифрованном тексте
    wcout << L"5. Тест с недопустимыми символами в зашифрованном тексте:" << endl;
    try {
        modAlphaCipher cipher(L"КЛЮЧ");
        wstring encrypted = cipher.encrypt(L"ПРИВЕТ");
        encrypted[0] = L'X'; // Испортим первый символ (латинская буква)
        wstring decrypted = cipher.decrypt(encrypted);
        wcout << L"Расшифрованный текст: " << decrypted << endl << endl;
    } catch (const cipher_error& e) {
        wcout << L"Поймано исключение: " << string_to_wstring(e.what()) << endl << endl;
    }
}

int main()
{
    setlocale(LC_ALL, "ru_RU.UTF-8");
    
    wcout << L"=== Нормальные тесты ===" << endl;
    // Тестирование с разными ключами
    check(L"ПРИВЕТМИР", L"КЛЮЧ");
    check(L"ПРОГРАММИРОВАНИЕ", L"ШАШЛЫК");
    check(L"КОМПЬЮТЕР", L"");
    
    // Тестирование обработки исключений
    test_exceptions();
    
    return 0;
}
