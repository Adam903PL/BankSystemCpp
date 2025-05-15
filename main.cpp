#include <iostream>
#include <string>
#include <limits>
#include <locale>
#include <codecvt>
#include <fcntl.h>
#include <io.h>
#include <sstream>
#include "BankSystem.h"


using namespace std;


void setupPolishEncoding() {
    _setmode(_fileno(stdout), _O_U8TEXT);
    _setmode(_fileno(stdin), _O_U8TEXT);
    
    try {
        locale::global(locale("Polish_Poland.1250"));
        wcout.imbue(locale());
        wcin.imbue(locale());
    } catch (const exception& e) {
        wcout << L"Uwaga: Nie można ustawić polskiej lokalizacji. Używam domyślnej." << endl;
        wcout << L"Błąd: " << e.what() << endl;
        
        try {
            locale::global(locale(""));
            wcout.imbue(locale());
            wcin.imbue(locale());
        } catch (const exception&) {
            locale::global(locale("C"));
            wcout.imbue(locale());
            wcin.imbue(locale());
        }
    }
}

void clearInputBuffer() {
    wcin.clear();
    wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
}

void showMenu() {
    wcout << L"\n===== SYMULATOR BANKU =====" << endl;
    wcout << L"1. Utwórz nowe konto" << endl;
    wcout << L"2. Wpłać środki" << endl;
    wcout << L"3. Wypłać środki" << endl;
    wcout << L"4. Sprawdź saldo" << endl;
    wcout << L"5. Wyświetl historię transakcji" << endl;
    wcout << L"6. Wyświetl wszystkie konta" << endl;
    wcout << L"7. Zapisz dane do pliku" << endl;
    wcout << L"8. Wczytaj dane z pliku" << endl;
    wcout << L"0. Wyjście" << endl;
    wcout << L"Wybierz opcję: ";
}

