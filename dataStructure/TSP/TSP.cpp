#include <stdio.h>
#include <time.h>
#include <stdlib.h>


#define MAX_CITY	8
#define INF			90000
#define TRUE		1
#define FALSE		0

// 각 도시간 거리를 저장하는 dist배열과 도시 수 N
int **dist;
int n;

// Linked List Node 구조체
typedef struct n{
	int num;	
	struct n* next;
	struct n* prev;
}Node;

// Linked List 관련 함수
void InitLinkedList(Node* Head, Node* Tail);
Node* CreateNode(int newData);
void PushNode( Node* Tail, Node* newNode);
int ReadPopNode( Node* Tail );
void DelPopNode( Node* Tail );
void Finalize(Node* Head, Node* Tail);
int GetNodeCount(Node* Head, Node* Tail);

// Main 기능 함수
void PrintAll();
void InsertData();
void RandomData();
void LoadData();
void SaveData();
int shortestPath(Node* Head, Node* Tail, int* visited, int currentLength);

// TSP 연산을 위한 함수 (재귀호출)
int shortestPath(Node* Head, Node* Tail, int* visited, int currentLength)
{
	Node* newNode;

	if (GetNodeCount(Head, Tail) == n)
		return currentLength;

	int ret = INF;

	for(int next = 0; next < n ; ++next)
	{
		if(visited[next]) continue;
		int here;

		if(GetNodeCount(Head, Tail) == 0)
			here = next;  
		else
			here = ReadPopNode(Tail);

		newNode = CreateNode(next);
		PushNode(Tail, newNode);

		visited[next] = TRUE;

		int cand = shortestPath(Head, Tail, visited, currentLength + dist[here][next]);
		ret = (ret < cand ? ret : cand);
		visited[next] = FALSE;

		DelPopNode(Tail);
	}
	return ret;
}

// 메뉴 출력
void printMenu()
{
	printf("\n========================================================================\n");
	printf("                  TSP \n");
	printf("========================================================================\n");
	printf("1. Print ALL\n");

	// 모드(입력/연산) 에 따라 출력 메뉴 변경
	if(n == 0)
	{
		printf("2. Insert Data\n");
		printf("3. Random Data Generation\n");
		printf("4. Load Data\n");
	}
	else
	{
		printf("5. Save Data\n");
		printf("6. Resolve\n");
		printf("7. Initialization\n");
	}

	printf("0. Exit\n");
	printf(">> Select Menu : ");
}

// 데이터 테이블 입력
void InsertData()
{
	int i, j;
	char temp[128];
	char *pStr;

	do
	{
		printf("How many City : ");
		scanf("%d", &n);
	}while(n > 8);		// 최대 도시 수 예외처리

	// 2차원 배열 동적할당
	dist = (int**)malloc(sizeof(double*) * n);
	for(i = 0 ; i < n ; i++)
		dist[i] = (int*)malloc(sizeof(double) * n);


	for(i = 0 ; i < n ; i++)
	{
		for(j = 0 ; j < n ; j++)
		{
			if(i == j)
				dist[i][j] = 0;
			else if (i > j)
				dist[i][j] = dist[j][i];
			else
			{
				do
				{
					printf("[ %d ][ %d ] : ", i+1, j+1);
					scanf("%d", &dist[i][j]);
				}while(dist[i][j] <= 0 || dist[i][j] >= 1000);	// 도시 간 거리 예외처리
			}
		}
	}

	printf(":: Success\n");
}

// 랜덤 입력
void RandomData()
{
	int i, j;
	char temp[128];
	char *pStr;


	do
	{
		printf("How many City : ");
		scanf("%d", &n);
	}while(n > 8);
	dist = (int**)malloc(sizeof(double*) * n);
	for(i = 0 ; i < n ; i++)
		dist[i] = (int*)malloc(sizeof(double) * n);


	// 시드값 설정
	srand(time(NULL));

	for(i = 0 ; i < n ; i++)
	{
		for(j = 0 ; j < n ; j++)
		{
			if(i == j)
				dist[i][j] = 0;
			else if (i > j)
				dist[i][j] = dist[j][i];
			else
				dist[i][j] = rand() % 999 + 1;
		}
	}

	printf(":: Success\n");
}

// 파일 저장
void SaveData()
{
	int i, j;
	FILE *fp;

	fp = fopen("question.txt", "w+");
	if(fp == NULL)
	{
		printf(":: File Open Error!\n"); 
		return;
	}

	fprintf(fp, "%d\n", n);

	for(i = 0 ; i < n ; i++)
	{
		for(j = 0 ; j < n ; j++)
		{
			fprintf(fp, "%d", dist[i][j]);
			if(j < n - 1)
				fprintf(fp, "\t");
		}
		fprintf(fp, "\n");
	}

	fclose(fp);
	printf(":: Success\n");

}

