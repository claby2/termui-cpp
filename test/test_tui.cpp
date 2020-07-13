#include <catch2/catch.hpp>
#include "../single_include/tui/tui.hpp"

#ifdef _WIN32
#   define IS_WIN
#include <windows.h>
#else
#   define IS_POSIX
#endif

TEST_CASE("Widget Set Dimensions", "[widget_set_dimensions]") {
    tui::Paragraph paragraph;
    paragraph.set_dimensions(1, 2, 3, 4);
    REQUIRE(paragraph.x == 1);
    REQUIRE(paragraph.y == 2);
    REQUIRE(paragraph.width == 3);
    REQUIRE(paragraph.height == 4);
    
    tui::List list;
    list.set_dimensions(5, 6, 7, 8);
    REQUIRE(list.x == 5);
    REQUIRE(list.y == 6);
    REQUIRE(list.width == 7);
    REQUIRE(list.height == 8);
}

#ifdef IS_WIN
#include <string>

std::string sample_string = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nullam pharetra.";

TEST_CASE("Widget Representation", "[widget_representation]") {

    // Testing for widget representation tests
    // the relative position of different characters
    // to account for varying terminal sizes.

    SECTION("Paragraph1", "[paragraph1]") {
        // Test large paragraph representation
        tui::Window window;
        tui::Paragraph paragraph;
        paragraph.title = sample_string;
        paragraph.text = sample_string;
        paragraph.x = 1;
        paragraph.y = 2;
        paragraph.width = 3;
        paragraph.height = 4;
        paragraph.border_style.foreground = tui::BLACK;
        paragraph.border_style.background = tui::MAGENTA;
        paragraph.text_style.foreground = tui::DARK_GREEN;
        paragraph.text_style.background = tui::YELLOW;
        paragraph.title_style.foreground = tui::DARK_RED;
        paragraph.title_style.background = tui::WHITE;
        window.add(paragraph);
        
        CHAR_INFO *content = window.get_content();
        std::vector<std::pair<std::string, std::string> > requirements = {
            {"43", "208"},
            {"45", "208"},
            {"76", "244"},
            {"124", "208"},
            {"76", "226"},
            {"124", "208"},
            {"124", "208"},
            {"111", "226"},
            {"124", "208"},
            {"43", "208"},
            {"45", "208"},
            {"43", "208"}
        };
        int current_char = 0;
        for(int i = 0; i < window.rows() * window.columns(); i++) {
            if(std::to_string(content[i].Char.AsciiChar) != "0" && std::to_string(content[i].Attributes) != "0") {
                REQUIRE(std::to_string(content[i].Char.AsciiChar) == requirements[current_char].first);
                REQUIRE(std::to_string(content[i].Attributes) == requirements[current_char].second);
                current_char++;
            }
        }
    }
    SECTION("Paragraph2", "[paragraph2") {
        // Test large paragraph representation
        tui::Window window;
        tui::Paragraph paragraph;
        paragraph.title = sample_string;
        paragraph.text = sample_string;
        paragraph.x = 0;
        paragraph.y = 0;
        paragraph.width = 5;
        paragraph.height = 10;
        paragraph.border_style.foreground = tui::BLACK;
        paragraph.border_style.background = tui::MAGENTA;
        paragraph.text_style.foreground = tui::DARK_GREEN;
        paragraph.text_style.background = tui::YELLOW;
        paragraph.title_style.foreground = tui::DARK_RED;
        paragraph.title_style.background = tui::WHITE;
        window.add(paragraph);
        
        CHAR_INFO *content = window.get_content();
        std::vector<std::pair<std::string, std::string> > requirements = {
            {"43", "208"},
            {"45", "208"},
            {"76", "244"},
            {"111", "244"},
            {"114", "244"},
            {"124", "208"},
            {"76", "226"},
            {"111", "226"},
            {"114", "226"},
            {"124", "208"},
            {"124", "208"},
            {"101", "226"},
            {"109", "226"},
            {"32", "226"},
            {"124", "208"},
            {"124", "208"},
            {"105", "226"},
            {"112", "226"},
            {"115", "226"},
            {"124", "208"},
            {"124", "208"},
            {"117", "226"},
            {"109", "226"},
            {"32", "226"},
            {"124", "208"},
            {"124", "208"},
            {"100", "226"},
            {"111", "226"},
            {"108", "226"},
            {"124", "208"},
            {"124", "208"},
            {"111", "226"},
            {"114", "226"},
            {"32", "226"},
            {"124", "208"},
            {"124", "208"},
            {"115", "226"},
            {"105", "226"},
            {"116", "226"},
            {"124", "208"},
            {"124", "208"},
            {"46", "226"},
            {"46", "226"},
            {"46", "226"},
            {"124", "208"},
            {"43", "208"},
            {"45", "208"},
            {"45", "208"},
            {"45", "208"},
            {"43", "208"}
        };
        int current_char = 0;
        for(int i = 0; i < window.rows() * window.columns(); i++) {
            if(std::to_string(content[i].Char.AsciiChar) != "0" && std::to_string(content[i].Attributes) != "0") {
                REQUIRE(std::to_string(content[i].Char.AsciiChar) == requirements[current_char].first);
                REQUIRE(std::to_string(content[i].Attributes) == requirements[current_char].second);
                current_char++;
            }
        }
    }
    SECTION("List1", "[list1]") {
        // Test list representation and scrolling functionality
        tui::Window window;
        tui::List list;
        list.title = sample_string;
        list.rows = {
            "[0] Hello World. Foo Bar",
            "[1] Foo Bar. Hello World.",
            "[2] Foo Bar",
            "[3] Hello World",
            "[4] Bar Foo",
            "[5] World Hello"
        };
        list.x = 1;
        list.y = 2;
        list.width = 12;
        list.height = 4;
        list.border_style.foreground = tui::BLACK;
        list.border_style.background = tui::MAGENTA;
        list.text_style.foreground = tui::DARK_GREEN;
        list.text_style.background = tui::YELLOW;
        list.title_style.foreground = tui::DARK_RED;
        list.title_style.background = tui::WHITE;
        window.add(list);
        
        CHAR_INFO *content = window.get_content();
        std::vector<std::pair<std::string, std::string> > requirements = {
            {"43", "208"},
            {"45", "208"},
            {"76", "244"},
            {"111", "244"},
            {"114", "244"},
            {"101", "244"},
            {"109", "244"},
            {"32", "244"},
            {"105", "244"},
            {"112", "244"},
            {"115", "244"},
            {"117", "244"},
            {"124", "208"},
            {"91", "226"},
            {"48", "226"},
            {"93", "226"},
            {"32", "226"},
            {"72", "226"},
            {"101", "226"},
            {"108", "226"},
            {"46", "226"},
            {"46", "226"},
            {"46", "226"},
            {"124", "208"},
            {"124", "208"},
            {"91", "226"},
            {"49", "226"},
            {"93", "226"},
            {"32", "226"},
            {"70", "226"},
            {"111", "226"},
            {"111", "226"},
            {"46", "226"},
            {"46", "226"},
            {"46", "226"},
            {"124", "208"},
            {"43", "208"},
            {"45", "208"},
            {"45", "208"},
            {"45", "208"},
            {"45", "208"},
            {"45", "208"},
            {"45", "208"},
            {"45", "208"},
            {"45", "208"},
            {"45", "208"},
            {"45", "208"},
            {"43", "208"}
        };
        int current_char = 0;
        for(int i = 0; i < window.rows() * window.columns(); i++) {
            if(std::to_string(content[i].Char.AsciiChar) != "0" && std::to_string(content[i].Attributes) != "0") {
                REQUIRE(std::to_string(content[i].Char.AsciiChar) == requirements[current_char].first);
                REQUIRE(std::to_string(content[i].Attributes) == requirements[current_char].second);
                current_char++;
            }
        }
        // Scroll down by factor of 100 and assert
        // factor should clamp in scroll down method with max
        list.scroll_down(window, 100);
        
        content = window.get_content();
        requirements = {
            {"43", "208"},
            {"45", "208"},
            {"76", "244"},
            {"111", "244"},
            {"114", "244"},
            {"101", "244"},
            {"109", "244"},
            {"32", "244"},
            {"105", "244"},
            {"112", "244"},
            {"115", "244"},
            {"117", "244"},
            {"124", "208"},
            {"91", "226"},
            {"52", "226"},
            {"93", "226"},
            {"32", "226"},
            {"66", "226"},
            {"97", "226"},
            {"114", "226"},
            {"46", "226"},
            {"46", "226"},
            {"46", "226"},
            {"124", "208"},
            {"124", "208"},
            {"91", "226"},
            {"53", "226"},
            {"93", "226"},
            {"32", "226"},
            {"87", "226"},
            {"111", "226"},
            {"114", "226"},
            {"46", "226"},
            {"46", "226"},
            {"46", "226"},
            {"124", "208"},
            {"43", "208"},
            {"45", "208"},
            {"45", "208"},
            {"45", "208"},
            {"45", "208"},
            {"45", "208"},
            {"45", "208"},
            {"45", "208"},
            {"45", "208"},
            {"45", "208"},
            {"45", "208"},
            {"43", "208"}
        };
        current_char = 0;
        for(int i = 0; i < window.rows() * window.columns(); i++) {
            if(std::to_string(content[i].Char.AsciiChar) != "0" && std::to_string(content[i].Attributes) != "0") {
                REQUIRE(std::to_string(content[i].Char.AsciiChar) == requirements[current_char].first);
                REQUIRE(std::to_string(content[i].Attributes) == requirements[current_char].second);
                current_char++;
            }
        }
        // Scroll up by a factor of 2 and assert
        list.scroll_up(window, 2);
        
        content = window.get_content();
        requirements = {
            {"43", "208"},
            {"45", "208"},
            {"76", "244"},
            {"111", "244"},
            {"114", "244"},
            {"101", "244"},
            {"109", "244"},
            {"32", "244"},
            {"105", "244"},
            {"112", "244"},
            {"115", "244"},
            {"117", "244"},
            {"124", "208"},
            {"91", "226"},
            {"50", "226"},
            {"93", "226"},
            {"32", "226"},
            {"70", "226"},
            {"111", "226"},
            {"111", "226"},
            {"46", "226"},
            {"46", "226"},
            {"46", "226"},
            {"124", "208"},
            {"124", "208"},
            {"91", "226"},
            {"51", "226"},
            {"93", "226"},
            {"32", "226"},
            {"72", "226"},
            {"101", "226"},
            {"108", "226"},
            {"46", "226"},
            {"46", "226"},
            {"46", "226"},
            {"124", "208"},
            {"43", "208"},
            {"45", "208"},
            {"45", "208"},
            {"45", "208"},
            {"45", "208"},
            {"45", "208"},
            {"45", "208"},
            {"45", "208"},
            {"45", "208"},
            {"45", "208"},
            {"45", "208"},
            {"43", "208"}
        };
        current_char = 0;
        for(int i = 0; i < window.rows() * window.columns(); i++) {
            if(std::to_string(content[i].Char.AsciiChar) != "0" && std::to_string(content[i].Attributes) != "0") {
                REQUIRE(std::to_string(content[i].Char.AsciiChar) == requirements[current_char].first);
                REQUIRE(std::to_string(content[i].Attributes) == requirements[current_char].second);
                current_char++;
            }
        }
    }
}

