#pragma once
#include <string>

namespace item6 {

// Varianta clasica din carte (C++98): clasa de baza necopiabila.
// Copierea e declarata private si NU e definita.
class Uncopyable {
protected:
    Uncopyable() = default;
    ~Uncopyable() = default;
private:
    Uncopyable(const Uncopyable&);
    Uncopyable& operator=(const Uncopyable&);
};

// Varianta moderna (C++11): "= delete".
// Un bilet este unic => copierea nu are sens, dar transferul (mutarea) da.
class Ticket {
public:
    explicit Ticket(std::string event);

    Ticket(const Ticket&) = delete;
    Ticket& operator=(const Ticket&) = delete;

    Ticket(Ticket&& other) noexcept;
    Ticket& operator=(Ticket&& other) noexcept;

    unsigned           id() const { return id_; }
    const std::string& event() const { return event_; }
    bool               valid() const { return id_ != 0; }
private:
    static inline unsigned nextId_ = 1;
    unsigned    id_;
    std::string event_;
};

// Aceeasi idee, dar cu varianta din carte: mostenire privata din Uncopyable.
class LegacyTicket : private Uncopyable {
public:
    explicit LegacyTicket(int seat) : seat_(seat) {}
    int seat() const { return seat_; }
private:
    int seat_;
};

void demo();

} // namespace item6