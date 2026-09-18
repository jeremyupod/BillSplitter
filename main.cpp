#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>
#include <limits>
#include <cmath>

using namespace std;

class Person {
public:
    string name;
    double subtotal = 0.0;
    double tax = 0.0;
    double tip = 0.0;
    double finalTotal = 0.0;

    Person(string personName) : name(personName) {}
};

class Item {
public:
    string name;
    double price;
    vector<int> assignedTo;
    
    Item(string itemName, double itemPrice, vector<int> people)
        : name(itemName), price(itemPrice), assignedTo(people) {}
};

class BillSplitter {
public:
    vector<Person> people;
    vector<Item> items;
    double taxRate;
    double tipRate;
    double tipAmount;
    bool useFixedTip;

    BillSplitter(double tax, double tip, double amount, bool fixed) : taxRate(tax), tipRate(tip), tipAmount(amount), useFixedTip(fixed){}

    void addPerson(string name) {
        people.push_back(Person(name));
    }

    void addItem(string name, double price, vector<int> assignedTo) {
        items.push_back(Item(name, price, assignedTo));
    }

    double calculateTotal() {
        double total = 0.0;
        for (const Item& item :items) {
            total += item.price;
        }
        return total;
    }

    void calculateSplits() {
        for (Person& person : people) {
            person.subtotal = 0.0;
        }

        for (const Item& item : items) {
            if (item.assignedTo.empty()) {
                continue;
            }
            double splitAmount = item.price / item.assignedTo.size();
            for (int personIndex: item.assignedTo) {
                people[personIndex].subtotal += splitAmount;
            }
        }

        double sumOfRoundedTotals = 0.0;
        for (Person& person : people) {
            person.tax = person.subtotal * taxRate;
            
            if (useFixedTip) {
                double total = calculateTotal();
            
                if (total > 0) {
                    person.tip = (person.subtotal / total) * tipAmount;
                }
                else {
                    person.tip = 0.0;
                }
            }
            else {
                person.tip = person.subtotal * tipRate;
            }

            double raw = person.subtotal + person.tax + person.tip;
            person.finalTotal = round(raw * 100.0) / 100.0;
            sumOfRoundedTotals += person.finalTotal;
        }

        double expectedGrandTotal;

        if (useFixedTip) {
            expectedGrandTotal = round((calculateTotal() * (1 + taxRate) + tipAmount) * 100.0) / 100.0;
        }
        
        else {
             expectedGrandTotal = round(calculateTotal() * (1 + taxRate + tipRate) * 100.0) / 100.0;
        }
   
            double leftover = expectedGrandTotal - sumOfRoundedTotals;
        
            if (abs(leftover) >= 0.01 && !people.empty()) {
                people[0].finalTotal += leftover;
        }
    }
    void printResults() {
        cout << fixed << setprecision(2);
        cout << "\n----------------------\n";
        cout << "Total: $" << calculateTotal() << "\n";
        cout << "----------------------\n\n";

        for (const Person& person : people) {
            cout << person.name << ":\n";
            cout << "  Subtotal: $" << person.subtotal << "\n";
            cout << "  Tax:      $" << person.tax << "\n";
            cout << "  Tip:      $" << person.tip << "\n";
            cout << "  Total:    $" << person.finalTotal << "\n\n";
        }
    }

    void settleUp(int payerIndex) {
        cout << "\n=== Settling Up ===\n";

        double totalOwedToPayer = 0.0;
       
        for (size_t i = 0; i < people.size(); i++) {
            if ((int)i == payerIndex) continue;
            cout << "  " << people[i].name << " pays " << people[payerIndex].name
                       << ": $" << people[i].finalTotal << "\n";
            totalOwedToPayer += people[i].finalTotal;
        }

        cout << "\n " << people[payerIndex].name << "'s own share: $" << people[payerIndex].finalTotal << "\n";
        cout << " " << people[payerIndex].name << " will be reimbursed a total of: $" << totalOwedToPayer << "\n";
    }
};

