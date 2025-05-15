#include "BankSystem.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

namespace BankSystem {

    BankSystem::BankSystem(const string& dataFilePath) : dataFilePath(dataFilePath) {
        if (!dataFilePath.empty()) {
            loadFromFile(dataFilePath);
        }
    }

    bool BankSystem::createAccount(const string& accountNumber, const string& ownerName, double initialBalance) {
        if (accountExists(accountNumber)) {
            cerr << "Konto o numerze " << accountNumber << " już istnieje!" << endl;
            return false;
        }

        BankAccount account(accountNumber, ownerName, initialBalance);

        accounts[accountNumber] = account;

        return true;
    }

    bool BankSystem::accountExists(const string& accountNumber) const {
        return accounts.find(accountNumber) != accounts.end();
    }

    BankAccount* BankSystem::getAccount(const string& accountNumber) {
        if (!accountExists(accountNumber)) {
            return nullptr;
        }

        return &accounts[accountNumber];
    }

    bool BankSystem::deposit(const string& accountNumber, double amount, const string& description) {
        BankAccount* account = getAccount(accountNumber);

        if (!account) {
            return false;
        }

        return account->deposit(amount, description);
    }

    bool BankSystem::withdraw(const string& accountNumber, double amount, const string& description) {
        BankAccount* account = getAccount(accountNumber);

        if (!account) {
            return false;
        }

        return account->withdraw(amount, description);
    }

    bool BankSystem::saveToFile(const string& filePath) const {
        string path = filePath.empty() ? dataFilePath : filePath;

        if (path.empty()) {
            path = "bank_data.txt";
        }

        // Otwieramy plik do zapisu
        ofstream file(path);

        if (!file.is_open()) {
            cerr << "Nie można otworzyć pliku " << path << " do zapisu!" << endl;
            return false;
        }

        file << accounts.size() << endl;

        for (const auto& pair : accounts) {
            file << pair.second.serialize() << endl;
        }

        return true;
    }

    bool BankSystem::loadFromFile(const string& filePath) {
        string path = filePath.empty() ? dataFilePath : filePath;

        if (path.empty()) {
            path = "bank_data.txt";
        }

        ifstream file(path);

        if (!file.is_open()) {
            cerr << "Nie można otworzyć pliku " << path << " do odczytu!" << endl;
            return false;
        }

        accounts.clear();

        int accountCount;
        file >> accountCount;
        file.ignore();

        for (int i = 0; i < accountCount; i++) {
            string line;
            getline(file, line);

            try {
                BankAccount account = BankAccount::deserialize(line);
                accounts[account.getAccountNumber()] = account;
            } catch (const exception& e) {
                cerr << "Błąd przy wczytywaniu konta: " << e.what() << endl;
            }
        }

        return true;
    }

    void BankSystem::displayAllAccounts() const {
        if (accounts.empty()) {
            cout << "Brak kont w systemie." << endl;
            return;
        }

        cout << "\n===== LISTA WSZYSTKICH KONT =====" << endl;
        cout << "Liczba kont: " << accounts.size() << endl;
        cout << "-----------------------------------" << endl;

        for (const auto& pair : accounts) {
            const BankAccount& account = pair.second;

            cout << "Numer konta: " << account.getAccountNumber() << endl;
            cout << "Właściciel: " << account.getOwnerName() << endl;
            cout << "Saldo: " << account.getBalance() << " zł" << endl;
            cout << "Liczba transakcji: " << account.getTransactionHistory().size() << endl;
            cout << "-----------------------------------" << endl;
        }
    }

} // namespace BankSystem