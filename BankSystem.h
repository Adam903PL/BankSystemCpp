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

        bool saveToFile(const string& filePath = "") const;
        bool loadFromFile(const string& filePath = "");

        void displayAllAccounts() const;
    };

}

#endif