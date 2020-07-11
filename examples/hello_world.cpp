#include "../single_include/tui/tui.hpp"

int main() {
    // Construct window with default dimensions
    tui::Window window;

    window.set_title("Hello, World!");

    tui::Paragraph p;
    p.text = "Hello, World!";
    p.set_dimensions(0, 0, 25, 5);

    bool quit = false;
    tui::Event event;

    // Add paragraph widget to the window
    window.add(p);

    while(!quit) {
        if(window.poll_event(event)) {
            if(event.type == tui::KEYDOWN) {
                quit = true;
            }
        }
        window.render();
    }

    window.close();
    return 0;
}