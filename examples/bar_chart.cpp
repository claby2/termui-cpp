#include "../single_include/tui/tui.hpp"

int main() {
    // Construct window with 1280x780 dimensions
    tui::Window window(1280, 720);

    window.set_title("Bar Chart Example");

    tui::BarChart bc;
    bc.data = {134, 145, 80, 70, 30};
    bc.labels = {"first", "second", "third", "fourth", "fifth"};
    bc.title = "Bar Chart";
    bc.set_dimensions(0, 0, 40, 20);
    bc.bar_width = 5;
    bc.bar_color = tui::RED;
    bc.label_style.foreground = tui::BLUE;
    bc.number_style.foreground = tui::YELLOW;

    bool quit = false;
    tui::Event event;

    // Add paragraph widgets to the window
    window.add(bc);

    while(!quit) {
        if(window.poll_event(event)) {
            if(event.type == tui::KEYDOWN) {
                switch(event.key) {
                    case 'Q':
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