# termui-cpp

C++ header-only terminal user interface library.

termui-cpp is currently available for Windows with partial support for Unix (through Ncurses).

## Features

- Premade widgets
- Event handling
- Custom styling
- Colors (Windows only)

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
- [Gauge](./examples/gauge.cpp)
- [List](./examples/list.cpp)
- [Paragraph](./examples/paragraph.cpp)

Examples in `/examples` are cross-platform; however, some features may be limited on unix.

Build all the examples with `make build-examples`.

## Testing

Testing requires [Catch2](https://github.com/catchorg/Catch2/).

Test with `make test`. Compile the tests with `make test-compile`.

The test output is written to the file `/test/test_output.txt`.
