#include "../scheme_editor.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

namespace fs = std::filesystem;

namespace
{
    int passed = 0;
    int failed = 0;

    void check(bool condition, const std::string& test_name)
    {
        if (condition)
        {
            ++passed;
            std::cout << "[PASS] " << test_name << '\n';
        }
        else
        {
            ++failed;
            std::cerr << "[FAIL] " << test_name << '\n';
        }
    }

    template <typename Function>
    void check_throws(Function&& function, const std::string& test_name)
    {
        try
        {
            function();
            check(false, test_name);
        }
        catch (const std::exception&)
        {
            check(true, test_name);
        }
    }

    struct TestFiles
    {
        fs::path directory = fs::temp_directory_path() / "caelestia_colors_tests";
        fs::path original = directory / "original.json";
        fs::path second = directory / "second.json";
        fs::path invalid = directory / "invalid.json";
        fs::path missing = directory / "missing.json";
        fs::path missing_colours = directory / "missing_colours.json";
        fs::path wrong_type = directory / "wrong_type.json";

        TestFiles()
        {
            fs::create_directories(directory);

            std::ofstream file(original);
            file << R"({
                "name": "Test Scheme",
                "flavour": "test",
                "mode": "dark",
                "colours": {
                    "background": "#112233",
                    "primary": "#AABBCC"
                }
            })";
        }

        ~TestFiles()
        {
            std::error_code error;
            fs::remove_all(directory, error);
        }
    };
}

int main()
{
    TestFiles files;

    // 1. Wczytanie poprawnego pliku
    check_throws(
        [] { SchemeEditor::from_file(fs::path{}); },
        "Pusta sciezka powoduje blad wczytywania"
    );

    auto editor = SchemeEditor::from_file(files.original);

    check(editor.get_current().get_name() == "Test Scheme",
          "Wczytano nazwe schematu");

    check(!editor.is_modified(),
          "Nowo wczytany schemat nie jest zmodyfikowany");

    // 2. Modyfikacja i reset
    const auto original_colour =
        editor.get_current().get_colour_data().get_colour("primary");

    editor.set_color("primary", Colour::from_hex("#010203"));

    check(editor.is_modified(),
          "Zmiana koloru ustawia is_modified");

    editor.reset();

    check(!editor.is_modified(),
          "Reset przywraca stan oryginalny");

    check(editor.get_current().get_colour_data().get_colour("primary")
              == original_colour,
          "Reset przywraca pierwotny kolor");

    // 3. Nieistniejacy kolor
    check_throws(
        [&] { editor.set_color("unknown", Colour::from_hex("#010203")); },
        "Nie mozna zmienic nieistniejacego koloru"
    );

    // 4. Save i ponowne wczytanie
    editor.set_color("primary", Colour::from_hex("#010203"));
    editor.save();

    check(!editor.is_modified(),
          "Po save schemat nie jest zmodyfikowany");

    auto loaded = SchemeEditor::from_file(files.original);

    check(loaded.get_current().get_colour_data().get_colour("primary")
              == Colour::from_hex("#010203"),
          "Zapisane dane mozna ponownie wczytac");

    // 5. Save as
    editor.set_color("background", Colour::from_hex("#FFFFFF"));
    editor.save_as(files.second);

    check(editor.get_path() == files.second,
          "save_as aktualizuje sciezke");

    check(!editor.is_modified(),
          "Po save_as schemat nie jest zmodyfikowany");

    check(fs::exists(files.second),
          "save_as tworzy plik docelowy");

    // 6. Reload po zewnetrznej zmianie pliku
    {
        std::ofstream file(files.second);
        file << R"({
            "name": "Reloaded",
            "flavour": "test",
            "mode": "light",
            "colours": {
                "background": "#000000",
                "primary": "#FFFFFF"
            }
        })";
    }

    editor.reload();

    check(editor.get_current().get_name() == "Reloaded",
          "reload odczytuje nowa nazwe");

    check(!editor.is_modified(),
          "Po reload stan jest czysty");

    // 7. Niepoprawny JSON
    {
        std::ofstream file(files.invalid);
        file << "{ invalid json";
    }

    check_throws(
        [&] { SchemeEditor::from_file(files.invalid); },
        "Niepoprawny JSON powoduje wyjatek"
    );

    // 8. Nieudany reload nie powinien niszczyc stanu
    editor.set_color("primary", Colour::from_hex("#010203"));

    const auto before_failed_reload = editor.get_current();

    {
        std::ofstream file(files.second);
        file << "{ invalid json";
    }

    check_throws(
        [&] { editor.reload(); },
        "Nieudany reload rzuca wyjatek"
    );

    check(editor.get_current() == before_failed_reload,
          "Nieudany reload zachowuje current");

    check(editor.is_modified(),
          "Nieudany reload zachowuje status modyfikacji");

        // 10. Nieudany zapis nie powinien zmieniać stanu edytora
    editor.set_color("primary", Colour::from_hex("#ABCDEF"));

    const auto before_failed_save = editor.get_current();
    const auto path_before_failed_save = editor.get_path();

    // Katalog nie może być użyty jako plik wyjściowy.
    check_throws(
        [&] { editor.save_as(files.directory); },
        "Zapis do katalogu powoduje wyjatek"
    );

    check(editor.get_current() == before_failed_save,
          "Nieudany save_as zachowuje current");

    check(editor.get_path() == path_before_failed_save,
          "Nieudany save_as zachowuje sciezke");

    check(editor.is_modified(),
          "Nieudany save_as zachowuje status modyfikacji");

              // 11. Niepoprawna struktura JSON
    {
        std::ofstream file(files.missing_colours);
        file << R"({
            "name": "Broken",
            "flavour": "test",
            "mode": "dark"
        })";
    }

    check_throws(
        [&] { SchemeEditor::from_file(files.missing_colours); },
        "Brak pola colours powoduje wyjatek"
    );

    {
        std::ofstream file(files.wrong_type);
        file << R"({
            "name": "Broken",
            "flavour": "test",
            "mode": "dark",
            "colours": {
                "primary": 123
            }
        })";
    }

    check_throws(
        [&] { SchemeEditor::from_file(files.wrong_type); },
        "Niepoprawny typ wartosci koloru powoduje wyjatek"
    );

    // ostateczne podsumowanie
    std::cout << "\nWynik: " << passed << " zaliczonych, "
              << failed << " niezaliczonych testow.\n";

    return failed == 0 ? 0 : 1;
}