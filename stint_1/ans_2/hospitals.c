#include "master_header.h"

// struct hospital
// {
//
//     int partner_company; //comapany whose batch currently using
//     int tot_vaccines;    //tot vaccines stocked
//     int left_vaccines;   //vaccines left in current stock
//     int curr_slots;      //current number of doctors present to administer the vaccine
//     int left_slots;      //doctors waiting for patient
//     pthread_mutex_t mutex;
//     int curr_served_students[10]; //ids of students part of current vaccination phase
// };

int get_min(int a, int b)
{
    if (a < b)
        return a;
    return b;
}
void buy_batch_from_company(int id)
{
    //keep buying until all college students have reached verdicts
    while (tot_conclusions_left > 0)
    {
        int curr_check_id = 0;
        bool found_supplier = false;

        while (tot_conclusions_left > 0)
        {
            found_supplier = false;
            curr_check_id = 0;
            while (!found_supplier)
            {
                // printf("trying to find supplier\n");
                //try locking, but if not available, then rather than waiting, move on
                // pthread_mutex_lock(&(comp_ptr[curr_check_id]->mutex));
                // if company has an unsold batch left
                //try locking, but if not available, then rather than waiting, move on
                // if (pthread_mutex_trylock(&(comp_ptr[curr_check_id]->mutex)) == EBUSY)

                if (pthread_mutex_trylock(&(comp_ptr[curr_check_id]->mutex)) != 0)
                {
                    //EBUSY  The mutex could not be acquired because it was already locked.
                    printf("Company with id %d is busy deciding transit changes, hence did not answer call from centre %d\n", curr_check_id, id);
                }
                else
                {
                    //company mutex acquired
                    if (comp_ptr[curr_check_id]->left_batches_num > 0)
                    {
                        printf(ANSI_YELLOW "Company %d\t is delivering a vaccine batch to Vaccination Zone %d which has success prob:%Lf\n" ANSI_RESET, curr_check_id, id, comp_ptr[curr_check_id]->prob_of_success);

                        hosp_ptr[id]->tot_vaccines = comp_ptr[curr_check_id]->capacity_of_batches;
                        hosp_ptr[id]->left_vaccines = hosp_ptr[id]->tot_vaccines;
                        hosp_ptr[id]->partner_company = curr_check_id;
                        comp_ptr[curr_check_id]->left_batches_num--;
                        found_supplier = true;
                    }

                    pthread_mutex_unlock(&(comp_ptr[curr_check_id]->mutex));
                }
                curr_check_id = (curr_check_id + 1) % num_companies;

                //I don't need to search for a company anymore as all studetns have reached verdicts
                if (tot_conclusions_left <= 0)
                {
                    break;
                }
            }
            if (tot_conclusions_left <= 0)
            {
                break;
            }

            //A batch has been bought from a company, now time to vaccinate

            invite_students(id);
        }
    }
}

void invite_students(int id)
{

    //on entering this function, hospital obviously has some number of new vaccines
    while (hosp_ptr[id]->left_vaccines > 0 && tot_conclusions_left > 0)
    {
        // debug(tot_conclusions_left);
        //No need to acquire mutex for hopeful_students as you are just reading the value and you don't mind the value being changed
        int up_lim = (int)get_min(get_min(8, hosp_ptr[id]->left_vaccines), hopeful_students_num);

        //may happen due to hopeful students decreasing
        if (up_lim <= 0)
        {
            up_lim = 2;
        }
        // int curr_slots = get_random_int(1, up_lim);
        int curr_slots = get_random_int2(1, up_lim, "in invite students");
        int filled_slots = 0;
        int curr_stu_id = 0;
        for (int j = 0; j < 10; j++)
        {
            hosp_ptr[id]->curr_served_students[j] = -1;
        }
        while (filled_slots < curr_slots && tot_conclusions_left > 0)
        {

            //break only if you have some students to vaccinate and no hopeful students are left
            if (hopeful_students_num <= 0 && filled_slots > 0)
            {
                break;
            }
            //start filling students
            //try locking, but if not available, then rather than waiting, move on
            if (pthread_mutex_trylock(&(stu_ptr[curr_stu_id]->mutex)) != 0)
            {
                //EBUSY  The mutex could not be acquired because it was already locked.
                printf("Student id is busy as is currently in contact with another hospital, hence did not answer call\n");
            }
            else
            {
                // if student's current status is waiting
                if (stu_ptr[curr_stu_id]->curr_stat == 0)
                {
                    stu_ptr[curr_stu_id]->curr_stat = 1; //ongoing
                    printf(ANSI_GREEN "Student %d   assigned a slot on the Vaccination Zone %d and waiting to be vaccinated\ns" ANSI_RESET, curr_stu_id, id);
                    fflush(stdout);

                    pthread_mutex_lock(&hopeful_mutex);
                    //Student is no longer needs to hopeful
                    hopeful_students_num--;
                    pthread_mutex_unlock(&hopeful_mutex);

                    hosp_ptr[id]->curr_served_students[filled_slots] = curr_stu_id;
                    filled_slots++;
                    stu_ptr[curr_stu_id]->vaccine_comp_id = hosp_ptr[id]->partner_company;
                }

                pthread_mutex_unlock(&(stu_ptr[curr_stu_id]->mutex));
            }
            curr_stu_id = (curr_stu_id + 1) % num_students;
        }

        //either all slots have been filled or no students were currently waiting

        //LOCK DOEs not seem ro be required for this though
        pthread_mutex_lock(&(hosp_ptr[id]->mutex));
        hosp_ptr[id]->left_vaccines -= filled_slots;
        pthread_mutex_unlock(&(hosp_ptr[id]->mutex));

        vaccinate_students(id, filled_slots);
    }

    int comp_partner_id = hosp_ptr[id]->partner_company;
    if (hosp_ptr[id]->left_vaccines == 0)
    {
        pthread_mutex_lock(&(comp_ptr[comp_partner_id]->mutex));
        comp_ptr[comp_partner_id]->done_batches++;
        pthread_cond_signal(&(comp_ptr[comp_partner_id]->cv));
        pthread_mutex_unlock(&(comp_ptr[comp_partner_id]->mutex));
    }

    return;

    //try to secure next batch
}

void vaccinate_students(int id, int filled_slots)
{
    int comp_partner_id = hosp_ptr[id]->partner_company;

    //PRINT ENTERING VACCINATION PHASE AND vaccinating students
    //--------------------------------------------------------------
    //Minor sleep to mention moodle requirements to make code more life-like
    sleep(1);

    for (int i = 0; i < filled_slots; i++)
    {
        printf(ANSI_GREEN "Student %d on Vaccination Zone %d has been vaccinated which has success probability %Lf\n" ANSI_RESET, hosp_ptr[id]->curr_served_students[i],
               id, comp_ptr[comp_partner_id]->prob_of_success);
        fflush(stdout);

        stu_ptr[hosp_ptr[id]->curr_served_students[i]]->curr_stat = 2;
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
    printf(ANSI_RED "Vaccination Centre %d\t HAS entered the simulation\n" ANSI_RESET, id);
    fflush(stdout);

    for (int j = 0; j < 10; j++)
    {
        hosp_ptr[id]->curr_served_students[j] = -1;
    }

    buy_batch_from_company(id);
    return NULL;
}