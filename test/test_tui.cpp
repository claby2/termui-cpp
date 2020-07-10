#include <catch2/catch.hpp>
#include "../single_include/tui/tui.hpp"

#ifdef _WIN32
#include <windows.h>
#include <fstream>
#include <string>
#include <unordered_map>
#include <utility>

std::string sample_string = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nullam pharetra.";

TEST_CASE("Widget Representation", "[widget_representation]") {

    // Format of widget requirements:
    // The requirements unordered map has a key
    // of type int and a pair value.
    // The int represents the index of content
    // of a specified cell.
    // The pair value holds two string values.
    // The first value in the pair is a string
    // representation of the ascii char;
    // The second value in the pair is string
    // representation of the attribute, which
    // is the color of the cell.

    SECTION("Paragraph1", "[paragraph1]") {
        // Test large paragraph representation
        tui::Window window(300, 300);
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
        std::unordered_map<int, std::pair<std::string, std::string> > requirements = {
            {65, std::make_pair("43", "208")},
            {66, std::make_pair("45", "208")},
            {67, std::make_pair("76", "244")},
            {97, std::make_pair("124", "208")},
            {98, std::make_pair("76", "226")},
            {99, std::make_pair("124", "208")},
            {129, std::make_pair("124", "208")},
            {130, std::make_pair("111", "226")},
            {131, std::make_pair("124", "208")},
            {161, std::make_pair("43", "208")},
            {162, std::make_pair("45", "208")},
            {163, std::make_pair("43", "208")},
        };
        for(int i = 0; i < window.rows() * window.columns(); i++) {
            if(std::to_string(content[i].Char.AsciiChar) != "0" && std::to_string(content[i].Attributes) != "0") {
                REQUIRE(std::to_string(content[i].Char.AsciiChar) == requirements[i].first);
                REQUIRE(std::to_string(content[i].Attributes) == requirements[i].second);
            }
        }
    }
    SECTION("Paragraph2", "[paragraph2") {
        // Test large paragraph representation
        tui::Window window(300, 300);
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
        std::unordered_map<int, std::pair<std::string, std::string> > requirements = {
            {0, std::make_pair("43", "208")},
            {1, std::make_pair("45", "208")},
            {2, std::make_pair("76", "244")},
            {3, std::make_pair("111", "244")},
            {4, std::make_pair("114", "244")},
            {32, std::make_pair("124", "208")},
            {33, std::make_pair("76", "226")},
            {34, std::make_pair("111", "226")},
            {35, std::make_pair("114", "226")},
            {36, std::make_pair("124", "208")},
            {64, std::make_pair("124", "208")},
            {65, std::make_pair("101", "226")},
            {66, std::make_pair("109", "226")},
            {67, std::make_pair("32", "226")},
            {68, std::make_pair("124", "208")},
            {96, std::make_pair("124", "208")},
            {97, std::make_pair("105", "226")},
            {98, std::make_pair("112", "226")},
            {99, std::make_pair("115", "226")},
            {100, std::make_pair("124", "208")},
            {128, std::make_pair("124", "208")},
            {129, std::make_pair("117", "226")},
            {130, std::make_pair("109", "226")},
            {131, std::make_pair("32", "226")},
            {132, std::make_pair("124", "208")},
            {160, std::make_pair("124", "208")},
            {161, std::make_pair("100", "226")},
            {162, std::make_pair("111", "226")},
            {163, std::make_pair("108", "226")},
            {164, std::make_pair("124", "208")},
            {192, std::make_pair("124", "208")},
            {193, std::make_pair("111", "226")},
            {194, std::make_pair("114", "226")},
            {195, std::make_pair("32", "226")},
            {196, std::make_pair("124", "208")},
            {224, std::make_pair("124", "208")},
            {225, std::make_pair("115", "226")},
            {226, std::make_pair("105", "226")},
            {227, std::make_pair("116", "226")},
            {228, std::make_pair("124", "208")},
            {256, std::make_pair("124", "208")},
            {257, std::make_pair("46", "226")},
            {258, std::make_pair("46", "226")},
            {259, std::make_pair("46", "226")},
            {260, std::make_pair("124", "208")},
            {288, std::make_pair("43", "208")},
            {289, std::make_pair("45", "208")},
            {290, std::make_pair("45", "208")},
            {291, std::make_pair("45", "208")},
            {292, std::make_pair("43", "208")},
        };
        for(int i = 0; i < window.rows() * window.columns(); i++) {
            if(std::to_string(content[i].Char.AsciiChar) != "0" && std::to_string(content[i].Attributes) != "0") {
                REQUIRE(std::to_string(content[i].Char.AsciiChar) == requirements[i].first);
                REQUIRE(std::to_string(content[i].Attributes) == requirements[i].second);
            }
        }
    }
    SECTION("List1", "[list1]") {
        // Test list representation and scrolling functionality
        tui::Window window(300, 300);
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
        std::unordered_map<int, std::pair<std::string, std::string> > requirements = {
            {65, std::make_pair("43", "208")},
            {66, std::make_pair("45", "208")},
            {67, std::make_pair("76", "244")},
            {68, std::make_pair("111", "244")},
            {69, std::make_pair("114", "244")},
            {70, std::make_pair("101", "244")},
            {71, std::make_pair("109", "244")},
            {72, std::make_pair("32", "244")},
            {73, std::make_pair("105", "244")},
            {74, std::make_pair("112", "244")},
            {75, std::make_pair("115", "244")},
            {76, std::make_pair("117", "244")},
            {97, std::make_pair("124", "208")},
            {98, std::make_pair("91", "226")},
            {99, std::make_pair("48", "226")},
            {100, std::make_pair("93", "226")},
            {101, std::make_pair("32", "226")},
            {102, std::make_pair("72", "226")},
            {103, std::make_pair("101", "226")},
            {104, std::make_pair("108", "226")},
            {105, std::make_pair("46", "226")},
            {106, std::make_pair("46", "226")},
            {107, std::make_pair("46", "226")},
            {108, std::make_pair("124", "208")},
            {129, std::make_pair("124", "208")},
            {130, std::make_pair("91", "226")},
            {131, std::make_pair("49", "226")},
            {132, std::make_pair("93", "226")},
            {133, std::make_pair("32", "226")},
            {134, std::make_pair("70", "226")},
            {135, std::make_pair("111", "226")},
            {136, std::make_pair("111", "226")},
            {137, std::make_pair("46", "226")},
            {138, std::make_pair("46", "226")},
            {139, std::make_pair("46", "226")},
            {140, std::make_pair("124", "208")},
            {161, std::make_pair("43", "208")},
            {162, std::make_pair("45", "208")},
            {163, std::make_pair("45", "208")},
            {164, std::make_pair("45", "208")},
            {165, std::make_pair("45", "208")},
            {166, std::make_pair("45", "208")},
            {167, std::make_pair("45", "208")},
            {168, std::make_pair("45", "208")},
            {169, std::make_pair("45", "208")},
            {170, std::make_pair("45", "208")},
            {171, std::make_pair("45", "208")},
            {172, std::make_pair("43", "208")},
        };
        for(int i = 0; i < window.rows() * window.columns(); i++) {
            if(std::to_string(content[i].Char.AsciiChar) != "0" && std::to_string(content[i].Attributes) != "0") {
                REQUIRE(std::to_string(content[i].Char.AsciiChar) == requirements[i].first);
                REQUIRE(std::to_string(content[i].Attributes) == requirements[i].second);
            }
        }
        // Scroll down by factor of 100 and assert
        // factor should clamp in scroll down method with max
        list.scroll_down(window, 100);
        
        content = window.get_content();
        requirements = {
            {65, std::make_pair("43", "208")},
            {66, std::make_pair("45", "208")},
            {67, std::make_pair("76", "244")},
            {68, std::make_pair("111", "244")},
            {69, std::make_pair("114", "244")},
            {70, std::make_pair("101", "244")},
            {71, std::make_pair("109", "244")},
            {72, std::make_pair("32", "244")},
            {73, std::make_pair("105", "244")},
            {74, std::make_pair("112", "244")},
            {75, std::make_pair("115", "244")},
            {76, std::make_pair("117", "244")},
            {97, std::make_pair("124", "208")},
            {98, std::make_pair("91", "226")},
            {99, std::make_pair("52", "226")},
            {100, std::make_pair("93", "226")},
            {101, std::make_pair("32", "226")},
            {102, std::make_pair("66", "226")},
            {103, std::make_pair("97", "226")},
            {104, std::make_pair("114", "226")},
            {105, std::make_pair("46", "226")},
            {106, std::make_pair("46", "226")},
            {107, std::make_pair("46", "226")},
            {108, std::make_pair("124", "208")},
            {129, std::make_pair("124", "208")},
            {130, std::make_pair("91", "226")},
            {131, std::make_pair("53", "226")},
            {132, std::make_pair("93", "226")},
            {133, std::make_pair("32", "226")},
            {134, std::make_pair("87", "226")},
            {135, std::make_pair("111", "226")},
            {136, std::make_pair("114", "226")},
            {137, std::make_pair("46", "226")},
            {138, std::make_pair("46", "226")},
            {139, std::make_pair("46", "226")},
            {140, std::make_pair("124", "208")},
            {161, std::make_pair("43", "208")},
            {162, std::make_pair("45", "208")},
            {163, std::make_pair("45", "208")},
            {164, std::make_pair("45", "208")},
            {165, std::make_pair("45", "208")},
            {166, std::make_pair("45", "208")},
            {167, std::make_pair("45", "208")},
            {168, std::make_pair("45", "208")},
            {169, std::make_pair("45", "208")},
            {170, std::make_pair("45", "208")},
            {171, std::make_pair("45", "208")},
            {172, std::make_pair("43", "208")},
        };
        for(int i = 0; i < window.rows() * window.columns(); i++) {
            if(std::to_string(content[i].Char.AsciiChar) != "0" && std::to_string(content[i].Attributes) != "0") {
                REQUIRE(std::to_string(content[i].Char.AsciiChar) == requirements[i].first);
                REQUIRE(std::to_string(content[i].Attributes) == requirements[i].second);
            }
        }
        // Scroll up by a factor of 2 and assert
        list.scroll_up(window, 2);
        
        content = window.get_content();
        requirements = {
            {65, std::make_pair("43", "208")},
            {66, std::make_pair("45", "208")},
            {67, std::make_pair("76", "244")},
            {68, std::make_pair("111", "244")},
            {69, std::make_pair("114", "244")},
            {70, std::make_pair("101", "244")},
            {71, std::make_pair("109", "244")},
            {72, std::make_pair("32", "244")},
            {73, std::make_pair("105", "244")},
            {74, std::make_pair("112", "244")},
            {75, std::make_pair("115", "244")},
            {76, std::make_pair("117", "244")},
            {97, std::make_pair("124", "208")},
            {98, std::make_pair("91", "226")},
            {99, std::make_pair("50", "226")},
            {100, std::make_pair("93", "226")},
            {101, std::make_pair("32", "226")},
            {102, std::make_pair("70", "226")},
            {103, std::make_pair("111", "226")},
            {104, std::make_pair("111", "226")},
            {105, std::make_pair("46", "226")},
            {106, std::make_pair("46", "226")},
            {107, std::make_pair("46", "226")},
            {108, std::make_pair("124", "208")},
            {129, std::make_pair("124", "208")},
            {130, std::make_pair("91", "226")},
            {131, std::make_pair("51", "226")},
            {132, std::make_pair("93", "226")},
            {133, std::make_pair("32", "226")},
            {134, std::make_pair("72", "226")},
            {135, std::make_pair("101", "226")},
            {136, std::make_pair("108", "226")},
            {137, std::make_pair("46", "226")},
            {138, std::make_pair("46", "226")},
            {139, std::make_pair("46", "226")},
            {140, std::make_pair("124", "208")},
            {161, std::make_pair("43", "208")},
            {162, std::make_pair("45", "208")},
            {163, std::make_pair("45", "208")},
            {164, std::make_pair("45", "208")},
            {165, std::make_pair("45", "208")},
            {166, std::make_pair("45", "208")},
            {167, std::make_pair("45", "208")},
            {168, std::make_pair("45", "208")},
            {169, std::make_pair("45", "208")},
            {170, std::make_pair("45", "208")},
            {171, std::make_pair("45", "208")},
            {172, std::make_pair("43", "208")},
        };
        for(int i = 0; i < window.rows() * window.columns(); i++) {
            if(std::to_string(content[i].Char.AsciiChar) != "0" && std::to_string(content[i].Attributes) != "0") {
                REQUIRE(std::to_string(content[i].Char.AsciiChar) == requirements[i].first);
                REQUIRE(std::to_string(content[i].Attributes) == requirements[i].second);
            }
        }
    }
}

TEST_CASE("Widget Equality", "[widget_equality]") {
    // Test widget operator overloading for widget equality
    SECTION("Paragraph", "[paragraph") {
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
    SECTION("List", "[list") {
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
#elif __linux__
// LINUX INCLUDES
#else
#if (defined (__APPLE__) && defined (__MACH__))
// APPLE INCLUDES
#else
    #error PLATFORM NOT SUPPORTED
#endif
#endif