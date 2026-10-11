#include "core.h"

int32_t main(int32_t argc, const char** argv) {

    err_t err = CORE_SUCCESS;
    catch(&err, startup(&err, argc, argv), 
        return 1;
    );
    
    while (true) catch(&err, update(&err),
        goto exit;
    );

exit:

    catch(&err, shutdown(&err), {});
    return (err == CORE_SUCCESS || err == CORE_EXIT)
        ? 0 
        : 1;
    
}