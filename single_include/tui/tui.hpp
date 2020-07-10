#ifndef TUI_HPP
#define TUI_HPP

#ifdef _WIN32
#include <stdexcept>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <windows.h>
namespace tui {
    // Widget definitions
    struct Paragraph {
        std::string text;
        int x;      // Position of left side of paragraph
        int y;      // Position of top of paragraph
        int width;  // Width of paragraph
        int height; // Height of paragraph
        bool border = true;
    };

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
                TCHAR new_title[str.size()+1];
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

            // Render (print) content
            void render() {
                update_dimensions();
                SMALL_RECT sr = {0, 0, (short)(columns_ - 1), (short)(rows_ - 1)};
                WriteConsoleOutput(handle, content, {columns_, rows_}, {0, 0}, &sr);
            }

            // Add a widget to the window 
            // using recursive template function
            template<typename Widget, typename ... Rest>
            void add(Widget first, Rest... rest) {
                add(first);
                add(rest...);
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
            // Draw border
            for(int i = paragraph.y; i < paragraph.y + paragraph.height; i++) {
                for(int j = paragraph.x; j < paragraph.x + paragraph.width; j++) {
                    if(
                        (i == paragraph.y && j == paragraph.x) || 
                        (i == paragraph.y && j == (paragraph.x + paragraph.width) - 1) ||
                        (i == (paragraph.y + paragraph.height) - 1 && j == paragraph.x) ||
                        (i == (paragraph.y + paragraph.height) - 1 && j == (paragraph.x + paragraph.width) - 1)) {
                        draw_char(j, i, '+');
                    } else if(i == paragraph.y || i == (paragraph.y + paragraph.height) - 1) {
                        draw_char(j, i, '-');
                    } else if(j == paragraph.x || j == (paragraph.x + paragraph.width) - 1) {
                        draw_char(j, i, '|');
                    }
                }
            }
        }
        // Draw text
        int maximum_characters = (paragraph.width - 2) * (paragraph.height - 2);
        for(int i = paragraph.y + 1; i < paragraph.y + paragraph.height - 1; i++) {
            int current_row = i - (paragraph.y + 1);
            for(int j = paragraph.x + 1; j < paragraph.x + paragraph.width - 1; j++) {
                int current_column = j - (paragraph.x + 1);
                int current_index = (current_row * (paragraph.width - 2)) + current_column;
                if(current_index < paragraph.text.length()) {
                    draw_char(j, i, paragraph.text[current_index]);
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
                draw_char(current_column, last_row, '.');
            }
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