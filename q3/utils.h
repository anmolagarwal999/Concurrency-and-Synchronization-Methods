#ifndef UTILS_HEADER
#define UTILS_HEADER

int get_random_int(int lower, int upper);
int get_random_int2(int lower, int upper, char *seg_id);
struct timespec *get_abs_time_obj(int sec_can_wait);

#endif