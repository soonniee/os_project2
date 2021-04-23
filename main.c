#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
typedef struct Process{
    int pid;
    int first_arr_time;
    int arr_time;
    int complete_time;
    int init_q;
    int cycles;
    int cycle_index;
    int total_burst;
    int* arr_burst;
} Process;
typedef struct Queue{
    struct Queue* next;
    Process* process;
}Queue;
int process_num;
int complete_num=0;
Process **Process_arr;
Queue **Ready_Queue;
Queue *Sleep_Queue;
int global_time = 0;
void push(Process *p,int which_queue){
    Queue *head = (Queue*)malloc(sizeof(Queue));
    Queue *node =(Queue*)malloc(sizeof(Queue));
    
    if(which_queue==0) head = Ready_Queue[p->init_q];
    else head = Sleep_Queue;
    
    node->process = p;
    
    int check = 0;
    while(head->next!=NULL){
        Queue *temp =(Queue*)malloc(sizeof(Queue));
        temp = head;
        head = head->next;
        if(head->process != NULL){
            if((head->process->arr_time) > (p->arr_time)){
                check = 1;
                temp -> next = node;
                node->next = head;
                break;
            }
        }
    }
    if(check == 0){
        node->next = NULL;
        head ->next = node;
    }
    
}
void delete(Process *p,int which_queue){
    Queue *head = (Queue*)malloc(sizeof(Queue));
    Queue *temp = (Queue*)malloc(sizeof(Queue));
    if(which_queue==0) 
    {
        head = Ready_Queue[p->init_q];
        temp = head->next;
        head->next = temp->next;
        free(temp);
    }
    else{
        head = Sleep_Queue;
        while(head->next!=NULL){
            temp = head->next;
            if(temp->process->pid == p->pid){
                head->next = temp->next;
                free(temp);
                break;
            }
            else head = head->next;
        }
        
    }
    
}
void set_process(){
    
    int pid,arr_time,init_q,cycles;
    int size_arr;
    FILE *fp;
    fp = fopen("input4.txt","r");
    
    fscanf(fp,"%d",&process_num);
    
    Process_arr = (Process**)malloc(sizeof(Process) * process_num);
    Ready_Queue = (Queue**)malloc(sizeof(Queue)*4);
    Sleep_Queue = (Queue*)malloc(sizeof(Queue)*process_num);
    for(int k=0;k<4;k++){
            Ready_Queue[k] = (Queue*)malloc(sizeof(Queue));
            Ready_Queue[k]->process = NULL;
            Ready_Queue[k]->next = NULL;
    }
    Sleep_Queue->process = NULL;
    Sleep_Queue->next = NULL;
    for(int i=0; i<process_num; i++){
        int total_burst=0;
        fscanf(fp,"%d %d %d %d",&pid,&arr_time,&init_q,&cycles);
        size_arr = cycles * 2 - 1;
        Process* new_process = (Process*)malloc(sizeof(Process));
        new_process->pid = pid;
        new_process->first_arr_time = arr_time;
        new_process->arr_time = arr_time;
        new_process->init_q = init_q;
        new_process->cycle_index=0;
        new_process->cycles = cycles;
        
        new_process->arr_burst = (int*)malloc(sizeof(int)*size_arr);
        for(int j=0;j<size_arr;j++){
            int burst_val;
            fscanf(fp,"%d",&burst_val);
            total_burst += burst_val;
            new_process->arr_burst[j] = burst_val; 
            
        }
        new_process->total_burst = total_burst;
        Process_arr[i] = new_process;
        // push(Process_arr[i]);
       
    }
    for(int i=0;i<process_num;i++){
        for(int j=i+1;j<process_num;j++){
            if(Process_arr[i]->arr_time > Process_arr[j]->arr_time){
                Process *temp;
                temp = Process_arr[i];
                Process_arr[i] = Process_arr[j];
                Process_arr[j] = temp;
            }
        }
    }
    for(int i=0;i<process_num;i++){
        push(Process_arr[i],0);
        // printf("%d\n",Process_arr[i]->arr_time);
    }
    
    
}

