#include "assignment1.h"
#include "assignment2.h"
#include "workload.h"
#include "scheduler.h"
#include "governor.h"
#include <stdio.h>
#include <limits.h>
#include <float.h>

#define LOW 0
#define HIGH 1
#define LIMIT 1.0
#define P_IDEL 50

// Note: Deadline of each workload is defined in the "workloadDeadlines" variable.
// i.e., You can access the dealine of the BUTTON thread using workloadDeadlines[BUTTON]
// See also deadlines.c and workload.h
// Assignment: You need to implement the following two functions.

typedef  void* (*thread_function_t)(void*);

long long currentDeadlines[] = {0,0,0,0,0,0,0,0};
int lastAliveTasks[] = {0, 0, 0, 0, 0, 0, 0, 0};

thread_function_t functions[] = {&thread_button, &thread_twocolor, &thread_temp,
               &thread_track, &thread_touch, &thread_rgbcolor,&thread_aled, &thread_buzzer };


int prefered_freq[] = {HIGH,HIGH,HIGH,HIGH,HIGH,HIGH,HIGH,HIGH};
int P_1200 = 600;
int P_600 = 450;
long long energy = 0;
int P_WORK[] = {450,600};

/* 
 * Calculating CPU utilization
 */
float calculate_utilization(int i, int *prefered_freq, long long *w_1200, long long *w_600, long long *deadlines){
    float util = 0.0;
        if(prefered_freq[i] == HIGH){
            util = ((float)w_1200[i])/deadlines[i];
        }else{
            util = ((float)w_600[i])/deadlines[i];
        }
    return util;
}

void updateCurrentDeadlines(long long time_difference, int* lastAliveTasks, const int* aliveTasks, long long idleTime){
    int i = 0;
    int x;
    for(;i<NUM_TASKS;i++){
        if(aliveTasks[i] == 1){
            if(lastAliveTasks[i] == 1 && idleTime == 0){
                x = currentDeadlines[i] - time_difference;
                currentDeadlines[i] = x > 0 ? x:workloadDeadlines[i];
            }else{
                currentDeadlines[i] = workloadDeadlines[i];
            }
        }else{
            currentDeadlines[i] = 0;
        }
    }
}

/* 
 * Choosing task based on edf
 */
int chooseTask(long long *currentDeadlines ,const int* aliveTasks){
    long long minDead = LONG_MAX;
    int taskindex = -1,i = 0;    
    for(;i<NUM_TASKS;i++){
        if(aliveTasks[i] == 1){
            if(currentDeadlines[i]< minDead){
                minDead = currentDeadlines[i];
                taskindex = i;
            }
        }
    }
    return taskindex;
}

/* 
 * Choosing the frequency based on workload.
 */
int chooseFreq(int i){
    return prefered_freq[i];
};

void updateLastAliveTasks(const int* aliveTasks){
    int i = 0;
    for(;i<NUM_TASKS;i++){
        lastAliveTasks[i] = *(aliveTasks+i);
    }
}

void printTask(TaskSelection t){
    printDBG("Task  id:%d, freq:%d \n", t.task, t.freq);
}

void learn_workloads(SharedVariable* sv) {
	
    long long start, end;    
    long long workloads_1200[] = {0,0,0,0,0,0,0,0};
    long long workloads_600[] = {0,0,0,0,0,0,0,0};
    unsigned int i = 0;
    for(i = 0; i<NUM_TASKS;i++){
        
        set_by_max_freq();
        start = get_current_time_us();            
        (*(functions[i]))(sv);        
        end = get_current_time_us();
        workloads_1200[i] = end-start;

        set_by_min_freq();
        start = get_current_time_us();            
        (*(functions[i]))(sv);        
        end = get_current_time_us();
        workloads_600[i] = end-start;
                
        currentDeadlines[i] = workloadDeadlines[i];
    }
    
    /* Check schedulablity */
    printDBG("Begin to check schedulablity\n");
    float util[8] = {0,0,0,0,0,0,0,0};
	
	for(i=0;i<NUM_TASKS;i++){
		
		util[i] = calculate_utilization(i, prefered_freq,workloads_1200, workloads_600, workloadDeadlines);
		if(util[i] > 0.1){
			prefered_freq[i] = HIGH;
		}
		else{
			prefered_freq[i] = LOW;
		}
		printDBG("util[%d] :: %f \n", i, util[i]);
    }
}

TaskSelection select_task(SharedVariable* sv, const int* aliveTasks, long long idleTime) {

    static int prev_freq = HIGH;
    static long long last_timestamp = -1;
    long long current_timestamp = get_scheduler_elapsed_time_us();
    long long time_difference = 0;
    
    if (last_timestamp != -1){
        time_difference = current_timestamp - last_timestamp;        
    }
    last_timestamp = current_timestamp;
    
    updateCurrentDeadlines(time_difference, lastAliveTasks, aliveTasks, idleTime);
        
    TaskSelection sel;
    sel.task = chooseTask(currentDeadlines, aliveTasks); 
    sel.freq = chooseFreq(sel.task);
    prev_freq = sel.freq;

    if(sel.task >= 0){
	printTask(sel);
    energy += (((float)idleTime)*P_IDEL + ((float)time_difference)* P_WORK[prev_freq]);
	printDBG("Energy being consumed for the workload: %lld\n", energy);
    }

    updateLastAliveTasks(aliveTasks);
	return sel;
}