#ifndef MASTER_HEADER
#define MASTER_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include "global_vars.h"
#include "companies.h"
#include "hospitals.h"
#include "students.h"

typedef long long LL;
#define part fprintf(stderr,"-----------------------------------------\n");
#define part2 fprintf(stderr,"====================================================\n");
#define part3 fprintf(stderr,"############################################################\n");
#define debug(x) printf("\n\"%s\" is: %d\n",#x,x);


#endif