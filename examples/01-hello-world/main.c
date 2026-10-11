#include <core.h>

void startup(err_t* err, int32_t argc, const char** argv) {}

void update(err_t* err) {

    throw(err, CORE_EXIT);

}

void shutdown(err_t* err) {}