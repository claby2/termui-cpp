#ifndef TUI_HPP
#define TUI_HPP

#ifdef _WIN32
#   define IS_WIN
#include <windows.h>
#else
#   define IS_POSIX
#include <ncurses.h>
#include <sys/ioctl.h>
#include <unistd.h>
#endif

#include <algorithm>
#include <climits>
#include <math.h>
#include <stdexcept>
#include <stdlib.h>
#include <string>
#include <vector>

namespace tui {
    // Event handling
    enum EventType {
        KEYDOWN,
        MOUSEBUTTONDOWN,
        UNDEFINED
    };

#ifdef IS_WIN
    struct Event {
        EventType type = UNDEFINED;
        uint8_t key;
    };

    // Color handling
    enum Color : short {
        BLACK        = 0x0000,
        DARK_BLUE    = 0x0001,
        DARK_GREEN   = 0x0002,
        DARK_CYAN    = 0x0003,
        DARK_RED     = 0x0004,
        DARK_MAGENTA = 0x0005,
        DARK_YELLOW  = 0x0006,
        GRAY         = 0x0007,
        DARK_GRAY    = 0x0008,
        BLUE         = 0x0009,
        GREEN        = 0x000A,
        CYAN         = 0x000B,
        RED          = 0x000C,
        MAGENTA      = 0x000D,
        YELLOW       = 0x000E,
        WHITE        = 0x000F
    };
#elif defined(IS_POSIX)
    struct Event {
        EventType type = UNDEFINED;
        int key;
    };

    // Color handling
    enum Color {
        BLACK   = 0,
        RED     = 1,
        GREEN   = 2,
        YELLOW  = 3,
        BLUE    = 4,
        MAGENTA = 5,
        CYAN    = 6,
        WHITE   = 7
    };
#endif

    // Return color after bitwise operation as short
    // Combines foreground and background
    inline short get_color(short foreground, short background) {
        return (foreground | (background << 4));
    }

    // Widget definitions
    struct Widget {
        struct {
            short foreground = WHITE;
            short background = BLACK;
        } border_style;
        struct {
            short foreground = WHITE;
            short background = BLACK;
        } text_style;
        struct {
            short foreground = WHITE;
            short background = BLACK;
        } title_style;
        std::string title;
        bool border = true;
        int x;      // Position of left side of widget
        int y;      // Position of top of widget
        int width;  // Width of widget
        int height; // Height of widget

        void set_dimensions(int x, int y, int width, int height);
    };

    struct Paragraph : Widget {
        std::string text;
    };

    struct List : Widget{
        std::vector<std::string> rows;
        int first_element = 0; // Element at the top of the list
        template<typename Window>
        void scroll_up(Window &window, int factor = 1);
        template<typename Window>
        void scroll_down(Window &window, int factor = 1);
    };

    struct BarChart : Widget {
        std::vector<int> data;
        std::vector<std::string> labels;
        int bar_width;
        short bar_color;
        struct {
            short foreground = WHITE;
            short background = BLACK;
        } label_style;
        struct {
            short foreground = WHITE;
            short background = BLACK;
        } number_style;
    };

    // Widget comparisons
    bool operator==(const Paragraph& paragraph1, const Paragraph& paragraph2) {
        return (
            paragraph1.title == paragraph2.title &&
            paragraph1.text == paragraph2.text &&
            paragraph1.x == paragraph2.x &&
            paragraph1.y == paragraph2.y &&
            paragraph1.width == paragraph2.width &&
            paragraph1.height == paragraph2.height &&
            paragraph1.border == paragraph2.border &&
            paragraph1.border_style.foreground == paragraph2.border_style.foreground &&
            paragraph1.border_style.background == paragraph2.border_style.background &&
            paragraph1.text_style.foreground == paragraph2.text_style.foreground &&
            paragraph1.text_style.background == paragraph2.text_style.background &&
            paragraph1.title_style.foreground == paragraph2.title_style.foreground &&
            paragraph1.title_style.background == paragraph2.title_style.background
        );
    };

