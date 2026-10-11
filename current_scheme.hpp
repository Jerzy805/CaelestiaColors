#pragma once

#include "flavour.hpp"
#include <filesystem>
#include <fstream>
#include <string>

namespace fs = std::filesystem;

class CurrentScheme
{
    std::string name;
    std::string flavour;
    std::string mode;
    Flavour colour_data; // tutaj trzymane są elementy klucz-wartość: np background-#FF0000

    public:

    const std::string& get_name() const { return name; }
    const std::string& get_flavour() const { return flavour; }
    const std::string& get_mode() const { return mode; }

    Flavour& get_colour_data()
    {
        return colour_data;
    }

    const Flavour& get_colour_data() const
    {
        return colour_data;
    }

    nl::json to_json() const
    {
        nl::json data;

        data["name"] = name;
        data["flavour"] = flavour;
        data["mode"] = mode;
        data["colours"] = colour_data.to_json();

        return data;
    }

    void save_to_file(const fs::path& path) const
    {
        std::ofstream file(path);

        if (!file)
        {
            throw std::runtime_error("Nie udało się otworzyć pliku wyjściowego");
        }

        file << to_json().dump(4);
    }

    static CurrentScheme from_file(const fs::path& path)
    {
        std::ifstream file(path);

        if (!file)
        {
            throw std::runtime_error("Nie udało się otworzyć pliku wejściowego");
        }

        nl::json data;
        file >> data;

        CurrentScheme scheme;

        scheme.name = data["name"];
        scheme.flavour = data["flavour"];
        scheme.mode = data["mode"];
        
        for (const auto& [color_name, color_value] : data["colours"].items())
        {
            scheme.colour_data.add_color(color_name, Color::from_hex(color_value)); // korzystamy z wewnętrznego API naszych struktur
        }

        return scheme;
    }

    bool operator==(const CurrentScheme& other) const
    {
        return name == other.name &&
            flavour == other.flavour &&
            mode == other.mode &&
            colour_data == other.colour_data;
    }

    bool operator!=(const CurrentScheme& other) const
    {
        return !(*this == other); // korzystamy z wcześniej zdefiniowanego operator ==
    }
};