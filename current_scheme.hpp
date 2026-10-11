#pragma once

#include "flavour.hpp"
#include <filesystem>
#include <fstream>
#include <string>
#include <unordered_map>

namespace fs = std::filesystem;

struct CurrentScheme
{
    std::string name;
    std::string flavour;
    std::string mode;
    Flavour color_data; // tutaj trzymane są elementy klucz-wartość: np background-#FF0000

    nl::json to_json() const
    {
        nl::json data;

        data["name"] = name;
        data["flavour"] = flavour;
        data["mode"] = mode;
        data["colours"] = color_data.to_json();

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
            scheme.color_data.add_color(color_name, Color::from_hex(color_value)); // korzystamy z wewnętrznego API naszych struktur
        }

        return scheme;
    }

    bool operator==(const CurrentScheme& other) const
    {
        return name == other.name &&
            flavour == other.flavour &&
            mode == other.mode &&
            color_data == other.color_data;
    }

    bool operator!=(const CurrentScheme& other) const
    {
        return !(*this == other); // korzystamy z wcześniej zdefiniowanego operator ==
    }
};