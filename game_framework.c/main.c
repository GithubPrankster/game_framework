#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <malloc.h>
#include <string.h>
#include "Screen.h"
#include "FPS.h"
#pragma warning(disable:4996)
#define GAME_MAX_WIDTH 40

FPSData FPS;

//char PLAYER_STR[] = "└─●─┘";
char PLAYER_STR[] = "l__O__l";
typedef struct Position
{
	int x, y;
}Position;

typedef struct
{
	Position position;
	Position center;
	char* strPlayer; // = "└─●─┘"; 주인공 캐릭터
	int nLength; // 주인공 캐릭터 전체 길이
}Player;

typedef struct Ball
{
	int isReady;
	Position position;
	clock_t moveTime;
	clock_t oldTime;
}Ball;

typedef struct GoalPost
{
	Position position;
	int nLength;
	int nLineX[7];
	int nDist;
	clock_t moveTime;
	clock_t oldTime;
}GoalPost;

Player g_Player;
Ball g_Ball;
GoalPost g_Post;
void Init()
{
	FPSInit(&FPS);
	
	//player 초기화
	g_Player.position.x = 4;
	g_Player.position.y = 22;
	g_Player.center.x = 4;
	g_Player.center.y = 0;

	g_Player.nLength = strlen(PLAYER_STR);

	g_Player.strPlayer = (char*)malloc(sizeof(char) * g_Player.nLength);
	strcpy(g_Player.strPlayer, PLAYER_STR);

	//ball 초기화
	g_Ball.isReady = 1;
	g_Ball.position.x = g_Player.position.x-1;
	g_Ball.position.y = g_Player.position.y-1;
	g_Ball.moveTime = 100;
	
	//GoalPost 초기화
	g_Post.position.x = 20;
	g_Post.position.y = 3;
	g_Post.nLength = 1;
	g_Post.moveTime = 100;
	g_Post.oldTime = clock();
	g_Post.nDist = 1;
	
	int nLength = g_Post.nLength * 2 + 1;
	//for문 수정
	int i = 0;
	for (; i < nLength; i++)
	{
		//원래는 이건데, 나는 칼럼이 아니라 --을 사용할 거니 *2를 안할거야 
		//g_Post.nLineX[i] = g_Post.position.x + 2 * (i + 1);
		// -을 사용할까.. -을 두 개 써서 --을 사용할까?	
		// --을 써도.. 여기서 *2는 안하는 게 맞지..? 
		g_Post.nLineX[i] = 	g_Post.position.x + i + 1; 
	}
}

void Update()
{
	//공의 이동처리
	clock_t Curtime = clock();
	if (g_Ball.isReady == 0)
	{
		if ((Curtime - g_Ball.oldTime) > g_Ball.moveTime)
		{
			if (g_Ball.position.y > 0)
			{
				g_Ball.position.y--;
				g_Ball.oldTime = Curtime;
			}
			else
			{
				g_Ball.isReady = 1;
				g_Ball.position.x = g_Player.position.x;
				g_Ball.position.y = g_Player.position.y;
			}
		}
	}
	else
	{
		g_Ball.position.x = g_Player.position.x - 1;
		g_Ball.position.y = g_Player.position.y - 1;

	}
	
	
	//goal post 이동처리
	clock_t CurTime = clock();
	if (CurTime - g_Post.oldTime > g_Post.moveTime)
	{
		int nLength = g_Post.nLength * 2 + 1;
		g_Post.oldTime = CurTime;
		if (g_Post.position.x + g_Post.nDist >= 0 && ((g_Post.nLineX[nLength - 1] + 3) + g_Post.nDist) <= GAME_MAX_WIDTH)
		{
			g_Post.position.x += g_Post.nDist;
			
			//for문 수정 
			int i = 0;
			for(;i < nLength ; i++)
			{
				g_Post.nLineX[i] = g_Post.position.x + 1 * (i+1);
			}
		}
		else
		{
			g_Post.nDist *= -1;
		}
	}
}

void Render()
{
	ScreenClear();
	// 출력코드
	DrawFPS(&FPS);
	char string[100] = { 0, };
	sprintf(string, "주인공 이동좌표 : %d, %d", g_Player.position.x, g_Player.position.y);
	ScreenPrint(0, 1, string);
	//ScreenPrint(0, 0, FPS.FPSTextBuffer);

	

	//player render
	int printX = g_Player.position.x - g_Player.center.x;
	if (printX < 0)
	{
		printX = 0;
		g_Player.position.x = 4;
		//원래는 위에 두줄 대신
		//이거 한줄만 넣기
		//ScreenPrint(0, g_Player.position.y, &g_Player.strPlayer[printX*-1]);
		ScreenPrint(printX, g_Player.position.y, g_Player.strPlayer);
	}
	else if (g_Player.position.x + g_Player.center.x + 1 > 40)
	{
		//원래는 이거 두줄 넣기
		//strncat(string, g_Player.strPlayer, g_Player.nLength - ((g_Player.position.x + g_Player.center.x + 1) - 40));
		//ScreenPrint(printX, g_Player.position.y, string);
	
		printX = 35;
		g_Player.position.x = 35;
		ScreenPrint(printX, g_Player.position.y, g_Player.strPlayer);
	}
	else
	{
		ScreenPrint(printX, g_Player.position.y, g_Player.strPlayer);
	}

	//공 화면 그리기
	ScreenPrint(g_Ball.position.x, g_Ball.position.y, "@");

	// goal post render
	ScreenPrint(g_Post.position.x, g_Post.position.y, "ㅁ");
	int nLength = g_Post.nLength * 2 + 1;
	//for문 수정
	int i = 0;
	for (; i < nLength; i++)
	{
		ScreenPrint(g_Post.nLineX[i], g_Post.position.y, "-");
	}
	ScreenPrint(g_Post.nLineX[nLength - 1] + 1, g_Post.position.y, "ㅁ");
	
	
	// screen flipping	
	ScreenFlipping();
}
void Release()
{
	DestroyFPSData(&FPS);
}
void WaitRender(clock_t OldTime)
{
	clock_t CurTime;
	while ( 1 )
	{
		CurTime = clock();
		if ( CurTime - OldTime > 33 )
			break;
	}
}
int GetKeyEvent()
{
	if ( _kbhit() )
		return _getch();

	return -1;
}
void KeyProcess(int Key)
{
	switch ( Key )
	{
	case 'j':
		g_Player.position.x--;
		break;
	case 'l':
		g_Player.position.x++;
		break;
	case 'k':
		if (g_Ball.isReady)
		{
			g_Ball.position.x = g_Player.position.x-1;
			g_Ball.position.y = g_Player.position.y;
			g_Ball.oldTime = clock();
			g_Ball.isReady = 0;
		}
		break;
	}
}

int main()
{
	ScreenInit();
	Init();

	while ( 1 )
	{
		int nKey = GetKeyEvent();
		if ( nKey == 'q' )
			break;
		KeyProcess(nKey);

		Update();
		Render();

		WaitRender(clock());
	}

	Release();
	ScreenRelease();
	return 0;
}
