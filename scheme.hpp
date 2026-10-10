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
    std::unordered_map<std::string, Flavour> flavours;

    Scheme() = default;

    Scheme(const std::string& name, const std::unordered_map<std::string, Flavour>& flavours) :
        flavours(flavours) {}

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
        return flavours == other.flavours;
    }

    bool operator!=(const Scheme& other) const
    {
        return !(*this == other);
    }
};