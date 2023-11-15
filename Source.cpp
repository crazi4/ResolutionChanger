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

//������ def(�������) � game(����) �������� ������ �� ��������� ����������(��� ��� �������,
//����� �����, ������ ��������� ������� � ������� ������)

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
    cout << "���� ������������:" << endl;
    cout << endl;
    cout << "1. ���������� ���� (" << gw << "x" << gh << ")" << endl;
    cout << "2. ���������� �������� �����(" << dw << "x" << dh << ")" << endl;
    cout << endl;
    cout << "������� ����� ������ �������, ����� ��������� ����." << endl;
}

void gameCh()
{
    ofstream foutg("game.txt");

    system("cls");
    cout << "������ ����������: " << gw << "x" << gh << endl;
    cout << endl;
    cout << "�����: " << endl;
    cin >> gw;
    cout << "������: " << endl;
    cin >> gh;

    foutg << gw << " " << gh << endl;

    foutg.close();
}
void dispCh()
{
    ofstream foutd("disp.txt");

    system("cls");
    cout << "������ ����������: " << dw << "x" << dh << endl;
    cout << endl;
    cout << "�����: " << endl;
    cin >> dw;
    cout << "������: " << endl;
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

    game();  //������ ���������� ����� �������� ����

    system("cls");

    ShellExecute(0, 0, L"steam://rungameid/730", 0, 0, SW_SHOW);  //������ ����

    Sleep(1000 * 10); //�����, ����� ���� ������ �����������, ���� �� ������� ��������, �� ��������� �������� ���������

    bool game = true;

    while (game)
    {
        wstring processName = L"cs2.exe";  //���� ������� ����, ���� ��� ��������, �� ��������� ��������

        if (IsProcessRunning(processName))
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

    def();
    system("cls");
    cout << "�� ������� ����." << endl;

    system("pause");
    return 0;
    ExitProcess(0);

}