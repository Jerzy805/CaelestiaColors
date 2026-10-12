#pragma once

#include <string>
#include <unordered_map>
#include <stdexcept>
#include "colour.hpp"
#include <nlohmann/json.hpp>
#include <vector>
#include <algorithm>

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

    std::vector<std::string> get_colour_names() const
    {
        static const std::vector<std::string> priority = {
            "primary",
            "secondary",
            "tertiary",
            "error",
            "success",

            "background",
            "surface",
            "surfaceVariant",
            "surfaceDim",
            "surfaceBright",
            "surfaceContainer",
            "surfaceContainerHigh",
            "surfaceContainerHighest",
            "surfaceContainerLow",
            "surfaceContainerLowest",

            "onPrimary",
            "onSecondary",
            "onTertiary",
            "onBackground",
            "onSurface",
            "onSurfaceVariant",
            "onError",
            "onErrorContainer",

            "primaryContainer",
            "secondaryContainer",
            "tertiaryContainer",
            "errorContainer",
            "successContainer",

            "outline",
            "outlineVariant",
            "shadow",
            "scrim"
        };

        std::vector<std::string> names;
        names.reserve(colours.size());

        for (const auto& name : priority)
        {
            if (colours.contains(name))
            {
                names.push_back(name);
            }
        }

        decltype(names) remaining;



        for (const auto& [name, colour] : colours)
        {
            if (std::find(names.begin(), names.end(), name) == names.end())
            {
                remaining.push_back(name);
            }
        }

        std::sort(remaining.begin(), remaining.end());

        names.insert(names.end(), remaining.begin(), remaining.end());

        return names;
    }

    std::vector<std::string> find_colours(const std::string& query) const
    {
        std::vector<std::string> results;

        for (const auto& name : get_colour_names())
        {
            if (name.find(query) != std::string::npos)
            {
                results.push_back(name);
            }
        }

        return results;
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