int readValidInt(const string& prompt){
    int value;
    while (true){
        cout << prompt;
        cin >> value;
        if (cin.fail()){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << " Invalid number, please try again!\n";
        }
        else{
            return value;
        }

    }
}

double readValidDouble(const string& prompt){
    double value;
    while (true){
        cout << prompt;
        cin >> value;
        if (cin.fail()){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << " Invalid number, please try again!\n";
        }
        else{
                return value;
            }
        }
}

int main()
{
    cout << "=== Bill Splitter ===\n\n";

    double taxPercent = readValidDouble("Enter the tax rate (%): ");
    double taxRate = taxPercent / 100.0;

    cout << "\nHow would you like to enter the tip?\n";
    cout << " 1: Percentage\n";
    cout << " 2: Dollar amount\n";

    int tipChoice;

    while(true) {
        tipChoice = readValidInt("Enter choice: ");

        if (tipChoice == 1 || tipChoice == 2) {
            break;
        }
        cout << " Invalid choice. Please enter 1 or 2.\n";
    }

    double tipRate = 0.0;      
    double tipAmount = 0.0;
    bool useFixedTip = false;

    if (tipChoice == 1) {
        double tipPercent = readValidDouble("Enter the tip rate (%): ");
        tipRate = tipPercent / 100.0;
    }
    else {
        tipAmount = readValidDouble("Enter the tip amount: $");
        useFixedTip = true;
    }

    BillSplitter splitter(taxRate, tipRate, tipAmount, useFixedTip);

    int numPeople = readValidInt("How many people are splitting the bill? ");
    for(int i = 0; i < numPeople; i++){
        string name;
        cout << " Name of person " << (i + 1) << ": ";
        cin >> name;
        splitter.addPerson(name);
    }

    int numItems = readValidInt("\nHow many items are on the receipt? ");
    cin.ignore();

    for(int i = 0; i < numItems; i++){
        string name;
        cout << "\n Item " << (i + 1) << " name: ";
        getline(cin, name);
        
        double price = readValidDouble(" Item " + to_string(i+1) + " price: $");
        
        vector<int> assignedTo;
        while(assignedTo.empty()){
            cout << " Who had this? Options:\n";
            for (size_t p = 0; p < splitter.people.size(); p++){
                cout << " " << (p + 1) << ": " << splitter.people[p].name << "\n";
            }
            cout << " Enter numbers separated by spaces: ";

            cin.ignore(); // discards leftover newline from previous cin
            string line;
            getline(cin, line);

            stringstream ss(line);
            int index;
            vector<int> attempt;
            bool lineIsValid = true;

            while (ss >> index){
                int realIndex = index - 1;
                if (realIndex < 0 || realIndex >= (int)splitter.people.size()){
                    cout << " Invalid index " << index << " - please re-enter the whole list.\n";
                    lineIsValid = false;
                    break;
                }
                attempt.push_back(realIndex);
            }
            if (lineIsValid && !attempt.empty()){
                assignedTo = attempt;
            }
                else if(lineIsValid && attempt.empty()){
                    cout << " You must assign this item to at least one person. Please try again.\n";
            }
        }
        splitter.addItem(name, price, assignedTo);
    }
    
    splitter.calculateSplits();
    splitter.printResults();

    cout << "----------------------\n";
    cout << "Who paid the bill? Enter their number:\n";
    for (size_t p = 0; p < splitter.people.size(); p++) {
        cout << "  " << (p + 1) << ": " << splitter.people[p].name << "\n";
    }

    int payerIndex;

    while(true) {
        payerIndex = readValidInt("Enter number: ") - 1;

        if (payerIndex >= 0 && payerIndex < (int)splitter.people.size()) {
            break;
        }
        cout << " Invalid person number. Please try again.\n";
    }
    splitter.settleUp(payerIndex);

    return 0;
}