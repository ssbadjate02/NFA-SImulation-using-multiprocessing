#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <pthread.h>
#include <math.h>
#include <stdbool.h>
#include <sys/types.h>
int n,m,flag=0;
char s[20];
    
struct args
{
    int i,j,k;
    int path[20];
};
FILE *out_file1;
void *f(void *b)
{
    if(flag) pthread_exit(NULL);
    struct args *a = (struct args*)b; 
    // fprintf("%d %d %d",a->i,a->j,a->k);
    int i = a->i,j=a->j,k=a->k,l=0;
    int *temp = (int*)malloc(20 * sizeof(int));
    // fprintf("i-%d,j-%d,k-%d\n",i,j,k);
    for(int l=0;l<20;l++) temp[l] = a->path[l];
    temp[k] = i+n*j;
    char pstr[30];
    int index = 0;
    for (l=0; l <= a->k; l++) index += sprintf(&pstr[index], "%d ", temp[l]);
    
    if(a->k<m) 
    {
        fprintf(out_file1,"[%d] Exploring path: %s\n",gettid(),pstr);
    }
    else
    {
        if(i==n-1 && j==n-1)
        {
            fprintf(out_file1,"Accepted path: %s\n",pstr);
            flag = 1;
        }
        else
        {
            fprintf(out_file1,"Failed path: %s\n",pstr);
        }
        pthread_exit(NULL);
    }
    struct args *c = (struct args *)malloc(sizeof(struct args));
    struct args *d = (struct args *)malloc(sizeof(struct args));
    pthread_t p1,p2;
    if(s[k]=='0')
    {
        c->i = i-1;
        c->j = j;
        c->k = k+1;
        memcpy(c->path, temp, sizeof(int)*(k+1));
        d->i = i+1;
        d->j = j;
        d->k = k+1;
        memcpy(d->path, temp, sizeof(int)*(k+1));
        if(i>0) pthread_create(&p1,NULL,f,(void *)c);
        if(i<n-1) pthread_create(&p2,NULL,f,(void *)d);
    }
    else
    {
        c->i = i;
        c->j = j-1;
        c->k = k+1;
        memcpy(c->path, temp, sizeof(int)*(k+1));
        d->i = i;
        d->j = j+1;
        d->k = k+1;
        memcpy(d->path, temp, sizeof(int)*(k+1));
        if(j>0) pthread_create(&p1,NULL,f,(void *)c);
        if(j<n-1) pthread_create(&p2,NULL,f,(void *)d);

    }
    pthread_join(p1,NULL);
    pthread_join(p2,NULL);
    free(c);
    free(d);
    free(temp);
    return NULL;
}

struct map
{
    int key, zero, one;
};

int find_parent(int parent[], int i)
{
    if(i==parent[i])
        return i;
    return find_parent(parent, parent[i]);
}

void swap(int* xp, int* yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}
void union_set(int parent[],int size[], int a, int b)
{
    a = find_parent(parent, b);
    b = find_parent(parent, b);
    if(b == a) return;
    if(size[b]>size[a]) swap(&a,&b);
    parent[b] = a;
    size[a]+=size[b];
}
    
