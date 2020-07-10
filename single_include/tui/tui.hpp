#ifndef TUI_HPP
#define TUI_HPP

#ifdef _WIN32
#define NOMINMAX
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <windows.h>
namespace tui {
    // Color handling
    enum Color {
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

    // Return color after bitwise operation as SHORT
    // Combines foreground and background
    SHORT get_color(SHORT foreground, SHORT background) {
        return (foreground | (background << 4));
    }

    // Event handling
    enum EventType {
        KEYDOWN,
        MOUSEBUTTONDOWN,
        UNDEFINED
    };

    struct Event {
        EventType type = UNDEFINED;
        uint8_t key;
    };

    // Widget definitions
    struct Widget {
        struct {
            SHORT foreground = WHITE;
            SHORT background = BLACK;
        } border_style;
        struct {
            SHORT foreground = WHITE;
            SHORT background = BLACK;
        } text_style;
        struct {
            SHORT foreground = WHITE;
            SHORT background = BLACK;
        } title_style;
    };

    struct Paragraph : Widget {
        std::string title;
        std::string text;
        int x;      // Position of left side of paragraph
        int y;      // Position of top of paragraph
        int width;  // Width of paragraph
        int height; // Height of paragraph
        bool border = true;
    };

    struct List : Widget{
        std::string title;
        std::vector<std::string> rows;
        int x;      // Position of left side of list
        int y;      // Position of top of list
        int width;  // Width of list
        int height; // Height of list
        int first_element = 0; // Element at the top of the list
        bool border = true;

        template<typename Window>
        void scroll_up(Window &window, int factor = 1);
        template<typename Window>
        void scroll_down(Window &window, int factor = 1);
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
            paragraph1.border == paragraph2.border
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
            list1.border == list2.border
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
            // Instantiate window width 
            Window(int window_width_, int window_height_) : window_width((LONG)window_width_), window_height((LONG)window_height_) {
                // Clear the screen first
                clear();
                if(window_width_ <= 0 || window_height_ < 0) {
                    // Window width or height given is 0 or less
                    throw TUIException("Invalid window dimensions");
                }
                // Resize console to window_width and window_height
                console = GetConsoleWindow();
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

            // Updates width, height, rows, and columns values
            void update_dimensions() {
                window_width = width();
                window_height = height();
                rows_ = rows();
                columns_ = columns();
            }

            // Remove scrollbar from console
            void remove_scrollbar() {
                GetConsoleScreenBufferInfo(handle, &csbi);
                COORD new_size = {
                    (SHORT)(csbi.srWindow.Right - csbi.srWindow.Left + 1),
                    (SHORT)(csbi.srWindow.Bottom - csbi.srWindow.Top + 1)
                };
                SetConsoleScreenBufferSize(handle, new_size);
            }

            // Clear window
            void clear(char fill = ' ') { 
                COORD tl = {0,0};
                GetConsoleScreenBufferInfo(console, &csbi);
                DWORD written, cells = csbi.dwSize.X * csbi.dwSize.Y;
                FillConsoleOutputCharacter(console, fill, cells, tl, &written);
                FillConsoleOutputAttribute(console, csbi.wAttributes, cells, tl, &written);
            }

            // Hide cursor from console
            void hide_cursor() {
                CONSOLE_CURSOR_INFO info;
                info.dwSize = 100;
                info.bVisible = FALSE;
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
            SHORT columns() {
                GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
                return csbi.srWindow.Right - csbi.srWindow.Left;
            }

            // Return number of rows
            SHORT rows() {
                GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
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
            void draw_char(int x, int y, char c, SHORT color = 0x000F) {
                if(x >= 0 && x < columns_ && y >= 0 && y < rows_) {
                    content[y * columns_ + x].Char.AsciiChar = c;
                    content[y * columns_ + x].Attributes = color;
                }
            }

            // Draw border with given widget dimensions
            template<typename Widget>
            void draw_border(Widget widget) {
                SHORT border_color = get_color(
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
                SHORT title_color = get_color(
                    widget.title_style.foreground, 
                    widget.title_style.background
                );
                for(int i = widget.x + 2; i < widget.x + std::min(widget.width, (int)(widget.title.length()) + 2); i++) {
                    draw_char(i, widget.y, widget.title[i - (widget.x + 2)], title_color);
                }
            }

            // Render (print) content
            void render() {
                SMALL_RECT sr = {0, 0, (short)(columns_ - 1), (short)(rows_ - 1)};
                clear();
                hide_cursor();
                remove_scrollbar();
                WriteConsoleOutput(handle, content, {columns_, rows_}, {0, 0}, &sr);
            }

            // Add a widget to the window 
            // using recursive template function
            template<typename Widget, typename ... Rest>
            void add(Widget first, Rest... rest) {
                add(first);
                add(rest...);
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
                        event.key = k;
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

        private:
            HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
            CONSOLE_SCREEN_BUFFER_INFO csbi;
            HWND console;
            LONG window_width; // Width of window
            LONG window_height; // Height of window
            SHORT columns_;
            SHORT rows_;
            CHAR_INFO *content; // Content to be rendered to buffer
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
        SHORT text_color = get_color(
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
        if(paragraph.text.length() > maximum_characters) {
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
        SHORT text_color = get_color(
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
                        draw_char(j, i, '.', text_color);
                    } else {
                        draw_char(j, i, list.rows[current_row][current_column], text_color);
                    }
                } else {
                    // No more elements to print
                    break;
                }
            }
        }
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
#elif __linux__
// LINUX INCLUDES
#else
#if (defined (__APPLE__) && defined (__MACH__))
// APPLE INCLUDES
#else
    #error PLATFORM NOT SUPPORTED

#endif
#endif
#endif