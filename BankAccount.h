#ifndef BANK_ACCOUNT_H
#define BANK_ACCOUNT_H

#include <iostream>
#include <vector>
#include <string>
#include <memory>

// Quentin Jenkins 10/5/26
// Operator Overload & Inheritance Lab

class BankAccount {
protected: // Changed from private so derived classes can read/write if needed
    std::string accountNumber;
    std::string* accountHolderName;
    double* balance;

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
    BankAccount& operator-=(double amount); // Will now call the virtual withdraw()

    bool operator==(const BankAccount& other) const;
    bool operator<(const BankAccount& other) const;
    bool operator>(const BankAccount& other) const;

    static void printAccount(const BankAccount& account);
};

// REQUIREMENT: CheckingAccount Class
class CheckingAccount : public BankAccount {
private:
    double transactionFee;

public:
    CheckingAccount(std::string accNum, std::string name, double initialBalance, double fee);

    // REQUIREMENT: Override withdrawal to apply the fee
    bool withdraw(double amount) override;
    double getTransactionFee() const { return transactionFee; }
};

// REQUIREMENT: SavingsAccount Class
class SavingsAccount : public BankAccount {
private:
    double interestRate; // e.g., 0.02 for 2%

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
