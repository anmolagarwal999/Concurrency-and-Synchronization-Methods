#include "master_header.h"

void take_input()
{
    printf(ANSI_RED "This text is RED!" ANSI_RESET "\n");
    printf(ANSI_GREEN "This text is GREEN!" ANSI_RESET "\n");
    printf(ANSI_YELLOW "This text is YELLOW!" ANSI_RESET "\n");
    printf(ANSI_BLUE "This text is BLUE!" ANSI_RESET "\n");
    printf(ANSI_MAGENTA "This text is MAGENTA!" ANSI_RESET "\n");
    printf(ANSI_CYAN "This text is CYAN!" ANSI_RESET "\n");

}

int main()
{
    // int i;
    //srand(time(0));
    take_input();

    printf(ANSI_MAGENTA "----Finished----\nSimulation Over\n" ANSI_RESET);
    fflush(stdout);
}