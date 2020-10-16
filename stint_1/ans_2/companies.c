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
    while (true)
    {
        if (tot_conclusions_left >= num_students)
        {
            break;
        }

        int prep_time = get_random_int(2, 5);
        comp_ptr[id]->capacity_of_batches = get_random_int(10, 20);
        comp_ptr[id]->done_batches = 0;
        int num_batches = get_random_int(1, 5);

        printf(ANSI_YELLOW "Company %d\t is preparing %d batches of vaccines which have success probability %Lf" ANSI_RESET, comp_ptr[id]->id, num_batches, comp_ptr[id]->prob_of_success);

        sleep(prep_time);

        /* Lock needed as if curr_batches_num is set to a finite value 
        and if all stock gets over between the time `while` evaluates
         to true and the company sleeps, then company will never wake up*/
        pthread_mutex_lock(&(comp_ptr[id]->mutex));

        comp_ptr[id]->curr_batches_num = num_batches;
        comp_ptr[id]->left_batches_num = comp_ptr[id]->curr_batches_num;
        printf(ANSI_YELLOW "Company %d\t HAS PREPARED %d batches of vaccines which have success probability %Lf" ANSI_RESET, comp_ptr[id]->id, num_batches, comp_ptr[id]->prob_of_success);

        dispatch_stock(id);
    }
}

void dispatch_stock(int id)
{

    //`if` should suffice I think, but still
    while (comp_ptr[id]->done_batches < comp_ptr[id]->curr_batches_num)
    {
        pthread_cond_wait(&(comp_ptr[id]->cv), &(comp_ptr[id]->mutex));
        if (tot_conclusions_left >= num_students)
        {
            break;
        }
    }
    pthread_mutex_unlock(&(comp_ptr[id]->mutex));

    //Reached here -> all of previous dispatch has been consumed-> time to manufacture again
    printf(ANSI_YELLOW"Company %d\t All the vaccines prepared are over. Resuming production now."ANSI_RESET, comp_ptr[id]->id);
}

void *init_company(void *ptr)
{
    // https://users.cs.cf.ac.uk/Dave.Marshall/C/node31.html#SECTION003120000000000000000
    int id = *((int *)ptr);
    printf(ANSI_RED "Pharma Company %d\t HAS entered the simulation\n" ANSI_RESET, id);
    comp_ptr[id]->capacity_of_batches = 0;
    comp_ptr[id]->curr_batches_num = 0;
    comp_ptr[id]->left_batches_num = 0;
    pthread_mutex_init(&(comp_ptr[id]->mutex), NULL);
    int ret = pthread_cond_init(&(comp_ptr[id]->cv), NULL);

    prep_stock(id);

    return NULL;
}