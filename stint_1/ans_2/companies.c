#include "master_header.h"

// struct company
// {
//     pthread_t thread_obj;
//     int id;
//     int thr_id;
//     int curr_batches_num;
//     int left_batches_num;
//     int done_batches;
//     int capacity_of_batches;
//     long double prob_of_success;
//     pthread_mutex_t mutex;
//     pthread_cond_t cv;

// };

void prep_stock(int id)
{
    printf("inside prep stock function\n");
    while (true)
    {
        if (tot_conclusions_left==0)
        {
            printf("BREAKING\n");
            break;
        }

        // int prep_time = get_random_int(2, 5);
        int prep_time = get_random_int2(2, 5,"in prep stock");
        // comp_ptr[id]->capacity_of_batches = get_random_int(10, 20);
        comp_ptr[id]->capacity_of_batches = get_random_int2(2, 5,"in prep stock-> 2");
        comp_ptr[id]->done_batches = 0;
        // int num_batches = get_random_int(1, 5);
        int num_batches = get_random_int2(2, 5,"in prep stock-> 3");
        printf(ANSI_YELLOW "Company %d\t is preparing %d batches of vaccines which have success probability %Lf\n" ANSI_RESET, comp_ptr[id]->id, num_batches, comp_ptr[id]->prob_of_success);

        sleep(prep_time);

        /* Lock needed as if curr_batches_num is set to a finite value 
        and if all stock gets over between the time `while` evaluates
         to true and the company sleeps, then company will never wake up*/
        pthread_mutex_lock(&(comp_ptr[id]->mutex));

        comp_ptr[id]->curr_batches_num = num_batches;
        comp_ptr[id]->left_batches_num = comp_ptr[id]->curr_batches_num;
        printf(ANSI_YELLOW "Company %d\t HAS PREPARED %d batches of vaccines which have success probability %Lf\n" ANSI_RESET, comp_ptr[id]->id, num_batches, comp_ptr[id]->prob_of_success);

        dispatch_stock(id);
    }
}

void dispatch_stock(int id)
{

    //`if` should suffice I think, but still
    int flag = 0;
    while (comp_ptr[id]->done_batches < comp_ptr[id]->curr_batches_num)
    {
        printf(ANSI_MAGENTA"Company with id %d is SLEEP \n"ANSI_RESET,id);
        pthread_cond_wait(&(comp_ptr[id]->cv), &(comp_ptr[id]->mutex));
        printf("I HAVE COME OUT OF CONDITIONAL WAIT\n");
        fflush(stdout);
        if (tot_conclusions_left <= 0)
        {
            printf(ANSI_CYAN "All students' have been tended to\n" ANSI_RESET);
            flag = 1;
            break;
        }
    }
    pthread_mutex_unlock(&(comp_ptr[id]->mutex));

    //Reached here -> all of previous dispatch has been consumed-> time to manufacture again
    if (flag != 1)
        printf(ANSI_YELLOW "Company %d\t All the vaccines prepared are over. Resuming production now.\n" ANSI_RESET, comp_ptr[id]->id);
}

void *init_company(void *ptr)
{
    // https://users.cs.cf.ac.uk/Dave.Marshall/C/node31.html#SECTION003120000000000000000
    int id = *((int *)ptr);
    printf(ANSI_YELLOW "Pharma Company %d\t HAS entered the simulation\n" ANSI_RESET, id);
    comp_ptr[id]->capacity_of_batches = 0;
    comp_ptr[id]->curr_batches_num = 0;
    comp_ptr[id]->left_batches_num = 0;
 
    prep_stock(id);

    return NULL;
}