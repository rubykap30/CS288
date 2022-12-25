#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 4
#define NxN (N*N)
#define TRUE 1
#define FALSE 0

struct node {
	int tiles[N][N];
	int f, g, h;
	short zero_row, zero_column;	/* location (row and colum) of blank tile 0 */
	struct node *next;
	struct node *parent;			/* used to trace back the solution */
};

int goal_rows[NxN];
int goal_columns[NxN];
struct node *start,*goal;
struct node *open = NULL, *closed = NULL;
struct node *succ_nodes[4];

void print_a_node(struct node *pnode) {
	int i,j;
	if(pnode != NULL){
		for (i=0;i<N;i++) {
			for (j=0;j<N;j++)
				printf("%2d ", pnode->tiles[i][j]);
			printf("\n");
		}
	}
	printf("\n");
}

struct node *initialize(char **argv){
	int i,j,k,index, tile;
	struct node *pnode;

	pnode=(struct node *) malloc(sizeof(struct node));
	index = 1;
	for (j=0;j<N;j++)
		for (k=0;k<N;k++) {
			tile=atoi(argv[index++]);
			pnode->tiles[j][k]=tile;
			if(tile==0) {
				pnode->zero_row=j;
				pnode->zero_column=k;
			}
		}
	pnode->f=0;
	pnode->g=0;
	pnode->h=0;
	pnode->next=NULL;
	pnode->parent=NULL;
	start=pnode;
	printf("initial state\n");
	print_a_node(start);

	pnode=(struct node *) malloc(sizeof(struct node));
	goal_rows[0]=3;
	goal_columns[0]=3;

	for(index=1; index<NxN; index++){
		j=(index-1)/N;
		k=(index-1)%N;
		goal_rows[index]=j;
		goal_columns[index]=k;
		pnode->tiles[j][k]=index;
	}
	pnode->tiles[N-1][N-1]=0;	      /* empty tile=0 */
	pnode->f=0;
	pnode->g=0;
	pnode->h=0;
	pnode->next=NULL;
	goal=pnode; 
	printf("goal state\n");
	print_a_node(goal);

	return start;
}

/* merge the remaining nodes pointed by succ_nodes[] into open list. 
 * Insert nodes based on their f values --- keep f values sorted. */
void merge_to_open() { 
	struct node *tmp;
    struct node *curr;
    struct node *cpy1;

    for (int i = 0; i < 4; i++) {
        curr = open;
        tmp = open;
        if (succ_nodes[i] != NULL) {

            cpy1=(struct node *) malloc(sizeof(struct node));
            for(int k = 0; k < N; k++){
                for(int j = 0; j < N; j++){
                    cpy1->tiles[k][j]=succ_nodes[i]->tiles[k][j];
                }
            }
            cpy1->zero_row = succ_nodes[i]->zero_row;
            cpy1->zero_column = succ_nodes[i]->zero_column;
            cpy1->parent = succ_nodes[i]->parent;
            cpy1->f = succ_nodes[i]->f;
            cpy1->g = succ_nodes[i]->g;
            cpy1->h = succ_nodes[i]->h;
            if (curr == NULL) {
                open = cpy1;
            } else if (cpy1->f <= curr->f) {

                if (cpy1->h <= curr->h) {
                    cpy1->next = curr;
                    open = cpy1;
                } else {
                    while (1) {
                        tmp = curr;
                        curr = curr->next;
                        if (curr == NULL) {
                            tmp->next = cpy1;
                            cpy1->next = NULL;
                            break;
                        }
                        if (cpy1->h <= curr->h) {
                            cpy1->next = curr;
                            tmp->next = cpy1;
                            break;
                        }
                    }
                }

            } else {
                while (TRUE) {
                    tmp = curr;
                    curr = curr->next;
                    if (curr == NULL) {
                        tmp->next = cpy1;
                        cpy1->next = NULL;
                        break;
                    }
                    if (cpy1->f <= curr->f) {
                        cpy1->next = curr;
                        tmp->next = cpy1;
                        break;
                    }
                }
            }
        }
    }
}


