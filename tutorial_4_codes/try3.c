void a_musician()
{
    sem_wait(sem2);
    [

        //GOT
        nothing to do


        //FAIL
        nothing to do
    ]

}



void (ae)_umusician()
{
    sem_wait(sem1);

    //got
    electrical 
    acq




    ///fail


}


//for all-1 -> free-acqu+free--electr
//for ac-2 -. free acq
//for ele-3  free ele
//for singer-4->    free-acqu + free--electr+ 1musician on stages

void acquostic()
{



    //done with previous performance


    signal_sem(sem2);
    signal_sem(sem1);
    signal_sem(sem4)
}

void electrcic()
{
     signal_sem(sem3);
    signal_sem(sem1);
    signal_sem(sem4)
}