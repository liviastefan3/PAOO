#pragma once
#include <string>

namespace item4 {

// Clasa-ajutor care isi "anunta" apelurile, ca sa vedem ce functii
// sunt chemate la initializare vs. atribuire.
class Label {
public:
    Label();
    explicit Label(const std::string& text);
    Label(const Label&) = default;
    Label& operator=(const Label& rhs);
    const std::string& text() const { return text_; }
private:
    std::string text_;
};

// VARIANTA GRESITA: membrii sunt "atribuiti" in corpul constructorului,
// nu initializati. lastValue_ ramane neinitializat (valoare nedefinita).
class BadSensor {
public:
    BadSensor(const std::string& name, int id);
    void print() const;
private:
    Label  label_;
    int    id_;
    double lastValue_;   // niciodata initializat!
    bool   active_;
};

// VARIANTA CORECTA: lista de initializare, toti membrii initializati,
// in aceeasi ordine in care sunt declarati.
class GoodSensor {
public:
    GoodSensor(const std::string& name, int id);
    GoodSensor();                 // constructor delegat
    void print() const;
private:
    Label      label_;
    int        id_;
    double     lastValue_;
    bool       active_;
    const int  maxReadings_;      // const => OBLIGATORIU in lista de initializare
};

// Singleton Meyers: obiect static local unei functii, initializat
// la prima utilizare (rezolva "static initialization order fiasco").
class AppConfig {
public:
    static AppConfig& instance();
    const std::string& appName() const { return appName_; }
    int verbosity() const { return verbosity_; }

    AppConfig(const AppConfig&) = delete;
    AppConfig& operator=(const AppConfig&) = delete;
private:
    AppConfig();
    std::string appName_{"PAOO Lab"};
    int         verbosity_{1};
};

void demo();

} // namespace item4