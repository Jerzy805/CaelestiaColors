#pragma once

#include <string>
#include <unordered_map>
#include <stdexcept>
#include "colour.hpp"
#include <nlohmann/json.hpp>

namespace nl = nlohmann;

class Flavour
{
    std::unordered_map<std::string, Colour> colours;
    
    public:

    const auto& get_colours() const { return colours; } // zwraca dostęp iteracyjny do kolekcji, którą modyfikujemy jedynie korzystając z API klasy flavour

    Colour& get_colour(const std::string& key) // rzuca wyjątkiem jeżeli kolor nie istnieje
    {
        auto it = colours.find(key);

        if (it == colours.end())
        {
            throw std::runtime_error("Nie istnieje kolor o tej nazwie");
        }

        return it->second;
    }

    const Colour& get_colour(const std::string& key) const
    {
        auto it = colours.find(key);

        if (it == colours.end())
        {
            throw std::runtime_error("Nie istnieje kolor o tej nazwie");
        }

        return it->second;
    }

    void set_colour(const std::string& key, const Colour& colour) // rzuca wyjątkiem jeżeli kolor nie jest jeszcze zdefiniowany
    {
        auto it = colours.find(key);

        if (it == colours.end())
        {
            throw std::runtime_error("Nie istnieje kolor o tej nazwie");
        }

        it->second = colour;
    }

    bool has_colour(const std::string& key) const noexcept
    {
        return colours.find(key) != colours.end();
    }

    void add_colour(const std::string& key, const Colour& Colour) // rzuca wyjątkiem kiedy kolor jest już zdefiniowany
    {
        auto result = colours.emplace(key, Colour);

        if (!result.second)
        {
            throw std::runtime_error("Istnieje już kolor o tej nazwie");
        }
    }

    void remove_colour(const std::string& key) // rzuca wyjątkiem kiedy kolor nie istnieje
    {
        auto result = colours.erase(key);

        if (result == 0)
        {
            throw std::runtime_error("Nie istnieje kolor o tej nazwie");
        }
    }

    nl::json to_json() const // generuje obiekt JSON, ale nie zapisuje go nigdzie
    {
        nl::json data;

        for (const auto& [Colour_name, Colour_value] : colours)
        {
            data[Colour_name] = Colour_value.to_hex();
        }

        return data;
    }

    bool operator==(const Flavour& other) const
    {
        return colours == other.colours;
        // korzystamy z istniejącego operatora porównania std::unordered_map
    }

    bool operator!=(const Flavour& other) const
    {
        return !(*this == other);
    }
};

// każdy schemat ma swoje warianty, nazwy kolorów i ich wartości są przechowywane w ramach struktury Flavour