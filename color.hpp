#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <stdexcept>

int hex_digit_to_int(const char c)
{
    if (c >= '0' &&  c <= '9')
    {
        return c - '0';
    }

    if (c >= 'A' && c <= 'F')
    {
        return c - 'A' + 10;
    }

    if (c >= 'a' && c <= 'f')
    {
        return c - 'a' + 10;
    }

    throw std::runtime_error("To nie jest kod szesnastkowy");
}

char int_to_hex_digit(const int value)
{
    if (value >= 0 && value <= 9)
    {
        return '0' + value;
    }

    if (value <= 15)
    {
        return 'A' + (value - 10);
    }

    throw std::runtime_error("Liczba spoza zakresu 0-15");
}

std::string int_hex_pair_to_string(const uint8_t hex)
{
    // hex = 242
    int as = hex / 16; // zwróci liczbę całkowitą zaokrągloną w dół, o to nam chodzi
    int bs = hex - 16 * as; // daje bs * 1 + 16 * as, o to chodzi

    char a = int_to_hex_digit(as);
    char b = int_to_hex_digit(bs);

    return std::string{a, b};
}

struct Color
{
    uint8_t r;
    uint8_t g;
    uint8_t b;

    Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}

    static Color from_hex(const std::string& hex)
    {
        if (hex.empty())
        {
            throw std::runtime_error("Pusty string");
        }

        int start_index = 0;

        if (hex.length() != 6)
        {
            if (hex[0] == '#' && hex.length() == 7)
            {
                start_index = 1; // rozszyfrowujemy hex od drugiego znaku
            }
            else
            {
                throw std::runtime_error("Podany format jest niepoprawny");
            }
        }

        uint8_t r, g, b;

        r = hex_digit_to_int(hex[start_index]) * 16;
        start_index++;

        r += hex_digit_to_int(hex[start_index]);
        start_index++;

        g = hex_digit_to_int(hex[start_index]) * 16;
        start_index++;

        g += hex_digit_to_int(hex[start_index]);
        start_index++;
        
        b = hex_digit_to_int(hex[start_index]) * 16;
        start_index++;

        b += hex_digit_to_int(hex[start_index]);
        start_index++;
        
        return Color(r, g, b);
    }

    std::string to_hex() const // zwraca std::stringa bez znaku '#'
    {
        auto _r = int_hex_pair_to_string(r);
        auto _g = int_hex_pair_to_string(g);
        auto _b = int_hex_pair_to_string(b);
        
        return _r + _g + _b;
    }

    bool operator==(const Color& other) const
    {
        return r == other.r && (g == other.g && b == other.b);
    }

    bool operator!=(const Color& other) const
    {
        return !(*this == other); // korzystamy ze zdefiniowanego wcześniej operatora ==
    }
};
