
#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h> 
typedef struct s_philo
{
    int id;
    void *data;
}					t_philo;

typedef struct s_data
{
    int nbm_philos;
    int time_to_die;
    int time_to_eat;
    int time_to_sleep;
    int what_enough ;
    long time_start ;
    t_philo *philos;
}					t_data;


long gettime(void)
{
        struct timeval tv;
    gettimeofday(&tv,NULL);
return tv.tv_sec*1000+tv.tv_usec/1000;

}


void *philo_func(void *arg)
{
    t_philo philo = *(t_philo*)arg;
    t_data data = *(t_data*)philo.data;
    while(1)
    {
    printf("%ld %d\n",gettime()-data.time_start,philo.id);
    usleep(1000000);
    }

    return 0;
}


int main(int ac,char **av)
{
    if(ac!=6 && ac != 5)
        return 0;

    t_data data;
    pthread_t thread;
    data.philos = malloc(sizeof(t_philo)*10);

    for(int i=0;i<5;i++)
    {
        data.philos[i].id=i;
        data.philos[i].data=&data;
    } 
    data.nbm_philos=atoi(av[1]);
    data.time_to_die=atoi(av[2]);
    data.time_to_eat=atoi(av[3]);
    data.time_to_sleep=atoi(av[4]);
    data.time_start=gettime();
    for(int i=0;i<5;i++)
    {
        pthread_create(&thread,NULL,philo_func,&data.philos[i]);
    }    
    // for(int i=0;i<5;i++)
    // {
    //     printf("lol %ld %d\n",gettime(),data.philos[i].id);
    //     usleep(5000);
    // }  
      pthread_join (thread, NULL);
}