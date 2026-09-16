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
        for (size_t i = 0; i < people.size(); i++) {
            if ((int)i == payerIndex) continue;
            std::cout << "  " << people[i].name << " pays " << people[payerIndex].name
                       << ": $" << people[i].finalTotal << "\n";
        }
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

    int numPeople = readValidInt("How many people are splitting the bill?");

    std::vector<std::string> people;
    for(int i = 0; i < numPeople; i++){
        std::string name;
        std::cout << " Name of person " << (i + 1) << ": ";
        std::cin >> name;
        people.push_back(name);
    }

    int numItems = readValidInt("\nHow many items are on the receipt? ");
    
    std::vector<Item> items;
    std::cin.ignore();

    for(int i = 0; i < numItems; i++){
        std::string name;
        std::cout << "\n Item " << (i + 1) << " name: ";
        std::getline(std::cin, name);
        
        double price = readValidDouble(" Item " + std::to_string(i+1) + " price: $");
        
        std::vector<int> assignedTo;
        while(assignedTo.empty()){
            std::cout << " Who had this? Options:\n";
            for (size_t p = 0; p < people.size(); p++){
                std::cout << " " << (p + 1) << ": " << people[p] << "\n";
            }
            std::cout << " Enter numbers separated by spaces: ";

            std::cin.ignore(); // discards leftover newline from previos cin
            std::string line;
            std::getline(std::cin, line);

            std::stringstream ss(line);
            int index;
            std::vector<int> attempt;
            bool lineIsValid = true;

            while (ss >> index){
                int realIndex = index - 1;
                if (realIndex < 0 || realIndex >= (int)people.size()){
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
    items.push_back({name, price, assignedTo});
}
    

    std::vector<double> owed(people.size(), 0.0);
    double total = 0.0;
    
    for (const Item& item : items){
        total += item.price;
        
        if (item.assignedTo.empty()){
            std::cout << " Warning: \"" << item.name << "\" was never assigned to anyone, skipping.\n";
            continue;
        }

        double splitAmount = item.price / item.assignedTo.size();
        for(int personIndex : item.assignedTo){
            owed[personIndex] += splitAmount;
        }
    }

    double taxRate = 0.1075; // 10.75% tax
    double tipRate = 0.2265; // 15% tip

    std::vector<double> finalTotals(people.size());
    double sumOfRoundedTotals = 0.0;

    for (size_t i = 0; i < people.size(); i++){
        double subtotal = owed[i];
        double tax = subtotal * taxRate;
        double tip = subtotal * tipRate;
        double finalTotal = subtotal + tax + tip;

        finalTotal = std::round(finalTotal * 100.0) / 100.0;

        finalTotals[i] = finalTotal;
        sumOfRoundedTotals += finalTotal;
    }

    double expectedGrandTotal = std::round(total * (1 + taxRate + tipRate) * 100.0) / 100.0;
    double leftover = expectedGrandTotal - sumOfRoundedTotals;

    if(std::abs(leftover) >= 0.01 && !finalTotals.empty()){
        finalTotals[0] += leftover;
    }

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\n----------------------\n";
    std::cout << "\nTotal: $" << total << "\n";
    std::cout << "\n----------------------\n";
    
    for (size_t i = 0; i < people.size(); i++){
        double subtotal = owed[i];
        double tax = subtotal * taxRate;
        double tip = subtotal * tipRate;

        std::cout << people[i] << ":\n";
        std::cout << " Subtotal: $" << subtotal << "\n";
        std::cout << " Tax: $" << tax << "\n";
        std::cout << " Tip: $" << tip << "\n";
        std::cout << " Total: $" << finalTotals[i] << "\n\n";

    }
    std::cout << "----------------------\n";
    std::cout << "Who paid the bill? Enter their number:\n";
    for (size_t p = 0; p < people.size(); p++){
        std::cout << " " << p + 1 << ": " << people[p] << "\n";
    }
    int payerIndex = readValidInt("Enter number: ") - 1;

    std::cout << "\n=== Settling Up ===\n";
    for (size_t i = 0; i < people.size(); i++){
        if ((int)i == payerIndex){
            continue;
        }
        std::cout << " " << people[i] << " pays " << people[payerIndex] << ": " << finalTotals[i] << "\n";
    }

    return 0;
}