/*swap two tiles in a node*/
void swap(int row1,int column1,int row2,int column2, struct node * pnode){
    int tmp;
    tmp = pnode->tiles[row1][column1];
    pnode->tiles[row1][column1] = pnode->tiles[row2][column2];
    pnode->tiles[row2][column2] = tmp;
}

int manhattan(struct node * pnode) {
    int sum = 0;
    int total = 0;
    int flag = 0;
    int i = 0;
    while( i < N){
        for (int j = 0; j < N; j++) {
            total = pnode->tiles[i][j];
            flag = 0;
            for (int m = 0; m < N; m++) {
                for (int n = 0; n < N; n++) {
                    if (total == goal->tiles[m][n]) {
                        sum += (abs(i - m) + abs(j - n));
                        flag = 1;
                        break;
                    }
                }
                if (flag == 1) {
                    break;
                }
            }
        }
	i++;
    }
    return sum;
}

/*update the f,g,h function values for a node */
void update_fgh(int i){
	struct node *pnode = succ_nodes[i];
    pnode->h = manhattan(pnode);
    pnode->g = pnode->parent-> g + 1;
	int gvalue = pnode->g;
	int hvalue = pnode->h;
    pnode->f = gvalue + hvalue;
}

/* 0 goes down by a row */
void move_down(struct node * pnode){
	swap(pnode->zero_row, pnode->zero_column, pnode->zero_row + 1, pnode->zero_column, pnode); 
	pnode->zero_row++;
}

/* 0 goes right by a column */
void move_right(struct node * pnode){
	swap(pnode->zero_row, pnode->zero_column, pnode->zero_row, pnode->zero_column + 1, pnode); 
    pnode->zero_column++;
}

/* 0 goes up by a row */
void move_up(struct node * pnode){
	swap(pnode->zero_row, pnode->zero_column, pnode->zero_row - 1, pnode->zero_column, pnode); 
	pnode->zero_row--;
}

/* 0 goes left by a column */
struct node * move_left(struct node * pnode){
	swap(pnode->zero_row, pnode->zero_column, pnode->zero_row, pnode->zero_column - 1, pnode); 
	pnode->zero_column--;
}


/* expand a node, get its children nodes, and organize the children nodes using
 * array succ_nodes.
 */
void expand(struct node *selected) {
    // set new nodes parents to selected
    int i, j;
    struct node *temp;
    for(int k=0;k<N;k++) {
        succ_nodes[k]=NULL;
    }
    i = selected->zero_row;
    j = selected->zero_column;


    if((selected->zero_row +1) < N){   
        temp = (struct node*) malloc (sizeof(struct node));
        for(int x = 0; x < N; x++){
            for(int y = 0; y < N; y++){
                temp->tiles[x][y] = selected->tiles[x][y];
            }
        }
        temp->next = NULL;
        temp->parent = selected;
        temp->zero_row = i;
        temp->zero_column = j;
	    move_down(temp);
        temp->h = 0;
        temp->g = 0;
        temp->f = 0;
        succ_nodes[0] = temp;
        update_fgh(0);
    }
    if((selected->zero_column +1) < N){ 
        temp = (struct node*) malloc (sizeof(struct node));
        for(int x = 0; x < N; x++){
            for(int y = 0; y < N; y++){
                temp->tiles[x][y] = selected->tiles[x][y];
            }
        }
        temp->next = NULL;
        temp->parent = selected;
        temp->zero_row = i;
        temp->zero_column = j;
	    move_right(temp);
        temp->h = 0;
        temp->g = 0;
        temp->f = 0;
        succ_nodes[1] = temp;
        update_fgh(1);
    }
    if((selected->zero_row-1) >= 0){ 
        temp = (struct node*) malloc (sizeof(struct node));
        for(int x = 0; x < N; x++){
            for(int y = 0; y < N; y++){
                temp->tiles[x][y] = selected->tiles[x][y];
            }
        }
        temp->next = NULL;
        temp->parent = selected;
        temp->zero_row = i;
        temp->zero_column = j;
	    move_up(temp);
        temp->h = 0;
        temp->g = 0;
        temp->f = 0;
        succ_nodes[2] = temp;
        update_fgh(2);
    }
    if((selected->zero_column -1) >= 0){      /* LEFT */
        temp = (struct node*) malloc (sizeof(struct node));
        for(int x = 0; x < N; x++){
            for(int y = 0; y < N; y++){
                temp->tiles[x][y] = selected->tiles[x][y];
            }
        }
        temp->next = NULL;
        temp->parent = selected;
        temp->zero_row = i;
        temp->zero_column = j;
	    move_left(temp);
        temp->h = 0;
        temp->g = 0;
        temp->f = 0;
        succ_nodes[3] = temp;
        update_fgh(3);
    }
}


