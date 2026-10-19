#include "BankAccount.h"
#include <limits>
#include <iomanip>
#include <utility>

BankAccount::BankAccount() {
    accountNumber = "";
    accountHolderName = new std::string("");
    balance = new double(0.0);
}

BankAccount::BankAccount(std::string accNum, std::string name, double initialBalance) {
    accountNumber = std::move(accNum);
    accountHolderName = new std::string(std::move(name));
    balance = new double(initialBalance >= 0.0 ? initialBalance : 0.0);
}

BankAccount::BankAccount(const BankAccount& other) {
    accountNumber = other.accountNumber;
    accountHolderName = new std::string(*other.accountHolderName);
    balance = new double(*other.balance);
}

BankAccount& BankAccount::operator=(const BankAccount& other) {
    if (this != &other) {
        std::string* newName = new std::string(*other.accountHolderName);
        double* newBalance = new double(*other.balance);
        delete accountHolderName;
        delete balance;
        accountNumber = other.accountNumber;
        accountHolderName = newName;
        balance = newBalance;
    }
    return *this;
}

BankAccount::~BankAccount() {
    delete accountHolderName;
    delete balance;
}

std::string BankAccount::getAccountNumber() const { return accountNumber; }
std::string BankAccount::getAccountHolderName() const { return *accountHolderName; }
double BankAccount::getBalance() const { return *balance; }

void BankAccount::setAccountHolderName(const std::string& name) {
    if (!name.empty()) {
        *accountHolderName = name;
    }
}

BankAccount& BankAccount::operator+=(double amount) {
    if (amount > 0) {
        *balance += amount;
    }
    return *this;
}

// Basic Base Class Withdrawal
bool BankAccount::withdraw(double amount) {
    if (amount > 0 && amount <= *balance) {
        *balance -= amount;
        return true;
    }
    return false;
}

// -= calls the virtual function to ensure runtime polymorphism works here too
BankAccount& BankAccount::operator-=(double amount) {
    withdraw(amount);
    return *this;
}

bool BankAccount::operator==(const BankAccount& other) const {
    return this->accountNumber == other.accountNumber;
}

bool BankAccount::operator<(const BankAccount& other) const {
    return *this->balance < *other.balance;
}

bool BankAccount::operator>(const BankAccount& other) const {
    return *this->balance > *other.balance;
}

void BankAccount::printAccount(const BankAccount& account) {
    std::cout << "Acc No: " << account.getAccountNumber()
              << " | Holder: " << account.getAccountHolderName()
              << " | Balance: $" << account.getBalance() << "\n";
}

// --- CheckingAccount Implementation ---
CheckingAccount::CheckingAccount(std::string accNum, std::string name, double initialBalance, double fee)
    : BankAccount(std::move(accNum), std::move(name), initialBalance) {
    transactionFee = (fee >= 0.0) ? fee : 0.0;
}

bool CheckingAccount::withdraw(double amount) {
    double totalDeduction = amount + transactionFee;
    if (amount > 0 && totalDeduction <= *balance) {
        *balance -= totalDeduction;
        std::cout << "[Fee Applied: $" << transactionFee << "]\n";
        return true;
    }
    return false;
}

// --- SavingsAccount Implementation ---
SavingsAccount::SavingsAccount(std::string accNum, std::string name, double initialBalance, double rate)
    : BankAccount(std::move(accNum), std::move(name), initialBalance) {
    interestRate = (rate >= 0.0) ? rate : 0.0;
}

void SavingsAccount::calculateInterest() {
    double interest = (*balance) * interestRate;
    *balance += interest;
    std::cout << "Interest added: $" << interest << " (Rate: " << (interestRate * 100) << "%)\n";
}

// --- Helper Functions ---
int findAccountIndex(const std::vector<std::unique_ptr<BankAccount>>& accounts, const std::string& accNum) {
    for (size_t i = 0; i < accounts.size(); ++i) {
        if (accounts[i]->getAccountNumber() == accNum) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

double getValidDouble(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        std::cout << "Invalid numeric input. Please try again.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

std::string getValidString(const std::string& prompt) {
    std::string value;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, value);
        if (!value.empty()) {
            return value;
        }
        std::cout << "Input cannot be empty. Please try again.\n";
    }
}
void BankAccount::printHistory() const {
    std::cout << "\n--- History for Acc #: " << accountNumber << " ---\n";
    if (transactionHistory.empty()) {
        std::cout << "No recorded transactions found.\n";
        return;
    }
    for (const auto& tx : transactionHistory) {
        std::cout << "[" << tx.timestamp << "] " << tx.type << ": $" << tx.amount << "\n";
    }
}