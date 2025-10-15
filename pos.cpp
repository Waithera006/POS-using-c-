#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <sstream>
#include <array>

using namespace std;

// Payment Details Structure
struct PaymentDetails {
    int paymentId;
    string paymentMethod;
    double amount;
    string transactionTime;
    string status; // "Pending", "Completed", "Failed"
    string authorizationCode;
};

// Payment Processor Class
class PaymentProcessor {
private:
    static int nextPaymentId;
    PaymentDetails* currentPayment;

public:
    // Constructor and Destructor
    PaymentProcessor();
    ~PaymentProcessor();

    // Core payment methods
    bool processCashPayment(double amount, double tendered);
    bool processCardPayment(double amount, string cardNumber, string expiry, string cvv, string cardType);
    bool processMobilePayment(double amount, string mobileProvider);

    // Utility functions
    void displayPaymentReceipt();
    double calculateChange(double amount, double tendered);
    string generateAuthorizationCode();
    bool validateCard(string cardNumber, string expiry, string cvv);
    string getCurrentTime();
    PaymentDetails* getCurrentPayment() { return currentPayment; }
};

// Initialize static member
int PaymentProcessor::nextPaymentId = 1001;

// Transaction Manager Class
class TransactionManager {
private:
    vector<PaymentDetails*> transactionHistory;
    PaymentDetails** dailyTransactions;
    int dailyCount;
    int dailyCapacity;
    map<string, double> paymentMethodStats;
    array<string, 4> supportedMethods;

public:
    TransactionManager();
    ~TransactionManager();

    void addTransaction(PaymentDetails* transaction);
    PaymentDetails* findTransactionById(int paymentId);
    void generateDailyReport();
    void saveTransactionsToFile();
    void displayTransactionHistory();
    void updatePaymentStats(string method, double amount);
    void saveBinaryBackup();
    void logError(string errorMessage);
};

// PaymentProcessor Implementation
PaymentProcessor::PaymentProcessor() {
    currentPayment = nullptr;
    srand(time(0));
}

PaymentProcessor::~PaymentProcessor() {
    if (currentPayment != nullptr) {
        delete currentPayment;
        currentPayment = nullptr;
    }
}

string PaymentProcessor::getCurrentTime() {
    time_t now = time(0);
    char* dt = ctime(&now);
    string timeStr(dt);
    timeStr.pop_back(); // Remove newline
    return timeStr;
}

string PaymentProcessor::generateAuthorizationCode() {
    const char alphanum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string code = "AUTH-";
    for (int i = 0; i < 6; i++) {
        code += alphanum[rand() % 36];
    }
    return code;
}

double PaymentProcessor::calculateChange(double amount, double tendered) {
    return tendered - amount;
}

bool PaymentProcessor::validateCard(string cardNumber, string expiry, string cvv) {
    // Remove spaces from card number
    string cleanCard = "";
    for (char c : cardNumber) {
        if (isdigit(c)) cleanCard += c;
    }

    // Check card number length (16 digits)
    if (cleanCard.length() != 16) {
        cout << "ERROR: Invalid card number length (must be 16 digits)" << endl;
        return false;
    }

    // Validate expiry format (MM/YY)
    if (expiry.length() != 5 || expiry[2] != '/') {
        cout << "ERROR: Invalid expiry format (use MM/YY)" << endl;
        return false;
    }

    // Check CVV length (3-4 digits)
    if (cvv.length() < 3 || cvv.length() > 4) {
        cout << "ERROR: Invalid CVV length (must be 3-4 digits)" << endl;
        return false;
    }

    for (char c : cvv) {
        if (!isdigit(c)) {
            cout << "ERROR: CVV must contain only digits" << endl;
            return false;
        }
    }

    return true;
}