int nodes_same(struct node *a,struct node *b) {
	int flg=FALSE;
	if (memcmp(a->tiles, b->tiles, sizeof(int)*NxN) == 0)
		flg=TRUE;
	return flg;
}

/* Filtering. Some states in the nodes pointed by succ_nodes may already be included in 
 * either open or closed list. There is no need to check these states. Release the 
 * corresponding nodes and set the corresponding pointers in succ_nodes to NULL. This is 
 * important to reduce execution time.
 * This function checks the (i)th node pointed by succ_nodes array. You must call this
 * function in a loop to check all the nodes in succ_nodes. Free the successor node and 
 * set the corresponding pointer to NULL in succ_node[] if a matching node is found in 
 * the list.
 */ 
void filter(int i, struct node *pnode_list) {

    struct node *temp;
    temp = pnode_list;

    if (succ_nodes[i] != NULL) {
        while (temp != NULL) {
            if (nodes_same(succ_nodes[i], temp)) {
                if (succ_nodes[i]->f >= temp->f) {
                    succ_nodes[i] = NULL;
                    break;
                }
            }
            temp = temp->next;
        }
    }
}


int main(int argc,char **argv) {
	int iter,cnt;
	struct node *copen, *cp, *solution_path;
	int ret, i, pathlen = 0, index[N-1];

	solution_path = NULL;
	start = initialize(argv);	/* init initial and goal states */
	open = start; 

	iter = 0; 
	while (open != NULL) {	/* Termination cond with a solution is tested in expand. */
		copen = open;
		open = open->next;  /* get the first node from open to expand */
		
		/* DEBUG: print the node (layout) in *copen 
		 * Fix segmentation faults first. If the program cannot produce correct results,
		 * select an initial layout close to the goal layout, and debug the program with it.
		 * gdb is recommended. You may also print data on the screen. But this is not as
		 * convenient as using gdb.
		 * */

		if(nodes_same(copen,goal)){ /* goal is found */
			do{ /* trace back and add the nodes on the path to a list */
				copen->next = solution_path;
				solution_path = copen;
				copen = copen->parent;
				pathlen++;
			} while(copen != NULL);
			printf("Path (length=%d):\n", pathlen); 
			copen = solution_path;
			cp = copen;
			while(cp != NULL){
				print_a_node(cp);
				cp = cp->next;
			}/* print out the nodes on the list */
			break;
		}
		expand(copen);       /* Find new successors */

		/* DEBUG: print the layouts/nodes organized by succ_nodes[] */

		for(i = 0; i < 4; i++){
			filter(i,open);
			filter(i,closed);
		}

		/* DEBUG: print the layouts/modes remaining in succ_nodes[] */

		merge_to_open(); /* New open list */

		/* DEBUG: print the layouts/nodes on the open list */

		copen->next = closed;
		closed = copen;		/* New closed */
		/* print out something so that you know your 
		 * program is still making progress 
		 */
		iter++;
		if(iter %1000 == 0)
			printf("iter %d\n", iter);
	}
	return 0;
}