    bool operator==(const List& list1, const List& list2) {
        return (
            list1.title == list2.title &&
            list1.rows == list2.rows &&
            list1.x == list2.x &&
            list1.y == list2.y &&
            list1.width == list2.width &&
            list1.height == list2.height &&
            list1.first_element == list2.first_element &&
            list1.border == list2.border &&
            list1.border_style.foreground == list2.border_style.foreground &&
            list1.border_style.background == list2.border_style.background &&
            list1.text_style.foreground == list2.text_style.foreground &&
            list1.text_style.background == list2.text_style.background &&
            list1.title_style.foreground == list2.title_style.foreground &&
            list1.title_style.background == list2.title_style.background
        );
    }

    bool operator==(const BarChart& bar_chart1, const BarChart& bar_chart2) {
        return (
            bar_chart1.title == bar_chart2.title &&
            bar_chart1.data == bar_chart2.data &&
            bar_chart1.labels == bar_chart2.labels &&
            bar_chart1.bar_width == bar_chart2.bar_width &&
            bar_chart1.bar_color == bar_chart2.bar_color &&
            bar_chart1.x == bar_chart2.x &&
            bar_chart1.y == bar_chart2.y &&
            bar_chart1.width == bar_chart2.width &&
            bar_chart1.height == bar_chart2.height &&
            bar_chart1.border == bar_chart2.border &&
            bar_chart1.label_style.foreground == bar_chart2.label_style.foreground &&
            bar_chart1.number_style.background == bar_chart2.number_style.background &&
            bar_chart1.text_style.foreground == bar_chart2.text_style.foreground &&
            bar_chart1.text_style.background == bar_chart2.text_style.background &&
            bar_chart1.border_style.foreground == bar_chart2.border_style.foreground &&
            bar_chart1.border_style.background == bar_chart2.border_style.background &&
            bar_chart1.text_style.foreground == bar_chart2.text_style.foreground &&
            bar_chart1.text_style.background == bar_chart2.text_style.background &&
            bar_chart1.title_style.foreground == bar_chart2.title_style.foreground &&
            bar_chart1.title_style.background == bar_chart2.title_style.background
        );
    }

    // Exception handling
    class TUIException : public std::runtime_error {
        public:
            template<typename... Args>
            TUIException(Args... args) : std::runtime_error(args...){}
    };

    class Window {
        public:
            // Updates width, height, rows, and columns values
            void update_dimensions() {
                window_width = width();
                window_height = height();
                rows_ = rows();
                columns_ = columns();
            }

            // Draw border with given widget dimensions
            template<typename Widget>
            void draw_border(Widget widget) {
                short border_color = get_color(
                    widget.border_style.foreground, 
                    widget.border_style.background
                );
                for(int i = widget.y; i < widget.y + widget.height; i++) {
                    for(int j = widget.x; j < widget.x + widget.width; j++) {
                        if(
                            (i == widget.y && j == widget.x) || 
                            (i == widget.y && j == (widget.x + widget.width) - 1) ||
                            (i == (widget.y + widget.height) - 1 && j == widget.x) ||
                            (i == (widget.y + widget.height) - 1 && j == (widget.x + widget.width) - 1)) {
                            draw_char(j, i, '+', border_color);
                        } else if(i == widget.y || i == (widget.y + widget.height) - 1) {
                            draw_char(j, i, '-', border_color);
                        } else if(j == widget.x || j == (widget.x + widget.width) - 1) {
                            draw_char(j, i, '|', border_color);
                        }
                    }
                }
            }

            // Draw title
            template<typename Widget>
            void draw_title(Widget widget) {
                short title_color = get_color(
                    widget.title_style.foreground, 
                    widget.title_style.background
                );
                for(int i = widget.x + 2; i < widget.x + std::min(widget.width, (int)(widget.title.length()) + 2); i++) {
                    draw_char(i, widget.y, widget.title[i - (widget.x + 2)], title_color);
                }
            }

            // Add a widget to the window 
            // using recursive template function
            template<typename Widget, typename ... Rest>
            inline void add(Widget first, Rest... rest) {
                add(first);
                add(rest...);
            }

#ifdef IS_WIN
            Window(int window_width_ = 0, int window_height_ = 0) : window_width((LONG)window_width_), window_height((LONG)window_height_) {
                // Resize console to window_width and window_height
                console = GetConsoleWindow();
                // Get default dimensions
                default_width = width();
                default_height = height();
                // Check if no width and height was given
                if(window_width_ <= 0 || window_height_ < 0) {
                    // Window width or height given is 0 or less
                    // Use default dimensions
                    window_width = default_width;
                    window_height = default_height;
                }
                RECT r;
                GetWindowRect(console, &r);
                MoveWindow(console, r.left, r.top, window_width, window_height, TRUE);
                // Tidy up the window
                hide_cursor();
                remove_scrollbar();
                update_dimensions();
                // Allocate memory for content
                content = new CHAR_INFO[columns_ * rows_];
                memset(content, 0, sizeof(CHAR_INFO) * rows_ * columns_);
            }

