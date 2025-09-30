#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COMMANDLEN 50
#define AGGIUNGIGRAFO 1
#define TOPK 2
#define INF 4294967295
#define ROWLEN 5000



typedef struct node_s
{
    unsigned long nodeNumber;
    unsigned long distFromRoot;
    unsigned long heapIndex;
} Node;


typedef struct minHeap_s
{
    Node** nodes;
    unsigned long size;
} MinHeap;

typedef struct graph_s
{
    unsigned long graphNumber;
    unsigned long weight;
} Graph;

typedef struct maxHeap_s
{
    Graph * graphs;
    unsigned long size;
    unsigned long k;
} MaxHeap;


// Graphs functions -------------------*
void printMatrix(unsigned long* matrix,unsigned long dim);
void initMatrix(unsigned long * matrix,unsigned long dim);


// MinHeap functions ------------------*
void initMinHeap(MinHeap *minHeap,Node *directAccessToNode,unsigned long dim);
void resetMinHeap(MinHeap *minHeap,Node *directAccessToNode,unsigned long dim);
void printMinHeap(MinHeap*);
void printMinHeapLinear(MinHeap* minHeap);
Node removeMinHeap(MinHeap* minHeap);
void refreshNMinHeap(MinHeap* minHeap,unsigned long n);
Node minMinHeap(MinHeap* minHeap);
void minHeapify(MinHeap* minHeap,unsigned long i);
//void insertMinHeap(MinHeap* minHeap,Node* node);
unsigned long parent(unsigned long i); // anche max heap

// MaxHeap functions ------------------*
void initMaxHeap(MaxHeap *maxHeap,Graph* graphs,unsigned long k);
void printMaxHeap(MaxHeap* maxHeap);
//void printMaxHeapN(MinHeap* minHeap);
void removeMaxHeap(MaxHeap* maxHeap);
Graph maxMaxHeap(MaxHeap* maxHeap);
void maxHeapify(MaxHeap* maxHeap,unsigned long i);
void insertMaxHeap(MaxHeap* maxHeap,Graph graph);


// String functions -------------------*
void togliInviodaString(char* stringa);


// Parsing functions ------------------*
int getCommandType(char* stringa);
void readGraph(unsigned long*,unsigned long);

void resetDirectAccessToNodes(Node *directAccessToNodes, unsigned long dim);




int main(int argc, char * argv[]) {

    Graph *leadingTable;
    Graph currentGraph;

    Node node;
    Node *directAccessToNodes;

    unsigned long cbuff;
    unsigned long j;

    unsigned long cont = -1;
    unsigned long * matrix;
    unsigned long d;
    unsigned long k;
    //int cmdType;
    unsigned long ndis;
    unsigned long currWeight;
    char command[COMMANDLEN];

    MinHeap minHeap;
    MaxHeap maxHeap;


    if (scanf("%ld %ld",&d,&k) != 0);

    matrix = (unsigned long *) malloc(sizeof(unsigned long)*d*d);
    directAccessToNodes = (Node*) malloc(sizeof(Node)*(d));
    minHeap.nodes = (Node**) malloc(sizeof(Node*)*(d+1));
    leadingTable = (Graph*) malloc(sizeof(Graph)*(k+1));

    initMatrix(matrix,d);

    resetDirectAccessToNodes(directAccessToNodes,d);

    initMaxHeap(&maxHeap,leadingTable,k);

    if(fgets(command,COMMANDLEN,stdin)!= NULL);

    while (fgets(command,COMMANDLEN,stdin)){
        if ( command[0] == 'A') {
            currWeight = 0;
            cont++;
            readGraph(matrix,d);
            resetDirectAccessToNodes(directAccessToNodes,d);
            initMinHeap(&minHeap,directAccessToNodes,d);

            while(minHeap.size > 0){
                node = removeMinHeap(&minHeap);
                if (node.distFromRoot != INF) {
                    currWeight += node.distFromRoot;

                    cbuff = node.nodeNumber * d;
                    for (j = 1; j < d; j++) {
                        if (matrix[cbuff + j] != 0 && j!=node.nodeNumber) {
                            ndis = node.distFromRoot + matrix[cbuff + j];
                            if (directAccessToNodes[j].distFromRoot > ndis) {
                                directAccessToNodes[j].distFromRoot = ndis;
                                refreshNMinHeap(&minHeap,directAccessToNodes[j].heapIndex);
                            }
                        }
                    }
                } else {
                    minHeap.size = 0;
                }
            }

            currentGraph.graphNumber = cont;
            currentGraph.weight = currWeight;
            insertMaxHeap(&maxHeap,currentGraph);
        }else {
            printMaxHeap(&maxHeap);
        }

    }

    free(matrix);
    free(directAccessToNodes);
    free(leadingTable);
    free(minHeap.nodes);

    return 0;

}


