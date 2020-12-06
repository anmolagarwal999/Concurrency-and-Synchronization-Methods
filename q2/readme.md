# Q2: Back to College


## Running the program

```bash=
make
```

## Details
There are 3 types of real-world entities-> vaccination zones (which I have interchangably referred to as 'hospitals'), pharma companies and Students.
I have made one thread for every instance of each of these real-world entities.

## Pharma companies
The main function involved here is `prep_stock`. Here, the company prepares batches after randomizing batch sizes, number of batches, preparation time etc. Once a manufacture term is completed, **the company goes on to sleep on its own personal conditional variable**. The company is woken only if one or more of its delivered batches is fully consumed by the students vaccinated in a particular vaccination zone. If this batch was the last batch to be consumed, then the comapny starts its next manufacturing term, else it goes on to sleep again until all batches of its previous manufacturing team has been consumed. 
(Implementation style by using conditional variables approved by TA on 17th Oct).

```c=

void prep_stock(int id)
{
    //printf("inside prep stock function\n");
    while (true)
    {
        if (tot_conclusions_left == 0)
        {
            //Comapny no longer wants to prepare vaccine if all students have reached a verdict
            printf(ANSI_RED "BREAKING: Company exiting simulation as all students have reached verdicts\n" ANSI_RESET);
            break;
        }

        int prep_time = get_random_int2(w_min_prepare_time, w_max_prepare_time, "in prep stock");
        comp_ptr[id]->capacity_of_batches = get_random_int2(p_min_val, p_max_val, "in prep stock-> 2");
        comp_ptr[id]->done_batches = 0;
        int num_batches = get_random_int2(r_min_val, r_max_val, "in prep stock-> 3");
        printf(ANSI_YELLOW "Company %d\t is preparing %d batches of vaccines of capacity %d each which have success probability %Lf\n" ANSI_RESET, comp_ptr[id]->id, num_batches, comp_ptr[id]->capacity_of_batches, comp_ptr[id]->prob_of_success);

        sleep(prep_time);


        /* Lock needed as if curr_batches_num is set to a finite value 
        and if all stock gets over between the time `while` evaluates
         to true and the company sleeps, then company will never wake up*/
        pthread_mutex_lock(&(comp_ptr[id]->mutex));

        //batches produced in the last manufacture stint
        comp_ptr[id]->curr_batches_num = num_batches;

        //batches yet to be fully consumed
        comp_ptr[id]->left_batches_num = comp_ptr[id]->curr_batches_num;

        comp_ptr[id]->done_batches = 0;
        printf(ANSI_YELLOW "Company %d\t HAS PREPARED %d batches of vaccines which have success probability %Lf\n" ANSI_RESET, comp_ptr[id]->id, num_batches, comp_ptr[id]->prob_of_success);

        dispatch_stock(id);
    }
}

void dispatch_stock(int id)
{

    int flag = 0;
    while (comp_ptr[id]->done_batches < comp_ptr[id]->curr_batches_num)
    {
        pthread_cond_wait(&(comp_ptr[id]->cv), &(comp_ptr[id]->mutex));
  
        if (tot_conclusions_left <= 0)
        {
            //printf(ANSI_CYAN "All students' have been tended to\n" ANSI_RESET);
            flag = 1;
            break;
        }
    }
    pthread_mutex_unlock(&(comp_ptr[id]->mutex));

    //Reached here -> all of previous dispatch has been consumed-> time to manufacture again
    if (flag != 1)
        printf(ANSI_YELLOW "Company %d\t All the vaccines prepared are over. Resuming production now.\n" ANSI_RESET, comp_ptr[id]->id);
}
```


## The Students
A student arrives in college after a randomized amount of time and makes himself available for being invited to a vaccination zone. 
The student has the following possible statuses:
* -1-> hasn't yet arrived
* 0-> if yet waiting
* 1-> ongoing
* 2-> successful vaccination (may or may not have developed antibodies)

The student changes his status to **waiting(0)** and then keeps on busy waiting until the vaccination zone thread changes its status to **vaccinated(2)**. Now, the student uses the success probability of the vaccination(based on the company which manufactured the vaccine) and after randomization, gets the verdict whether he/she has developed antibodies or not.
* If he has developed antibodies the student exits the simulation.
* If he hasn't developed antibodies,student checks if he has used all 3 attempts:
    * If yes, then the corresponding thread exits
    * Otherwise, student changes his status back to '**waiting(0)**' and starts busy waiting again.

## Vaccination zones (interchangably used with term 'hospitals')

