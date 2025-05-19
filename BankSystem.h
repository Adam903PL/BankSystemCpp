#ifndef BANKSIMULATOR_BANKSYSTEM_H
#define BANKSIMULATOR_BANKSYSTEM_H

#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include "BankAccount.h"

namespace BankSystem {

    class BankSystem {
    private:
        map<string, BankAccount> accounts;
        string dataFilePath;

    public:
        BankSystem(const string& dataFilePath = "");

        bool createAccount(const string& accountNumber, const string& ownerName, double initialBalance = 0.0);
        bool accountExists(const string& accountNumber) const;
        BankAccount* getAccount(const string& accountNumber);

        bool deposit(const string& accountNumber, double amount, const string& description = "Standardowa wpłata");
        bool withdraw(const string& accountNumber, double amount, const string& description = "Standardowa wypłata");
        bool transfer(const string& fromAccount, const string& toAccount, double amount, const string& description = "Przelew");

        bool saveToFile(const string& filePath = "") const;
        bool saveAccountToFile(const string& accountNumber, const string& filePath = "") const;
        bool loadFromFile(const string& filePath = "");

        void displayAllAccounts() const;
        
        // Metody raportowania
        string generateMonthlyReport(const string& accountNumber, int month, int year) const;
        string generateTransactionReport(const string& accountNumber, const string& startDate, const string& endDate) const;
        bool saveReportToFile(const string& report, const string& filePath) const;
    };

}

#endif