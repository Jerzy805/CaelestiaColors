#include "scheme.hpp"
#include <iostream>
#include <sstream>
#include <string>

constexpr auto filename = "schemat";

int main()
{
    Scheme scheme = parse_scheme_file(filename);

    // scheme.set_color("primary", Color::from_hex("FF0000"));

    // scheme.add_color("test", Color::from_hex("123456"));

    // scheme.remove_color("error");

    std::cout << scheme.to_string();

    // scheme.write_to_file(".");
}