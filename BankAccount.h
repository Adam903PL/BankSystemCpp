#ifndef BANKSIMULATOR_BANKACCOUNT_H
#define BANKSIMULATOR_BANKACCOUNT_H

#include <string>
#include <vector>
#include <algorithm>
#include "Transaction.h"


using namespace std;


namespace BankSystem {

    class BankAccount {
    private:
        string accountNumber;
        string ownerName;
        double balance;
        vector<Transaction> transactionHistory;

    public:
        BankAccount() = default;
        BankAccount(const string& accountNumber, const string& ownerName, double initialBalance = 0.0);

        string getAccountNumber() const { return accountNumber; }
        string getOwnerName() const { return ownerName; }
        double getBalance() const { return balance; }
        const vector<Transaction>& getTransactionHistory() const { return transactionHistory; }

        bool deposit(double amount, const string& description = "");
        bool withdraw(double amount, const string& description = "");

        void sortTransactionHistory();

        string serialize() const;

        static BankAccount deserialize(const string& data);
    };

}
#endif