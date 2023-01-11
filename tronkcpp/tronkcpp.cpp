#include <Windows.h>
#include <iostream>
#include <chrono>
using namespace std;
using namespace std::chrono;

void click(INPUT in[])
{
    SendInput(2, in, sizeof(INPUT));

    return;
}

int main()
{
    //init click
    INPUT inputs[2] = {};
    ZeroMemory(inputs, sizeof(inputs));

    inputs[0].type = INPUT_MOUSE;
    inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

    inputs[1].type = INPUT_MOUSE;
    inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;

    // screen res
    RECT scrRect;
    HWND hCurrent = GetDesktopWindow();
    GetWindowRect(hCurrent, &scrRect);
    int sHeight = scrRect.bottom - scrRect.top;
    int sWidth = scrRect.right - scrRect.left;

    HDC hdcCurrent = CreateDCA("DISPLAY", NULL, NULL, NULL);

    //init color vars
    int threshold = 30;
    COLORREF spx = GetPixel(hdcCurrent, sWidth / 2, sHeight / 2);
    COLORREF cpx = GetPixel(hdcCurrent, sWidth / 2, sHeight / 2);
    byte rgbc[3] = {GetRValue(cpx), GetGValue(cpx) , GetBValue(cpx)}; // RGB CURRENT
    byte rgbs[3] = { GetRValue(spx), GetGValue(spx) , GetBValue(spx) }; // RGB START

    for (;;)
    {
        if (GetAsyncKeyState(0x54))// key t down
        {
            spx = GetPixel(hdcCurrent, sWidth / 2, sHeight / 2);
            rgbs[0] = GetRValue(spx);
            rgbs[1] = GetBValue(spx);
            rgbs[2] = GetGValue(spx);

            while (GetAsyncKeyState(0x54))// key t down
            {
                auto start = high_resolution_clock::now();
                cpx = GetPixel(hdcCurrent, sWidth / 2, sHeight / 2);
                rgbc[0] = GetRValue(cpx);
                rgbc[1] = GetBValue(cpx);
                rgbc[2] = GetGValue(cpx);

                if (rgbs[0] > (rgbc[0] + threshold) || rgbs[0] < (rgbc[0] - threshold) || rgbs[1] > (rgbc[1] + threshold) || rgbs[1] < (rgbc[1] - threshold) || rgbs[2] > (rgbc[2] + threshold) || rgbs[2] < (rgbc[2] - threshold))
                {
                    click(inputs);

                    auto stop = high_resolution_clock::now();
                    auto duration = duration_cast<microseconds>(stop - start);

                    cout << "Time taken by function: " << duration.count() << " microseconds" << endl;
                    Sleep(500);
                    main();
                }
            }
        }
        Sleep(1);
    }
}

//