int main(int argc, char* argv[]) {
    setupPolishEncoding();
    
    wstring dataFilePath;

    if (argc > 1) {
        wstring_convert<codecvt_utf8<wchar_t>> converter;
        dataFilePath = converter.from_bytes(argv[1]);
        wcout << L"Wczytywanie danych z pliku: " << dataFilePath << endl;
    }

    string dataFilePathStr;
    if (!dataFilePath.empty()) {
        wstring_convert<codecvt_utf8<wchar_t>> converter;
        dataFilePathStr = converter.to_bytes(dataFilePath);
    }
    BankSystem::BankSystem bankSystem(dataFilePathStr);

    int choice;
    wstring wAccountNumber, wOwnerName, wDescription;
    string accountNumber, ownerName, description;
    double amount;
    BankSystem::BankAccount* account = nullptr;

    while (true) {
        showMenu();
        wcin >> choice;
        clearInputBuffer();

        switch (choice) {
            case 0:
                wcout << L"Dziękujemy za skorzystanie z symulatora banku!" << endl;
                return 0;

            case 1:
                wcout << L"Podaj numer konta: ";
                getline(wcin, wAccountNumber);

                wcout << L"Podaj imię i nazwisko właściciela: ";
                getline(wcin, wOwnerName);

                wcout << L"Podaj saldo początkowe: ";
                wcin >> amount;
                clearInputBuffer();

                // Konwersja z wstring do string
                {
                    wstring_convert<codecvt_utf8<wchar_t>> converter;
                    accountNumber = converter.to_bytes(wAccountNumber);
                    ownerName = converter.to_bytes(wOwnerName);
                }

                if (bankSystem.createAccount(accountNumber, ownerName, amount)) {
                    wcout << L"Konto zostało utworzone pomyślnie!" << endl;
                } else {
                    wcout << L"Nie udało się utworzyć konta." << endl;
                }
                break;

            case 2: // Wpłać środki
                wcout << L"Podaj numer konta: ";
                getline(wcin, wAccountNumber);

                // Konwersja z wstring do string
                {
                    wstring_convert<codecvt_utf8<wchar_t>> converter;
                    accountNumber = converter.to_bytes(wAccountNumber);
                }

                if (!bankSystem.accountExists(accountNumber)) {
                    wcout << L"Konto o podanym numerze nie istnieje!" << endl;
                    break;
                }

                wcout << L"Podaj kwotę wpłaty: ";
                wcin >> amount;
                clearInputBuffer();

                wcout << L"Podaj opis transakcji (opcjonalnie): ";
                getline(wcin, wDescription);

                // Konwersja z wstring do string
                {
                    wstring_convert<codecvt_utf8<wchar_t>> converter;
                    description = converter.to_bytes(wDescription);
                }

                if (bankSystem.deposit(accountNumber, amount, description)) {
                    wcout << L"Wpłata została zrealizowana pomyślnie!" << endl;
                    wcout << L"Nowe saldo: " << bankSystem.getAccount(accountNumber)->getBalance() << L" zł" << endl;
                } else {
                    wcout << L"Nie udało się zrealizować wpłaty." << endl;
                }
                break;

            case 3: // Wypłać środki
                wcout << L"Podaj numer konta: ";
                getline(wcin, wAccountNumber);

                // Konwersja z wstring do string
                {
                    wstring_convert<codecvt_utf8<wchar_t>> converter;
                    accountNumber = converter.to_bytes(wAccountNumber);
                }

                if (!bankSystem.accountExists(accountNumber)) {
                    wcout << L"Konto o podanym numerze nie istnieje!" << endl;
                    break;
                }

                wcout << L"Podaj kwotę wypłaty: ";
                wcin >> amount;
                clearInputBuffer();

                wcout << L"Podaj opis transakcji (opcjonalnie): ";
                getline(wcin, wDescription);

                // Konwersja z wstring do string
                {
                    wstring_convert<codecvt_utf8<wchar_t>> converter;
                    description = converter.to_bytes(wDescription);
                }

                if (bankSystem.withdraw(accountNumber, amount, description)) {
                    wcout << L"Wypłata została zrealizowana pomyślnie!" << endl;
                    wcout << L"Nowe saldo: " << bankSystem.getAccount(accountNumber)->getBalance() << L" zł" << endl;
                } else {
                    wcout << L"Nie udało się zrealizować wypłaty. Sprawdź saldo konta." << endl;
                }
                break;

            case 4: // Sprawdź saldo
                wcout << L"Podaj numer konta: ";
                getline(wcin, wAccountNumber);

                // Konwersja z wstring do string
                {
                    wstring_convert<codecvt_utf8<wchar_t>> converter;
                    accountNumber = converter.to_bytes(wAccountNumber);
                }

                if (!bankSystem.accountExists(accountNumber)) {
                    wcout << L"Konto o podanym numerze nie istnieje!" << endl;
                    break;
                }

                wcout << L"Saldo konta " << wAccountNumber << L": "
                          << bankSystem.getAccount(accountNumber)->getBalance() << L" zł" << endl;
                break;

            case 5: // Wyświetl historię transakcji
                wcout << L"Podaj numer konta: ";
                getline(wcin, wAccountNumber);

                // Konwersja z wstring do string
                {
                    wstring_convert<codecvt_utf8<wchar_t>> converter;
                    accountNumber = converter.to_bytes(wAccountNumber);
                }

                if (!bankSystem.accountExists(accountNumber)) {
                    wcout << L"Konto o podanym numerze nie istnieje!" << endl;
                    break;
                }

                account = bankSystem.getAccount(accountNumber);
                account->sortTransactionHistory(); // Sortowanie historii według daty

                wcout << L"Historia transakcji dla konta " << wAccountNumber << L":" << endl;
                wcout << L"---------------------------------------------------" << endl;

                for (const auto& transaction : account->getTransactionHistory()) {
                    // Konwersja z string do wstring
                    wstring_convert<codecvt_utf8<wchar_t>> converter;
                    wstring wTransactionString = converter.from_bytes(transaction.toString());
                    wcout << wTransactionString << endl;
                }

                wcout << L"---------------------------------------------------" << endl;
                break;

            case 6: // Wyświetl wszystkie konta
                // Wywołujemy oryginalną metodę displayAllAccounts, ale z przekierowaniem wyjścia
                {
                    // Przekierowanie cout na tymczasowy bufor
                    stringstream buffer;
                    streambuf* old = cout.rdbuf(buffer.rdbuf());
                    
                    // Wywołanie oryginalnej metody
                    bankSystem.displayAllAccounts();
                    
                    // Przywrócenie oryginalnego strumienia
                    cout.rdbuf(old);
                    
                    // Konwersja wyniku na wstring i wyświetlenie
                    string output = buffer.str();
                    wstring_convert<codecvt_utf8<wchar_t>> converter;
                    wstring woutput = converter.from_bytes(output);
                    
                    // Wyświetlenie wyniku
                    wcout << woutput;
                }
                break;

            case 7: // Zapisz dane do pliku
                wcout << L"Podaj ścieżkę do pliku (lub Enter dla domyślnej): ";
                getline(wcin, dataFilePath);

                // Konwersja z wstring do string
                {
                    wstring_convert<codecvt_utf8<wchar_t>> converter;
                    dataFilePathStr = converter.to_bytes(dataFilePath);
                }

                if (bankSystem.saveToFile(dataFilePathStr)) {
                    wcout << L"Dane zostały zapisane pomyślnie!" << endl;
                } else {
                    wcout << L"Nie udało się zapisać danych." << endl;
                }
                break;

            case 8: // Wczytaj dane z pliku
                wcout << L"Podaj ścieżkę do pliku (lub Enter dla domyślnej): ";
                getline(wcin, dataFilePath);

                // Konwersja z wstring do string
                {
                    wstring_convert<codecvt_utf8<wchar_t>> converter;
                    dataFilePathStr = converter.to_bytes(dataFilePath);
                }

                if (bankSystem.loadFromFile(dataFilePathStr)) {
                    wcout << L"Dane zostały wczytane pomyślnie!" << endl;
                } else {
                    wcout << L"Nie udało się wczytać danych." << endl;
                }
                break;

            default:
                wcout << L"Nieprawidłowa opcja. Spróbuj ponownie." << endl;
        }
    }

    return 0;
}