TEST_CASE("Widget Equality", "[widget_equality]") {
    // Test widget operator overloading for widget equality
    SECTION("Paragraph") {
        tui::Paragraph paragraphFoo;
        paragraphFoo.title = sample_string;
        paragraphFoo.text = (sample_string + sample_string);
        paragraphFoo.x = 1;
        paragraphFoo.y = 2;
        paragraphFoo.width = 3;
        paragraphFoo.height = 4;
        paragraphFoo.border = true;
        paragraphFoo.border_style.foreground = tui::BLACK;
        paragraphFoo.border_style.background = tui::MAGENTA;
        paragraphFoo.text_style.foreground = tui::DARK_GREEN;
        paragraphFoo.text_style.background = tui::YELLOW;
        paragraphFoo.title_style.foreground = tui::DARK_RED;
        paragraphFoo.title_style.background = tui::WHITE;

        tui::Paragraph paragraphBar;
        paragraphBar.title = sample_string;
        paragraphBar.text = (sample_string + sample_string);
        paragraphBar.x = 1;
        paragraphBar.y = 2;
        paragraphBar.width = 3;
        paragraphBar.height = 4;
        paragraphBar.border = true;
        paragraphBar.border_style.foreground = tui::BLACK;
        paragraphBar.border_style.background = tui::MAGENTA;
        paragraphBar.text_style.foreground = tui::DARK_GREEN;
        paragraphBar.text_style.background = tui::YELLOW;
        paragraphBar.title_style.foreground = tui::DARK_RED;
        paragraphBar.title_style.background = tui::WHITE;

        tui::Paragraph paragraphDifferent;
        paragraphDifferent.title = "";
        paragraphDifferent.text = "";
        paragraphDifferent.x = 100;
        paragraphDifferent.y = 100;
        paragraphDifferent.width = 100;
        paragraphDifferent.height = 100;
        paragraphDifferent.border = false;
        paragraphDifferent.border_style.foreground = tui::BLUE;
        paragraphDifferent.border_style.background = tui::BLUE;
        paragraphDifferent.text_style.foreground = tui::BLUE;
        paragraphDifferent.text_style.background = tui::BLUE;
        paragraphDifferent.title_style.foreground = tui::BLUE;
        paragraphDifferent.title_style.background = tui::BLUE;

        REQUIRE(paragraphFoo == paragraphBar);
        REQUIRE(!(paragraphFoo == paragraphDifferent));
    }
    SECTION("List") {
        tui::List listFoo;
        listFoo.title = sample_string;
        listFoo.rows = {
            "[0] Hello World!",
            "[1] Foo Bar"
        };
        listFoo.x = 1;
        listFoo.y = 2;
        listFoo.width = 3;
        listFoo.height = 4;
        listFoo.first_element = 0;
        listFoo.border = true;
        listFoo.border_style.foreground = tui::BLACK;
        listFoo.border_style.background = tui::MAGENTA;
        listFoo.text_style.foreground = tui::DARK_GREEN;
        listFoo.text_style.background = tui::YELLOW;
        listFoo.title_style.foreground = tui::DARK_RED;
        listFoo.title_style.background = tui::WHITE;

        tui::List listBar;
        listBar.title = sample_string;
        listBar.rows = {
            "[0] Hello World!",
            "[1] Foo Bar"
        };
        listBar.x = 1;
        listBar.y = 2;
        listBar.width = 3;
        listBar.height = 4;
        listBar.first_element = 0;
        listBar.border = true;
        listBar.border_style.foreground = tui::BLACK;
        listBar.border_style.background = tui::MAGENTA;
        listBar.text_style.foreground = tui::DARK_GREEN;
        listBar.text_style.background = tui::YELLOW;
        listBar.title_style.foreground = tui::DARK_RED;
        listBar.title_style.background = tui::WHITE;

        tui::List listDifferent;
        listDifferent.title = "";
        listDifferent.rows = {};
        listDifferent.x = 100;
        listDifferent.y = 100;
        listDifferent.width = 100;
        listDifferent.height = 100;
        listDifferent.first_element = 100;
        listDifferent.border = false;
        listDifferent.border_style.foreground = tui::BLUE;
        listDifferent.border_style.background = tui::BLUE;
        listDifferent.text_style.foreground = tui::BLUE;
        listDifferent.text_style.background = tui::BLUE;
        listDifferent.title_style.foreground = tui::BLUE;
        listDifferent.title_style.background = tui::BLUE;

        REQUIRE(listFoo == listBar);
        REQUIRE(!(listFoo == listDifferent));
    }
}