bool PaymentProcessor::processCashPayment(double amount, double tendered) {
    try {
        // Validate amount
        if (amount <= 0) {
            throw invalid_argument("Amount must be greater than zero");
        }

        if (tendered < amount) {
            throw invalid_argument("Insufficient cash tendered");
        }

        // Clean up previous payment
        if (currentPayment != nullptr) {
            delete currentPayment;
        }

        // Create new payment object
        currentPayment = new PaymentDetails();
        currentPayment->paymentId = nextPaymentId++;
        currentPayment->paymentMethod = "Cash";
        currentPayment->amount = amount;
        currentPayment->transactionTime = getCurrentTime();
        currentPayment->status = "Completed";
        currentPayment->authorizationCode = "CASH-" + to_string(currentPayment->paymentId);

        return true;
    }
    catch (exception& e) {
        cout << "ERROR: " << e.what() << endl;
        return false;
    }
}

bool PaymentProcessor::processCardPayment(double amount, string cardNumber, string expiry, string cvv, string cardType) {
    try {
        // Validate amount
        if (amount <= 0) {
            throw invalid_argument("Amount must be greater than zero");
        }

        // Validate card details
        if (!validateCard(cardNumber, expiry, cvv)) {
            throw invalid_argument("Card validation failed");
        }

        // Clean up previous payment
        if (currentPayment != nullptr) {
            delete currentPayment;
        }

        // Create new payment object
        currentPayment = new PaymentDetails();
        currentPayment->paymentId = nextPaymentId++;
        currentPayment->paymentMethod = cardType;
        currentPayment->amount = amount;
        currentPayment->transactionTime = getCurrentTime();

        // Simulate authorization (90% success rate)
        int authResult = rand() % 100;
        if (authResult < 90) {
            currentPayment->status = "Completed";
            currentPayment->authorizationCode = generateAuthorizationCode();
            return true;
        } else {
            currentPayment->status = "Failed";
            currentPayment->authorizationCode = "DECLINED";
            throw runtime_error("Payment Declined - Insufficient Funds");
        }
    }
    catch (exception& e) {
        cout << "ERROR: " << e.what() << endl;
        if (currentPayment != nullptr) {
            currentPayment->status = "Failed";
        }
        return false;
    }
}

bool PaymentProcessor::processMobilePayment(double amount, string mobileProvider) {
    try {
        // Validate amount
        if (amount <= 0) {
            throw invalid_argument("Amount must be greater than zero");
        }

        // Clean up previous payment
        if (currentPayment != nullptr) {
            delete currentPayment;
        }

        // Create new payment object
        currentPayment = new PaymentDetails();
        currentPayment->paymentId = nextPaymentId++;
        currentPayment->paymentMethod = "Mobile (" + mobileProvider + ")";
        currentPayment->amount = amount;
        currentPayment->transactionTime = getCurrentTime();

        // Simulate mobile payment processing (95% success rate)
        cout << "Processing mobile payment via " << mobileProvider << "..." << endl;
        int authResult = rand() % 100;
        if (authResult < 95) {
            currentPayment->status = "Completed";
            currentPayment->authorizationCode = generateAuthorizationCode();
            return true;
        } else {
            currentPayment->status = "Failed";
            currentPayment->authorizationCode = "TIMEOUT";
            throw runtime_error("Payment Timeout - Please try again");
        }
    }
    catch (exception& e) {
        cout << "ERROR: " << e.what() << endl;
        if (currentPayment != nullptr) {
            currentPayment->status = "Failed";
        }
        return false;
    }
}

