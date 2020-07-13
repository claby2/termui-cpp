#include "../single_include/tui/tui.hpp"

int main() {
    // Construct window
    tui::Window window;

    window.set_title("Gauge Example");

    tui::Gauge g0;
    g0.title = "Small Gauge";
    g0.set_dimensions(0, 0, 30, 3);
    g0.label = "75%";
    g0.percent = 75;
    g0.label_style.foreground = tui::BLACK;
    g0.bar_color = tui::YELLOW;

    tui::Gauge g1;
    g1.title = "Big Gauge";
    g1.set_dimensions(0, 3, 30, 10);
    g1.label = "30%";
    g1.percent = 30;
    g1.label_style.foreground = tui::BLACK;
    g1.bar_color = tui::GREEN;

    bool quit = false;
    tui::Event event;

    // Add bar gauge widgets to the window
    window.add(g0, g1);

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