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
    std::string name;
    double price;
    std::vector<int> assignedTo;
    
    Item(std::string itemName, double itemPrice, std::vector<int> people)
        : name(itemName), price(itemPrice), assignedTo(people) {}
};

class BillSplitter {
public:
    vector<Person> people;
    vector<Item> items;
    double taxRate;
    double tipRate;

    BillSplitter(double tax, double tip) : taxRate(tax), tipRate(tip) {}

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
            person.tip = person.subtotal * tipRate;
            double raw = person.subtotal + person.tax + person.tip;
            person.finalTotal = round(raw * 100.0) / 100.0;
            sumOfRoundedTotals += person.finalTotal;
        }

        double expectedGrandTotal = round(calculateTotal() * (1 + taxRate + tipRate) * 100.0) / 100.0;
        double leftover = expectedGrandTotal - sumOfRoundedTotals;
        if (abs(leftover) >= 0.01 && !people.empty()) {
            people[0].finalTotal += leftover;
        }
    }
    void printResults() {
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "\n----------------------\n";
        std::cout << "Total: $" << calculateTotal() << "\n";
        std::cout << "----------------------\n\n";

        for (const Person& person : people) {
            std::cout << person.name << ":\n";
            std::cout << "  Subtotal: $" << person.subtotal << "\n";
            std::cout << "  Tax:      $" << person.tax << "\n";
            std::cout << "  Tip:      $" << person.tip << "\n";
            std::cout << "  Total:    $" << person.finalTotal << "\n\n";
        }
    }

    void settleUp(int payerIndex) {
        std::cout << "\n=== Settling Up ===\n";

        double totalOwedToPayer = 0.0;
       
        for (size_t i = 0; i < people.size(); i++) {
            if ((int)i == payerIndex) continue;
            std::cout << "  " << people[i].name << " pays " << people[payerIndex].name
                       << ": $" << people[i].finalTotal << "\n";
            totalOwedToPayer += people[i].finalTotal;
        }

        std::cout << "\n " << people[payerIndex].name << "'s own share: $" << people[payerIndex].finalTotal << "\n";
        std::cout << " " << people[payerIndex].name << " will be reimbursed a total of: $" << totalOwedToPayer << "\n";
    }
};

int readValidInt(const std::string& prompt){
    int value;
    while (true){
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail()){
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << " Invalid number, please try again!\n";
        }
        else{
            return value;
        }

    }
}

double readValidDouble(const std::string& prompt){
    double value;
    while (true){
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail()){
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << " Invalid number, please try again!\n";
        }
        else{
                return value;
            }
        }
}

int main()
{
    std::cout << "=== Bill Splitter ===\n\n";

    double taxPercent = readValidDouble("Enter the tax rate (%): ");
    double tipPercent = readValidDouble("Enter the tip rate (%): ");

    double taxRate = taxPercent / 100.0;
    double tipRate = tipPercent / 100.0;

    BillSplitter splitter (taxRate, tipRate);

    int numPeople = readValidInt("How many people are splitting the bill? ");
    for(int i = 0; i < numPeople; i++){
        std::string name;
        std::cout << " Name of person " << (i + 1) << ": ";
        std::cin >> name;
        splitter.addPerson(name);
    }

    int numItems = readValidInt("\nHow many items are on the receipt? ");
    std::cin.ignore();

    for(int i = 0; i < numItems; i++){
        std::string name;
        std::cout << "\n Item " << (i + 1) << " name: ";
        std::getline(std::cin, name);
        
        double price = readValidDouble(" Item " + std::to_string(i+1) + " price: $");
        
        std::vector<int> assignedTo;
        while(assignedTo.empty()){
            std::cout << " Who had this? Options:\n";
            for (size_t p = 0; p < splitter.people.size(); p++){
                std::cout << " " << (p + 1) << ": " << splitter.people[p].name << "\n";
            }
            std::cout << " Enter numbers separated by spaces: ";

            std::cin.ignore(); // discards leftover newline from previous cin
            std::string line;
            std::getline(std::cin, line);

            std::stringstream ss(line);
            int index;
            std::vector<int> attempt;
            bool lineIsValid = true;

            while (ss >> index){
                int realIndex = index - 1;
                if (realIndex < 0 || realIndex >= (int)splitter.people.size()){
                    std::cout << " Invalid index " << index << " - please re-enter the whole list.\n";
                    lineIsValid = false;
                    break;
                }
                attempt.push_back(realIndex);
            }
            if (lineIsValid && !attempt.empty()){
                assignedTo = attempt;
            }
                else if(lineIsValid && attempt.empty()){
                    std::cout << " You must assign this item to at least one person. Please try again.\n";
            }
        }
        splitter.addItem(name, price, assignedTo);
    }
    
    splitter.calculateSplits();
    splitter.printResults();

    std::cout << "----------------------\n";
    std::cout << "Who paid the bill? Enter their number:\n";
    for (size_t p = 0; p < splitter.people.size(); p++) {
        std::cout << "  " << (p + 1) << ": " << splitter.people[p].name << "\n";
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