#pragma once
// Item 12: Copy all parts of an object
// In copy constructor / operator= scrise de mana:
//   1) copiaza TOATE variabilele membre
//   2) apeleaza explicit copierea pentru TOATE clasele de baza

#include <iostream>
#include <string>

class Customer {
public:
    explicit Customer(const std::string& name = "") : name_(name) {}

    Customer(const Customer& rhs) : name_(rhs.name_) {}

    Customer& operator=(const Customer& rhs) {
        name_ = rhs.name_;
        return *this;
    }

    const std::string& name() const { return name_; }

private:
    std::string name_;
};

// Varianta GRESITA: uita partea din clasa de baza
class PriorityCustomerBad : public Customer {
public:
    PriorityCustomerBad(const std::string& name, int priority)
        : Customer(name), priority_(priority) {}

    // Copiaza doar priority_. Baza e initializata cu constructorul
    // implicit => name_ ramane gol!
    PriorityCustomerBad(const PriorityCustomerBad& rhs)
        : priority_(rhs.priority_) {}

    PriorityCustomerBad& operator=(const PriorityCustomerBad& rhs) {
        priority_ = rhs.priority_;     // Customer::operator= nu e apelat
        return *this;
    }

    int priority() const { return priority_; }

private:
    int priority_;
};

// Varianta CORECTA: copiaza si baza, si membrii proprii
class PriorityCustomer : public Customer {
public:
    PriorityCustomer(const std::string& name, int priority)
        : Customer(name), priority_(priority) {}

    PriorityCustomer(const PriorityCustomer& rhs)
        : Customer(rhs),               // <-- copiem partea din baza
          priority_(rhs.priority_) {}  // <-- copiem membrii proprii

    PriorityCustomer& operator=(const PriorityCustomer& rhs) {
        Customer::operator=(rhs);      // <-- atribuim partea din baza
        priority_ = rhs.priority_;     // <-- atribuim membrii proprii
        return *this;
    }

    int priority() const { return priority_; }

private:
    int priority_;
};

inline void demoItem12() {
    std::cout << "=== Item 12: copiaza toate partile obiectului ===\n";

    PriorityCustomerBad bad1("Ana", 5);
    PriorityCustomerBad bad2(bad1);    // copy constructor
    std::cout << "GRESIT:  original = '" << bad1.name() << "' (prio " << bad1.priority()
              << "), copie = '" << bad2.name() << "' (prio " << bad2.priority() << ")\n";

    PriorityCustomer ok1("Ana", 5);
    PriorityCustomer ok2(ok1);         // copy constructor
    std::cout << "CORECT:  original = '" << ok1.name() << "' (prio " << ok1.priority()
              << "), copie = '" << ok2.name() << "' (prio " << ok2.priority() << ")\n";

    PriorityCustomer ok3("Bogdan", 1);
    ok3 = ok1;                         // operator=
    std::cout << "CORECT (operator=): ok3 = '" << ok3.name() << "' (prio "
              << ok3.priority() << ")\n\n";
}