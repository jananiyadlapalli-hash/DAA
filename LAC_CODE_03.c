// find the sum of all number i array and then average of the numbers in the array
#include<stdio.h>
int main(){
    int arr[]={10,20,30,40,50,60};
    int n=6;
    int sum=0;
    for(int i=0;i<n;i++){
        sum+=arr[i];
    }
    printf("%d",sum);
    int average = sum/n;
    printf("\nAverage of the numbers in the array is %d",average);
    return 0;
}

