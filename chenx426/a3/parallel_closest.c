#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "point.h"
#include "serial_closest.h"
#include "utilities_closest.h"


/*
 * Multi-process (parallel) implementation of the recursive divide-and-conquer
 * algorithm to find the minimal distance between any two pair of points in p[].
 * Assumes that the array p[] is sorted according to x coordinate.
 */
double cross_min(struct Point *p,int size,double partial_min)
{
    double min = partial_min;
    qsort(p, size, sizeof(struct Point), compare_y);
    for(int i=0;i<size;++i)
        for (int j = i+1; j < size && (p[j].y - p[i].y) < min; ++j)
            if (dist(p[i],p[j]) < min)
                min = dist(p[i], p[j]);
    return min;
}

double closest_parallel(struct Point *p, int n, int pdmax, int *pcount) {
    if(n<4||pdmax==0) return closest_serial(p,n); //base case
    
    int mid = n/2;
    struct Point mid_p = p[mid];
    
    int fd[2];
    if(pipe(fd)==-1) perror("pipe");
    
    pid_t pid_1,pid_2;
    double result_l,result_r;
    
    pid_1 = fork();

    if(pid_1>0)
    {
        if((close(fd[1])) == -1) perror("close");
        if(read(fd[0],&result_l,sizeof(double))==-1) perror("read from pipe");
        if((close(fd[0])) == -1) perror("close");
    }
    
    //find the min in left part
    if(pid_1==0)
    {
        if((close(fd[0])) == -1) perror("close");
        result_l = closest_parallel(p,mid,pdmax-1,pcount);
        if(write(fd[1],&result_l,sizeof(double))==-1) perror("write to pipe");
        exit(*(pcount));
    }
    
    int df[2];
    if(pipe(df)==-1) perror("pipe");
    
    pid_2 = fork();
    
    if(pid_2>0)
    {
        if((close(df[1])) == -1) perror("close");
        if(read(df[0],&result_r,sizeof(double))==-1) perror("read from pipe");
        if((close(df[0])) == -1) perror("close");
    }
    
    //find the min in right part
    if(pid_2==0)
    {
        if((close(df[0])) == -1) perror("close");
        result_r = closest_parallel(p+mid,n-mid,pdmax-1,pcount);
        if(write(df[1],&result_r,sizeof(double))==-1) perror("write to pipe");
        if((close(df[1])) == -1) perror("close");
        exit(*(pcount));
    }
    
    //wait for child process and calculate # of process
    for(int i=0;i<2;++i)
    {
        int status = 0;
        
        if(wait(&status)==-1)
        {
            perror("wait");
            exit(1);
        }
        
        if(WIFEXITED(status)) *pcount += WEXITSTATUS(status)+1;
    }
    
    double partial_min = result_l<result_r ? result_l:result_r;
    
    //find possible points across mid
    struct Point tmp[n];
    int j = 0;
    for(int i=0;i<n;++i)
    {
        if(abs(p[i].x-mid_p.x)<partial_min)
        {
            tmp[j]=p[i];
            ++j;
        }
    }
    
    return partial_min<cross_min(tmp,j,partial_min) ? partial_min:cross_min(tmp,j,partial_min);
}

