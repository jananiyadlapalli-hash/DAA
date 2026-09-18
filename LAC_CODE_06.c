//FIND THE EXECUTION TIME OF BINARY SEARCH ALGORITHM
#include<stdio.h>
#include<time.h>
void binary_search(int arr[],int l,int r, int x){
    if (l>r){
        printf("elem not present\n");
    }
    else{
        int mid=(l+r)/2;
        if(arr[mid]==x){
            printf("elem present at index %d\n",mid);
        }
        else if(arr[mid]>x){
            binary_search(arr,l,mid-1,x);
        }
        else{
            binary_search(arr,mid+1,r,x);
        }
    }
}
int main() {
    int arr[] = {5,10,15,20,25,30,35,40,45,50,55,60,65,70,75};
    int n = sizeof(arr) / sizeof(arr[0]);
    clock_t start,end;
    double cpu_time_used;
    start=clock();
    binary_search(arr,0,n-1,65);
    end=clock();
    cpu_time_used=((double)(end-start))/CLOCKS_PER_SEC;
    printf("%f seconds\n",cpu_time_used);
    return 0;
}

