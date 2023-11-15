#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <Psapi.h>
#include <stdio.h>
#include <string.h>
#include <shellapi.h>
#include <conio.h>
#include <fstream>
#include <ostream>

using namespace std;

int dw, dh, gw, gh;

//методы def(обычный) и game(игра) отвечают только за изменение разрешения(мне так удобнее,
//когда нужно, просто вызываешь функцию в главном методе)

void def()
{

    DEVMODE dm;
    dm.dmSize = sizeof(DEVMODE);
    dm.dmPelsWidth = dw;
    dm.dmPelsHeight = dh;
    dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
    ChangeDisplaySettings(&dm, CDS_UPDATEREGISTRY);
}
void game()
{

    DEVMODE dm;
    dm.dmSize = sizeof(DEVMODE);
    dm.dmPelsWidth = gw;
    dm.dmPelsHeight = gh;
    dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
    ChangeDisplaySettings(&dm, CDS_UPDATEREGISTRY);
}

bool IsProcessRunning(const wstring& processName)
{
    DWORD processIds[1024], bytesNeeded;
    if (!EnumProcesses(processIds, sizeof(processIds), &bytesNeeded))
    {
        return false;
    }

    DWORD processCount = bytesNeeded / sizeof(DWORD);

    for (DWORD i = 0; i < processCount; i++)
    {

        HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processIds[i]);

        if (processHandle)
        {

            WCHAR processFileName[MAX_PATH];
            if (GetModuleBaseNameW(processHandle, NULL, processFileName, sizeof(processFileName) / sizeof(WCHAR)))
            {

                if (processName.compare(processFileName) == 0)
                {
                    CloseHandle(processHandle);
                    return true;
                }
            }

            CloseHandle(processHandle);
        }
    }

    return false;
}

void menu()
{
    cout << "Меню конфигурации:" << endl;
    cout << endl;
    cout << "1. Разрешение игры (" << gw << "x" << gh << ")" << endl;
    cout << "2. Разрешение рабочего стола(" << dw << "x" << dh << ")" << endl;
    cout << endl;
    cout << "Нажмите любую другую клавишу, чтобы запустить игру." << endl;
}

void gameCh()
{
    ofstream foutg("game.txt");

    system("cls");
    cout << "Данное разрешение: " << gw << "x" << gh << endl;
    cout << endl;
    cout << "Длина: " << endl;
    cin >> gw;
    cout << "Ширина: " << endl;
    cin >> gh;

    foutg << gw << " " << gh << endl;

    foutg.close();
}
void dispCh()
{
    ofstream foutd("disp.txt");

    system("cls");
    cout << "Данное разрешение: " << dw << "x" << dh << endl;
    cout << endl;
    cout << "Длина: " << endl;
    cin >> dw;
    cout << "Ширина: " << endl;
    cin >> dh;

    foutd << dw << " " << dh << endl;

    foutd.close();
}

int main()
{
    setlocale(LC_ALL, "rus");

    char a = '1';

    while (a == '1' or a == '2')
    {
        ifstream fing("game.txt");

        fing >> gw;
        fing >> gh;

        fing.close();

        ifstream find("disp.txt");

        find >> dw;
        find >> dh;

        find.close();

        system("cls");
        menu();

        a = _getch();

        if (a == '1') gameCh();
        if (a == '2') dispCh();
    }

    game();  //меняем разрешение перед запуском игры

    system("cls");

    ShellExecute(0, 0, L"steam://rungameid/730", 0, 0, SW_SHOW);  //запуск игры

    Sleep(1000 * 10); //время, чтобы игра успела запуститься, если не сделать задержку, то программа попросту закроется

    bool game = true;

    while (game)
    {
        wstring processName = L"cs2.exe";  //ищем процесс игры, если она запущена, то программа работает

        if (IsProcessRunning(processName))
        {
            wcout << L"CS2 запущена." << endl;
        }
        else
        {
            game = false;
        }
        Sleep(1000 * 2);  //небольшая задержка, чтобы сильно не перегружать программу
        system("cls");
    }

    //как только мы закрываем игру, вызываем метод возвращения разрешения

    def();
    system("cls");
    cout << "Вы закрыли игру." << endl;

    system("pause");
    return 0;
    ExitProcess(0);

}