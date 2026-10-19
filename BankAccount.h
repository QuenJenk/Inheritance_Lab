#ifndef BANK_ACCOUNT_H
#define BANK_ACCOUNT_H

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <chrono>
#include <iomanip>
#include <sstream>

// Quentin Jenkins 10/5/26
// Operator Overload & Inheritance Lab (Updated with Composition)

class Transaction {
public:
    std::string type;
    double amount;
    std::string timestamp;

    Transaction(std::string t, double amt, std::string timeStr)
        : type(t), amount(amt), timestamp(timeStr) {}
};


class BankAccount {
private:
    // REQUIREMENT: Private transaction history composition
    std::vector<Transaction> transactionHistory;

protected:
    std::string accountNumber;
    std::string* accountHolderName;
    double* balance;

    // Protected helper to easily log transactions from base or derived classes
    void logTransaction(const std::string& type, double amount) {
        auto now = std::chrono::system_clock::now();
        std::time_t timeNow = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;

        #if defined(_MSC_VER)
            struct tm buf;
            localtime_s(&buf, &timeNow);
            ss << std::put_time(&buf, "%Y-%m-%d %H:%M:%S");
        #else
            ss << std::put_time(std::localtime(&timeNow), "%Y-%m-%d %H:%M:%S");
        #endif

        transactionHistory.emplace_back(type, amount, ss.str());
    }

public:
    BankAccount();
    BankAccount(std::string accNum, std::string name, double initialBalance);
    BankAccount(const BankAccount& other);
    BankAccount& operator=(const BankAccount& other);

    // REQUIREMENT: Virtual destructor ensures proper cleanup of derived objects
    virtual ~BankAccount();

    std::string getAccountNumber() const;
    std::string getAccountHolderName() const;
    double getBalance() const;
    void setAccountHolderName(const std::string& name);

    // REQUIREMENT: Virtual withdraw method to allow polymorphic overriding
    virtual bool withdraw(double amount);
    BankAccount& operator+=(double amount);
    BankAccount& operator-=(double amount);

    bool operator==(const BankAccount& other) const;
    bool operator<(const BankAccount& other) const;
    bool operator>(const BankAccount& other) const;

    // REQUIREMENT: Public method to print the transaction log
    void printHistory() const;

    static void printAccount(const BankAccount& account);
};


class CheckingAccount : public BankAccount {
private:
    double transactionFee;

public:
    CheckingAccount(std::string accNum, std::string name, double initialBalance, double fee);

    // REQUIREMENT: Override withdrawal to apply the fee
    bool withdraw(double amount) override;
    double getTransactionFee() const { return transactionFee; }
};


class SavingsAccount : public BankAccount {
private:
    double interestRate;

public:
    SavingsAccount(std::string accNum, std::string name, double initialBalance, double rate);

    // REQUIREMENT: Public function to add interest to the balance
    void calculateInterest();
    double getInterestRate() const { return interestRate; }
};

// Global helpers
int findAccountIndex(const std::vector<std::unique_ptr<BankAccount>>& accounts, const std::string& accNum);
double getValidDouble(const std::string& prompt);
std::string getValidString(const std::string& prompt);

#endif // BANK_ACCOUNT_H