void initMinHeap(MinHeap* minHeap,Node *directAccessToNode,unsigned long dim){

    minHeap->size = dim;
    minHeap->nodes[0] = NULL;

    for (unsigned long i = 1; i<=dim ;i++){
        minHeap->nodes[i] = &(directAccessToNode[i-1]);
        minHeap->nodes[i]->heapIndex = i;
    }
}


void resetMinHeap(MinHeap* minHeap,Node *directAccessToNode,unsigned long dim){
    minHeap->size = dim;

    for (unsigned long i = 1; i<=dim;i++){
        minHeap->nodes[i] = &(directAccessToNode[i-1]);
        minHeap->nodes[i]->heapIndex = i;
    }
}


Node minMinHeap(MinHeap* minHeap){
    Node min;
    min = *(minHeap->nodes[1]);
    return min;
}

Node removeMinHeap(MinHeap* minHeap){
    Node min;

    if (minHeap->size < 1){
        min.distFromRoot =INF;
        min.nodeNumber = -1;
        return min;
    }

    min.nodeNumber = (minHeap->nodes[1])->nodeNumber;
    min.distFromRoot = (minHeap->nodes[1])->distFromRoot;
    min.heapIndex = (minHeap->nodes[1]->heapIndex);

    minHeap->nodes[1] = minHeap->nodes[minHeap->size];
    minHeap->nodes[1]->heapIndex = 1;
    minHeap->size--;

    minHeapify(minHeap,1);

    return min;
}


void refreshNMinHeap(MinHeap* minHeap,unsigned long n){
    unsigned long i;
    Node* temp;

    for( i = n; i>1 && minHeap->nodes[parent(i)]->distFromRoot > minHeap->nodes[i]->distFromRoot; i = parent(i) ){
        temp = minHeap->nodes[parent(i)];
        minHeap->nodes[parent(i)] = minHeap->nodes[i];
        minHeap->nodes[i] = temp;

        minHeap->nodes[i]->heapIndex = i;
        minHeap->nodes[parent(i)]->heapIndex = parent(i);
    }

}

/*
void insertMinHeap(MinHeap* minHeap,Node* node){
    Node* temp;
    int size;

    (minHeap->size)++;
    minHeap->nodes[minHeap->size] = *node;

    for (int i=minHeap->size;i > 1 && ( minHeap->nodes[i].distFromRoot <= minHeap->nodes[parent(i)].distFromRoot) ; i = parent(i)){
        temp = minHeap->nodes[parent(i)];
        minHeap->nodes[parent(i)] = minHeap->nodes[i];
        minHeap->nodes[i] = temp;
    }
    return;
}
*/

void minHeapify(MinHeap* minHeap,unsigned long i){
    unsigned long l,r;
    unsigned long min;
    Node * temp;

    l = 2*i;
    r = 2*i+1;

    if (l <= minHeap->size && minHeap->nodes[l]->distFromRoot < minHeap->nodes[i]->distFromRoot)
        min = l;
    else
        min = i;
    if (r <= minHeap->size && minHeap->nodes[r]->distFromRoot < minHeap->nodes[min]->distFromRoot)
        min = r;

    if (min != i){
        temp = minHeap->nodes[i];
        minHeap->nodes[i] = minHeap->nodes[min];
        minHeap->nodes[min] = temp;
        minHeap->nodes[min]->heapIndex = min;
        minHeap->nodes[i]->heapIndex = i;
        minHeapify(minHeap,min);
    }

    return;
}


unsigned long parent(unsigned long i){
    if (i==1)
        return i;

    if (i%2==1)
        i--;

    return i/2;
}


void initMaxHeap(MaxHeap* maxHeap,Graph * graphs,unsigned long k){

    maxHeap->k = k;
    maxHeap->size = 0;
    maxHeap->graphs = graphs;

    maxHeap->graphs[0].graphNumber = -1;
    maxHeap->graphs[0].weight = INF;

}

void printMaxHeap(MaxHeap * maxHeap){

    if (maxHeap->size > 0){
        for(unsigned long i = 1 ; i < maxHeap->size ; i++){
            printf("%ld ",maxHeap->graphs[i].graphNumber);
        }
        printf("%ld",maxHeap->graphs[maxHeap->size].graphNumber);
    }

    printf("\n");

}

void removeMaxHeap(MaxHeap* maxHeap){

    if (maxHeap->size < 1){
        return;
    }

    maxHeap->graphs[1] = maxHeap->graphs[maxHeap->size];
    (maxHeap->size)--;

    maxHeapify(maxHeap,1);

    return;
}


