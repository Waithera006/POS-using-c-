Point of Sale - Payment Processing System 
Subject: Object-Oriented Programming with C++ 
PROBLEM STATEMENT 
You are required to implement a Payment Processing System for a Point of Sale (POS) 
application. The system should handle multiple payment methods including Cash, Credit 
Card, Debit Card, and Mobile Payments. 
REQUIREMENTS 
1. Data Structures & Classes 
2. Pointer Operations 
Implement the following pointer-based functionality: 
 Use raw pointers for dynamic payment object creation 
 Implement array of pointers for transaction history 
 Create pointer arithmetic for transaction searching 
 Ensure proper memory management with delete operations 
3. File Handling
 Save successful transactions to a text file transactions.txt 
 Log failed payment attempts to payment_errors.log 
 Implement binary file backup for daily transaction summaries 
4. STL Containers 
 Use vector<PaymentDetails*> for transaction history 
 Implement map<string, double> for payment method statistics 
 Use array<string, 4> for supported payment methods 
5. Error Handling & Validation 
 Implement exception handling for payment failures 
 Add input validation for card numbers and amounts 
 Create custom error messages for different failure scenarios 
 
IMPLEMENTATION TASKS 
Task 1: Basic Class Implementation 
cpp 
// Implement constructor and destructor 
PaymentProcessor::PaymentProcessor() { 
    nextPaymentId = 1001; 
    currentPayment = nullptr; 
} 
 
PaymentProcessor::~PaymentProcessor() { 
    // Clean up dynamic memory 
    if (currentPayment != nullptr) { 
        delete currentPayment; 
        currentPayment = nullptr; 
    } 
} 
Task 2: Cash Payment Processing 
cpp 
bool PaymentProcessor::processCashPayment(double amount, double tendered) { 
    // TODO: Implement cash payment logic 
    // - Validate amount and tendered cash 
    // - Calculate change 
    // - Create PaymentDetails object 
    // - Update transaction history 
    // - Return success status 
} 
Task 3: Card Payment Validation 
cpp 
bool PaymentProcessor::validateCard(string cardNumber, string expiry, string cvv) { 
    // TODO: Implement card validation 
    // - Check card number length (16 digits) 
    // - Validate expiry date format (MM/YY) 
    // - Check CVV length (3-4 digits) 
    // - Return validation result 
} 
Task 4: Transaction History Management 
cpp 
class TransactionManager { 
private: 
    vector<PaymentDetails*> transactionHistory; 
    PaymentDetails** dailyTransactions; // Array of pointers 
    int dailyCount; 
     
public: 
    void addTransaction(PaymentDetails* transaction); 
    PaymentDetails* findTransactionById(int paymentId); 
    void generateDailyReport(); 
    void saveTransactionsToFile(); 
}; 
Task 5: Main Application Loop 
cpp 
void runPOSSystem() { 
    PaymentProcessor processor; 
    int choice; 
    double amount; 
     
    do { 
        cout << "\n=== PAYMENT PROCESSING SYSTEM ===" << endl; 
        cout << "1. Cash Payment" << endl; 
        cout << "2. Credit Card Payment" << endl; 
        cout << "3. Debit Card Payment" << endl; 
        cout << "4. Mobile Payment" << endl; 
        cout << "5. View Transaction History" << endl; 
        cout << "6. Exit" << endl; 
        cout << "Enter your choice: "; 
        cin >> choice; 
         
        switch(choice) { 
            case 1: 
                cout << "Enter amount: $"; 
                cin >> amount; 
                // TODO: Implement cash payment interface 
                break; 
            case 2: 
                // TODO: Implement credit card payment interface 
                break; 
            // TODO: Implement other cases 
        } 
    } while(choice != 6); 
} 
 
SPECIFIC IMPLEMENTATION GUIDELINES 
For Cash Payments: 
 Calculate and display change 
 Handle insufficient cash scenarios 
 Generate receipt with breakdown 
For Card Payments: 
 Simulate authorization process 
 Generate random authorization codes 
 Handle declined transactions 
For Mobile Payments: 
 Support multiple providers (PayPal, Apple Pay, Google Pay) 
 Simulate mobile payment processing 
 Handle timeout scenarios 
EXPECTED OUTPUT SAMPLES 
Sample 1: Successful Cash Payment 
text 
=== CASH PAYMENT === 
Amount Due: $45.50 
Cash Tendered: $50.00 
Change Due: $4.50 
Payment Status: APPROVED 
Transaction ID: PAY-1001 
Thank you for your purchase! 
Sample 2: Credit Card Payment 
text 
=== CREDIT CARD PAYMENT === 
Amount: $125.75 
Card Number: **** **** **** 1234 
Expiry: 12/25 
Processing... 
Authorization Code: AUTH-7B92X8 
Transaction ID: PAY-1002 
Status: COMPLETED 
Sample 3: Declined Card 
text 
=== DEBIT CARD PAYMENT === 
Amount: $89.99 
Card Number: **** **** **** 5678 
ERROR: Payment Declined 
Reason: Insufficient Funds 
Please use alternative payment method 
Sample 4: Transaction History 
text 
=== TRANSACTION HISTORY === 
1. PAY-1001 | Cash       
| $45.50  | COMPLETED 
2. PAY-1002 | Credit Card| $125.75 | COMPLETED   
3. PAY-1003 | Debit Card | $89.99  | FAILED 
4. PAY-1004 | Mobile     | $32.25  | COMPLETED 
Total Completed: $203.50 
Success Rate: 75% 

created by MARY WAITHERA
