#pragma once
#pragma warning(disable:4996)
#include <time.h>
#include <stdio.h>
#include <malloc.h>
#include "Screen.h"
typedef struct FPSData
{
	int FrameCnt;
	char* FPSTextBuffer;
	clock_t FPSOldTime;
}FPSData;

void FPSInit(FPSData* FPSData);
void DrawFPS(FPSData* FPSData);
void DestroyFPSData(FPSData* FPSData);