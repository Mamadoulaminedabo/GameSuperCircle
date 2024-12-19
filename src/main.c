// main.c
#include "application.h"
#include "sdl2.h"
#include <stdio.h>

int main() {
    struct application *application = application_initialize();
    if (application != NULL) {
        application_run(application);
    } else {
        fprintf(stderr, "Failed to initialize the application...");
        return -1;
    }
    application_shut_down(application);
}
