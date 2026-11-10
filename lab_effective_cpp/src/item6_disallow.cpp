#include "item6_disallow.hpp"
#include <iostream>
#include <type_traits>
#include <utility>

namespace item6 {

Ticket::Ticket(std::string event)
    : id_(nextId_++), event_(std::move(event)) {}

Ticket::Ticket(Ticket&& other) noexcept
    : id_(std::exchange(other.id_, 0)), event_(std::move(other.event_)) {}

Ticket& Ticket::operator=(Ticket&& other) noexcept {
    if (this != &other) {
        id_ = std::exchange(other.id_, 0);
        event_ = std::move(other.event_);
    }
    return *this;
}

// Verificari facute LA COMPILARE: daca cineva scoate "= delete",
// proiectul nu se mai compileaza.
static_assert(!std::is_copy_constructible<Ticket>::value,       "Ticket nu trebuie sa fie copiabil");
static_assert(!std::is_copy_assignable<Ticket>::value,          "Ticket nu trebuie sa fie copiabil");
static_assert( std::is_move_constructible<Ticket>::value,       "Ticket trebuie sa fie mutabil");
static_assert(!std::is_copy_constructible<LegacyTicket>::value, "LegacyTicket nu trebuie sa fie copiabil");
static_assert(!std::is_copy_assignable<LegacyTicket>::value,    "LegacyTicket nu trebuie sa fie copiabil");

void demo() {
    std::cout << "\n==================== ITEM 6 ====================\n";
    std::cout << std::boolalpha;

    std::cout << "\n-- 1) Ticket (= delete) --\n";
    Ticket t1("Concert");
    std::cout << "    t1: id=" << t1.id() << ", event=" << t1.event() << "\n";

    // Liniile de mai jos NU compileaza (decomenteaza-le ca sa vezi eroarea):
    // Ticket t2 = t1;      // error: use of deleted function 'Ticket::Ticket(const Ticket&)'
    // Ticket t3("X"); t3 = t1;   // error: use of deleted function 'operator='

    Ticket t4 = std::move(t1);  // transferul e permis
    std::cout << "    dupa mutare: t4.id=" << t4.id()
              << ", t1.valid()=" << t1.valid() << "\n";

    Ticket t5("Teatru");
    std::cout << "    t5: id=" << t5.id() << " (id-urile raman unice)\n";

    std::cout << "\n-- 2) LegacyTicket (mostenire din Uncopyable) --\n";
    LegacyTicket lt(42);
    std::cout << "    locul " << lt.seat() << "\n";
    // LegacyTicket lt2 = lt;   // error: 'Uncopyable::Uncopyable(const Uncopyable&)' is private

    std::cout << "\n-- 3) Verificari static_assert (la compilare) --\n";
    std::cout << "    Ticket copiabil?       " << std::is_copy_constructible<Ticket>::value << "\n";
    std::cout << "    Ticket mutabil?        " << std::is_move_constructible<Ticket>::value << "\n";
    std::cout << "    LegacyTicket copiabil? " << std::is_copy_constructible<LegacyTicket>::value << "\n";
}

} // namespace item6