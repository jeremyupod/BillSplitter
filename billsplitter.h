#ifndef BILLSPLITTER_H
#define BILLSPLITTER_H

#include <string>
#include <vector>

#include "item.h"
#include "person.h"

class BillSplitter {
public:
    std::vector<Person> people;
    std::vector<Item> items;
    double taxRate;
    double tipRate;
    double tipAmount;
    bool useFixedTip;

    BillSplitter(double tax, double tip, double amount, bool fixed);

    void addPerson(std::string name);
    void addItem(std::string name, double price, std::vector<int> assignedTo);
    double calculateTotal();
    void calculateSplits();
    void printResults();
    void settleUp(int payerIndex);
};

#endif
