#ifndef UTILS_HEADER
#define UTILS_HEADER

int get_random_int(int lower, int upper);
int get_random_int2(int lower, int upper, char *seg_id);
struct timespec *get_abs_time_obj(int sec_can_wait);


// void red_color();

// void yellow_color();

// void reset_color();

// void cyan_color();

// void green_color();

//https://stackoverflow.com/questions/3219393/stdlib-and-colored-output-in-c


// void green_printf(char *str);
#endif