void PaymentProcessor::displayPaymentReceipt() {
    if (currentPayment == nullptr) {
        cout << "No payment to display" << endl;
        return;
    }

    cout << "\n========================================" << endl;
    if (currentPayment->paymentMethod == "Cash") {
        cout << "===     CASH PAYMENT RECEIPT        ===" << endl;
    } else if (currentPayment->paymentMethod.find("Mobile") != string::npos) {
        cout << "===   MOBILE PAYMENT RECEIPT        ===" << endl;
    } else {
        cout << "===     CARD PAYMENT RECEIPT        ===" << endl;
    }
    cout << "Transaction ID: PAY-" << currentPayment->paymentId << endl;
    cout << "Payment Method: " << currentPayment->paymentMethod << endl;
    cout << "Amount: $" << fixed << setprecision(2) << currentPayment->amount << endl;
    cout << "Time: " << currentPayment->transactionTime << endl;
    cout << "Status: " << currentPayment->status << endl;
    cout << "Authorization: " << currentPayment->authorizationCode << endl;

    if (currentPayment->status == "Completed") {
        cout << "     Thank you for your purchase!      " << endl;
    } else {
        cout << "   Please use alternative payment      " << endl;
    }
    cout << "========================================\n" << endl;
}

// TransactionManager Implementation
TransactionManager::TransactionManager() {
    dailyCapacity = 100;
    dailyCount = 0;
    dailyTransactions = new PaymentDetails*[dailyCapacity];

    supportedMethods[0] = "Cash";
    supportedMethods[1] = "Credit Card";
    supportedMethods[2] = "Debit Card";
    supportedMethods[3] = "Mobile Payment";
}

TransactionManager::~TransactionManager() {
    // Clean up transaction history
    for (PaymentDetails* payment : transactionHistory) {
        delete payment;
    }
    transactionHistory.clear();

    // Clean up daily transactions array
    delete[] dailyTransactions;
}

void TransactionManager::addTransaction(PaymentDetails* transaction) {
    if (transaction == nullptr) return;

    // Create a copy for history
    PaymentDetails* historyCopy = new PaymentDetails(*transaction);
    transactionHistory.push_back(historyCopy);

    // Add to daily transactions array
    if (dailyCount < dailyCapacity) {
        dailyTransactions[dailyCount] = historyCopy;
        dailyCount++;
    }

    // Update payment statistics
    updatePaymentStats(transaction->paymentMethod, transaction->amount);

    // Save to file
    saveTransactionsToFile();

    // Log errors if payment failed
    if (transaction->status == "Failed") {
        logError("Payment ID: PAY-" + to_string(transaction->paymentId) +
                " | Method: " + transaction->paymentMethod +
                " | Amount: $" + to_string(transaction->amount) +
                " | Reason: " + transaction->authorizationCode);
    }
}

void TransactionManager::updatePaymentStats(string method, double amount) {
    paymentMethodStats[method] += amount;
}

PaymentDetails* TransactionManager::findTransactionById(int paymentId) {
    // Use pointer arithmetic to search daily transactions
    for (int i = 0; i < dailyCount; i++) {
        PaymentDetails* transaction = *(dailyTransactions + i);
        if (transaction->paymentId == paymentId) {
            return transaction;
        }
    }
    return nullptr;
}

void TransactionManager::saveTransactionsToFile() {
    ofstream outFile("transactions.txt", ios::app);
    if (!outFile.is_open()) {
        cout << "Error opening transactions file" << endl;
        return;
    }

    if (!transactionHistory.empty()) {
        PaymentDetails* lastTransaction = transactionHistory.back();
        if (lastTransaction->status == "Completed") {
            outFile << "PAY-" << lastTransaction->paymentId << " | "
                   << lastTransaction->paymentMethod << " | $"
                   << fixed << setprecision(2) << lastTransaction->amount << " | "
                   << lastTransaction->status << " | "
                   << lastTransaction->transactionTime << " | "
                   << lastTransaction->authorizationCode << endl;
        }
    }

    outFile.close();
}

void TransactionManager::logError(string errorMessage) {
    ofstream logFile("payment_errors.log", ios::app);
    if (!logFile.is_open()) {
        cout << "Error opening log file" << endl;
        return;
    }

    time_t now = time(0);
    char* dt = ctime(&now);
    string timeStr(dt);
    timeStr.pop_back();

    logFile << "[" << timeStr << "] " << errorMessage << endl;
    logFile.close();
}

