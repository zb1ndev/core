#include <utils/debug/debug.h>

internal const char* ERROR_MESSAGES[] = {
    "Success",
    "Exit Successfully",
    "Invalid Parameters",
    "Max Window Count",
    "Failed to Create Surface",
    "XDG Failed to Get Surface",
    "XDG Failed to Add Listener to Surface",
    "XDG Failed to Get Top-Level",
    "XDG Failed to Add Listener to Top-Level",
    "Failed to do a Display Round Trip",
    "Window Not Read / Initialized",
    "Display Connection Refused",
    "Display Registry Not Found",
    "Display Registry Failed to Add Listener",
    "Display Dispatch Failure",
    "Display Flush Failure",
    "No EGL Display Found",
    "EGL Failed to Initialize",
    "EGL Failed to Bind GAPI",
    "EGL Failed to Load Config",
    "EGL Failed to Create Context",
    "EGL Failed to Make Context Current",
    "GLAD Failed to Load",
    "EGL Failed to Create Window",
    "EGL Failed to Create Surface",
};

void _cdbg_eprint(int64_t line, const char* function, const char* etype, const char* fmt, ...) {

    va_list list;
    va_start(list, fmt);
    fprintf(stderr, "%s:%ld [%s]: ", function, line, etype);
    vfprintf(stderr, fmt, list);
    printf("\n");
    va_end(list);

}

inline const char* _cdbg_get_errmsg(err_t code) {

    return ERROR_MESSAGES[code];

}