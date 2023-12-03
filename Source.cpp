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
#include <vector>
#include "config_file.h"

using namespace std;

int DisplayLength, DisplayHeight, GameLength, GameHeight;

vector<string> ln = { "GameHeight", "GameLength", "DisplayLength", "DisplayHeigth" };

wstring processName = L"cs2.exe";
wstring csgo = L"steam://rungameid/730";

//������ DefaultResolution(�������) � GameResolution(����) �������� ������ �� ��������� ����������(��� ��� �������,
//����� �����, ������ ��������� ������� � ������� ������)

void NoConfig()  //�������� ��������������� �������
{
    ofstream f_out("config.cfg");

    GameLength = 1920, GameHeight = 1080, DisplayLength = 1920, DisplayHeight = 1080;

    CFG::WriteFile(f_out, ln, GameLength, GameHeight, DisplayLength, DisplayHeight);
    f_out.close();
}

void DefaultResolution()
{
    ifstream f_in("config.cfg");

    CFG::ReadFile(f_in, ln, GameLength, GameHeight, DisplayLength, DisplayHeight);
    f_in.close();

    DEVMODE dm;
    dm.dmSize = sizeof(DEVMODE);
    dm.dmPelsWidth = DisplayLength;
    dm.dmPelsHeight = DisplayHeight;
    dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
    ChangeDisplaySettings(&dm, CDS_UPDATEREGISTRY);
}
void GameResolution()
{
    ifstream f_in("config.cfg");

    CFG::ReadFile(f_in, ln, GameLength, GameHeight, DisplayLength, DisplayHeight);
    f_in.close();

    DEVMODE dm;
    dm.dmSize = sizeof(DEVMODE);
    dm.dmPelsWidth = GameLength;
    dm.dmPelsHeight = GameHeight;
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

void Menu()
{
    ifstream f_in("config.cfg");

    CFG::ReadFile(f_in, ln, GameLength, GameHeight, DisplayLength, DisplayHeight);
    f_in.close();

    cout << "���� ������������:" << endl;
    cout << endl;
    cout << "1. ���������� ���� (" << GameLength << "x" << GameHeight << ")" << endl;
    cout << "2. ���������� �������� ����� (" << DisplayLength << "x" << DisplayHeight << ")" << endl;
    cout << endl;
    cout << "������� ����� ������ �������, ����� ��������� ����." << endl;
}

void GameChanger()
{
    
    ifstream f_in("config.cfg");

    CFG::ReadFile(f_in, ln, GameLength, GameHeight, DisplayLength, DisplayHeight);
    f_in.close();

    system("cls");
    cout << "������ ����������: " << GameLength << "x" << GameHeight << endl;
    cout << endl;
    cout << "�����: " << endl;
    cin >> GameLength;
    cout << "������: " << endl;
    cin >> GameHeight;

    ofstream f_out("config.cfg");
    CFG::WriteFile(f_out, ln, GameLength, GameHeight, DisplayLength, DisplayHeight);
    f_out.close();
}

void DisplayChanger()
{
    ifstream f_in("config.cfg");

    CFG::ReadFile(f_in, ln, GameLength, GameHeight, DisplayLength, DisplayHeight);
    f_in.close();

    system("cls");
    cout << "������ ����������: " << DisplayLength << "x" << DisplayHeight << endl;
    cout << endl;
    cout << "�����: " << endl;
    cin >> DisplayLength;
    cout << "������: " << endl;
    cin >> DisplayHeight;

    ofstream f_out("config.cfg");
    CFG::WriteFile(f_out, ln, GameLength, GameHeight, DisplayLength, DisplayHeight);
    f_out.close();
}

int main()
{
    setlocale(LC_ALL, "rus"); 

    char a = '1';

    while (a == '1' or a == '2')
    {
        ifstream f_in("config.cfg");

        if (!f_in)
        {
            NoConfig();
        }
        else
        {
            CFG::ReadFile(f_in, ln, GameLength, GameHeight, DisplayLength, DisplayHeight);
            f_in.close();
        }        

        system("cls");
        Menu();

        a = _getch();

        if (a == '1') GameChanger();
        if (a == '2') DisplayChanger();
    }

    if (!IsProcessRunning(processName)) //���� ���� �� ��������, �� � ���������, ����� ����������
    {
        GameResolution();  //������ ���������� ����� �������� ����

        system("cls");

        ShellExecute(0, 0, L"steam://rungameid/730", 0, 0, SW_SHOW);  //������ ����

        Sleep(1000 * 10); //�����, ����� ���� ������ �����������, ���� �� ������� ��������, �� ��������� �������� ���������
    }

    bool game = true;

    while (game)
    {

        if (IsProcessRunning(processName))  //���� ������� ����, ���� ��� ��������, �� ��������� ��������
        {
            wcout << L"CS2 ��������." << endl;
        }
        else
        {
            game = false;
        }
        Sleep(1000 * 2);  //��������� ��������, ����� ������ �� ����������� ���������
        system("cls");
    }

    //��� ������ �� ��������� ����, �������� ����� ����������� ����������

    DefaultResolution();
    system("cls");
    cout << "�� ������� ����." << endl;

    system("pause");
    return 0;
}