// 파일 읽기
void LoadData()
{
	int i, j;
	FILE *fp;

	fp = fopen("question.txt", "r+");
	if(fp == NULL)
	{
		printf(":: File Open Error!\n"); 
		return;
	}

	fscanf(fp, "%d", &n);

	dist = (int**)malloc(sizeof(double*) * n);
	for(i = 0 ; i < n ; i++)
		dist[i] = (int*)malloc(sizeof(double) * n);


	for(i = 0 ; i < n ; i++)
	{
		for(j = 0 ; j < n ; j++)
		{
			fscanf(fp, "%d", &dist[i][j]);
		}
	}

	fclose(fp);
	printf(":: Success\n");
}


// 데이터 테이블 출력
void PrintAll()
{
	int i, j;

	if (n == 0)
		printf(":: Stack is empty \n");

	printf("      ");
	for(i = 0 ; i < n ; i++)
		printf("[%3d]", i+1);

	printf("\n");

	for(i = 0 ; i < n ; i++)
	{
		printf("[%2d]  ", i+1);
		for(j = 0 ; j < n ; j++)
		{
			printf(" %3d ", dist[i][j]);
		}
		printf("\n");
	}
	printf("\n");

}

int main(int argc, char* argv[])
{
	Node pathHead, pathTail;
	int visited[MAX_CITY] = {0,};
	char inputData = 0;

	int i;
	InitLinkedList(&pathHead, &pathTail);

	while(inputData != '0')
	{	

		fflush(stdin);
		printMenu();
		scanf("%c", &inputData);

		printf("\n");

		switch(inputData)
		{
		case '1':
			PrintAll();
			break;			
		case '2':
			if(n > 0) continue;
			InsertData();
			break;
		case '3':
			if(n > 0) continue;
			RandomData();
			break;
		case '4':
			if(n > 0) continue;
			LoadData();
			break;
		case '5':
			if(n == 0) continue;
			SaveData();
			break;
		case '6':
			if(n == 0) continue;
			printf(":: Result : %d\n", shortestPath(&pathHead, &pathTail, visited, 0));
			break;
		case '7':
			if(n == 0) continue;

			for(i = 0 ; i < n ; i++)
				free(dist[i]);
			free(dist);

			n = 0;

			printf(":: Success\n");
			break;

		case '0':
			Finalize(&pathHead, &pathTail);

			if( n > 0)
			{
				for(i = 0 ; i < n ; i++)
					free(dist[i]);
				free(dist);
			}

			break;
		}
	}

	return 0;
}



// [ Linked-List ] 노드 생성
Node* CreateNode(int newData)
{
	Node* newNode = (Node *)malloc(sizeof(Node));

	if(newNode != NULL)
	{
		newNode->num = newData;
		newNode->prev = NULL;
		newNode->next = NULL;
	}

	return newNode;
}

// [ Linked-List ] 노드 추가 
void PushNode( Node* Tail, Node* newNode)
{
	newNode->prev = Tail->prev;

	Tail->prev->next = newNode;
	Tail->prev = newNode;

	newNode->next = Tail;
}

// [ Linked-List ] 노드 읽기
int ReadPopNode( Node* Tail)
{
	return Tail->prev->num;
}

// [ Linked-List ] 노드 삭제
void DelPopNode( Node* Tail )
{
	Node* tmpNode;

	tmpNode = Tail->prev;

	tmpNode->prev->next = Tail;
	Tail->prev = tmpNode->prev;

	free(tmpNode);
}

// [ Linked-List ] 노드 전체 삭제
void Finalize(Node* Head, Node *Tail)
{
	Node* curNode;
	Node* tmp;

	tmp = Head->next;

	while(tmp != Tail)
	{
		curNode = tmp;
		tmp = tmp->next;

		free(curNode);
	}
}

// [ Linked-List ] 노드 카운트
int GetNodeCount(Node* Head, Node* Tail)
{
	Node* tmpNode;
	int cnt = 0;

	tmpNode = Head->next;

	while(tmpNode != Tail)
	{
		cnt++;
		tmpNode = tmpNode->next;
	}

	return cnt;
}

// [ Linked-List ] 초기화 함수 (head, tail 초기화)
void InitLinkedList(Node* Head, Node* Tail)
{
	Head->prev = NULL;
	Head->next = Tail;
	Tail->prev = Head;
	Tail->next = Head;
}
