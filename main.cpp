#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

struct Item {
    std::string name;
    double price;
};

int main()
{
    // hardcoded receipt
    std::vector<Item> items = {
        {"Burger", 12.50},
        {"Fries", 4.00},
        {"Shake", 2.50}
    };

    std::vector<std::string> people = {"Mykah", "Ivy"};

    // total of the receipt
    double total = 0.0;
    for (const Item& item : items){
        total += item.price;
    }

    // split bill evenly
    double share = total / people.size();
    
    // set precision
    std::cout << std::fixed << std::setprecision(2);

    std::cout << "Total: $" << total << "\n";
    for (const std::string& person : people){
        std::cout << person << " owes: $" << share << "\n";
    }

    return 0;
}
