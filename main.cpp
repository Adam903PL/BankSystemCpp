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
    wcout << L"9. Wykonaj przelew" << endl;
    wcout << L"10. Generuj raport miesięczny" << endl;
    wcout << L"11. Generuj raport z okresu" << endl;
    wcout << L"12. O projekcie" << endl;
    wcout << L"0. Wyjście" << endl;
    wcout << L"Wybierz opcję: ";
}

void showWelcomeBanner() {
    wcout << L"\n";
    wcout << L"╔═════════════════════════════════════════════════════════════════╗" << endl;
    wcout << L"║                                                                 ║" << endl;
    wcout << L"║           ████████╗███████╗ ██████╗██╗  ██╗███╗   ██╗ ██╗       ║" << endl;
    wcout << L"║           ╚══██╔══╝██╔════╝██╔════╝██║  ██║████╗  ██║ ██║       ║" << endl;
    wcout << L"║              ██║   █████╗  ██║     ███████║██╔██╗ ██║ ██║       ║" << endl;
    wcout << L"║              ██║   ██╔══╝  ██║     ██╔══██║██║╚██╗██║ ██║       ║" << endl;
    wcout << L"║              ██║   ███████╗╚██████╗██║  ██║██║ ╚████║ ██║       ║" << endl;
    wcout << L"║              ╚═╝   ╚══════╝ ╚═════╝╚═╝  ╚═╝╚═╝  ╚═══╝ ╚═╝       ║" << endl;
    wcout << L"║                                                                 ║" << endl;
    wcout << L"║               ██████╗  █████╗ ███╗   ██╗██╗  ██╗                ║" << endl;
    wcout << L"║               ██╔══██╗██╔══██╗████╗  ██║██║ ██╔╝                ║" << endl;
    wcout << L"║               ██████╔╝███████║██╔██╗ ██║█████╔╝                 ║" << endl;
    wcout << L"║               ██╔══██╗██╔══██║██║╚██╗██║██╔═██╗                 ║" << endl;
    wcout << L"║               ██████╔╝██║  ██║██║ ╚████║██║  ██╗                ║" << endl;
    wcout << L"║               ╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝  ╚═╝                ║" << endl;
    wcout << L"║                                                                 ║" << endl;
    wcout << L"║                    System Bankowy v2137.0                          ║" << endl;
    wcout << L"╚═════════════════════════════════════════════════════════════════╝" << endl;
    wcout << L"\n";
}

void showRepositoryInfo() {
    wcout << L"\n===== O PROJEKCIE =====" << endl;
    wcout << L"TechniBank - System Bankowy v2.0" << endl;
    wcout << L"Projekt dostępny na GitHub:" << endl;
    wcout << L"https://github.com/Karman1818/TechniBank" << endl;
    wcout << L"\nTechnologie:" << endl;
    wcout << L"- HTML CSS JS" << endl;
    wcout << L"- Standard Template Library (STL)" << endl;
    wcout << L"- Obsługa plików" << endl;
    wcout << L"- Obsługa transakcji" << endl;
    wcout << L"- System raportowania" << endl;
    wcout << L"========================" << endl;
}