Graph maxMaxHeap(MaxHeap* maxHeap){
    Graph max;
    max = maxHeap->graphs[1];
    return max;
}

void maxHeapify(MaxHeap * maxHeap,unsigned long i){
    unsigned long l,r;
    unsigned long max;
    Graph temp;

    l = 2*i;
    r = 2*i+1;

    if (l <= maxHeap->size && maxHeap->graphs[l].weight > maxHeap->graphs[i].weight)
        max = l;
    else if (l <= maxHeap->size && maxHeap->graphs[l].weight == maxHeap->graphs[i].weight){
        if (maxHeap->graphs[l].graphNumber > maxHeap->graphs[i].graphNumber)
            max = l;
        else
            max = i;
    } else {
        max = i;
    }

    if (r <= maxHeap->size && maxHeap->graphs[r].weight > maxHeap->graphs[max].weight)
        max = r;
    else if (r <= maxHeap->size && (maxHeap->graphs[r].weight == maxHeap->graphs[max].weight)){
        if (maxHeap->graphs[r].graphNumber > maxHeap->graphs[max].graphNumber)
            max = r;
    }

    if (max!=i){
        temp = maxHeap->graphs[i];
        maxHeap->graphs[i] =  maxHeap->graphs[max];
        maxHeap->graphs[max] = temp;
        maxHeapify(maxHeap,max);
    }

    return;
}


void resetDirectAccessToNodes(Node * directAccessToNodes,unsigned long dim){

    directAccessToNodes[0].nodeNumber = 0;
    directAccessToNodes[0].distFromRoot = 0;

    for (unsigned long i = 1 ; i < dim ; i++){
        directAccessToNodes[i].nodeNumber = i;
        directAccessToNodes[i].distFromRoot = INF;
    }
}


void insertMaxHeap(MaxHeap * maxHeap,Graph graph){
    Graph temp;


    if (maxHeap->size == maxHeap->k){
        if (graph.weight < maxHeap->graphs[1].weight) {

            removeMaxHeap(maxHeap);

            insertMaxHeap(maxHeap,graph);
        }
    } else {
        if (maxHeap->size < maxHeap->k){
            maxHeap->size = maxHeap->size+1;
            maxHeap->graphs[maxHeap->size].graphNumber = graph.graphNumber;
            maxHeap->graphs[maxHeap->size].weight = graph.weight;

            for (unsigned long i = maxHeap->size ; i > 1 &&
                                                   ( maxHeap->graphs[i].weight >= maxHeap->graphs[parent(i)].weight) ; i = parent(i)){

                temp = maxHeap->graphs[parent(i)];
                maxHeap->graphs[parent(i)] = maxHeap->graphs[i];
                maxHeap->graphs[i] = temp;
            }
        }
    }

    return;
}



void readGraph(unsigned long* graph,unsigned long dim){
    unsigned long j;
    unsigned long i;
    unsigned long k;
    char row[ROWLEN];

    for( i=0 ; i< dim; i++ ){
        if ( fgets(row,ROWLEN,stdin) != NULL){
            graph[i*dim] = 0;
            for ( k=0 ,j=0; row[k] != '\0' && row[k] != '\n' ; k++){
                if (row[k] != ','){
                    graph[i*dim+j] *= 10;
                    graph[i*dim+j] += (row[k]-'0');
                } else {
                    j++;
                    graph[i*dim+j] = 0;
                }
            }
        }
    }

}

/*
int getCommandType(char * stringa){

    if (stringa[0] == 'A')
        return AGGIUNGIGRAFO;
    if (stringa[0] == 'T')
        return TOPK;

    return TOPK;
}*/

void printMinHeap(MinHeap* minHeap){
    unsigned long i;

    for(i=1; i<=minHeap->size ;i++){
        printf("{%ld %ld %ld} ",minHeap->nodes[i]->nodeNumber,minHeap->nodes[i]->distFromRoot,minHeap->nodes[i]->heapIndex);
    }
    printf("\n");

}

void printMinHeapLinear(MinHeap* minHeap){

    while(minHeap->size>0){
        printf("%ld ", (removeMinHeap(minHeap)).distFromRoot);
    }
    printf("\n");
}

void printMatrix(unsigned long* matrix,unsigned long dim){
    for(unsigned long i = 0;i<dim;i++){
        for(unsigned long j = 0;j<dim;j++)
            printf("%ld ",matrix[i*dim+j]);
        printf("\n");
    }
    return;
}

void initMatrix(unsigned long * matrix, unsigned long dim){
    unsigned long i;
    unsigned long j;

    for(i = 0 ; i<dim ; i++ ){
        for(j = 0; j<dim ; j++)
            matrix[i*dim+j] = 0;
    }
    return;
}