#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>

struct Item {
    std::string name;
    double price;
    std::vector<int> assignedTo;
};

int main()
{
    std::cout << "=== Bill Splitter ===\n\n";

    int numPeople;
    std::cout << "How many people are splitting the bill? ";
    std::cin >> numPeople;

    std::vector<std::string> people;
    for(int i = 0; i < numPeople; i++){
        std::string name;
        std::cout << " Name of person " << (i + 1) << ": ";
        std::cin >> name;
        people.push_back(name);
    }

    std::cout << "\nHow many items are on the receipt? ";
    int numItems;
    std::cin >> numItems;
    
    std::vector<Item> items;
    for(int i = 0; i < numItems; i++){
        std::string name;
        double price;
        std::cout << "\n Item" << (i + 1) << " name: ";
        std::cin >> name;
        std::cout << " Item " << (i + 1) << " price: $";
        std::cin >> price;
        
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
            assignedTo.push_back(index);
        }
        items.push_back({name, price, assignedTo});
        }

    std::vector<double> owed(people.size(), 0.0);
    double total = 0.0;
    
    for (const Item& item : items){
        total += item.price;
        double splitAmount = item.price / item.assignedTo.size();
        for(int personIndex : item.assignedTo){
            owed[personIndex] += splitAmount;
        }
    }

    double taxRate = 0.1075; // 10.75% tax
    double tipRate = 0.15; // 15% tip

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\n----------------------\n";
    std::cout << "\nTotal: $" << total << "\n";
    std::cout << "\n----------------------\n";
    
    for (size_t i = 0; i < people.size(); i++){
        double subtotal = owed[i];
        double tax = subtotal * taxRate;
        double tip = subtotal * tipRate;
        double finalTotal = subtotal + tax + tip;

        std::cout << people[i] << ":\n";
        std::cout << " Subtotal: $" << subtotal << "\n";
        std::cout << " Tax: $" << tax << "\n";
        std::cout << " Tip: $" << tip << "\n";
        std::cout << " Total: $" << finalTotal << "\n\n";

    }
    return 0;
}