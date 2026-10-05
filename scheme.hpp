#pragma once

#include "color.hpp"
#include <string>
#include <unordered_map>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <filesystem>
#include <cerrno>
#include <cstring>

namespace fs = std::filesystem;

struct Scheme
{
    std::string name;
    std::string flavour;
    std::unordered_map<std::string, Color> colors;

    Scheme() = default;

    Scheme(const std::string& name, const std::string& flavour, const std::unordered_map<std::string, Color> colors) :
        name(name), flavour(flavour), colors(colors) {}

    std::string to_string() const // generuje tekst do strumieniowania w dowolny sposób, NIE zapisuje nazwy
    {
        std::string buffer;// do tego obiektu wszystko zapisujemy

        for (const auto& [key, color]: colors)
        {
            buffer += key + ' ' + color.to_hex() + '\n';
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

    Color& get_color(const std::string& key)
    {
        auto it = colors.find(key);

        if (it == colors.end())
        {
            throw std::runtime_error("Podany klucz nie istnieje");
        }

        return it->second;
    }

    void set_color(const std::string& key, const Color& color) // zmienia istniejący kolor, jeżeli  nie jest jeszcze zdefiniowany, to rzuca wyjątek
    {
        auto it = colors.find(key);

        if (it == colors.end()) // nigdy nie tworzymy nowego zapisu w ten sposób
        {
            throw std::runtime_error("Podany klucz nie istnieje");
        }

        it->second = color; // nadpisujemy wartość znajdującą się pod kluczem
    }

    bool has_color(const std::string& key) const
    {
        return colors.find(key) != colors.end();
    }

    void add_color(const std::string& key, const Color& color) // dodaje kolor, w przypadku gdy już istnieje to rzuca wyjątkiem
    {
        auto result = colors.emplace(key, color);

        // pytamy o to czy element został faktycznie wstawiony, znajduje się to pod result.second
        if (!result.second)
        {
            throw std::runtime_error("Podany klucz już istnieje");
        }
    }

    void remove_color(const std::string& key) // usuwa kolor po kluczu, jeżeli taki istnieje
    {
        if (colors.erase(key) == 0)
        {
            throw std::runtime_error("Podany klucz nie istnieje w schemacie");
        }
    }
};

void parse_color_line(Scheme& scheme, const std::string& line)
{
    if (!line.empty()) // nie wiem jeszcze co robić w przypadku gdy jest pusta0
    {
        std::stringstream ss(line);

        std::string key, value, buffer;

        ss >> key; // jeżeli się nie powiodło, to key.empty(), co wychwycimy dalej

        if (key == "#")
        {
            return; // napotykamy na komentarz, nie czytamy linii dalej
        }

        ss >> value;

        if (ss >> buffer) // sprawdzamy czy jest tam coś jeszcze
        {
            std::cout << "Niepoprawny format, oczekiwano: <key> <value>(nadmiar)\n";
            throw std::runtime_error("Unexpected value");
        }

        if (key.empty() || value.empty())
        {
            std::cout << "Niepoprawny format, oczekiwano: <key> <value>(pusta wartość)\n";
            throw std::runtime_error("Empty value");
        }

        auto result = scheme.colors.emplace(key, Color::from_hex(value)); // od razu rzutujemy stringa na color

        if (!result.second)
        {
            throw std::runtime_error("Zduplikowany klucz koloru");
        }
    }
}

Scheme parse_scheme_file(const fs::path& path)
{
    std::ifstream file(path);

    if (!file) // krytyczny błąd, panikujemy
    {
        throw std::runtime_error("Nie udało się wczytać pliku schematu");
    }

    std::string line;
    Scheme scheme; // nie wiem w sumie jaką powiniene mieć wartosć name KONIECZNIE ROZWAŻYĆ
    scheme.name = "schemat"; // tymczasowo, potem trzeba będzie to jakoś rozwiązać, np filename czy coś

    while (std::getline(file, line))
    {
        parse_color_line(scheme, line); // samo w sobie ogarnia
    }

    return scheme;
}