int main(int argc, char* argv[]) {
    setupPolishEncoding();
    showWelcomeBanner();
    
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
    string targetAccount;
    string startDate;
    string endDate;
    int month, year;

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

            case 2:
                wcout << L"Podaj numer konta: ";
                getline(wcin, wAccountNumber);

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

            case 3:
                wcout << L"Podaj numer konta: ";
                getline(wcin, wAccountNumber);

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

            case 4:
                wcout << L"Podaj numer konta: ";
                getline(wcin, wAccountNumber);

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

                {
                    wstring_convert<codecvt_utf8<wchar_t>> converter;
                    accountNumber = converter.to_bytes(wAccountNumber);
                }

                if (!bankSystem.accountExists(accountNumber)) {
                    wcout << L"Konto o podanym numerze nie istnieje!" << endl;
                    break;
                }

                account = bankSystem.getAccount(accountNumber);
                account->sortTransactionHistory();

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
                wcout << L"Podaj numer konta do zapisania: ";
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

                wcout << L"Podaj ścieżkę do pliku (lub Enter dla domyślnej): ";
                getline(wcin, dataFilePath);

                // Konwersja z wstring do string
                {
                    wstring_convert<codecvt_utf8<wchar_t>> converter;
                    dataFilePathStr = converter.to_bytes(dataFilePath);
                }

                if (bankSystem.saveAccountToFile(accountNumber, dataFilePathStr)) {
                    wcout << L"Dane konta zostały zapisane pomyślnie!" << endl;
                } else {
                    wcout << L"Nie udało się zapisać danych konta." << endl;
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

            case 9: // Wykonaj przelew
                wcout << L"Podaj numer konta źródłowego: ";
                getline(wcin, wAccountNumber);
                {
                    wstring_convert<codecvt_utf8<wchar_t>> converter;
                    accountNumber = converter.to_bytes(wAccountNumber);
                }

                wcout << L"Podaj numer konta docelowego: ";
                getline(wcin, wAccountNumber);
                {
                    wstring_convert<codecvt_utf8<wchar_t>> converter;
                    targetAccount = converter.to_bytes(wAccountNumber);
                }

                wcout << L"Podaj kwotę przelewu: ";
                wcin >> amount;
                clearInputBuffer();

                wcout << L"Podaj opis przelewu (opcjonalnie): ";
                getline(wcin, wDescription);
                {
                    wstring_convert<codecvt_utf8<wchar_t>> converter;
                    description = converter.to_bytes(wDescription);
                }

                if (bankSystem.transfer(accountNumber, targetAccount, amount, description)) {
                    wcout << L"Przelew został zrealizowany pomyślnie!" << endl;
                } else {
                    wcout << L"Nie udało się zrealizować przelewu. Sprawdź saldo konta." << endl;
                }
                break;

            case 10:
                wcout << L"Podaj numer konta: ";
                getline(wcin, wAccountNumber);
                {
                    wstring_convert<codecvt_utf8<wchar_t>> converter;
                    accountNumber = converter.to_bytes(wAccountNumber);
                }

                wcout << L"Podaj miesiąc (1-12): ";
                wcin >> month;
                wcout << L"Podaj rok: ";
                wcin >> year;
                clearInputBuffer();

                {
                    string report = bankSystem.generateMonthlyReport(accountNumber, month, year);
                    wstring_convert<codecvt_utf8<wchar_t>> converter;
                    wstring wReport = converter.from_bytes(report);
                    wcout << wReport << endl;
                }
                break;

            case 11:
                wcout << L"Podaj numer konta: ";
                getline(wcin, wAccountNumber);
                {
                    wstring_convert<codecvt_utf8<wchar_t>> converter;
                    accountNumber = converter.to_bytes(wAccountNumber);
                }

                wcout << L"Podaj datę początkową (YYYY-MM-DD): ";
                getline(wcin, wDescription);
                {
                    wstring_convert<codecvt_utf8<wchar_t>> converter;
                    startDate = converter.to_bytes(wDescription);
                }

                wcout << L"Podaj datę końcową (YYYY-MM-DD): ";
                getline(wcin, wDescription);
                {
                    wstring_convert<codecvt_utf8<wchar_t>> converter;
                    endDate = converter.to_bytes(wDescription);
                }

                {
                    string report = bankSystem.generateTransactionReport(accountNumber, startDate, endDate);
                    wstring_convert<codecvt_utf8<wchar_t>> converter;
                    wstring wReport = converter.from_bytes(report);
                    wcout << wReport << endl;
                }
                break;

            case 12:
                showRepositoryInfo();
                break;

            default:
                wcout << L"Nieprawidłowa opcja. Spróbuj ponownie." << endl;
        }
    }

    return 0;
}