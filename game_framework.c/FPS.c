#include "FPS.h"

void FPSInit(FPSData* FPSData)
{
	FPSData->FrameCnt = 0;
	FPSData->FPSTextBuffer = (char*)malloc(sizeof(char) * 10);
	sprintf(FPSData->FPSTextBuffer, "FPS:%d", FPSData->FrameCnt);
	FPSData->FPSOldTime = clock();
}

void DrawFPS(FPSData* FPSData)
{
	FPSData->FrameCnt++;
	clock_t FPSCurTime = clock();
	if ( FPSCurTime - FPSData->FPSOldTime >= 1000 )
	{
		sprintf(FPSData->FPSTextBuffer, "FPS:%d", FPSData->FrameCnt);
		FPSData->FPSOldTime = clock();
		FPSData->FrameCnt = 0;
	}
	ScreenPrint(0, 0, FPSData->FPSTextBuffer);
}

void DestroyFPSData(FPSData* FPSData)
{
	free(FPSData->FPSTextBuffer);
	
}
