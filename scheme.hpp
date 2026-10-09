#pragma once

#include <string>
#include <unordered_map>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <filesystem>
#include <cerrno>
#include <vector>
#include <cstring>
#include "flavour.hpp"
#include <algorithm>
#include <nlohmann/json.hpp>

namespace fs = std::filesystem;
namespace nl = nlohmann;

struct Scheme
{
    std::string name;
    std::unordered_map<std::string, Flavour> flavours;

    Scheme() = default;

    Scheme(const std::string& name, const std::unordered_map<std::string, Flavour>& flavours) :
        name(name), flavours(flavours) {}

    Flavour& get_flavour(const std::string& key)
    {
        auto it = flavours.find(key);

        if (it == flavours.end())
        {
            throw std::runtime_error("Nie istnieje wariant o tej nazwie");
        }

        return it->second; // zwracamy sam flavour
    }

    const Flavour& get_flavour(const std::string& key) const
    {
        auto it = flavours.find(key);

        if (it == flavours.end())
        {
            throw std::runtime_error("Nie istnieje wariant o tej nazwie");
        }

        return it->second;
    }

    bool has_flavour(const std::string& key) const noexcept
    {
        return flavours.find(key) != flavours.end();
    }

    void add_flavour(const std::string& key, const Flavour& flavour) // rzuca wyjątkiem jeżeli wariant już istnieje
    {
        auto result = flavours.emplace(key, flavour);

        if (!result.second)
        {
            throw std::runtime_error("Istnieje już wariant o tej nazwie");
        }
    }

    // rozważyć czy to w ogóle jest potrzebne
    void set_flavour(const std::string& key, const Flavour& flavour)
    {
        auto it = flavours.find(key);

        if (it == flavours.end())
        {
            throw std::runtime_error("Nie istnieje wariant o tej nazwie");
        }

        it->second = flavour;
    }

    void remove_flavour(const std::string& key)
    {
        auto result = flavours.erase(key);

        if (result == 0)
        {
            throw std::runtime_error("Nie istnieje wariant o tej nazwie");
        }
    }

    std::string to_string() const // generuje tekst do strumieniowania w dowolny sposób, NIE zapisuje nazwy
    {
        std::string buffer;// do tego obiektu wszystko zapisujemy

        for (const auto& element : flavours)
        {
            for (const auto& [key, color]: element.second.colors)
            {
                buffer += key + ' ' + color.to_hex() + '\n';
            }
        }

        return buffer;   
    }

    void write_to_file(const fs::path& path)
    {
        if (!fs::is_directory(path))
        {
            std::cout << "Podana ścieżka nie jest katalogiem\n";
            throw std::runtime_error("Not a directory");
        }

        auto filename = path/name; // chyba tak można?
        
        if (fs::exists(filename))
        {
            // tutaj jeszcze trzeba wymyślić co robimy w takiej sytuacji, póki co pozwalam użytkownikowi zdecydować

            std::cout << "Ten schemat został już zapisany do pliku, czy chcesz zapisać go ponownie?[Y/n]\n";
            std::string answer;

            std::cin >> answer;

            if (answer[0] == 'N' || answer[0] == 'n')
            {
                return; // użytkownik wyraźnie nie chce nadpisywać istniejącego pliku
            }
        }

        std::ofstream file(filename);

        if (!file)
        {
            std::cout << "Nie udało się otworzyć pliku wyjściowego\n";
            throw std::runtime_error("Out file open error"); // ewentualnie na odwrót jak chodzi o wypisywanie u rzucanie
        }

        auto text = this->to_string();

        if (!(file << text))
        {
            std::cout << "Nie udało się zapisać do pliku\n";
            throw std::runtime_error("Write error");
        }
    }

    nl::json to_json() const
    {
        nl::json data;

        for (const auto& [flavour_name, flavour_value] : flavours)
        {
            data[flavour_name] = flavour_value.to_json();
        }

        return data;
    }

    bool operator==(const Scheme& other) const
    {
        return name == other.name && flavours == other.flavours;
    }

    bool operator!=(const Scheme& other) const
    {
        return !(*this == other);
    }
};

std::unordered_map<std::string, Scheme> parse_schemes_file(const std::string& path) // tutaj rodzaj ścieżki nie ma większego znaczenia
{
    std::ifstream file(path);

    if (!file)
    {
        throw std::runtime_error("Nie udało się otworzyć pliku");
    }

    nl::json data;

    file >> data; // strumieniujemy całą zawartość pliku do data

    std::unordered_map<std::string, Scheme> schemes; // tutaj wszystko wrzucamy

    for (const auto& [name, flavours] : data.items()) // iterujemy po nazwach a następnie po wariantach
    {
        Scheme scheme;
        scheme.name = name;

        for (const auto& [flavour_name, colors] : flavours.items())
        {
            Flavour scheme_flavour;

            for (const auto& [color_name, color_value] : colors.items())
            {
                auto text_value = color_value.get<std::string>(); // rzutujemy obiekt json na string C++

                scheme_flavour.colors.emplace(color_name, Color::from_hex(text_value));
                // na późniejszym etapie zmienię na try_emplace, ewentualnie std::move, to jest wersja tymczasowa
            }

            scheme.add_flavour(flavour_name, scheme_flavour);
        }

        schemes.emplace(name, scheme);
    }

    return schemes; // zwracamy poprawnie wczytane dane
}

nl::json schemes_to_json(const std::unordered_map<std::string, Scheme>& schemes)
{
    nl::json data;

    for (const auto& [scheme_name, scheme_value] : schemes)
    {
        data[scheme_name] = scheme_value.to_json();
    }

    return data;
}

void save_schemes_to_file(const fs::path& path, const std::unordered_map<std::string, Scheme>& schemes)
{
    if (fs::exists(path) && !fs::is_regular_file(path))
    {
        throw std::runtime_error("Podana ścieżka nie jest plikiem");
    }

    // najpierw próbujemy otworzyć plik, jeżeli się nie uda to nie ma co serializować schematów których może być bardzo wiele
    std::ofstream file(path); // domyślnie nadpisujemy, ale rozważyć czy nie ma edge casów w których to nie działa

    if (!file)
    {
        throw std::runtime_error("Nie udało się otworzyć pliku");
    }

    auto data = schemes_to_json(schemes);

    file << data.dump(4); // od razu definiujemy formatowanie danych
}