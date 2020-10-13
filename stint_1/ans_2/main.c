#include "master_header.h"

int num_hospitals;
int num_companies;
int num_students;

void get_random_int(int lower, int upper)
{

    int num = (rand() % (upper - lower + 1)) + lower;
    return num;
}
void take_input()
{
    int latest_arrival_permitted = 10;
    scanf("Enter number of companies: %d", num_companies);
    scanf("Enter number of vaccination zones: %d", num_hospitals);
    scanf("Enter number of students: %d", num_students);
}

int main()
{
    LL i, j, k, t;

    take_input();

    // intitialize companies and take their probalities
    for (i = 0; i < num_companies; i++)
    {
        comp_ptr[i] = (struct comp *)malloc(sizeof(struct company));
        scanf("%Lf", &comp_ptr[i]->prob_of_success);
        comp_ptr[i]->id = i;
        comp_ptr[i]->thr_id = pthread_create(&(comp_ptr[i]->thread_obj), NULL, init_company, (void *)(&(comp_ptr[i]->id)));
    }

    //initialize hospitals
    for (i = 0; i < num_hospitals; i++)
    {
        hosp_ptr[i] = (struct hosp *)malloc(sizeof(struct hospital));
        hosp_ptr[i]->id = i;
        hosp_ptr[i]->thr_id = pthread_create(&(hosp_ptr[i]->thread_obj), NULL, init_company, (void *)(&(hosp_ptr[i]->id)));
    }

    //intialize students
    for (i = 0; i < num_students; i++)
    {
        stu_ptr[i] = (struct stu *)malloc(sizeof(struct student));
        stu_ptr[i]->id = i;
        stu_ptr[i]->thr_id = pthread_create(&(stu_ptr[i]->thread_obj), NULL, init_company, (void *)(&(stu_ptr[i]->id)));
    }
}