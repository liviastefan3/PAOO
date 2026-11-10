#include "item4_init.hpp"
#include <iostream>

namespace item4 {

// ---------- Label ----------
Label::Label() { std::cout << "    [Label] constructor implicit\n"; }

Label::Label(const std::string& text) : text_(text) {
    std::cout << "    [Label] constructor(\"" << text << "\")\n";
}

Label& Label::operator=(const Label& rhs) {
    text_ = rhs.text_;
    std::cout << "    [Label] operator= (atribuire)\n";
    return *this;
}

// ---------- BadSensor ----------
// Inainte de a intra in corpul constructorului, label_ a fost deja
// construit implicit. Apoi il "atribuim" => munca dubla.
BadSensor::BadSensor(const std::string& name, int id) {
    label_  = Label(name);
    id_     = id;
    active_ = false;
    // lastValue_ nu e atins => continut aleator din memorie
}

void BadSensor::print() const {
    // NU afisam lastValue_ - citirea lui ar fi comportament nedefinit.
    std::cout << "    BadSensor{label=" << label_.text()
              << ", id=" << id_ << ", active=" << active_ << "}\n";
}

// ---------- GoodSensor ----------
// Lista de initializare: label_ e construit direct cu valoarea buna,
// un singur apel de constructor, fara atribuire.
GoodSensor::GoodSensor(const std::string& name, int id)
    : label_(name), id_(id), lastValue_(0.0), active_(false), maxReadings_(100) {}

GoodSensor::GoodSensor() : GoodSensor("necunoscut", 0) {}

void GoodSensor::print() const {
    std::cout << "    GoodSensor{label=" << label_.text()
              << ", id=" << id_ << ", lastValue=" << lastValue_
              << ", active=" << active_
              << ", maxReadings=" << maxReadings_ << "}\n";
}

// ---------- AppConfig ----------
AppConfig::AppConfig() {
    std::cout << "    [AppConfig] initializat la prima utilizare\n";
}

AppConfig& AppConfig::instance() {
    static AppConfig cfg;   // creat o singura data, cand ajunge executia aici
    return cfg;
}

// ---------- Demo ----------
void demo() {
    std::cout << "\n==================== ITEM 4 ====================\n";

    std::cout << "\n-- 1) Tipuri built-in: initializare explicita --\n";
    int    counter{};        // 0
    double values[3]{};      // {0, 0, 0}
    std::cout << "    counter=" << counter << ", values=["
              << values[0] << "," << values[1] << "," << values[2] << "]\n";

    std::cout << "\n-- 2) BadSensor (atribuire in corpul constructorului) --\n";
    {
        BadSensor bad("temperatura", 1);
        bad.print();
    }

    std::cout << "\n-- 3) GoodSensor (lista de initializare) --\n";
    {
        GoodSensor good("temperatura", 2);
        good.print();
        GoodSensor def;
        def.print();
    }

    std::cout << "\n-- 4) Obiect static local (Singleton Meyers) --\n";
    std::cout << "    Inainte de primul apel instance()...\n";
    AppConfig& cfg = AppConfig::instance();
    std::cout << "    appName=" << cfg.appName()
              << ", verbosity=" << cfg.verbosity() << "\n";
    std::cout << "    Al doilea apel nu mai reconstruieste obiectul:\n";
    AppConfig::instance();
}

} // namespace item4