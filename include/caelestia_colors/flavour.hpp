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

    const auto& get_colours() const { return colours; }

    const Colour& get_colour(const std::string& key) const
    {
        auto it = colours.find(key);

        if (it == colours.end())
        {
            throw std::runtime_error("Colour with that name not found");
        }

        return it->second;
    }

    void set_colour(const std::string& key, const Colour& colour) // throws an exception if colour doesn't exist
    {
        auto it = colours.find(key);

        if (it == colours.end())
        {
            throw std::runtime_error("Colour with that name not found");
        }

        it->second = colour;
    }

    bool has_colour(const std::string& key) const noexcept
    {
        return colours.find(key) != colours.end();
    }

    void add_colour(const std::string& key, const Colour& colour) // throw an exception when colour already exists
    {
        auto result = colours.emplace(key, colour);

        if (!result.second)
        {
            throw std::runtime_error("Colour with that name already exists");
        }
    }

    void remove_colour(const std::string& key) // throws an exception when colours doesn't exist
    {
        auto result = colours.erase(key);

        if (result == 0)
        {
            throw std::runtime_error("Colour with that name doesn't exist");
        }
    }

    nl::json to_json() const // generates JSON object but doesn't save it anywhere
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
    }

    bool operator!=(const Flavour& other) const
    {
        return !(*this == other);
    }
};