            // Close the tui and revert to default settings
            void close() {
                clear();
                // Render the cleared content
                render();
                // Show the cursor
                show_cursor();
                RECT r;
                GetWindowRect(console, &r);
                MoveWindow(console, r.left, r.top, default_width, default_height, TRUE);
            }

            // Remove scrollbar from console
            void remove_scrollbar() {
                GetConsoleScreenBufferInfo(handle, &csbi);
                COORD new_size = {
                    (short)(csbi.srWindow.Right - csbi.srWindow.Left + 1),
                    (short)(csbi.srWindow.Bottom - csbi.srWindow.Top + 1)
                };
                SetConsoleScreenBufferSize(handle, new_size);
            }

            // Clear content
            inline void clear() { 
                // Set content to ' '
                memset(content, 0, sizeof(CHAR_INFO) * rows_ * columns_);
            }

            // Hide cursor from console
            void hide_cursor() {
                CONSOLE_CURSOR_INFO info;
                info.bVisible = FALSE;
                info.dwSize = 100;
                SetConsoleCursorInfo(handle, &info);
            }

            // Show cursor from console
            void show_cursor() {
                CONSOLE_CURSOR_INFO info;
                info.bVisible = TRUE;
                SetConsoleCursorInfo(handle, &info);
            }

            // Return width of window
            LONG width() {
                RECT r;
                GetWindowRect(console, &r);
                return r.right - r.left;
            }

            // Return height of window
            LONG height() {
                RECT r;
                GetWindowRect(console, &r);
                return r.bottom - r.top;
            }

            // Return number of columns
            short columns() {
                GetConsoleScreenBufferInfo(handle, &csbi);
                return csbi.srWindow.Right - csbi.srWindow.Left;
            }

            // Return number of rows
            short rows() {
                GetConsoleScreenBufferInfo(handle, &csbi);
                return csbi.srWindow.Bottom - csbi.srWindow.Top;
            }

            // Set window title
            void set_title(std::string str) {
                TCHAR new_title[MAX_PATH];
                new_title[str.size()] = 0;
                std::copy(str.begin(), str.end(), new_title);
                SetConsoleTitle(new_title);
            }

            // Set character in content
            void draw_char(int x, int y, char c, short color = 0x000F) {
                if(x >= 0 && x < columns_ && y >= 0 && y < rows_) {
                    content[y * columns_ + x].Char.AsciiChar = c;
                    content[y * columns_ + x].Attributes = color;
                }
            }

            // Render (print) content
            void render() {
                SMALL_RECT sr = {0, 0, (short)(columns_ - 1), (short)(rows_ - 1)};
                hide_cursor();
                remove_scrollbar();
                WriteConsoleOutput(handle, content, {columns_, rows_}, {0, 0}, &sr);
            }

            // Poll for event
            bool poll_event(Event &event) {
                event = Event{};
                for(uint8_t k = VK_LBUTTON; k <= VK_OEM_CLEAR; k++) {
                    if(GetKeyState(k) & 0x8000) {
                        if(k >= VK_LBUTTON && k <= VK_XBUTTON2 && k != VK_CANCEL) {
                            event.type = MOUSEBUTTONDOWN;
                        } else {
                            event.type = KEYDOWN;
                        }
                        event.key = tolower(k);
                        // Wait for key unpress
                        while(GetKeyState(k) & 0x8000);
                    }
                }
                if(event.type != UNDEFINED) {
                    // Event has been registered
                    return true;
                } else {
                    return false;
                }
            }

            // Return the content of the buffer
            inline CHAR_INFO * get_content() {
                return content;
            }
#elif defined(IS_POSIX)
            Window() {
                initscr();
                raw();
                keypad(stdscr, TRUE);
                noecho();
                start_color();
                hide_cursor();
                update_dimensions();
                timeout(1);
            }

