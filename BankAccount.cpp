#include "BankAccount.h"
#include <sstream>
#include <stdexcept>

using namespace std;

namespace BankSystem {

    BankAccount::BankAccount(const string& accountNumber, const string& ownerName, double initialBalance)
        : accountNumber(accountNumber), ownerName(ownerName), balance(initialBalance) {

        if (initialBalance > 0) {
            deposit(initialBalance, "Saldo początkowe");
        }
    }

    bool BankAccount::deposit(double amount, const string& description) {
        if (amount <= 0) {
            return false;
        }
        balance += amount;

        transactionHistory.emplace_back(TransactionType::DEPOSIT, amount, description);

        return true;
    }

    bool BankAccount::withdraw(double amount, const string& description) {
        if (amount <= 0 || amount > balance) {
            return false;
        }

        balance -= amount;

        transactionHistory.emplace_back(TransactionType::WITHDRAWAL, amount, description);

        return true;
    }

    void BankAccount::sortTransactionHistory() {
        sort(transactionHistory.begin(), transactionHistory.end());
    }

    string BankAccount::serialize() const {
        stringstream ss;

        ss << accountNumber << "|" << ownerName << "|" << balance;

        ss << "|" << transactionHistory.size();

        for (const auto& transaction : transactionHistory) {
            ss << "|" << static_cast<int>(transaction.getType())
               << "|" << transaction.getAmount()
               << "|" << transaction.getDescription()
               << "|" << transaction.getTimestamp();
        }

        return ss.str();
    }

    BankAccount BankAccount::deserialize(const string& data) {
        stringstream ss(data);
        string token;
        vector<string> tokens;

        while (getline(ss, token, '|')) {
            tokens.push_back(token);
        }

        if (tokens.size() < 4) {
            throw runtime_error("Nieprawidłowy format danych konta");
        }

        BankAccount account(tokens[0], tokens[1], stod(tokens[2]));
        account.balance = stod(tokens[2]);

        int transactionCount = stoi(tokens[3]);

        if (tokens.size() < 4 + transactionCount * 4) {
            throw runtime_error("Nieprawidłowa liczba tokenów dla transakcji");
        }

        account.transactionHistory.clear();

        for (int i = 0; i < transactionCount; i++) {
            int baseIndex = 4 + i * 4;

            TransactionType type = static_cast<TransactionType>(stoi(tokens[baseIndex]));
            double amount = stod(tokens[baseIndex + 1]);
            string description = tokens[baseIndex + 2];
            time_t timestamp = stol(tokens[baseIndex + 3]);

            Transaction transaction(type, amount, description);

            transaction.setTimestamp(timestamp);

            account.transactionHistory.push_back(transaction);
        }

        return account;
    }

}