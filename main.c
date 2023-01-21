
#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <sys/time.h>
typedef struct s_philo
{
    int id;
}					t_philo;

typedef struct s_data
{
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
     printf("MILISeconds since 1/1/1970: %lu\n",tv.tv_sec*1000+tv.tv_usec/1000);
     printf("MILISeconds since 1/1/1970: %lu\n",tv.tv_sec*1000+tv.tv_usec/1000);
return 0;

}



int main(int ac,char **av)
{
    if(ac!=6 && ac != 5)
        return 0;

    t_data data;
    data.philos = malloc(sizeof(t_philo)*10);

    for(int i=0;i<5;i++)
    {
        data.philos[i].id=i;
    }    
    for(int i=0;i<5;i++)
    {
        printf("%d\n",data.philos[i].id);
        gettime();
        usleep(500);
    }  
}