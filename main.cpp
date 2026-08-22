#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

struct Item {
    std::string name;
    double price;
    std::vector<int> assignedTo;
};

int main()
{
    std::vector<std::string> people = {"Mykah", "Ivy"};
    
    // hardcoded receipt
    std::vector<Item> items = {
        {"Burger", 12.50, {0}},
        {"Fries", 4.00, {0, 1}},
        {"Shake", 2.50, {1}}
    };

    std::vector<double> owed(people.size(), 0.0);
    // total of the receipt
    double total = 0.0;
    
    for (const Item& item : items){
        total += item.price;
        double splitAmount = item.price / item.assignedTo.size();

        for (int personIndex : item.assignedTo){
            owed[personIndex] += splitAmount;
        }
    }
    
    // set precision
    std::cout << std::fixed << std::setprecision(2);

    std::cout << "Total: $" << total << "\n";
    for (size_t i = 0; i < people.size(); i++){
        std::cout << people[i] << " owes: $" << owed[i] << "\n";
    }

    return 0;
}
