#include "billsplitter.h"

#include <cmath>
#include <iomanip>
#include <iostream>

BillSplitter::BillSplitter(double tax, double tip, double amount, bool fixed)
    : taxRate(tax), tipRate(tip), tipAmount(amount), useFixedTip(fixed) {}

void BillSplitter::addPerson(std::string name) {
    people.push_back(Person(name));
}

void BillSplitter::addItem(std::string name, double price, std::vector<int> assignedTo) {
    items.push_back(Item(name, price, assignedTo));
}

double BillSplitter::calculateTotal() {
    double total = 0.0;
    for (const Item& item : items) {
        total += item.price;
    }
    return total;
}

void BillSplitter::calculateSplits() {
    for (Person& person : people) {
        person.subtotal = 0.0;
    }

    for (const Item& item : items) {
        if (item.assignedTo.empty()) {
            continue;
        }
        double splitAmount = item.price / item.assignedTo.size();
        for (int personIndex : item.assignedTo) {
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
        person.finalTotal = std::round(raw * 100.0) / 100.0;
        sumOfRoundedTotals += person.finalTotal;
    }

    double expectedGrandTotal;

    if (useFixedTip) {
        expectedGrandTotal = std::round((calculateTotal() * (1 + taxRate) + tipAmount) * 100.0) / 100.0;
    }

    else {
        expectedGrandTotal = std::round(calculateTotal() * (1 + taxRate + tipRate) * 100.0) / 100.0;
    }

    double leftover = expectedGrandTotal - sumOfRoundedTotals;

    if (std::abs(leftover) >= 0.01 && !people.empty()) {
        people[0].finalTotal += leftover;
    }
}

void BillSplitter::printResults() {
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

void BillSplitter::settleUp(int payerIndex) {
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
