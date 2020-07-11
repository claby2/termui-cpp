# termui-cpp

C++ header-only terminal user interface library.

termui-cpp is currently only available for Windows.

## Installation

1. Download the latest [single header version](https://raw.githubusercontent.com/claby2/termui-cpp/master/single_include/tui/tui.hpp).
2. Either put the header file in a central location (with a specified path) or directly in your project tree.

## Hello World

```cpp
#include <tui/tui.hpp>

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
```

## Widgets

- [Bar Chart](./examples/bar_chart.cpp)
- [List](./examples/list.cpp)
- [Paragraph](./examples/paragraph.cpp)
