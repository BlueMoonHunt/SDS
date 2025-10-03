#include "sds_UI.h"
#ifndef _WIN32
struct Window {

};
#else
#include <Windows.h>
struct Window {
    HWND handle;
    HINSTANCE instance;
};

LRESULT CALLBACK MainWindowCallback(HWND window, UINT message, WPARAM WParam, LPARAM LParam) {
    void* appState = (void*)GetWindowLongPtr(window, GWLP_USERDATA);
    Event event = { 0 };

    switch (message) {
    case WM_CLOSE: {
        event.type = EVENT_WINDOW_CLOSE;
        app_event(appState, &event);
    }break;

    case WM_DESTROY: {
        PostQuitMessage(0);
    } break;

    case WM_SIZE: {
        event.type = EVENT_WINDOW_RESIZE;
        event.window.x = LOWORD(LParam);  // New width
        event.window.y = HIWORD(LParam); // New height
        app_event(appState, &event);
    } break;

    case WM_KEYDOWN:
    case WM_SYSKEYDOWN: {
        event.type = EVENT_KEY_PRESSED;
        event.input.keyboard.key = (int)WParam;
        event.input.keyboard.isRepeat = (LParam & (1 << 30)) != 0;
        app_event(appState, &event);
    } break;

    case WM_KEYUP:
    case WM_SYSKEYUP: {
        event.type = EVENT_KEY_RELEASED;
        event.input.keyboard.key = (int)WParam;
        event.input.keyboard.isRepeat = 0;
        app_event(appState, &event);
    } break;
    }

    return DefWindowProc(window, message, WParam, LParam);
}

Window* window_create(const wchar_t* name, i32vec2 size, void* appstate, Arena* arena) {
    Window* window = (Window*)arena_alloc(arena, sizeof(Window), alignof(struct Window));
    window->instance = GetModuleHandle(0);

    WNDCLASSEX windowClass = { 0 };
    windowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = MainWindowCallback;
    windowClass.hInstance = window->instance;
    windowClass.cbSize = sizeof(WNDCLASSEX);
    // windowClass.hIcon = ;
    windowClass.lpszClassName = L"KnellClass";

    if (RegisterClassEx(&windowClass)) {
        window->handle = CreateWindowEx(
            0,
            windowClass.lpszClassName,
            name,
            WS_OVERLAPPEDWINDOW | WS_VISIBLE,
            CW_USEDEFAULT, CW_USEDEFAULT,
            CW_USEDEFAULT, CW_USEDEFAULT,
            0,
            0,
            window->instance,
            0
        );
        if (window->handle != NULL)
            SetWindowLongPtr(window->handle, GWLP_USERDATA, (LONG_PTR)appstate);
    }
    return window;
}

AppResult os_process_events() {
    MSG message;
    while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
        if (message.message == WM_QUIT)
            return APP_SUCCESS;

        TranslateMessage(&message);
        DispatchMessage(&message);
    }
    return APP_CONTINUE;
}

#endif