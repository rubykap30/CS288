#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char *argv[], char *envp[]){
	int index = 0;
	printf("before\n");
	while(envp[index]){
		printf("envp[%d] = \"%s\n", index, envp[index]);
		index++;
	}
	//printf("index: %d", index);
	for(int i = 0; i < index-1; i++){
		for(int j = 0; j < index-1-i; j++){
			char *t = strdup(envp[j]);
			char *split = strtok(t, "=");
			char *t2 = strdup(envp[j+1]);
			char *split2 = strtok(t2, "=");
			if(strcmp(split, split2) > 0){
				char *t3 = envp[j];
				envp[j] = envp[j+1];
				envp[j+1] = t3;
			}
		}
	}
	printf("\nafter\n");
	for(int i = 0; i < index; i++){
		printf("envp[%d] = \"%s\n", i, envp[i]);
	}
	exit(0);
}
