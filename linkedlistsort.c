#include <stdio.h>
#include <stdlib.h>

struct node{
	int value;
	struct node *next;
};

int main (void){
	struct node *head = NULL, *tail = NULL, *pnode;
	int v1;
	int count = 0;
	while(scanf("%d",&v1) != EOF){
		pnode = (struct node *) malloc(sizeof(struct node));
		pnode->value = v1;
		count++;
		pnode->next = NULL;
		if (head == NULL){
			head = pnode;
		}
		if (tail != NULL){
			tail->next = pnode;
		}
		tail = pnode;
	}
	struct node **start, *end, *temp, *ptr1, *ptr2;
	int swap;
	for(int i = 0; i < count-1; i++){
		start = &(head);
		swap = 0;
		for(int j = 0; j < count-1-i; j++){
			ptr1 = *start;
			ptr2 = ptr1->next;
			if(ptr1->value > ptr2->value){
				temp = ptr2->next;
				ptr2->next = ptr1;
				ptr1->next = temp;
				*start = ptr2;
				swap++;
			}start = &(*start)->next;
		}if(swap == 0){
			break; } }
			
	struct node *pnode3;
	pnode3 = head;
	while(pnode3 != NULL){
		printf("%d\n", pnode3->value);
		pnode3 = pnode3->next;
	}
	return 0;
}
