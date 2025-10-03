#ifndef SDS_UI_IMPLEMENTATION
#define SDS_UI_IMPLEMENTATION
#include "sds.h"
#include "stdbool.h"

typedef enum AppResult AppResult;
enum AppResult {
    APP_SUCCESS = 0,
    APP_CONTINUE,
    APP_FAILURE
};

typedef enum EventType EventType;
enum EventType {
    EVENT_NONE = 0,
    EVENT_WINDOW_RESIZE,
    EVENT_WINDOW_MAXIMIZED,
    EVENT_WINDOW_FULLSCREEN,
    EVENT_WINDOW_MINIMIZED,
    EVENT_WINDOW_CLOSE,
    EVENT_KEY_PRESSED,
    EVENT_KEY_RELEASED,
    EVENT_KEY_REPEAT,
    EVENT_MOUSE_BUTTON_PRESSED,
    EVENT_MOUSE_BUTTON_RELEASED,
    EVENT_MOUSE_BUTTON_REPEAT,
    EVENT_MOUSE_MOVE,
    EVENT_MOUSE_SCROLL,
};

typedef enum Key Key;
enum Key {
    KEY_UNKNOWN = 0,

    // --- Alphanumeric Keys ---
    KEY_A = 0x04,
    KEY_B = 0x05,
    KEY_C = 0x06,
    KEY_D = 0x07,
    KEY_E = 0x08,
    KEY_F = 0x09,
    KEY_G = 0x0A,
    KEY_H = 0x0B,
    KEY_I = 0x0C,
    KEY_J = 0x0D,
    KEY_K = 0x0E,
    KEY_L = 0x0F,
    KEY_M = 0x10,
    KEY_N = 0x11,
    KEY_O = 0x12,
    KEY_P = 0x13,
    KEY_Q = 0x14,
    KEY_R = 0x15,
    KEY_S = 0x16,
    KEY_T = 0x17,
    KEY_U = 0x18,
    KEY_V = 0x19,
    KEY_W = 0x1A,
    KEY_X = 0x1B,
    KEY_Y = 0x1C,
    KEY_Z = 0x1D,

    // --- Number Row ---
    KEY_1 = 0x1E,
    KEY_2 = 0x1F,
    KEY_3 = 0x20,
    KEY_4 = 0x21,
    KEY_5 = 0x22,
    KEY_6 = 0x23,
    KEY_7 = 0x24,
    KEY_8 = 0x25,
    KEY_9 = 0x26,
    KEY_0 = 0x27,

    // --- Control & Whitespace Keys ---
    KEY_ENTER = 0x28,
    KEY_ESCAPE = 0x29,
    KEY_BACKSPACE = 0x2A,
    KEY_TAB = 0x2B,
    KEY_SPACE = 0x2C,

    // --- Symbol Keys ---
    KEY_MINUS = 0x2D,          // - and _
    KEY_EQUALS = 0x2E,         // = and +
    KEY_LEFT_BRACKET = 0x2F,   // [ and {
    KEY_RIGHT_BRACKET = 0x30,  // ] and }
    KEY_BACKSLASH = 0x31,      // \ and |
    KEY_SEMICOLON = 0x33,      // ; and :
    KEY_QUOTE = 0x34,          // ' and "
    KEY_GRAVE = 0x35,          // ` and ~
    KEY_COMMA = 0x36,          // , and <
    KEY_PERIOD = 0x37,         // . and >
    KEY_SLASH = 0x38,          // / and ?

    // --- Lock Keys ---
    KEY_CAPS_LOCK = 0x39,
    KEY_SCROLL_LOCK = 0x47,
    KEY_NUM_LOCK = 0x53,

    // --- Function Keys ---
    KEY_F1 = 0x3A,
    KEY_F2 = 0x3B,
    KEY_F3 = 0x3C,
    KEY_F4 = 0x3D,
    KEY_F5 = 0x3E,
    KEY_F6 = 0x3F,
    KEY_F7 = 0x40,
    KEY_F8 = 0x41,
    KEY_F9 = 0x42,
    KEY_F10 = 0x43,
    KEY_F11 = 0x44,
    KEY_F12 = 0x45,

