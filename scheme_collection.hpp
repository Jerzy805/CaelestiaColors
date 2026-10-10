#pragma once

#include "scheme.hpp"

class SchemeCollection
{
    std::unordered_map<std::string, Scheme> schemes;

    public:
    Scheme& get_scheme(const std::string& name)
    {
        auto it = schemes.find(name);

        if (it == schemes.end())
        {
            throw std::runtime_error("Schemat o podanej nazwie nie istnieje");
        }

        return it->second;
    }

    const Scheme& get_scheme(const std::string& name) const
    {
        auto it = schemes.find(name);

        if (it == schemes.end())
        {
            throw std::runtime_error("Schemat o podanej nazwie nie istnieje");
        }

        return it->second;
    }

    bool has_scheme(const std::string& name) const
    {
        return schemes.find(name) != schemes.end();
    }

    void add_scheme(const std::string& name, const Scheme& scheme)
    {
        auto result = schemes.emplace(name, scheme);

        if (!result.second)
        {
            throw std::runtime_error("Schemat o podanej nazwie już istnieje w kolekcji");
        }
    }

    void remove_scheme(const std::string& name)
    {
        auto result = schemes.erase(name);

        if (result == 0)
        {
            throw std::runtime_error("Schemat o podanej nazwie nie istnieje");
        }
    }

    nl::json schemes_to_json() const
    {
        nl::json data;

        for (const auto& [scheme_name, scheme_value] : schemes)
        {
            data[scheme_name] = scheme_value.to_json();
        }

        return data;
    }

    void save_schemes_to_file(const fs::path& path) const
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

        auto data = schemes_to_json();

        file << data.dump(4); // od razu definiujemy formatowanie danych
    }

    static SchemeCollection from_file(const std::string& path) // tutaj rodzaj ścieżki nie ma większego znaczenia
    {
        std::ifstream file(path);

        if (!file)
        {
            throw std::runtime_error("Nie udało się otworzyć pliku");
        }

        nl::json data;

        file >> data; // strumieniujemy całą zawartość pliku do data

        SchemeCollection collection; // tutaj wszystko wrzucamy

        for (const auto& [name, flavours] : data.items()) // iterujemy po nazwach a następnie po wariantach
        {
            Scheme scheme;

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

            collection.add_scheme(name, scheme);
        }

        return collection; // zwracamy poprawnie wczytane dane
    }
};

