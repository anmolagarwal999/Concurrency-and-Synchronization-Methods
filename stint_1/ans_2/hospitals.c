#include "master_header.h"

// struct hospital
// {
//     pthread_t thread_obj;

//     int id;
//     int thr_id;

//     int partner_company;
//     int tot_vaccines;
//     int left_vaccines;
//     int curr_slots;
//     int left_slots;
//     pthread_mutex_t mutex;
// };

void buy_batch_from_company(int id)
{
    //keep buying until all college students have been vaccinated
    while (tot_conclusions_left > 0)
    {
        int curr_check_id = 0;
        bool found_supplier = false;

        while (tot_conclusions_left)
        {
            found_supplier = false;
            curr_check_id = 0;
            while (!found_supplier)
            {

                //try locking, but if not available, then rather than waiting move on
                if (pthread_mutex_trylock(&(comp_ptr[curr_check_id]->mutex)) == EBUSY)
                {
                    //EBUSY  The mutex could not be acquired because it was already locked.
                    printf("Company with id %d is busy deciding transit changes, hence did not answer call\n");
                }
                else
                {
                    if (comp_ptr[curr_check_id]->left_batches_num > 0)
                    {
                        hosp_ptr[id]->tot_vaccines = comp_ptr[curr_check_id]->capacity_of_batches;
                        hosp_ptr[id]->left_vaccines = hosp_ptr[id]->tot_vaccines;
                        hosp_ptr[id]->partner_company = curr_check_id;
                        comp_ptr[curr_check_id]->left_batches_num--;
                        found_supplier = true;
                    }

                    pthread_mutex_unlock(&(comp_ptr[curr_check_id]->mutex));
                    curr_check_id = (curr_check_id + 1) % num_companies;
                }
            }

            //A batch has been bought from a company, now time to vaccinate

            vaccinate_students(id);
        }
    }
}

void vaccinate_students(int id)
{
    while (hosp_ptr[id]->left_vaccines > 0)
    {
        int curr_slots = get_random_int(1, (int)min(8, hosp_ptr[id]->left_vaccines));
        int filled_slots=0;
        int curr_stu_id=0;
        while(filled_slots<curr_slots && hopeful_students_num>0)
        {
            //start filling students
            
        }
    }
}

void *init_hospitals(void *ptr)
{
    int id = *((int *)ptr);
    hosp_ptr[id]->curr_slots = 0;
    hosp_ptr[id]->left_slots = 0;
    hosp_ptr[id]->left_vaccines = 0;
    hosp_ptr[id]->partner_company = -1;
    hosp_ptr[id]->tot_vaccines = 0;
    pthread_mutex_init(&(hosp_ptr[id]->mutex), NULL);

    buy_batch_from_company(id);
    return NULL;
}