#include <stdio.h>
#pragma warning (disable:4996)
#include <conio.h>

int main(void)
{
	int a;
	
	a = _getch();
	if ( a == 'a' )
		printf("맞아용");
	else
		printf("틀려용");
	return 0;
}