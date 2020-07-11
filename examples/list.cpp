#include "../single_include/tui/tui.hpp"

int main() {
    // Construct window with 1280x780 dimensions
    tui::Window window(1280, 720);
    
    window.set_title("List Example");

    tui::List l;
    l.title = "List";
    l.rows = {
        "[0] This",
        "[1] is",
        "[2] a",
        "[3] list.",
        "[4] This line is longer than the other rows.",
        "[5] You can control lists!",
        "[6] Hello, World!",
        "[8] Foo",
        "[9] Bar",
        "[10] Last element"
    };
    l.text_style.foreground = tui::YELLOW;
    l.set_dimensions(0, 0, 25, 8);

    bool quit = false;
    tui::Event event;

    while(!quit) {
        if(window.poll_event(event)) {
            if(event.type == tui::KEYDOWN) {
                switch(event.key) {
                    case 'Q':
                        quit = true;
                        break;
                    case 'J':
                    case VK_DOWN:
                        l.scroll_down(window);
                        break;
                    case 'K':
                    case VK_UP:
                        l.scroll_up(window);
                        break;
                }
            }
        }
        // Add list widget to the window
        // in the while loop to update
        // the list for any changes.
        window.add(l);
        window.render();
    }

    window.close();
    return 0;
}