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

//char PLAYER_STR[] = "�����ܦ���";
char PLAYER_STR[] = "l__O__l";
typedef struct Position
{
	int x, y;
}Position;

typedef struct
{
	Position position;
	Position center;
	char* strPlayer; // = "�����ܦ���"; ���ΰ� ĳ����
	int nLength; // ���ΰ� ĳ���� ��ü ����
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
	
	//player �ʱ�ȭ
	g_Player.position.x = 4;
	g_Player.position.y = 22;
	g_Player.center.x = 4;
	g_Player.center.y = 0;

	g_Player.nLength = strlen(PLAYER_STR);

	g_Player.strPlayer = (char*)malloc(sizeof(char) * g_Player.nLength);
	strcpy(g_Player.strPlayer, PLAYER_STR);

	//ball �ʱ�ȭ
	g_Ball.isReady = 1;
	g_Ball.position.x = g_Player.position.x-1;
	g_Ball.position.y = g_Player.position.y-1;
	g_Ball.moveTime = 100;
	
	//GoalPost �ʱ�ȭ
	g_Post.position.x = 20;
	g_Post.position.y = 3;
	g_Post.nLength = 1;
	g_Post.moveTime = 100;
	g_Post.oldTime = clock();
	g_Post.nDist = 1;
	
	int nLength = g_Post.nLength * 2 + 1;
	//for�� ����
	int i = 0;
	for (; i < nLength; i++)
	{
		//������ �̰ǵ�, ���� Į���� �ƴ϶� --�� ����� �Ŵ� *2�� ���Ұž� 
		//g_Post.nLineX[i] = g_Post.position.x + 2 * (i + 1);
		// -�� ����ұ�.. -�� �� �� �Ἥ --�� ����ұ�?	
		// --�� �ᵵ.. ���⼭ *2�� ���ϴ� �� ����..? 
		g_Post.nLineX[i] = 	g_Post.position.x + i + 1; 
	}
}

void Update()
{
	//���� �̵�ó��
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
	
	
	//goal post �̵�ó��
	clock_t CurTime = clock();
	if (CurTime - g_Post.oldTime > g_Post.moveTime)
	{
		int nLength = g_Post.nLength * 2 + 1;
		g_Post.oldTime = CurTime;
		if (g_Post.position.x + g_Post.nDist >= 0 && ((g_Post.nLineX[nLength - 1] + 3) + g_Post.nDist) <= GAME_MAX_WIDTH)
		{
			g_Post.position.x += g_Post.nDist;
			
			//for�� ���� 
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
	// ����ڵ�
	DrawFPS(&FPS);
	char string[100] = { 0, };
	sprintf(string, "���ΰ� �̵���ǥ : %d, %d", g_Player.position.x, g_Player.position.y);
	ScreenPrint(0, 1, string);
	//ScreenPrint(0, 0, FPS.FPSTextBuffer);

	

	//player render
	int printX = g_Player.position.x - g_Player.center.x;
	if (printX < 0)
	{
		printX = 0;
		g_Player.position.x = 4;
		//������ ���� ���� ���
		//�̰� ���ٸ� �ֱ�
		//ScreenPrint(0, g_Player.position.y, &g_Player.strPlayer[printX*-1]);
		ScreenPrint(printX, g_Player.position.y, g_Player.strPlayer);
	}
	else if (g_Player.position.x + g_Player.center.x + 1 > 40)
	{
		//������ �̰� ���� �ֱ�
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

	//�� ȭ�� �׸���
	ScreenPrint(g_Ball.position.x, g_Ball.position.y, "@");

	// goal post render
	ScreenPrint(g_Post.position.x, g_Post.position.y, "��");
	int nLength = g_Post.nLength * 2 + 1;
	//for�� ����
	int i = 0;
	for (; i < nLength; i++)
	{
		ScreenPrint(g_Post.nLineX[i], g_Post.position.y, "-");
	}
	ScreenPrint(g_Post.nLineX[nLength - 1] + 1, g_Post.position.y, "��");
	
	
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
