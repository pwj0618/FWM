// Floyd-Warshall Algorithm in C

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#define INF 999
int n, m;

// y/n strings
char answer[1];

void printMatrix(int matrix[n][n]);

// Implementing floyd warshall algorithm
void floydWarshall(int graph[n][n]) {
  int matrix[n][n], i, j, k;

  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++)
      matrix[i][j] = graph[i][j];

  // Adding vertices individually
  for (k = 0; k < n; k++) {
    for (i = 0; i < n; i++) {
      for (j = 0; j < n; j++) {
        if (matrix[i][k] + matrix[k][j] < matrix[i][j])
          matrix[i][j] = matrix[i][k] + matrix[k][j];
      }
    }
  }
  printMatrix(matrix);
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


void automatic(void) { 
	printf("Give value for n x n matrix (SINGLETHREADED VERSION): ");
	scanf("%d", &n);
	srand(time(NULL));
	//m = random number between 4 - 10
	m = (rand() % 7) + 4;
	printf("%d \n", m);
	printf("%d x %d matrix with %d weighted edges created", n, n, m);	
	printf("\n");
		
	int distMulti[n][n];
	int inputMulti = 0;
	
	
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
	printf("\n=========Single Thread calculation========\n");
	printMatrix(distMulti);
	printf("Floyd Warshall algorithm gives us: \n");
	
	
	clock_t begin = clock();
	floydWarshall(distMulti);
	
	
	printf("============================================\n");
	printf("Program ended successfully.\n");
	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("\nTime it took to complete the program: %f\n", time_spent);
	printf("============================================");

}

int main() {
	
	printf("randomized inputs? (type y/n): ");
	scanf("%s", answer);
	
	if(answer[0] == 'y') {
		automatic();
	}
	else{
				printf("Give value n for n x n matrix and m value for undirected edges: ");
		scanf("%d %d", &n, &m);
		printf("%d x %d matrix with %d weighted edges created", n, n, m);	
		printf("\n");
		int dist[n][n];
		int input = 0;
		for(int i = 0; i < n; i++){
			for(int j = 0; j < n; j++){
				if(i == j){
					dist[i][j] = 0;
				}
				else{
					dist[i][j] = INF;
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
			dist[u-1][v-1] = w;
			dist[v-1][u-1] = w;
		}
		
		printf("The %d by %d matrix you input is: \n", n, n);
		
		printf("\n=========Single Thread calculation========\n");
		printMatrix(dist);
		printf("Floyd Warshall algorithm gives us: \n");
		
		clock_t begin = clock();
		floydWarshall(dist);
		
		
		printf("============================================\n");
		printf("Program ended successfully.\n");
		clock_t end = clock();
		double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
		printf("\nTime it took to complete the program: %f\n", time_spent);
		printf("============================================");
	}
}