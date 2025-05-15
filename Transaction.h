#ifndef BANKSIMULATOR_TRANSACTION_H
#define BANKSIMULATOR_TRANSACTION_H

#include <string>
#include <ctime>


using namespace std;


namespace BankSystem {

    enum class TransactionType {
        DEPOSIT,
        WITHDRAWAL
    };

    class Transaction {
    private:
        TransactionType type;
        double amount;
        string description;
        time_t timestamp;

    public:
        // Konstruktor
        Transaction(TransactionType type, double amount, const string& description = "");

        // Setter for timestamp (for deserialization)
        void setTimestamp(time_t ts) { timestamp = ts; }

        // Gettery
        TransactionType getType() const { return type; }
        double getAmount() const { return amount; }
        string getDescription() const { return description; }
        time_t getTimestamp() const { return timestamp; }

        string toString() const;

        bool operator<(const Transaction& other) const {
            return timestamp < other.timestamp;
        }
    };

}

#endif