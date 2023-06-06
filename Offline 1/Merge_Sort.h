#include<bits/stdc++.h>
using namespace std;

int merge(vector<int>&arr, int left, int mid, int right){
    int inversion_pair = 0;
    int left_array_size = mid-left+1;
    int right_array_size = right - mid;

    vector<int>left_array(left_array_size);
    vector<int>right_array(right_array_size);
    

    // copy the left and right subarray
    for(int i = 0;i<left_array_size; i++){
        left_array[i] = arr[left+i];
    }
    for(int i = 0;i<right_array_size; i++){
        right_array[i] = arr[i+mid+1];
    }

    int left_array_index = 0, right_array_index = 0, merged_array_index = left;

    // merge when both the arrays are not empty
    while(left_array_index < left_array_size && right_array_index < right_array_size){
        if(left_array[left_array_index] <= right_array[right_array_index]){
            // a[i]<a[j] and i<j so no inversion
            arr[merged_array_index++] = left_array[left_array_index ++];
            
        }else{
            // a[i] > a[j] but i<j so inversion pair
            // infact all the remaining elements in the left array form inversion pair
            // with the right array element since the left array is sorted
            arr[merged_array_index++ ] = right_array[right_array_index++];


            inversion_pair = inversion_pair + (left_array_size - left_array_index);
            
        }
        
    }
    
    // merge if left array is non-empty
    while(left_array_index < left_array_size){
        arr[merged_array_index++] = left_array[left_array_index++];
    }
    
    // merge if right array is non-empty
    while(right_array_index < right_array_size){
        arr[merged_array_index++] = right_array[right_array_index++];
    }

    return inversion_pair;

}

int  merge_sort(vector<int>&arr, int left, int right){
    int inversion_pair = 0;
    if(left < right){
        int mid = left + (right - left)/2;
        inversion_pair += merge_sort(arr, left, mid);
        inversion_pair += merge_sort(arr, mid+1, right);
        inversion_pair += merge(arr, left, mid, right);
    }
    return inversion_pair;
}



