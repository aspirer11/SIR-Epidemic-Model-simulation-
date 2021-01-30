#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_VERTICES 10000
#define MAX_EDGES 3000

int day=1;																																								

typedef struct Node{																											
  int vertex;
  struct node* next;
}Node;

typedef struct Graph{																			
  int numVertices;
  struct node** adjLists;
}Graph;

Node* createNode(int);													
Graph* createGraph(int);												
void addEdge(Graph*, int, int);											

typedef struct List{													
	int id;
	struct list *next,*prev;
}List;

List *susp,*inf,*rec;													

void insert(List**, int);												
void delete(List**, int);												
int susceptible(int);													
int number(List**);														

typedef struct Queue{																			
   int data, priority, type;
   struct queue* next;
}Queue;

Queue* pq;											

Queue* newEvent(int, int, int);										
void pop(Queue**);														
void enqueue(Queue**, int, int, int);									
int isEmpty(Queue**);											

int numberofdays(float);												
void addtransmit(Graph*, int);											
void addrecovery(int);													
void process_trans_SIR(Graph*, int);									
void process_rec_SIR(int);


	
int main(void) 
{
	srand(time(NULL));
	int ver = 1 + rand()% MAX_VERTICES;									
	srand(time(NULL));
	int edg = 1 + rand()% MAX_EDGES;							
	printf("Vertices:%d, Edges:%d\n", ver, edg);
	
	int i;
	Graph* G = createGraph(ver);										
	for(i=0; i<ver; i++)
	{
		for(int j=0; j<edg; j++)
		{
			if(rand()%2 ==1){
				int neighbour = rand()% ver;
				if(neighbour != i)
				addEdge(G,i,neighbour);									
			}
		}
	}			
	printf("Created population\n");
	
	for(i=0; i<ver; i++)
		insert(&susp, i);									
	
	FILE *fp = fopen("output.txt", "w");								
	
	int initial = rand()%ver;											
	delete(&susp, initial);												
	insert(&inf, initial);											
	
	Node *temp = G->adjLists[initial];																	
	pq = newEvent(temp->vertex,1,1);							
	temp = temp->next;
	while(temp){
		int ti= day;
		ti+= numberofdays(0.5);
		enqueue(&pq,temp->vertex,ti,1);			
		temp = temp->next;
	}
	free(temp);
	temp = NULL;
	addrecovery(initial);				
	
	while(!isEmpty(&pq)){
		
		day= pq->priority;											
		
		fprintf(fp, "%d\t%d\t%d\t%d\n", day, number(&susp), number(&inf), number(&rec));	
		
		if((pq->type == 1) && (susceptible(pq->data)))
			process_trans_SIR(G,pq->data);								
		
		else if(pq->type == 2)									
			process_rec_SIR(pq->data);					
			
		pop(&pq);														
	}	
	
	fclose(fp);														
	
	printf("Executed");	
	return 0;
}



Node* createNode(int v)													
{
	Node* newNode = (Node*)malloc(sizeof(Node));						
	newNode->vertex = v;
	newNode->next = NULL;
	return newNode;
}

Graph* createGraph(int vertices) 
{
	Graph* graph = (Graph*)malloc(sizeof(Graph));
	graph->numVertices = vertices;
	graph->adjLists = malloc(vertices * sizeof(Node*));

	for(int i=0; i<vertices; i++)
		graph->adjLists[i] = NULL;										

	return graph;														
}

void addEdge(Graph* graph, int s, int d) 
{
	Node* newNode = createNode(d);
	newNode->next = graph->adjLists[s];
	graph->adjLists[s] = newNode;

	/*newNode = createNode(s);											//removing comment part will make undirected graph
	newNode->next = graph->adjLists[d];
	graph->adjLists[d] = newNode;*/
}


void insert(List** head, int d)
{
	List *temp = (List*)malloc(sizeof(List));
	temp->id = d;
	temp->prev = temp->next = NULL;
	
	if((*head) == NULL)
		(*head) = temp;
	else{
		temp->next = (*head);
		(*head)->prev = temp;
		(*head) = temp;
	}	
}	
	
void delete(List** head, int d)
{
	if((*head) != NULL){
		
		List* temp = (*head);
		
		while(temp){
			if(temp->id == d)
				break;
			else
				temp = temp->next;
		}
		
		if((temp->prev == NULL) && (temp->next == NULL))
			(*head)=NULL;
		
		else if(temp->prev == NULL){
			(*head) = temp->next;
			(*head)->prev = NULL;
		}
		
		else if(temp->next == NULL)
			temp->prev->next = NULL;
		
		else{
			temp->prev->next = temp->next;
			temp->next->prev = temp->prev;
		}				
		
		free(temp);
		temp=NULL;
	}
	else 
		printf("List is empty");
}		

int susceptible(int d)
{
	List *temp = susp;
	while(temp){
		if(temp->id == d)
			return 1;										
		else
			temp = temp->next;
	}
	return 0;								
}			

int number(List** head)
{	
	List *temp = (*head);
	int count=0;
	while(temp){
		count++;
		temp = temp->next;
	}	
	return count;									
}



Queue* newEvent(int d, int p, int ty) 
{
   Queue* temp = (Queue*)malloc(sizeof(Queue));
   temp->data = d;
   temp->priority = p;
   temp->type = ty;
   temp->next = NULL;
   return temp;										
}

void pop(Queue** head) 
{
   Queue* temp = *head;
   (*head) = (*head)->next;
   free(temp);
}

void enqueue(Queue** head, int d, int p, int ty) 
{
   Queue* start = (*head);
   Queue* temp = newEvent(d, p, ty);
   
   if((*head)->priority > p){
      temp->next = *head;
      (*head) = temp;
   } 
   else{
		while((start->next != NULL) && (start->next->priority <= p)) 
		start = start->next;
      
      temp->next = start->next;
      start->next = temp;
   }
}

int isEmpty(Queue** head) 
{
	return (*head) == NULL;
}


int numberofdays(float pr)
{
	pr= pr*100;
	int i=0;
	for(i=1; i<300; i++)
	{
		if(rand()% 100 < pr)
			break;
	}
	return i;
}			

void addtransmit(Graph *G, int d)
{
	Node *temp = G->adjLists[d];
	while(temp){
		int ti= day;
		ti+= numberofdays(0.5);
		if(ti < 300)
			enqueue(&pq, temp->vertex, ti, 1);
		temp = temp->next;
	}
}		
	
void addrecovery(int d)
{
	int ti= day;
	ti+= numberofdays(0.2);
	enqueue(&pq, d, ti, 2);
}		

void process_trans_SIR(Graph* G, int d)
{
	delete(&susp, d);													
	insert(&inf, d);
	addtransmit(G, d);
	addrecovery(d);
}	

void process_rec_SIR(int d)
{
	delete(&inf, d);
	insert(&rec, d);
}		
