#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>
#include <limits>
#include <cmath>

#include "billsplitter.h"
#include "item.h"
#include "person.h"

using namespace std;

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
