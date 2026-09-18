//find largest element in the array
#include<stdio.h>
int main(){
    int arr[]={23,89,45,12,67};
    int n = 5;
    int max = arr[0];
    for(int i=1;i<n;i++){
        if(arr[i]>max){
            max = arr[i];
        }
    }
    printf("Largest element in the array is %d", max);
    return 0;
}




