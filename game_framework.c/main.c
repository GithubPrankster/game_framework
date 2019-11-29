#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <malloc.h>
#include "Screen.h"
#include "FPS.h"
#pragma warning(disable:4996)

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
Player g_Player;

typedef struct Ball
{
	int isReady;
	Position position;
	clock_t moveTime;
	clock_t oldTime;
}Ball;
Ball g_Ball;

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
}
void Render()
{
	ScreenClear();
	// ����ڵ�
	DrawFPS(&FPS);

	char string[100] = { 0, };

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


	sprintf(string, "���ΰ� �̵���ǥ : %d, %d", g_Player.position.x, g_Player.position.y);
	ScreenPrint(0, 3, string);
	//ScreenPrint(0, 0, FPS.FPSTextBuffer);
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