    // --- System & Navigation Keys ---
    KEY_PRINT_SCREEN = 0x46,
    KEY_PAUSE = 0x48,
    KEY_INSERT = 0x49,
    KEY_HOME = 0x4A,
    KEY_PAGE_UP = 0x4B,
    KEY_DELETE = 0x4C,
    KEY_END = 0x4D,
    KEY_PAGE_DOWN = 0x4E,

    // --- Arrow Keys ---
    KEY_RIGHT = 0x4F,
    KEY_LEFT = 0x50,
    KEY_DOWN = 0x51,
    KEY_UP = 0x52,

    // --- Keypad ---
    KEY_NUMPAD_DIVIDE = 0x54,
    KEY_NUMPAD_MULTIPLY = 0x55,
    KEY_NUMPAD_SUBTRACT = 0x56,
    KEY_NUMPAD_ADD = 0x57,
    KEY_NUMPAD_ENTER = 0x58,
    KEY_NUMPAD_1 = 0x59,
    KEY_NUMPAD_2 = 0x5A,
    KEY_NUMPAD_3 = 0x5B,
    KEY_NUMPAD_4 = 0x5C,
    KEY_NUMPAD_5 = 0x5D,
    KEY_NUMPAD_6 = 0x5E,
    KEY_NUMPAD_7 = 0x5F,
    KEY_NUMPAD_8 = 0x60,
    KEY_NUMPAD_9 = 0x61,
    KEY_NUMPAD_0 = 0x62,
    KEY_NUMPAD_PERIOD = 0x63,

    // --- Other ---
    KEY_MENU = 0x65, // The Application key

    // --- Modifier Keys ---
    KEY_LEFT_CTRL = 0xE0,
    KEY_LEFT_SHIFT = 0xE1,
    KEY_LEFT_ALT = 0xE2,
    KEY_LEFT_GUI = 0xE3,    // Windows / Command / Super key
    KEY_RIGHT_CTRL = 0xE4,
    KEY_RIGHT_SHIFT = 0xE5,
    KEY_RIGHT_ALT = 0xE6,
    KEY_RIGHT_GUI = 0xE7,   // Windows / Command / Super key
};

typedef enum MouseButton MouseButton;
enum MouseButton {
    MOUSE_BUTTON_UNKNOWN = 0,
    MOUSE_BUTTON_LEFT = 1,    // Primary button
    MOUSE_BUTTON_RIGHT = 2,   // Secondary button
    MOUSE_BUTTON_MIDDLE = 3,  // Middle-click / wheel-click
    MOUSE_BUTTON_4 = 4,       // Typically the 'Back' side button
    MOUSE_BUTTON_5 = 5,       // Typically the 'Forward' side button
    MOUSE_BUTTON_6 = 6,
    MOUSE_BUTTON_7 = 7,
    MOUSE_BUTTON_8 = 8,
};

typedef struct WindowEvent WindowEvent;
struct WindowEvent {
    int x, y;
};

typedef struct InputEvent InputEvent;
struct InputEvent {
    union {
        struct { Key key; bool isRepeat; } keyboard;
        struct { MouseButton button; } mouse;
    };
};

typedef struct Cursor Cursor;
struct Cursor {
    int x, y;
};

typedef struct ScrollWheel ScrollWheel;
struct ScrollWheel {
    int delta_x, delta_y;
};

typedef struct Event Event;
struct Event {
    EventType type;

    union {
        WindowEvent window;
        InputEvent input;
        Cursor cursor;
        ScrollWheel scrollWheel;
    };
};


typedef struct Window Window;
typedef struct Arena Arena;

extern AppResult app_init(void* appState);                   // To be defined by the user.
extern AppResult app_update(void* appState);                 // To be defined by the user.
extern AppResult app_event(void* appState, Event* event);    // To be defined by the user.
extern void app_quit(void* appState);                        // To be defined by the user.

extern Window* window_create(const wchar_t* name, i32vec2 size, void* appstate, Arena* arena);

extern AppResult os_process_events();

#endif // SDS_UI_IMPLEMENTATION