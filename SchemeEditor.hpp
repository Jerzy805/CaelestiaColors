// Klasa do wykonywania wszystkich operacji na schematach, przechowuje informacje o wczytanych danych

#pragma once

#include "current_scheme.hpp"

class SchemeEditor
{
    CurrentScheme original; // pierwotnie wczytany schemat
    CurrentScheme current; // obecne dane, potencjalnie nadpisane przez użytkownika

    public:
    SchemeEditor(const CurrentScheme& scheme) : original(scheme), current(scheme) {} // na początku są takie same

    const CurrentScheme& get_original() const { return original; } // nie pozwalamy na zmianę oryginału
    const CurrentScheme& get_current() const { return current; } // zmiana obecnego odbywa się explicite przez API

    void reset() // cofa wszystkie modyfikacje
    {
        current = original;
    }

    void set_color(const std::string& key, const Color& colour)
    {
        current.get_colour_data().set_color(key, colour);
    }

    

    bool is_modified() const { return original != current; } // korzystamy z operatorów porównania klasy CurrentScheme
};