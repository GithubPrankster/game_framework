#include <stdio.h>
#pragma warning (disable:4996)
#include <conio.h>

int main(void)
{
	int a;
	
	a = _getch();
	if ( a == 'a' )
		printf("�¾ƿ�");
	else
		printf("Ʋ����");
	return 0;
}