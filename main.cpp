#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>
#include <limits>
#include <cmath>

struct Item {
    std::string name;
    double price;
    std::vector<int> assignedTo;
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
    for(int i = 0; i < numItems; i++){
        std::string name;
        std::cout << "\n Item" << (i + 1) << " name: ";
        std::cin >> name;
        
        double price = readValidDouble(" Item " + std::to_string(i+1) + " price: $");
        
        std::cout << " Who had this? Options:\n";
        for (size_t p = 0; p < people.size(); p++){
            std::cout << " " << p << ": " << people[p] << "\n";
        }
        std::cout << " Enter numbers separated by spaces: ";

        std::cin.ignore(); // discards leftover newline from previos cin
        std::string line;
        std::getline(std::cin, line);

        std::stringstream ss(line);
        int index;
        std::vector<int> assignedTo;
        while (ss >> index){
            if (index < 0 || index >= (int)people.size()){
                std::cout << " (ignoring invalid index " << index << ")\n";
                continue;
            }
            assignedTo.push_back(index);
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
    double tipRate = 0.15; // 15% tip

    std::vector<double> finalTotals(people.size());
    double sumOfRoundedTotals = 0.0;

    for (size_t i = 0; i < people.size(); i++){
        double subtotal = owed[i];
        double tax = subtotal * taxRate;
        double tip = subtotal * tipRate;
        double finalTotal = subtotal + tax + tip;

        finalTotal = std::round(finalTotal * 100.0 / 100.0);

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
        std::cout << " " << p << ": " << people[p] << "\n";
    }
    int payerIndex = readValidInt("Enter number: ");

    std::cout << "\n=== Settling Up ===\n";
    for (size_t i = 0; i < people.size(); i++){
        if ((int)i == payerIndex){
            continue;
        }
        std::cout << " " << people[i] << " pays " << people[payerIndex] << ": " << finalTotals[i] << "\n";
    }

    return 0;
}