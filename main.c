
#include<stdio.h>
#include<stdlib.h>


typedef struct s_list
{
    int id;
}					t_philo;


int main(int ac,char **av)
{
    if(ac!=6 && ac != 5)
        return 0;

    t_philo *philos;
    philos = malloc(sizeof(t_philo)*10);

    for(int i=0;i<5;i++)
    {
        philos[i].id=i;
    }    
    for(int i=0;i<5;i++)
    {
        printf("%d\n",philos[i].id);
    }  
}