> **Buying a batch of vaccines from a company**
> The hospital tries to buy a batch from a company as long as all students haven't reached a conclusion.**It iterates among all the companies until it finds a company which has an unsold batch**. If it is able to find a batch, it buys it, decrements the batch from the company stock and now goes on to invite waiting students to come for vaccination.
```c=
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
         
                if (pthread_mutex_trylock(&(comp_ptr[curr_check_id]->mutex)) != 0)
                {
                    //EBUSY  The mutex could not be acquired because it was already locked.
                    // printf("Company with id %d is busy deciding transit changes, hence did not answer call from centre %d\n", curr_check_id, id);
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
```



> **Inviting students to take part in its next vaccination phase**
> In this function, the vaccination zones decides the number of students to allow in current vaccination phase and then keeps iterating through the students as long as its does not fulfill its current quota. However, if it discovers that no students are waiting, then it begins it's vaccination phase even if the quota is partially filled.


```c=
void invite_students(int id)
{
    printf(ANSI_CYAN "Pharmaceutical Company %d has delivered vaccines to Vaccination zone %d, resuming vaccinations now\n" ANSI_RESET, hosp_ptr[id]->partner_company, id);
    sleep(1);
    printf(ANSI_CYAN "Vaccination Zone %d is ready to vaccinate with %d slots\n" ANSI_RESET, id, hosp_ptr[id]->left_vaccines);
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
        //precuation
        int curr_stu_id = get_random_int(0, num_students - 1) % num_students;
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
                // printf("Student id is busy as is currently in contact with another hospital, hence did not answer call\n");
            }
            else
            {
                // if student's current status is waiting
                if (stu_ptr[curr_stu_id]->curr_stat == 0)
                {
                    stu_ptr[curr_stu_id]->curr_stat = 1; //ongoing
                    printf(BMAG "Student %d   assigned a slot on the Vaccination Zone %d and waiting to be vaccinated\n" ANSI_RESET, curr_stu_id, id);
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

        pthread_mutex_lock(&(hosp_ptr[id]->mutex));
        hosp_ptr[id]->left_vaccines -= filled_slots;
        pthread_mutex_unlock(&(hosp_ptr[id]->mutex));

        vaccinate_students(id, filled_slots);
    }

    int comp_partner_id = hosp_ptr[id]->partner_company;
    if (hosp_ptr[id]->left_vaccines == 0)
    {
        printf(ANSI_CYAN "Vaccination Zone %d has run out of vaccines\n" ANSI_RESET, id);

        pthread_mutex_lock(&(comp_ptr[comp_partner_id]->mutex));
        comp_ptr[comp_partner_id]->done_batches++;
        pthread_cond_signal(&(comp_ptr[comp_partner_id]->cv));
        pthread_mutex_unlock(&(comp_ptr[comp_partner_id]->mutex));
    }

    return;

    //try to secure next batch
}
```

> ** The vaccination phase**
> Here, all students who were allotted a slot in the current vaccination phase are vaccinated and their status is changed correspondingly.

```c=
void vaccinate_students(int id, int filled_slots)
{
    int comp_partner_id = hosp_ptr[id]->partner_company;

    if (filled_slots != 0)
    {
        printf(BCYN "Vaccination zone %d entering vaccination phase\n" ANSI_RESET, id);
        sleep(1);
    }

    for (int i = 0; i < filled_slots; i++)
    {
        printf(ANSI_MAGENTA "Student %d on Vaccination Zone %d has been vaccinated which has success probability %Lf\n" ANSI_RESET, hosp_ptr[id]->curr_served_students[i],
               id, comp_ptr[comp_partner_id]->prob_of_success);
        fflush(stdout);

        //No need to acquire lock as only hospital can change state
        stu_ptr[hosp_ptr[id]->curr_served_students[i]]->curr_stat = 2;
    }
}
```

## Other implementation details
* I have used a `num_students_hopeful` variable which keeps track of the number of students who are waiting in queue to be vaccinated. If at any given moment, the vaccination zones notice that there are no waiting students, they go ahead with their vaccination phase irrespective of whether all slots have been filled or not.
* I have used a `tot_conclusions_left` variable. This variable signifies whether all students have reached a definitve verdict and is used by comapnies and vaccination zones to judge whether they should keep on manufacturing/vaccination or whether they shoudl now stop.



## Assumptions and other details
* The constraint in pdf that at max 8 students(maybe lesser) can be vaccinated in a single vaccination phase by hospital will be adhered to.
* The number of vaccination zones, students or companies won't exceed 500
* I have induced a hardcoded 1 sec gap between vaccine batch being ordered and it being delivered
* The pharma companies will resume vaccine production only after the consumption(and not 'sale') of all its current baches. So if a case arises where the total number of vaccine batches produced by all the companies is less than 'm', the vaccination zones remain wothout a batch even though there is a company which is idle at the moment.