void TransactionManager::saveBinaryBackup() {
    ofstream binFile("daily_summary.dat", ios::binary);
    if (!binFile.is_open()) {
        cout << "Error creating binary backup" << endl;
        return;
    }

    // Write number of transactions
    binFile.write(reinterpret_cast<char*>(&dailyCount), sizeof(int));

    // Write each transaction
    for (int i = 0; i < dailyCount; i++) {
        PaymentDetails* trans = dailyTransactions[i];
        binFile.write(reinterpret_cast<char*>(&trans->paymentId), sizeof(int));

        size_t methodLen = trans->paymentMethod.length();
        binFile.write(reinterpret_cast<char*>(&methodLen), sizeof(size_t));
        binFile.write(trans->paymentMethod.c_str(), methodLen);

        binFile.write(reinterpret_cast<char*>(&trans->amount), sizeof(double));
    }

    binFile.close();
    cout << "Binary backup saved successfully!" << endl;
}

void TransactionManager::displayTransactionHistory() {
    cout << "===   TRANSACTION HISTORY           ===" << endl;

    if (transactionHistory.empty()) {
        cout << "No transactions yet." << endl;
        cout << "========================================\n" << endl;
        return;
    }

    double totalCompleted = 0.0;
    int completedCount = 0;

    for (size_t i = 0; i < transactionHistory.size(); i++) {
        PaymentDetails* trans = transactionHistory[i];
        cout << (i + 1) << ". PAY-" << trans->paymentId << " | "
             << trans->paymentMethod << " | $"
             << fixed << setprecision(2) << trans->amount << " | "
             << trans->status << endl;

        if (trans->status == "Completed") {
            totalCompleted += trans->amount;
            completedCount++;
        }
    }

    cout << "----------------------------------------" << endl;
    cout << "Total Completed: $" << fixed << setprecision(2) << totalCompleted << endl;
    cout << "Success Rate: " << (transactionHistory.size() > 0 ?
           (completedCount * 100.0 / transactionHistory.size()) : 0) << "%" << endl;

    // Display payment method statistics
    cout << "\nPayment Method Statistics:" << endl;
    for (const auto& stat : paymentMethodStats) {
        cout << "  " << stat.first << ": $" << fixed << setprecision(2) << stat.second << endl;
    }

    cout << "========================================\n" << endl;
}

void TransactionManager::generateDailyReport() {
    cout << "\n========================================" << endl;
    cout << "===      DAILY REPORT               ===" << endl;
    cout << "========================================" << endl;
    cout << "Total Transactions: " << dailyCount << endl;

    double totalRevenue = 0.0;
    int successCount = 0;

    for (int i = 0; i < dailyCount; i++) {
        if (dailyTransactions[i]->status == "Completed") {
            totalRevenue += dailyTransactions[i]->amount;
            successCount++;
        }
    }

    cout << "Successful Transactions: " << successCount << endl;
    cout << "Total Revenue: $" << fixed << setprecision(2) << totalRevenue << endl;
    cout << "Success Rate: " << (dailyCount > 0 ? (successCount * 100.0 / dailyCount) : 0) << "%" << endl;
    cout << "========================================\n" << endl;
}