int main()
{
    FILE *in_file  = fopen("input.txt", "r"); // read only 
    out_file1 = fopen("2020A7PS0028H_t1.txt", "w"); // write only
    FILE *out_file2 = fopen("2020A7PS0028H_t2.txt", "w"); // write only
    if (in_file == NULL || out_file1 == NULL || out_file2 == NULL) 
    {   
        printf("Error! Could not open file\n"); 
        exit(-1); // must include stdlib.h 
    } 
    fscanf(in_file,"%d",&n);
    n++;
    fscanf(in_file,"%s", s);
    m = strlen(s);
    // int path[20];
    pthread_t t;
    struct args b;
    b.i=0;
    b.j=0;
    b.k=0;
    pthread_create(&t,NULL,f,(void *)&b);
    pthread_join(t,NULL);
    
    //Part B starts Here

    struct map *a =  (struct map*)malloc(sizeof(struct map));
    int x=0,i=0,f0=0,f1=0,capacity=1,strCap=1,siz=1;
  
    int** arr = (int**)malloc(1 * sizeof(int*));
        for (i = 0; i < 1; i++)
            arr[i] = (int*)malloc(n*n * sizeof(int));
    arr[0][0] = 1;
    i=0;
    while(i<n*n)
    {
        if(x==siz) break;
        int st[n*n];
        for(int l=0;l<n*n;l++) st[l] = arr[x][l];
        f1=0;
        f0=0;
        
        int j=0;
        int r0[n*n],r1[n*n];
        for(int l=0;l<n*n;l++) r1[l] = 0,r0[l]=0;
        for(int j=0;j<n*n;j++)
        { 
            if(st[j]==1)
            {
                int x = j%n;
                int y = j/n;
                if(x>0)r0[((x-1+y*n))]=1;
                if(x<n-1) r0[((x + 1 + y*n))]=1;
                if(y>0) r1[((x+(y-1)*n))]=1;
                if(y<n-1) r1[((x + (y+1)*n))]=1;
            }
        }
        for(j=0;j<siz;j++)
        {
            int ff1=0,ff0=0;
            for(int l=0;l<n*n;l++) if(r0[l]!=arr[j][l]) ff0=1;
            for(int l=0;l<n*n;l++) if(r1[l]!=arr[j][l]) ff1=1;
            if(!ff0) f0=j;
            if(!ff1) f1=j;
        }   
        if(!f0)
        {
            if(siz==capacity)
            {
                arr = (int**)realloc(arr,sizeof(int*)*(2*capacity));
                capacity*=2; 
                for(int l=siz;l<capacity;l++) arr[l] = (int*)calloc(n*n,sizeof(int));
            }
            for(int l=0;l<n*n;l++) arr[siz][l] = r0[l];
            f0=siz;
            siz++;
        }
        if(!f1)
        {
            if(siz==capacity)
            {
                arr = (int**)realloc(arr,sizeof(int*)*(2*capacity));
                capacity*=2; 
                for(int l=siz;l<capacity;l++) arr[l] = (int*)calloc(n*n,sizeof(int));
            }
            for(int l=0;l<n*n;l++) arr[siz][l] = r1[l];
            f1 = siz;
            siz++;
        }
        
        if(strCap == x)
        {
            a = (struct map *)realloc(a,sizeof(struct map)*(2*strCap));
            strCap*=2;
        }
        a[x].key = x;
        a[x].zero = f0;
        a[x].one = f1;
        x++;
    }

    int final_states[n*n];

    for(int i=0;i<siz;i++) final_states[i] = arr[i][n*n-1];
    
    int dist[siz][siz];
    for(int i = 0;i<siz;i++)
    {
        for(int j = 0;j<siz;j++)
        {
            if(final_states[i] != final_states[j]){
                dist[i][j] = 1;
                dist[j][i] = 1;
            }   
            else{
                dist[i][j] = 0;
                dist[j][i] = 0;
            }
        }
    }
    bool change = true;
    while(change)
    {
        change = false;
        for(int i = 0;i<siz;i++)
        {
            for(int j = 0;j<siz;j++)
            {
                if(dist[i][j] != 1)
                {
                    if(dist[a[i].zero][a[j].zero] == 1 || dist[a[i].one][a[j].one] == 1){
                        dist[i][j] = 1;
                        dist[j][i] = 1;
                        change = true;
                    }
                }
            }
        }
    }
    int parent[siz];
    int size[siz];
    for(int i=0;i<siz;i++) parent[i] = i,size[i] = 1;
    
    for(int i=0;i<siz;i++)
    {
        for(int j=0;j<siz;j++)
        {
            if(dist[i][j]==0){
                if(final_states[i]==1 || final_states[j]==1)
                {
                    final_states[i] = 1;
                    final_states[j] = 1;
                }
                union_set(parent,size, i, j);
            }
        }
    }
    struct map *min_dfa =  (struct map*)malloc(sizeof(struct map));
    int *min_dfa_final_states =  (int*)malloc(sizeof(int));
    int min_dfa_capacity = 1;
    int min_dfa_siz = 0;
    for(int i = 0;i<siz;i++)
    {
        if(parent[i]==i)
        {
            if(min_dfa_capacity == min_dfa_siz)
            {
                min_dfa_capacity *= 2;
                min_dfa_final_states = (int*)realloc(min_dfa_final_states, min_dfa_capacity*sizeof(int));
                min_dfa = (struct map*)realloc(min_dfa, min_dfa_capacity*sizeof(struct map));
            }
            min_dfa[min_dfa_siz].key = min_dfa_siz;
            min_dfa[min_dfa_siz].zero = parent[a[i].zero];
            min_dfa[min_dfa_siz].one = parent[a[i].one];
            if(final_states[i]==1)
                min_dfa_final_states[min_dfa_siz] = 1;
            else
                min_dfa_final_states[min_dfa_siz] = 0;
            min_dfa_siz++;
        }
    }
    for(int i=0;i<min_dfa_siz;i++) fprintf(out_file2,"%d ",min_dfa_final_states[i]);
    fprintf(out_file2,"\n");
    int v[siz][siz];
    for(int i=0;i<siz;i++) for(int j=0;j<siz;j++) v[i][j] = 0;
    
    for(int i=0;i<x;i++) v[min_dfa[i].key][min_dfa[i].zero] = 1;
    
    for(int i=0;i<siz;i++)
    {
        for(int j=0;j<siz;j++) fprintf(out_file2,"%d ",v[i][j]);
        fprintf(out_file2,"\n");
    }
    for(int i=0;i<siz;i++) for(int j=0;j<siz;j++) v[i][j] = 0;
    
    for(int i=0;i<x;i++) v[min_dfa[i].key][min_dfa[i].one] = 1;
    
    for(int i=0;i<siz;i++)
    {
        for(int j=0;j<siz;j++) fprintf(out_file2,"%d ",v[i][j]);
        fprintf(out_file2,"\n");
    }

}