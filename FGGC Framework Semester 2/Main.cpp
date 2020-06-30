#include "Application.h"
#include <time.h>
#include "GameTimer.h"
#include <sstream>
#define MS_PER_UPDATE 16.6666666667

GameTimer mTimer;

void GameLoopDelay(float frameStartTime)
{
	float frameProcessingTime, currentFrameTime;

	currentFrameTime = GetTickCount();

	frameProcessingTime = currentFrameTime - frameStartTime;

	if (frameProcessingTime < 16.67f)
	{
		Sleep(16.67f - frameProcessingTime);
	}
}

void CalculateFrameStats(HWND mainWin)
{
	// Code computes the average frames per second, and also the 
	// average time it takes to render one frame.  These stats 
	// are appended to the window caption bar.

	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

	// Compute averages over one second period.
	if ((mTimer.TotalTime() - timeElapsed) >= 1.0f)
	{
		float fps = (float)frameCnt; // fps = frameCnt / 1
	
		float mspf = 1000.0f / fps;

		std::wostringstream outs;
		outs.precision(6);
		outs << L"FPS: " << fps << L"    " << L"Frame Time: " << mspf << L" (ms)";
		SetWindowText(mainWin, outs.str().c_str());

		// Reset for next average.
		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	Application * theApp = new Application();

	if (FAILED(theApp->Initialise(hInstance, nCmdShow)))
	{
		return -1;
	}

    // Main message loop
    MSG msg = {0};

	double _previous = GetTickCount64();
	double _lag = 0.0;


	clock_t current_ticks, delta_ticks;
	clock_t  fps = 0;


	mTimer.Reset();

    while (WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
			bool handled = false;

			if (msg.message >= WM_KEYFIRST && msg.message <= WM_KEYLAST)
			{
				handled = theApp->HandleKeyboard(msg);
			}
			else if (WM_QUIT == msg.message)
				break;

			if (!handled)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
        else
        {
			if (GetAsyncKeyState(VK_ESCAPE))
				exit(0);

			mTimer.Tick();

			float frameStartTime = GetTickCount();

			current_ticks = clock();
			
			CalculateFrameStats(theApp->GetHWND());

			theApp->Update(mTimer.DeltaTime());
		
		
			theApp->Draw();
		
			
			//GameLoopDelay(frameStartTime);
        }
    }

	delete theApp;
	theApp = nullptr;

    return (int) msg.wParam;
}