            // Close the tui and revert to default settings
            void close() {
                show_cursor();
                endwin();
            }

            // Remove scrollbar from console (no op)
            inline void remove_scrollbar() { };

            // Clear content (no op)
            inline void clear() { };

            // Hide cursor from console
            inline void hide_cursor() {
                curs_set(0);
            }

            // Show cursor from console
            inline void show_cursor() {
                curs_set(1);
            }

            // Return width of window
            short width() {
                struct winsize size;
                ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
                return size.ws_xpixel;
            }

            // Return height of window
            short height() {
                struct winsize size;
                ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
                return size.ws_ypixel;
            }

            // Return number of columns
            short columns() {
                struct winsize size;
                ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
                return size.ws_col;
            }

            // Return number of rows
            short rows() {
                struct winsize size;
                ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
                return size.ws_row;
            }

            // Set window title (no op)
            inline void set_title(std::string str){ };

            // Set character in tui
            inline void draw_char(int x, int y, char c, short color = 0x000F) {
                // TODO: Add color functionality
                mvaddch(y, x, c);
            }

            // Render tui
            inline void render() {
                refresh();
            }

            // Poll for event
            bool poll_event(Event &event) {
                event.key = 0;
                event.type = UNDEFINED;
                int ch;
                ch = getch();
                if(ch != EOF) {
                    if(ch == KEY_MOUSE) {
                        return false;
                    } else {
                        event.type = KEYDOWN;
                        event.key = ch;
                    }
                    return true;
                }
                return false;
            }

            // Return the content of the buffer (no op)
            inline void get_content(){ };
#endif
        private:
#ifdef IS_WIN
            HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
            CONSOLE_SCREEN_BUFFER_INFO csbi;
            HWND console;
            LONG window_width; // Width of window
            LONG window_height; // Height of window
            short columns_;
            short rows_;
            CHAR_INFO *content; // Content to be rendered to buffer
            LONG default_width; // Width of the window before tui started
            LONG default_height; // Height of the window before tui started
#elif defined(IS_POSIX)
            short window_width;
            short window_height;
            short columns_;
            short rows_;
            int current_pair = 1;
#endif
    };

    // Widget add to window method definitions
    template<>
    void Window::add(Paragraph paragraph) {
        if(paragraph.border == true) {
            draw_border(paragraph);
        }
        if(paragraph.title.empty() == false) {
            draw_title(paragraph);
        }
        // Get color
        short text_color = get_color(
            paragraph.text_style.foreground, 
            paragraph.text_style.background
        );
        // Draw text
        int maximum_characters = (paragraph.width - 2) * (paragraph.height - 2);
        for(int i = paragraph.y + 1; i < paragraph.y + paragraph.height - 1; i++) {
            int current_row = i - (paragraph.y + 1);
            for(int j = paragraph.x + 1; j < paragraph.x + paragraph.width - 1; j++) {
                int current_column = j - (paragraph.x + 1);
                int current_index = (current_row * (paragraph.width - 2)) + current_column;
                if(current_index < paragraph.text.length()) {
                    draw_char(j, i, paragraph.text[current_index], text_color);
                } else {
                    // Paragraph text is shorter than maximum characters
                    break;
                }
            }
        }
        if(paragraph.text.length() > maximum_characters && paragraph.width - 2 >= 3) {
            // Draw ellipsis
            int last_row = (paragraph.y + paragraph.height) - 2;
            for(int i = 0; i < 3; i++) {
                int current_column = ((paragraph.x + paragraph.width) - 2) - i;
                draw_char(current_column, last_row, '.', text_color);
            }
        }
    }

    template<>
    void Window::add(List list) {
        if(list.border == true) {
            draw_border(list);
        }
        if(list.title.empty() == false) {
            draw_title(list);
        }
        // Get color
        short text_color = get_color(
            list.text_style.foreground, 
            list.text_style.background
        );
        for(int i = list.y + 1; i < list.y + list.height - 1; i++) {
            // Calculate current row with list's first element
            int current_row = list.first_element + (i - (list.y + 1));
            for(int j = list.x + 1; j < list.x + list.width - 1; j++) {
                int current_column = j - (list.x + 1);
                // Naively assume character is empty
                draw_char(j, i, ' ');
                if(current_row < list.rows.size() && current_column < list.rows[current_row].length()) {
                    if(list.rows[current_row].length() > list.width - 2 && j >= (list.x + list.width - 4)) {
                        // Draw ellipsis
                        if(list.width - 2 >= 3) {
                            // Only draw ellipsis if inner width is at least 3
                            draw_char(j, i, '.', text_color);
                        }
                    } else {
                        draw_char(j, i, list.rows[current_row][current_column], text_color);
                    }
                }
            }
        }
    }

