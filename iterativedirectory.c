#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

struct node{
	char* filename;
	off_t data;
    struct node *next;
};

struct node *head = NULL;
struct node *dirhead = NULL;

void traverse(char* dirname){
    DIR* dir = opendir(dirname);
    struct stat statBuf;
    struct node **headref = &head;
    if (dir == NULL){
        return;
    }struct dirent* entity; entity = readdir(dir);
    while (entity != NULL){
        if(strcmp(entity -> d_name, ".") != 0 && strcmp(entity -> d_name, "..") != 0){
            char path[1024] = {0};
			strcat(path, dirname); strcat(path, "/"); strcat(path, entity->d_name);
            stat(path, &statBuf);
            if (S_ISREG(statBuf.st_mode)){
                struct node *pnode = (struct node*) malloc (sizeof(struct node));
				pnode->data = statBuf.st_size;
                pnode->filename = (char *)malloc(strlen(path) + 1);
				strcpy(pnode->filename,path);
                pnode->next = *headref;
				*headref = pnode; }
            else if(S_ISDIR(statBuf.st_mode)){
                struct node *pnode2 = (struct node*) malloc (sizeof(struct node));
				pnode2->data = 0;
                pnode2->filename = (char *)malloc(strlen(path) + 1);
				strcpy(pnode2->filename,path);
                if(dirhead == NULL){
                    dirhead = pnode2;
                    dirhead->next = NULL;
                } else{
                    struct node *iter = dirhead;
                    while(iter->next != NULL){
                        iter = iter->next;
                    } iter->next = pnode2;
                    pnode2->next = NULL;
                } } } entity = readdir(dir);
    } closedir(dir); }

int main(int argc, char* argv[]){
    struct node* directories;
    if(argc != 2){
        exit(1);
    }
    else{
        traverse(argv[1]);
        directories = dirhead;
        while(directories != NULL){
            traverse(directories->filename);
            directories = directories->next;
        }
    }
    //LENGTH FOR BUBBLE SORT
    struct node* pnode1 = head;
    int count = 0;
	struct node *len = head;
	while(len != NULL){
		count++;
		len = len->next;
	}
    //BUBBLE SORT
	struct node **start, *temp, *ptr1, *ptr2;
    int swap;
    for(int i = 0; i < count-1; i++){
        start = &(head);
        swap = 0;
        for(int j = 0; j < count-1-i; j++){
            ptr1 = *start;
            ptr2 = ptr1->next;
            if(ptr1->data > ptr2->data){
                temp = ptr2->next;
                ptr2->next = ptr1;
                ptr1->next = temp;
                *start = ptr2;
                swap++;
            }
            start = &(*start)->next;
        }
        if(swap == 0){
        	break;
            }
    }	
	struct node *ptr = head;
	while(ptr != NULL){
		printf("%lu\t%s\n", ptr->data, ptr->filename);
		ptr = ptr->next;
	}
    return 0;
}
