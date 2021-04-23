#include<iostream>
#include<algorithm>
#include<vector>
#include<cstdio>
using namespace std;
int num_process, num_resource;
vector<int> resourceUnits;
vector<vector<int>> requested;
vector<vector<int>> allocated;
vector<int> sum_allocated;
void input(){
    
    FILE *fp;
    fp = fopen("input6.txt","r");
    fscanf(fp,"%d %d",&num_process,&num_resource);
    
    for(int i=0;i<num_resource;i++){
        int resource_unit;
        fscanf(fp,"%d",&resource_unit);
        resourceUnits.push_back(resource_unit);
        sum_allocated.push_back(0);
    }

    for(int i=0;i<num_process;i++){
        vector<int> elem;
        for(int j=0;j<num_resource;j++){
            int num_allocated;
            fscanf(fp,"%d",&num_allocated);
            sum_allocated[j] += num_allocated; 
            elem.push_back(num_allocated);
        }
        allocated.push_back(elem);
    }
    
     for(int i=0;i<num_process;i++){
         vector<int> elem;
        for(int j=0;j<num_resource;j++){
            int num_requested;
            fscanf(fp,"%d",&num_requested);
            elem.push_back(num_requested);
        }
        requested.push_back(elem);
    }
    fclose(fp);
}
int findUnblocked(int reductedProcess[]){
    int findProcess = -1;
    
    for(int i=0;i<num_process;i++){
        bool satisfy = true;
        if(reductedProcess[i] == 1) continue;     
        for(int j=0;j<num_resource;j++){       
            if(requested[i][j] > (resourceUnits[j] - sum_allocated[j])){
                satisfy = false;
                break;
            }
        }
        if(satisfy){
            findProcess = i;
            break;
        }
        satisfy = true;        
    }
    if(findProcess==-1) return -1;
    else return findProcess;
}
int main(){
    input();
    int reductedProcess[num_process];
    for(int i=0;i<num_process;i++) reductedProcess[i]=0;
    while(1){
        int unblockedProcess = findUnblocked(reductedProcess);
        if(unblockedProcess == -1){
            break;
        }
        else{
            for(int i=0;i<num_resource;i++){
                sum_allocated[i] -= allocated[unblockedProcess][i];
            }
            
            reductedProcess[unblockedProcess] = 1; 
        }
    }
    int cnt = 0;
    for(int i=0;i<num_process;i++){
        if(reductedProcess[i] == 0){
            cnt = 1;
            printf("The Deadlocked Process is P%d \n",i+1);
        }
    }
    if(cnt == 0) printf("There is no Deadlocked Process!!\n");
    
    return 0;
}