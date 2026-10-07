#ifndef PERSON_H
#define PERSON_H

#include <string>

class Person {
public:
    std::string name;
    double subtotal = 0.0;
    double tax = 0.0;
    double tip = 0.0;
    double finalTotal = 0.0;

    Person(std::string personName) : name(personName) {}
};

#endif
