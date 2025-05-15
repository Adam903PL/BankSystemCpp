#include "Transaction.h"
#include <sstream>
#include <iomanip>
#include <codecvt>
#include <locale>







using namespace std;


namespace BankSystem {

    Transaction::Transaction(TransactionType type, double amount, const std::string& description)
        : type(type), amount(amount), description(description) {
        timestamp = std::time(nullptr);
    }

    std::string Transaction::toString() const {
        std::stringstream ss;

        struct tm* timeinfo = localtime(&timestamp);
        char buffer[80];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);

        ss << "[" << buffer << "] ";

        if (type == TransactionType::DEPOSIT) {
            ss << "WPŁATA: +";
        } else {
            ss << "WYPŁATA: -";
        }

        ss << std::fixed << std::setprecision(2) << amount << " zł";

        if (!description.empty()) {
            ss << " - " << description;
        }

        return ss.str();
    }

}