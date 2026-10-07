#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <vector>

class Item {
public:
    std::string name;
    double price;
    std::vector<int> assignedTo;

    Item(std::string itemName, double itemPrice, std::vector<int> people)
        : name(itemName), price(itemPrice), assignedTo(people) {}
};

#endif
