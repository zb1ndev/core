#include "window.h"

internal struct internal_window_t {

    cvec2u_t            size;
    cvec2f_t            sizef;

    const char*         title;

} windows[CORE_CWINDOW_MAX] = {0};
internal cwindow_t window_count = 0;

/*
    The Plan is to keep the rendering code and 
    data completely seperate from the windowing 
    system in the case I want to either add more
    graphics apis or do software rendering.
*/