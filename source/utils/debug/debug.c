#include <utils/debug/debug.h>

void _cdbg_eprint(int64_t line, const char* function, const char* etype, const char* fmt, ...) {

    va_list list;
    va_start(list, fmt);
    fprintf(stderr, "%s:%ld [%s]: ", function, line, etype);
    vfprintf(stderr, fmt, list);
    printf("\n");
    va_end(list);

}