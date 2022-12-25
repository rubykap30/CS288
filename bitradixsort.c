#include <stdio.h>

unsigned int* radix_sort(unsigned int *A, int n, int k){
	//create two buckets for 0 and 1s
	unsigned int buck1[100];
	unsigned int buck2[100];
	for(int d = 0; d < k; d++){
		//bucket size variables to clear after every for loop iteration
		int bucket1size = 0;
		int bucket2size = 0;
		// sort A using the d-th bit as the key
		for (int i = 0; i < n; i++){
			//checks if bit is a 1
			if(((A[i]>>d) & 1) == 1){
				buck2[bucket2size] = A[i];
				bucket2size++;
			}
			else{
				// add A[i] to bucket with 0s
				buck1[bucket1size] = A[i];
				bucket1size++;
			}
			// A = join the buckets
		}
		int val = 0;
		int i = 0;
		while (i < bucket1size){
			if (buck1[i] != 0){
				A[val] = buck1[i];
				val++;
			}
			i++;
		}
		int j = 0;
		while ( j < bucket2size){
			if (buck2[j] != 0){
				A[val] = buck2[j];
				val++;
			}
			j++;
		}
	}

	return A;
}


int main(){
	unsigned int data[100];
	int sizeofdata = 0;
	scanf("%d", &sizeofdata);
	for(int i = 0; i < sizeofdata; i++){
		scanf("%d", &data[i]);
		}
	int bitsize = 32;
	radix_sort(data, sizeofdata, bitsize);
	for (int j = 0; j < sizeofdata; j++){
		printf("%u\n", data[j]);
	}
	return 0;
}
