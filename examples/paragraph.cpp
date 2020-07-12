#include "../single_include/tui/tui.hpp"

int main() {
    // Construct window
    tui::Window window;

    window.set_title("Paragraph Example");

    tui::Paragraph p0;
    p0.text = "Borderless Text";
    p0.set_dimensions(0, 0, 20, 5);
    p0.border = false;

    tui::Paragraph p1;
    p1.text = "Hello";
    p1.text = "Hello, World!";
    p1.set_dimensions(23, 0, 36, 5);

    tui::Paragraph p2;
    p2.title = "Multiline";
    p2.text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Phasellus lacinia dui ut augue eleifend, at condimentum felis porta. Aenean at.";
    p2.set_dimensions(0, 5, 59, 5);
    p2.border_style.foreground = tui::YELLOW;

    tui::Paragraph p3;
    p3.title = "Colored title";
    p3.text = "You can customize foreground and background color!";
    p3.set_dimensions(0, 10, 59, 5);
    p3.title_style.foreground = tui::BLACK;
    p3.title_style.background = tui::WHITE;
    p3.text_style.foreground = tui::YELLOW;
    p3.text_style.background = tui::CYAN;

    tui::Paragraph p4;
    p4.title = "Text Box with Wrapping";
    p4.text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Duis sed dui bibendum, consequat mauris molestie, ultricies enim. Nunc ut pellentesque orci. Nullam at maximus sapien. Quisque egestas posuere ex tempor mattis. In diam ex, vestibulum eget tortor a, convallis euismod dui. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nunc.";
    p4.set_dimensions(60, 0, 40, 15);
    p4.border_style.foreground = tui::BLUE;

    bool quit = false;
    tui::Event event;

    // Add paragraph widgets to the window
    window.add(p0, p1, p2, p3, p4);

    while(!quit) {
        if(window.poll_event(event)) {
            if(event.type == tui::KEYDOWN) {
                switch(event.key) {
                    case 'q':
                        quit = true;
                        break;
                }
            }
        }
        window.render();
    }

    window.close();
    return 0;
}