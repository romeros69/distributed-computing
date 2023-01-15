
#include "my_lamport.h"


timestamp_t my_get_lamport_time(global* gl) {
    gl->time_now = gl->time_now + 1;
    return gl->time_now;
}
