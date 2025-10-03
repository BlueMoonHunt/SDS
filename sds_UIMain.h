#ifdef SDS_USE_INTERNAL_MAIN
#include "sds_UI.h"

int main(void) {

    void* appState = 0;

    AppResult result = app_init(appState);
    while (result == APP_CONTINUE) {

        result = os_process_events();

        if (result == APP_CONTINUE)
            result = app_update(appState);
    }
    app_quit(appState);
    return 0;
}
#endif