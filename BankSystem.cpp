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

    bool BankSystem::transfer(const string& fromAccount, const string& toAccount, double amount, const string& description) {
        BankAccount* sourceAccount = getAccount(fromAccount);
        BankAccount* targetAccount = getAccount(toAccount);

        if (!sourceAccount || !targetAccount) {
            return false;
        }

        if (sourceAccount->getBalance() < amount) {
            return false;
        }

        string transferDesc = "Przelew do: " + toAccount + " - " + description;
        string receiveDesc = "Przelew od: " + fromAccount + " - " + description;

        if (sourceAccount->withdraw(amount, transferDesc)) {
            targetAccount->deposit(amount, receiveDesc);
            return true;
        }

        return false;
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

    bool BankSystem::saveAccountToFile(const string& accountNumber, const string& filePath) const {
        if (!accountExists(accountNumber)) {
            cerr << "Konto o numerze " << accountNumber << " nie istnieje!" << endl;
            return false;
        }

        string path = filePath.empty() ? dataFilePath : filePath;
        if (path.empty()) {
            path = "account_" + accountNumber + ".txt";
        }

        ofstream file(path);
        if (!file.is_open()) {
            cerr << "Nie można otworzyć pliku " << path << " do zapisu!" << endl;
            return false;
        }

        const BankAccount& account = accounts.at(accountNumber);
        file << "1" << endl; // Liczba kont
        file << account.serialize() << endl;

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

    string BankSystem::generateMonthlyReport(const string& accountNumber, int month, int year) const {
        if (!accountExists(accountNumber)) {
            return "Konto nie istnieje!";
        }

        const BankAccount& account = accounts.at(accountNumber);
        stringstream report;
        
        report << "===== RAPORT MIESIĘCZNY =====" << endl;
        report << "Konto: " << accountNumber << endl;
        report << "Właściciel: " << account.getOwnerName() << endl;
        report << "Miesiąc: " << month << "/" << year << endl;
        report << "-----------------------------------" << endl;

        double totalDeposits = 0;
        double totalWithdrawals = 0;
        int transactionCount = 0;

        for (const auto& transaction : account.getTransactionHistory()) {
            time_t timestamp = transaction.getTimestamp();
            struct tm* timeinfo = localtime(&timestamp);
            if (timeinfo->tm_mon + 1 == month && timeinfo->tm_year + 1900 == year) {
                transactionCount++;
                if (transaction.getType() == TransactionType::DEPOSIT) {
                    totalDeposits += transaction.getAmount();
                } else if (transaction.getType() == TransactionType::WITHDRAWAL) {
                    totalWithdrawals += transaction.getAmount();
                }
            }
        }

        report << "Liczba transakcji: " << transactionCount << endl;
        report << "Suma wpłat: " << totalDeposits << " zł" << endl;
        report << "Suma wypłat: " << totalWithdrawals << " zł" << endl;
        report << "Saldo końcowe: " << account.getBalance() << " zł" << endl;
        report << "-----------------------------------" << endl;

        return report.str();
    }

    string BankSystem::generateTransactionReport(const string& accountNumber, const string& startDate, const string& endDate) const {
        if (!accountExists(accountNumber)) {
            return "Konto nie istnieje!";
        }

        const BankAccount& account = accounts.at(accountNumber);
        stringstream report;
        
        report << "===== RAPORT TRANSAKCJI =====" << endl;
        report << "Konto: " << accountNumber << endl;
        report << "Właściciel: " << account.getOwnerName() << endl;
        report << "Okres: " << startDate << " - " << endDate << endl;
        report << "-----------------------------------" << endl;

        for (const auto& transaction : account.getTransactionHistory()) {
            time_t timestamp = transaction.getTimestamp();
            struct tm* timeinfo = localtime(&timestamp);
            char buffer[80];
            strftime(buffer, sizeof(buffer), "%Y-%m-%d", timeinfo);
            string dateStr(buffer);
            
            if (dateStr >= startDate && dateStr <= endDate) {
                report << transaction.toString() << endl;
            }
        }

        report << "-----------------------------------" << endl;
        return report.str();
    }

    bool BankSystem::saveReportToFile(const string& report, const string& filePath) const {
        ofstream file(filePath);
        if (!file.is_open()) {
            return false;
        }
        file << report;
        return true;
    }

} // namespace BankSystem