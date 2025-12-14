#include <iostream>
#include <locale>
#include "modTableCipher.h"

using namespace std;

void test(const wstring& text, int key) {
    try {
        Table cipher(key);              /// Экземпляр шифратора
        wstring encrypted = cipher.encrypt(text);   /// Зашифрованный текст
        wstring decrypted = cipher.decrypt(encrypted); /// Расшифрованный текст
        
        // Вывод результатов тестирования
        wcout << L"Key: " << key << L" | Text: '" << text << L"'" << endl;
        wcout << L"Encrypted: '" << encrypted << L"'" << endl;
        wcout << L"Decrypted: '" << decrypted << L"'" << endl;
        wcout << endl;
    } catch (const cipher_error& e) {
        // Обработка исключений шифрования
        wcout << L"Error with key " << key << L" and text '" << text << L"': " << e.what() << endl << endl;
    }
}

int main() {
    setlocale(LC_ALL, ""); ///< Установка локали для поддержки кириллицы
    
    wcout << L"=== ТЕСТИРОВАНИЕ ТАБЛИЧНОЙ МАРШРУТНОЙ ПЕРЕСТАНОВКИ ===" << endl << endl;
    
    wcout << L"1. КОРРЕКТНЫЕ ДАННЫЕ:" << endl;
    // Тест с обычным текстом и ключом 4
    test(L"Добрый вечер", 4);
    // Тест с одним символом
    test(L"А", 2);
    // Тест с текстом в разных регистрах
    test(L"ПРИВЕТ мир", 3);
    // Тест с последовательными символами
    test(L"АБВГД", 4);
    
    wcout << L"2. ТЕСТЫ С ОШИБКАМИ:" << endl;
    // Тест с нулевым ключом
    test(L"Добрый вечер", 0);
    // Тест с отрицательным ключом
    test(L"Добрый вечер", -5);
    // Тест с очень большим ключом
    test(L"Добрый вечер", 150);
    // Тест с не-буквенными символами
    test(L"123!@#", 3);
    // Тест с пустым текстом
    test(L"", 3);
    
    return 0; /// Успешное завершение программы
}
