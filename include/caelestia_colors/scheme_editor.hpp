// Klasa do wykonywania wszystkich operacji na schematach, przechowuje informacje o wczytanych danych

#pragma once

#include "current_scheme.hpp"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

class SchemeEditor
{
    CurrentScheme original; // pierwotnie wczytany schemat
    CurrentScheme current; // obecne dane, potencjalnie nadpisane przez użytkownika
    fs::path path; // ścieżka pliku z którego pobraliśmy oryginalne dane

    public:
    SchemeEditor(const CurrentScheme& scheme, const fs::path& path) : original(scheme), 
        current(scheme), path(path) {} // original i current na początku zawsze takie same

    const CurrentScheme& get_original() const { return original; } // nie pozwalamy na zmianę oryginału
    const CurrentScheme& get_current() const { return current; } // zmiana obecnego odbywa się explicite przez API

    const fs::path& get_path() const { return path; } // zwraca ścieżkę, niekoniecznie oryginalną

    void reset() // cofa wszystkie modyfikacje
    {
        current = original;
    }

    void set_colour(const std::string& key, const Colour& colour)
    {
        current.get_colour_data().set_colour(key, colour);
        // za sprawdzenie poprawności tej operacji odpowiada klasa Flavour, jeżeli rzuci wyjątkiem to w tym miejscu go propagujemy
    }

    bool is_modified() const { return original != current; } // korzystamy z operatorów porównania klasy CurrentScheme

    static SchemeEditor from_file(const fs::path& path)
    {
        auto scheme = CurrentScheme::from_file(path); // jeżeli się nie uda to propagujemy rzucony wyjątek

        return SchemeEditor(scheme, path);
    }

    void save() // zapisuje do oryginalnej ścieżki jedynie current, original zostaje utracony, ewentualnie później się przed tym zabezpieczymy
    {
        current.save_to_file(path);
        original = current;
    }

    void save_as(const fs::path& new_path)
    {
        current.save_to_file(new_path);
        path = new_path;
        original = current; 
    }

    void reload() // wczytuje z obecnej ścieżki edytor jeszcze raz
    {
        original = CurrentScheme::from_file(path);
        current = original;
    }
};