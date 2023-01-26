// Floyd-Warshall Algorithm in C

//#includes
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <stdlib.h>

//#defines
#define INF 999
#define NEGATIVE "-1"

//matrix and edges
int n = 10000, m;

// y/n strings
char answer[1];

//possible semaphore for later
//sem_t semaLock;

//read-write locks
pthread_mutex_t readLock, writeLock;
int readerCount;

//structure
struct mystruct {
		int a;
		int b;
	};

//dist
int **dist;

//
void printMatrix(int matrix[n][n]);
void multiPrintMatrix(void);

//read and writing functions
void lock_for_writing(void) {
	pthread_mutex_lock(&writeLock);
}

void unlock_for_writing(void) {
	pthread_mutex_unlock(&writeLock);
}

void lock_for_reading(void) {
	pthread_mutex_lock(&readLock);
	readerCount++;
	//if first reader, no writers allowed
	if(readerCount == 1) {
		pthread_mutex_lock(&writeLock);
	}
	pthread_mutex_unlock(&readLock);
}

void unlock_for_reading(void) {
	pthread_mutex_lock(&readLock);
	readerCount--;
	//allow writers back in
	if(readerCount == 0) {
		pthread_mutex_unlock(&writeLock);
	}
	pthread_mutex_unlock(&readLock);

}



//jloop
void *jLoop(void *val1) {
	struct mystruct *myvar = (struct mystruct*) val1;

	int k = (int)myvar->a;
	int i = (int)myvar->b;

	//printf("thread: %d, %d \n", k, i);

	//printf("read Count: %d \n", readerCount);

	for (int j = 0; j < n; j++) {
		//printf("thread: %d, %d currently reading \n", k, i);
		lock_for_reading();
	       	if (dist[i][k] + dist[k][j] < dist[i][j]){
			//printf("thread: %d, %d currently unlocked for reading \n", k, i);
			unlock_for_reading();
			//printf("thread: %d, %d currently writing \n", k, i);
		 	lock_for_writing();
         	 	//critical section
	 	 	dist[i][j] = dist[i][k] + dist[k][j];
			//printf("thread: %d, %d currently unlocked for writing \n", k, i);
			unlock_for_writing();
			}
		else{
			//printf("thread: %d, %d currently unlocked for reading \n", k, i);
			unlock_for_reading();
		}
        }
}


//Implementing multi threaded floyd warshall algorithm
void multiFloydWarshall(int graph[n][n]) {
	clock_t begin = clock();
	int i, j, k, b, v, c;

  //adding to dist
  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++)
      dist[i][j] = graph[i][j];


  //main loop
  // Adding vertices individually
  for (k = 0; k < n; k++) {

  pthread_t threads[n];
  pthread_mutex_t mutexLocks[n];
  struct mystruct myvar[n];

     for (i = 0; i < n; i++) {

	myvar[i].a = k;
	myvar[i].b = i;

	//multithreading stuff
	pthread_create(&threads[i],NULL ,jLoop, &myvar[i]);

    }
    //for mutual exclusion
    for (i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }
  }
  multiPrintMatrix();

  printf("============================================\n");
	printf("Program ended successfully.\n");
	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("\nTime it took to complete the program: %f\n", time_spent);
	printf("============================================\n");

}



void printMatrix(int matrix[n][n]) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (matrix[i][j] == INF)
        printf("%4s", "INF");
      else
        printf("%4d", matrix[i][j]);
    }
    printf("\n");
  }
}


void multiPrintMatrix(void) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (dist[i][j] == INF)
        printf("%4s", "INF");
      else
        printf("%4d", dist[i][j]);
    }
    printf("\n");
  }


}
void changeDistSize(void) {

	dist = malloc( sizeof *dist * n );
	if ( dist )
	{
 	for ( size_t i = 0; i < n; i++ )
   	dist[i] = malloc( sizeof *dist[i] * n );
	}
}

void automatic(void) {
	printf("Give value for n x n matrix (MULTITHREADED VERSION): ");
	scanf("%d", &n);
	srand(time(NULL));
	//m = random number between 4 - 10
	m = (rand() % 7) + 4;
	printf("%d \n", m);
	printf("%d x %d matrix with %d weighted edges created", n, n, m);
	printf("\n");

	int distMulti[n][n];
	int inputMulti = 0;

	//change size of global matrix dist
	changeDistSize();

	for(int k = 0; k < n; k++){
		for(int i = 0; i < n; i++){
			if(k == i){
				distMulti[k][i] = 0;
			}
			else{
				distMulti[k][i] = INF;
			}
		}
	}
	int u, v, w;

	for(int i = 0; i < m; i++){
		//random numbers between 0-n, where n is the max size of matrix.
		u = (rand() % n-1);
		v = (rand() % n-1);
		//random numbers between 0-10
		w = (rand() % 11);
		//if the edge goes to the same node, make the nodes different
		if(u == v){
			v = v + 1;
		}
		while(w < 0){
			printf("w cannot have a negative weight\n");
			printf("Give values u, v, and w for Node u to Node v which has a weight of w: ");
			scanf("%d %d %d", &u, &v, &w);
		}

		printf("Node %d to Node %d has a weight of %d \n", u, v, w);
		distMulti[u-1][v-1] = w;
		distMulti[v-1][u-1] = w;
	}

	printf("The %d by %d matrix you input is: \n", n, n);
	printf("\n=========Multi Thread calculation========\n");
	printMatrix(distMulti);
	printf("Floyd Warshall algorithm gives us: \n");


	multiFloydWarshall(distMulti);

}


int main() {
	//MULTITHREADED F.M
    while(1){
        printf("randomized inputs? (type y/n/q): ");
        scanf("%s", answer);

        if(answer[0] == 'y') {
            automatic();
        }
        else if(answer[0] == 'q') {
            break;
        }
        else {
            printf("Give value n for n x n matrix and m value for undirected edges(MULTITHREADED VERSION): ");
            scanf("%d %d", &n, &m);
            printf("%d x %d matrix with %d weighted edges created", n, n, m);
            printf("\n");
            int distMulti[n][n];
            int inputMulti = 0;

            //change size of global matrix dist
            changeDistSize();

            for(int k = 0; k < n; k++){
                for(int i = 0; i < n; i++){
                    if(k == i){
                        distMulti[k][i] = 0;
                    }
                    else{
                        distMulti[k][i] = INF;
                    }
                }
            }
            int u, v, w;

            for(int i = 0; i < m; i++){
                printf("Give values u, v, and w for Node u to Node v which has a weight of w: ");
                scanf("%d %d %d", &u, &v, &w);

                while(w < 0){
                    printf("w cannot have a negative weight\n");
                    printf("Give values u, v, and w for Node u to Node v which has a weight of w: ");
                    scanf("%d %d %d", &u, &v, &w);
                }

                printf("Node %d to Node %d has a weight of %d \n", u, v, w);
                distMulti[u-1][v-1] = w;
                distMulti[v-1][u-1] = w;
            }

            printf("The %d by %d matrix you input is: \n", n, n);
            printf("\n=========Multi Thread calculation========\n");
            printMatrix(distMulti);

            printf("Floyd Warshall algorithm gives us: \n");
            multiFloydWarshall(distMulti);
        }
    }
}


