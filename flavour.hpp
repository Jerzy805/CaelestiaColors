#pragma once

#include <string>
#include <unordered_map>
#include <stdexcept>
#include "color.hpp"

struct Flavour
{
    std::unordered_map<std::string, Color> colors;

    Color& get_color(const std::string& key) // rzuca wyjątkiem jeżeli kolor nie istnieje
    {
        auto it = colors.find(key);

        if (it == colors.end())
        {
            throw std::runtime_error("Nie istnieje kolor o tej nazwie");
        }

        return it->second;
    }

    const Color& get_color(const std::string& key) const
    {
        auto it = colors.find(key);

        if (it == colors.end())
        {
            throw std::runtime_error("Nie istnieje kolor o tej nazwie");
        }

        return it->second;
    }

    void set_color(const std::string& key, const Color& color) // rzuca wyjątkiem jeżeli kolor nie jest jeszcze zdefiniowany
    {
        auto it = colors.find(key);

        if (it == colors.end())
        {
            throw std::runtime_error("Nie istnieje kolor o tej nazwie");
        }

        it->second = color;
    }

    bool has_color(const std::string& key) const noexcept
    {
        return colors.find(key) != colors.end();
    }

    void add_color(const std::string& key, const Color& color) // rzuca wyjątkiem kiedy kolor jest już zdefiniowany
    {
        auto result = colors.emplace(key, color);

        if (!result.second)
        {
            throw std::runtime_error("Istnieje już kolor o tej nazwie");
        }
    }

    void remove_color(const std::string& key) // rzuca wyjątkiem kiedy kolor nie istnieje
    {
        auto result = colors.erase(key);

        if (result == 0)
        {
            throw std::runtime_error("Nie istnieje kolor o tej nazwie");
        }
    }
};

// każdy schemat ma swoje warianty, nazwy kolorów i ich wartości są przechowywane w ramach struktury Flavour, a obiekty struktury Scheme przechowują
// vectora flavourów