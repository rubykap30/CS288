#include <stdio.h>

float* rad_sort(float* A, int n, int k) {
	float posbuck0[100];
	float posbuck1[100];
	float negbuck0[100];
	float negbuck1[100];

	for (int d = 0; d < k; d++) {

		unsigned int *b = (unsigned int *) A;
		int psize0 = 0;
		int nsize0 = 0;
		int psize1 = 0;
		int nsize1 = 0;
		int val = 0;

		for (int i = 0; i < n; i++) {
			if (((b[i] >> 31) & 1) > 0) {
				if (((b[i] >> d) & 1) > 0) {
					negbuck1[nsize1] = A[i];
					nsize1++;
				}
				else {
					negbuck0[nsize0] = A[i];
					nsize0++;
				}
			}
			else {
				if (((b[i] >> d) & 1) > 0) {
					posbuck1[psize1] = A[i];
					psize1++;
				}
				else {
					posbuck0[psize0] = A[i];
					psize0++;
				}
			}
		}
		int i = 0;
		while(i < nsize1){
			if(negbuck1[i] != 0){
				A[val] = negbuck1[i];
				val++;
			}
			i++;
		}
		int j = 0;
		while(j < nsize0){
			if(negbuck0[j] != 0){
				A[val] = negbuck0[j];
				val++;
			}
			j++;
		}
		int x = 0;
		while(x < psize0){
			if(posbuck0[x] != 0){
				A[val] = posbuck0[x];
				val++;
			}
			x++;
		}
		int z = 0;
		while(z < psize1){
			if(posbuck1[z] != 0){
				A[val] = posbuck1[z];
				val++;
			}
			z++;
		}

	}
	return A;
}

int main(){

	float vals[100];
	int itemnum = 0;
	scanf("%d", &itemnum);
	for(int i = 0; i < itemnum; i++){
		scanf("%f",&vals[i]);
	}
	int bitsize = 32;
	rad_sort(vals, itemnum, bitsize);
	for (int j = 0; j < itemnum; j++){
		printf("%f\n", vals[j]);
	}

	return 0;
}