// Main POS System
void runPOSSystem() {
    PaymentProcessor processor;
    TransactionManager manager;

    int choice;
    double amount, tendered;
    string cardNumber, expiry, cvv, provider;

    do {
        cout << "\n=======================================" << endl;
        cout << "=== PAYMENT PROCESSING SYSTEM      ===" << endl;
        cout << "=======================================" << endl;
        cout << "1. Cash Payment" << endl;
        cout << "2. Credit Card Payment" << endl;
        cout << "3. Debit Card Payment" << endl;
        cout << "4. Mobile Payment" << endl;
        cout << "5. View Transaction History" << endl;
        cout << "6. Generate Daily Report" << endl;
        cout << "7. Save Binary Backup" << endl;
        cout << "8. Exit" << endl;
        cout << "=======================================" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch(choice) {
            case 1: {
                cout << "\n=== CASH PAYMENT ===" << endl;
                cout << "Enter amount due: $";
                cin >> amount;
                cout << "Enter cash tendered: $";
                cin >> tendered;

                if (processor.processCashPayment(amount, tendered)) {
                    double change = processor.calculateChange(amount, tendered);
                    cout << "\nChange Due: $" << fixed << setprecision(2) << change << endl;
                    processor.displayPaymentReceipt();
                    manager.addTransaction(processor.getCurrentPayment());
                }
                break;
            }

            case 2: {
                cout << "\n=== CREDIT CARD PAYMENT ===" << endl;
                cout << "Enter amount: $";
                cin >> amount;
                cin.ignore();
                cout << "Enter card number (16 digits): ";
                getline(cin, cardNumber);
                cout << "Enter expiry (MM/YY): ";
                getline(cin, expiry);
                cout << "Enter CVV: ";
                getline(cin, cvv);

                if (processor.processCardPayment(amount, cardNumber, expiry, cvv, "Credit Card")) {
                    processor.displayPaymentReceipt();
                    manager.addTransaction(processor.getCurrentPayment());
                } else {
                    processor.displayPaymentReceipt();
                    manager.addTransaction(processor.getCurrentPayment());
                }
                break;
            }

            case 3: {
                cout << "\n=== DEBIT CARD PAYMENT ===" << endl;
                cout << "Enter amount: $";
                cin >> amount;
                cin.ignore();
                cout << "Enter card number (16 digits): ";
                getline(cin, cardNumber);
                cout << "Enter expiry (MM/YY): ";
                getline(cin, expiry);
                cout << "Enter CVV: ";
                getline(cin, cvv);

                if (processor.processCardPayment(amount, cardNumber, expiry, cvv, "Debit Card")) {
                    processor.displayPaymentReceipt();
                    manager.addTransaction(processor.getCurrentPayment());
                } else {
                    processor.displayPaymentReceipt();
                    manager.addTransaction(processor.getCurrentPayment());
                }
                break;
            }

            case 4: {
                cout << "\n=== MOBILE PAYMENT ===" << endl;
                cout << "Enter amount: $";
                cin >> amount;
                cin.ignore();
                cout << "Select Provider:" << endl;
                cout << "1. PayPal" << endl;
                cout << "2. Apple Pay" << endl;
                cout << "3. Google Pay" << endl;
                cout << "Choice: ";
                int providerChoice;
                cin >> providerChoice;

                switch(providerChoice) {
                    case 1: provider = "PayPal"; break;
                    case 2: provider = "Apple Pay"; break;
                    case 3: provider = "Google Pay"; break;
                    default: provider = "Unknown"; break;
                }

                if (processor.processMobilePayment(amount, provider)) {
                    processor.displayPaymentReceipt();
                    manager.addTransaction(processor.getCurrentPayment());
                } else {
                    processor.displayPaymentReceipt();
                    manager.addTransaction(processor.getCurrentPayment());
                }
                break;
            }

            case 5:
                manager.displayTransactionHistory();
                break;

            case 6:
                manager.generateDailyReport();
                break;

            case 7:
                manager.saveBinaryBackup();
                break;

            case 8:
                cout << "\nThank you for using the POS System!" << endl;
                cout << "Saving final backup..." << endl;
                manager.saveBinaryBackup();
                break;

            default:
                cout << "Invalid choice. Please try again." << endl;
        }

    } while(choice != 8);
}

// Main function
int main() {
    cout << "========================================" << endl;
    cout << "   POINT OF SALE PAYMENT SYSTEM        " << endl;
    cout << "   Object-Oriented Programming in C++  " << endl;
    cout << "========================================\n" << endl;

    runPOSSystem();

    return 0;
}
