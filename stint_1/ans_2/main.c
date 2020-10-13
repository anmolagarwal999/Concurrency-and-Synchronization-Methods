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
    scanf("Enter number of companies: %d", num_companies);
    scanf("Enter number of vaccination zones: %d", num_hospitals);
    scanf("Enter number of students: %d", num_students);
}

int main()
{
    LL i, j, k, t;

    take_input();

    struct company *comp_ptr[num_companies];
    struct company *hosp_ptr[num_hospitals];
    struct company *stu_ptr[num_students];

    // intitialize companies and take their probalities
    for (i = 0; i < num_companies; i++)
    {
        comp_ptr[i] = (struct comp *)malloc(sizeof(struct comp));
    }

    //initialize hospitals
    for (i = 0; i < num_hospitals; i++)
    {
        hosp_ptr[i] = (struct hosp *)malloc(sizeof(struct hosp));
    }

    //intialize students
    for (i = 0; i < num_students; i++)
    {
        stu_ptr[i] = (struct stu *)malloc(sizeof(struct stu));
    }
}