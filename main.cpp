#include "BankAccount.h"
#include <iostream>
#include <vector>
#include <memory>
#include <limits>
#include <iomanip>

int main() {
    // REQUIREMENT: Container utilizing unique_ptr managing base class allocations
    std::vector<std::unique_ptr<BankAccount>> bank;
    int choice = 0;

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "=== Welcome to the Banking Management System ===\n";

    do {
        std::cout << "\n--- Main Menu ---\n";
        std::cout << "1. Create New Checking Account\n";
        std::cout << "2. Create New Savings Account\n";
        std::cout << "3. Deposit Funds (via +=)\n";
        std::cout << "4. Withdraw Funds (Demonstrating Polymorphism)\n";
        std::cout << "5. Apply Savings Interest\n";
        std::cout << "6. Display All Accounts\n";
        std::cout << "7. Find Wealthiest & Poorest Accounts\n";
        std::cout << "8. Exit\n";

        std::cout << "Enter your choice (1-8): ";
        if (!(std::cin >> choice)) {
            std::cout << "Invalid input.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1: { // REQUIREMENT: Add instances of CheckingAccount
                std::string accNum = getValidString("Enter Unique Account Number: ");
                if (findAccountIndex(bank, accNum) != -1) {
                    std::cout << "Error: Account number already exists!\n";
                    break;
                }
                std::string name = getValidString("Enter Account Holder Name: ");
                double initialBalance = getValidDouble("Enter Initial Balance: ");
                double fee = getValidDouble("Enter Transaction Fee: ");

                bank.push_back(std::make_unique<CheckingAccount>(accNum, name, initialBalance, fee));
                std::cout << "Checking Account created successfully!\n";
                break;
            }
            case 2: { // REQUIREMENT: Add instances of SavingsAccount
                std::string accNum = getValidString("Enter Unique Account Number: ");
                if (findAccountIndex(bank, accNum) != -1) {
                    std::cout << "Error: Account number already exists!\n";
                    break;
                }
                std::string name = getValidString("Enter Account Holder Name: ");
                double initialBalance = getValidDouble("Enter Initial Balance: ");
                double rate = getValidDouble("Enter Interest Rate (e.g. 0.05 for 5%): ");

                bank.push_back(std::make_unique<SavingsAccount>(accNum, name, initialBalance, rate));
                std::cout << "Savings Account created successfully!\n";
                break;
            }
            case 3: {
                std::string accNum = getValidString("Enter Account Number: ");
                int index = findAccountIndex(bank, accNum);
                if (index == -1) {
                    std::cout << "Account not found.\n";
                } else {
                    double amount = getValidDouble("Enter deposit amount: ");
                    *bank[index] += amount;
                    std::cout << "Deposit successful! New Balance: $" << bank[index]->getBalance() << "\n";
                }
                break;
            }
            case 4: { // REQUIREMENT: Demonstrate polymorphism via Base pointer withdrawal
                std::string accNum = getValidString("Enter Account Number: ");
                int index = findAccountIndex(bank, accNum);
                if (index == -1) {
                    std::cout << "Account not found.\n";
                } else {
                    double amount = getValidDouble("Enter withdrawal amount: ");

                    // Capturing base class pointer reference
                    BankAccount* accountPtr = bank[index].get();

                    // Polymorphic behavior triggered right here!
                    if (accountPtr->withdraw(amount)) {
                        std::cout << "Withdrawal verified. New Balance: $" << accountPtr->getBalance() << "\n";
                    } else {
                        std::cout << "Withdrawal failed. Check balances or transaction fee conditions.\n";
                    }
                }
                break;
            }
            case 5: { // Apply Interest
                std::string accNum = getValidString("Enter Savings Account Number: ");
                int index = findAccountIndex(bank, accNum);
                if (index == -1) {
                    std::cout << "Account not found.\n";
                } else {
                    // Try downcasting to find out if it's a Savings account
                    SavingsAccount* savingsPtr = dynamic_cast<SavingsAccount*>(bank[index].get());
                    if (savingsPtr) {
                        savingsPtr->calculateInterest();
                    } else {
                        std::cout << "This account is not a Savings Account.\n";
                    }
                }
                break;
            }
            case 6: {
                if (bank.empty()) {
                    std::cout << "No accounts registered.\n";
                } else {
                    std::cout << "\nRegistered Accounts\n";
                    for (const auto &account: bank) {
                        BankAccount::printAccount(*account);
                    }
                }
                break;
            }
            case 7: {
                if (bank.size() < 2) {
                    std::cout << "At least two accounts are needed to perform a balance comparison.\n";
                    break;
                }
                size_t wealthiestIdx = 0;
                size_t poorestIdx = 0;

                for (size_t i = 1; i < bank.size(); ++i) {
                    if (*bank[i] > *bank[wealthiestIdx]) wealthiestIdx = i;
                    if (*bank[i] < *bank[poorestIdx]) poorestIdx = i;
                }

                std::cout << "\n--- Financial Ranking Comparison ---\n";
                std::cout << "Highest Balance: ";
                BankAccount::printAccount(*bank[wealthiestIdx]);
                std::cout << "Lowest Balance:  ";
                BankAccount::printAccount(*bank[poorestIdx]);
                break;
            }
            case 8:
                std::cout << "Thank you for using the Banking Management System. Goodbye!\n";
                break;
            default:
                std::cout << "Invalid choice.\n";
                break;
        }
    } while (choice != 8);

    return 0;
}
