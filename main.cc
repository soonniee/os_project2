#include<iostream>
#include<algorithm>
#include<vector>
#include<cstdio>
using namespace std;
int num_process, num_resource;
vector<int> resourceUnits;
vector<vector<pair<int,int>>> requested;
vector<vector<pair<int,int>>> allocated;
vector<int> sum_allocated;
void input(){
    
    FILE *fp;
    fp = fopen("input6.txt","r");
    fscanf(fp,"%d %d",&num_process,&num_resource);
    
    for(int i=0;i<num_resource;i++){
        int resource_unit;
        fscanf(fp,"%d",&resource_unit);
        resourceUnits.push_back(resource_unit);
    }
    
    // for(int i=0;i<num_process;i++){
    //     vector<pair<int,int>> elem;
    //     elem.resize(num_resource);
    //     allocated.push_back(elem);
    // }
    for(int i =0;i<num_resource;i++) sum_allocated.push_back(0);
    for(int i=0;i<num_process;i++){
        vector<pair<int,int>> elem;
        for(int j=0;j<num_resource;j++){
            int num_allocated;
            fscanf(fp,"%d",&num_allocated);
            sum_allocated[j] += num_allocated; 
            elem.push_back(make_pair(j,num_allocated));
        }
        allocated.push_back(elem);
    }
    // for(int i =0;i<num_resource;i++) printf("%d",sum_allocated[i]);
     for(int i=0;i<num_process;i++){
         vector<pair<int,int>> elem;
        for(int j=0;j<num_resource;j++){
            int num_requested;
            fscanf(fp,"%d",&num_requested);
            elem.push_back(make_pair(j,num_requested));
        }
        requested.push_back(elem);
    }
    fclose(fp);
}
int findUnblocked(int reductedProcess[]){
    int findProcess;
    int find = 0;
    for(int i=0;i<num_process;i++){
        if(reductedProcess[i] == 1) continue;
        int unblockedCnt = 0;
        for(int j=0;j<num_resource;j++){
            printf("P%d -> R%d : %d %d %d\n",i,j,requested[i][j].second,resourceUnits[j],sum_allocated[j]);
            if(requested[i][j].second <= (resourceUnits[j] - sum_allocated[j])){
                unblockedCnt++;
            }
        }
        if(unblockedCnt == num_resource){
            find = 1;
            findProcess = i;
            break;
        }
        
    }
    if(find == 0) return -1;
    else {
        return findProcess;
    }
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
                sum_allocated[i] -= allocated[unblockedProcess][i].second;
            }
            printf("The reducted Process is P%d\n",unblockedProcess+1);
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
    // for(int i=0;i<num_resource;i++) printf("%d ",resourceUnits[i]);
    // printf("\n"); 
    // for(int i=0;i<num_process;i++){
    //     for(auto it = requested[i].begin();it!=requested[i].end();it++){
    //         printf("%d %d\n",it->first,it->second);
    //     }
    // }
    // for(int i=0;i<num_process;i++){
    //     if()
    // }
    return 0;
}