TEST_CASE("Color Handling", "[color_handling]") {
    // Test color constants and bitwise operations
    REQUIRE(tui::get_color(tui::BLACK, tui::WHITE)            == 0x00F0);
    REQUIRE(tui::get_color(tui::DARK_BLUE, tui::YELLOW)       == 0x00E1);
    REQUIRE(tui::get_color(tui::DARK_GREEN, tui::MAGENTA)     == 0x00D2);
    REQUIRE(tui::get_color(tui::DARK_CYAN, tui::RED)          == 0x00C3);
    REQUIRE(tui::get_color(tui::DARK_RED, tui::CYAN)          == 0x00B4);
    REQUIRE(tui::get_color(tui::DARK_MAGENTA, tui::GREEN)     == 0x00A5);
    REQUIRE(tui::get_color(tui::DARK_YELLOW, tui::BLUE)       == 0x0096);
    REQUIRE(tui::get_color(tui::GRAY, tui::DARK_GRAY)         == 0x0087);
    
    REQUIRE(tui::get_color(tui::WHITE, tui::BLACK)            == 0x000F);
    REQUIRE(tui::get_color(tui::YELLOW, tui::DARK_BLUE)       == 0x001E);
    REQUIRE(tui::get_color(tui::MAGENTA, tui::DARK_GREEN)     == 0x002D);
    REQUIRE(tui::get_color(tui::RED, tui::DARK_CYAN)          == 0x003C);
    REQUIRE(tui::get_color(tui::CYAN, tui::DARK_RED)          == 0x004B);
    REQUIRE(tui::get_color(tui::GREEN, tui::DARK_MAGENTA)     == 0x005A);
    REQUIRE(tui::get_color(tui::BLUE, tui::DARK_YELLOW)       == 0x0069);
    REQUIRE(tui::get_color(tui::DARK_GRAY, tui::GRAY)         == 0x0078);
}
#elif defined(IS_POSIX)

#endif