    template<>
    void Window::add(BarChart bar_chart) {
        if(bar_chart.border == true) {
            draw_border(bar_chart);
        }
        if(bar_chart.title.empty() == false) {
            draw_title(bar_chart);
        }
        // Get colors
        short label_color = get_color(
            bar_chart.label_style.foreground, 
            bar_chart.label_style.background
        );
        short number_color = get_color(
            bar_chart.number_style.foreground, 
            bar_chart.number_style.background
        );
        // Get minimum and maximum values
        int minimum = INT_MAX;
        int maximum = INT_MIN;
        for(int i = 0; i < bar_chart.data.size(); i++) {
            if(bar_chart.data[i] < minimum) {
                minimum = bar_chart.data[i];
            }
            if(bar_chart.data[i] > maximum) {
                maximum = bar_chart.data[i];
            }
        }
        // Draw
        int current_bar = 0;
        int current_character = 0;
        for(int i = bar_chart.x + 1; i < bar_chart.x + bar_chart.width - 1; i += bar_chart.bar_width + 1) {
            if(current_bar < bar_chart.labels.size()) {
                for(int j = 0; j < bar_chart.bar_width; j++) {
                    if(j < bar_chart.labels[current_bar].size() && i + j < bar_chart.x + bar_chart.width - 1) {
                        draw_char(
                            (i + j), 
                            (bar_chart.y + bar_chart.height - 2),
                            bar_chart.labels[current_bar][j],
                            label_color
                        );
                    }
                }
            }
            if(current_bar < bar_chart.data.size()) {
                auto draw_numbers = [&]() {
                    for(int j = 0; j < bar_chart.bar_width; j++) {
                        std::string current_number = std::to_string(bar_chart.data[current_bar]);
                        if(j < current_number.length() && i + j < bar_chart.x + bar_chart.width - 1) {
                            draw_char(
                                (i + j),
                                (bar_chart.y + bar_chart.height - 3),
                                current_number[j],
                                number_color
                            );
                        }
                    }
                };

#ifdef IS_WIN
                // If defined IS_WIN, draw numbers before bars
                draw_numbers();
#endif
                float normalized = (floor)(bar_chart.data[current_bar]) / (maximum);
                int maximum_height = bar_chart.height - 3;
                int height = floor(normalized * maximum_height);
                for(int y = bar_chart.y + maximum_height - height + 2; y < bar_chart.y + maximum_height + 1; y++) {
                    for(int x = 0; x < bar_chart.bar_width; x++) {
#ifdef IS_WIN
                            // Skip draw char to avoid overriding char
                            content[y * columns_ + (i + x)].Attributes = get_color(
                                number_color,
                                bar_chart.bar_color
                            );
#elif defined(IS_POSIX)
                            draw_char(
                                (i + x),
                                y,
                                '#',
                                get_color(number_color, bar_chart.bar_color)
                            );
#endif
                    }
                }
#ifdef IS_POSIX
            // If defined IS_POSIX, draw numbers before bars
            draw_numbers();
#endif
            }
            current_bar++;
        }
    }

    // Widget set dimensions shortcut
    void Widget::set_dimensions(int x_, int y_, int width_, int height_) {
        x = x_;
        y = y_;
        width = width_;
        height = height_;
    }

    // Scroll up the list
    template<>
    void List::scroll_up(Window &window, int factor) {
        if(rows.size() > height - 2) {
            first_element = std::max(0, (int)(first_element - factor));
            window.add(*this);
        }
    }

    // Scroll down the list
    template<>
    void List::scroll_down(Window &window, int factor) {
        if(rows.size() > height - 2) {
            first_element = std::min((int)(rows.size() - (height - 2)), (first_element + factor));
            window.add(*this);
        }
    }
};
#endif