int main(){
    set_process();
    simulator();
    return 0;
}
int arr_check(Process *p){
    if(global_time >= p->arr_time){
        return 1;
    }
    else{
        
        return -1;
    } 
}
void wake_check(int num){
    int wake_priority=0;
    Queue *head = (Queue*)malloc(sizeof(Queue));
    Queue *temp = (Queue*)malloc(sizeof(Queue));
    head = Sleep_Queue;
    while(head->next!=NULL){
        temp = head;
        head = head ->next;
        Process *p = head->process;
        int index = p->cycle_index;
        int arr_time = p->arr_time;
        int io_time = p->arr_burst[index*2+1];
        if(arr_time + io_time <= global_time){
            head = temp;
            delete(p,1);
            
            if(p->init_q !=3) {
                p->init_q -=1;
                
            }
            p->arr_time += io_time;
            
            p->cycle_index += 1;
            
            push(p,0);
        }
    }
    
}
void min_wakeup(){
    int wait_start_time =global_time;
    int min;
    int check = 0;
    while(1){
        Queue *head = (Queue*)malloc(sizeof(Queue));
        Queue *temp = (Queue*)malloc(sizeof(Queue));
        head = Sleep_Queue;
        while(head->next!=NULL){
            temp = head;
            head = head ->next;
            Process *p = head->process;
            int index = p->cycle_index;
            int arr_time = p->arr_time;
            int io_time = p->arr_burst[index*2+1];
            if(arr_time + io_time <= global_time){
                head = temp;
                check = 1;
                delete(p,1);
                
                if(p->init_q !=3) {
                    p->init_q -=1;
                    
                }
                p->arr_time += io_time;
                
                p->cycle_index += 1;
                 
                push(p,0);
            }
            
        }
        if(check == 1) break;
        else global_time +=1;
    }
    printf("[ Waiting ( %ds ~ %ds ) ]\n",wait_start_time,global_time);
}
void simulator(){
    scheduling(Process_arr[0]->init_q,Process_arr[0]->cycle_index,Process_arr[0]);
    int empty = 0;
    while(1){
        if(empty == 1) min_wakeup();
        
        for(int i=0;i<4;i++){
            int wake=0;
            int sch = 0;
            while(1){
                Queue *head = (Queue*)malloc(sizeof(Queue));
                head = Ready_Queue[i];
                head = head ->next;
                if(head== NULL){
                    empty = 1;
                    break;
                }
                empty = 0;
                Process *curr_p = (Process*)malloc(sizeof(Process));
                curr_p = head->process;
                int arrival_check = arr_check(curr_p);
                if(arrival_check == -1) break;
                else{
                    sch = 1;
                    int index = curr_p -> cycle_index;
                    scheduling(i,index,curr_p);
                    break;
                }
            }
            if(complete_num == process_num) break;
            if(sch == 1) break;
        }
        if(complete_num == process_num) break;  
    }
    print_time();
}
void scheduling(int queue_num,int index,Process *p){
    int start_time = global_time;
    int schedule_time;
    int time_quantum;
    int wake;
    int curr_q = p->init_q;
    if(queue_num<3){
        time_quantum = pow(2,queue_num);
        schedule_time = time_quantum;
    }
    else{
        time_quantum = p->arr_burst[index*2]+1;
        schedule_time = time_quantum - 1;
    }
    if(p->arr_burst[index*2] > time_quantum){
        delete(p,0);
        p->arr_burst[index*2] -=time_quantum;
        if(p->init_q !=3) p->init_q +=1;
        // p->cycle_index+=1;
        global_time += time_quantum;
        p->arr_time = global_time;
        push(p,0);
        
        wake_check(queue_num);
    }
    else{
        
        int cpu_time = p->arr_burst[index*2];
        global_time += cpu_time;
        if(p->cycle_index+1 >= p->cycles) {
            complete_num +=1;
            p->complete_time = global_time;
            delete(p,0);
        }
        else{
            p->arr_time = global_time;
            delete(p,0);
            push(p,1);
        }
        
        wake_check(queue_num);
    }
    int finish_time = global_time;
    print_gantt(start_time,finish_time,p->pid,curr_q);
    
}
void print_gantt(int start_time,int finish_time,int pid,int queue){
    int schedule_time = finish_time - start_time;
    printf("[ ");
    printf("P%d ( %ds ~ %ds ) at Q%d",pid,start_time,finish_time,queue);
    printf(" ]  \n");
}
void print_time(){
    int sum_tt=0;
    int sum_wt=0;
    for(int i=0;i<process_num;i++){
        int tt = Process_arr[i]->complete_time - Process_arr[i]->first_arr_time;
        int wt = tt - Process_arr[i] -> total_burst;
        sum_tt += tt;
        sum_wt += wt;
        int pid = Process_arr[i]->pid;
        printf("P%d's Turnaround Time : %ds\n",pid,tt);
        printf("P%d's Waiting Time : %ds\n",pid,wt);
    }
    float mean_tt = (float)sum_tt / process_num;
    float mean_wt = (float)sum_wt / process_num;
    printf("Mean of All Processes's Turnaround Time : %.2fs\n",mean_tt);
    printf("Mean of All Processes's Waiting Time : %.2fs\n",mean_wt);
}
