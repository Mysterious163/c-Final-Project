#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <map>
#include <stdexcept>

using namespace std;

enum class Type {
    Income,
    Expense
};

class Transaction {
private:
    string category;
    double amount;
    Type type;

public:
    Transaction(const string& category, double amount, Type type) {
        if (amount < 0)
            throw invalid_argument("Amount cannot be negative!");

        this->category = category;
        this->amount = amount;
        this->type = type;
    }

    double getAmount() const { return amount; }
    string getCategory() const { return category; }
    Type getType() const { return type; }

    void display() const {
        cout << left << setw(15) << category
             << setw(10) << amount
             << (type == Type::Income ? "Income" : "Expense") << endl;
    }
};

class FinanceManager {
private:
    vector<unique_ptr<Transaction>> transactions;

public:
    void addTransaction(const string& category, double amount, Type type) {
        transactions.push_back(make_unique<Transaction>(category, amount, type));
    }

    void showAll() const {
        if (transactions.empty()) {
            cout << "No transactions found.\n";
            return;
        }

        cout << "\nCategory        Amount    Type\n";
        cout << "----------------------------------\n";

        for (const auto& t : transactions)
            t->display();
    }

    void sortByAmount() {
        sort(transactions.begin(), transactions.end(),
            [](const unique_ptr<Transaction>& a, const unique_ptr<Transaction>& b) {
                return a->getAmount() < b->getAmount();
            });
    }

    double calculateBalance() const {
        double balance = 0;

        for (const auto& t : transactions) {
            if (t->getType() == Type::Income)
                balance += t->getAmount();
            else
                balance -= t->getAmount();
        }

        return balance;
    }

    void showStatistics() const {
        map<string, double> categoryTotals;

        for (const auto& t : transactions) {
            if (t->getType() == Type::Expense)
                categoryTotals[t->getCategory()] += t->getAmount();
        }

        cout << "\nExpense Statistics:\n";
        for (const auto& pair : categoryTotals) {
            cout << pair.first << ": " << pair.second << endl;
        }
    }

    void saveToFile() const {
        ofstream file("finance.txt");
        for (const auto& t : transactions) {
            file << t->getCategory() << " "
                 << t->getAmount() << " "
                 << (t->getType() == Type::Income ? 1 : 0)
                 << endl;
        }
        file.close();
        cout << "Saved to file successfully.\n";
    }

    void loadFromFile() {
        ifstream file("finance.txt");
        if (!file.is_open()) {
            cout << "No saved file found.\n";
            return;
        }

        string category;
        double amount;
        int typeInt;

        while (file >> category >> amount >> typeInt) {
            Type type = (typeInt == 1 ? Type::Income : Type::Expense);
            transactions.push_back(make_unique<Transaction>(category, amount, type));
        }

        file.close();
        cout << "Loaded data from file.\n";
    }
};

int main() {
    FinanceManager manager;
    manager.loadFromFile();

    int choice;

    while (true) {
        cout << "\n===== SMART FINANCE MANAGER =====\n";
        cout << "1. Add Income\n";
        cout << "2. Add Expense\n";
        cout << "3. Show All Transactions\n";
        cout << "4. Show Balance\n";
        cout << "5. Sort by Amount\n";
        cout << "6. Show Expense Statistics\n";
        cout << "7. Save & Exit\n";
        cout << "Choice: ";

        cin >> choice;

        try {
            if (choice == 1 || choice == 2) {
                string category;
                double amount;

                cout << "Enter category: ";
                cin >> category;
                cout << "Enter amount: ";
                cin >> amount;

                Type type = (choice == 1 ? Type::Income : Type::Expense);
                manager.addTransaction(category, amount, type);
            }
            else if (choice == 3) {
                manager.showAll();
            }
            else if (choice == 4) {
                cout << "Current Balance: "
                     << manager.calculateBalance() << endl;
            }
            else if (choice == 5) {
                manager.sortByAmount();
                cout << "Sorted successfully.\n";
            }
            else if (choice == 6) {
                manager.showStatistics();
            }
            else if (choice == 7) {
                manager.saveToFile();
                break;
            }
            else {
                cout << "Invalid choice.\n";
            }
        }
        catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }

    return 0;
}
