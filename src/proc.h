#ifndef PROC_H
#define PROC_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


typedef struct
{
    char id;            // name of the proc
    int stat;           /* status {0: new, 1: completed, 2: ready, 3: blocking, 4: running, 
                         5: context switch on to CPU, 6: context switch out of CPU, 
                         7: a process will be preempted, need to be swapped in cs_t/2, 
                         8: a process is being preempted, need to be put back to ready queue in cs_t/2} */
    int cpu_b;          // number of cpu bursts it has
    int arrival_t;      // arrival time
    int cpu_t[100];     // array CPU burst length
    int io_t[100];      // array of IO burst length
    int cpu_b_static;   // reserved for final calculation
    int arrival_t_static; // reserved for final calculation
    int wait_t;         // keep track of wait time
    int tau;            // estimated burst time
    int remain_tau;     // remaining tau
    int sample_t;       // actual burst time for the current running process
    int remain_sample_t;       // remaining time of a cpu burst
    int original_burst_t; // keep track of original burst time for processes that are preempted
    bool preempt;      // indicates if the current cpu burst was preempted or not

    // int wait_t;         /* For calculating average wait time in simout*/
    int wait_t_ctr;     /* For calculating average wait time in simout*/
    int io_complete_t;
    int preemption_ctr;
    int cxt_s_ctr;
} Proc;

float alpha; // defined as a global variable for ease

void FCFS(Proc *procs, Proc **ready, int procs_num, int t, int cs_t, int ctr_ready, int fd, int cpu_burst);
void SJF(Proc *procs, Proc **ready, int procs_num, int t, int cs_t, int ctr_ready, int fd, int cpu_burst);
void SRT(Proc *procs, Proc **ready, int procs_num, int t, int cs_t, int ctr_ready, int fd, int cpu_burst);
void RR(Proc *procs, Proc **ready, int procs_num, int t, int cs_t, int ctr_ready, int slice, char *rr_add, int fd, int cpu_burst);


/*Some helper functions*/
double get_rand(int ub, double lambda);
void gen_procs(Proc *procs, int seed, int procs_num, int ub, double lambda);

int sort (const void * a, const void * b);

void get_Q (Proc * ready[], int procs_num, char * queue);

int append_io_to_ready_queue (Proc * ready_procs[], Proc * procs, int procs_num, int * ctr_ready, int t, bool add);
int append_new_to_ready_queue (Proc * ready_procs[], Proc * procs, int procs_num, int * ctr_ready, int t, bool add);

void sort_queue (Proc * ready[], int ctr_ready, bool srt_sort);

void burst_begin (Proc * proc, int t);
bool RR_burst_begin (Proc * proc, int t, int slice);

int update_est_burst (Proc * proc, int cs_t, int t);
int RR_check_burst (Proc * ready[], int cs_t, int t, int slice, int procs_num, int ctr_ready);
void update_remain_t (Proc * procs, int proc_num);

void rm_running_proc (Proc * ready[], int procs_num, int * ctr_ready);

int check_proc_completion (Proc * ready[], int procs_num, int t);
int RR_check_proc_completion (Proc * ready[], int procs_num, int t, int ctr_ready);

void cxt_s_in (Proc * proc[], int cs_t, int t);
void cxt_s_out (Proc * proc, int cs_t, int t);


/*A helper function to check if all processes are completed*/
int check_all_procs(Proc *procs, int procs_num);


/*Check the ready queue (and begin to burst) before appending new ready procs*/
void check_rdy_que(Proc *procs,Proc **ready, int cs_t, int procs_num, int t,  bool srt_flag, int ctr_ready);
void RR_check_rdy_que(Proc *procs,Proc **ready, int cs_t, int procs_num, int t, int ctr_ready, int slice);

void check_cpub_context(Proc **ready, int cs_t, int procs_num, int t, int *ctr_ready);


/*Check preemptions*/
bool check_preem (Proc *procs, Proc **ready, char q[], int procs_num, int t, int cs_t, int ctr_ready);
bool check_preem_from_io (Proc *procs, int procs_num, Proc **ready, int completed_i, int t, int ctr_ready, int cs_t);


void print_stat (Proc *procs, int procs_num, int fd, int cs_t, int cpu_burst);

/*helper function for debugging*/
void print_info(Proc *procs, int procs_num, int ctr_ready);

#endif