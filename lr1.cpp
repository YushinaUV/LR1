
#pragma warning(disable : 4996)

#define WINVER 0x0A00
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <locale.h>

using namespace std;

#define BUFSIZE MAX_PATH
#define FILESYSNAMEBUFSIZE MAX_PATH
#define INFO_BUFFER_SIZE 32767
#define MAX_KEY_LENGTH 255

//2. Измерение производительности ЦП Замер рабочей частоты f
double PCFreq = 0.0;
__int64 CounterStart = 0;

void StartCounter()
{
    LARGE_INTEGER AS;
    if (!QueryPerformanceFrequency(&AS))
        cout << "Function QueryPerformanceFrequency() failed!\n";

    PCFreq = double(AS.QuadPart);

    printf("\n2.1.  CPU frequency: %u  Hz\n", AS);
    QueryPerformanceCounter(&AS);
    CounterStart = AS.QuadPart;
}

//Подсчет кол-ва тактов ЦП, которе занимает исполнение предыдущего пункта
double GetCounter()
{
    LARGE_INTEGER AS;
    QueryPerformanceCounter(&AS);
    return double ((AS.QuadPart - CounterStart)* 1000000) / PCFreq;
}

int main()
{

    // 1.1
      // Версия операционнной системы
     OSVERSIONINFOA ver_info {};
    ver_info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);
    test_status_print(GetVersionEx(&ver_info),
                      " occured during determining OS version!",
                      "VERSIA OPERTSIONNOI SISTEMI: ",
                      ver_info.dwMajorVersion, '.', ver_info.dwMinorVersion, '\n');
    std::cout << '\n';

    // 1.2. Определение и вывод системных каталогов
 GetSystemDirectory(infoBuf, sizeof(infoBuf));
    printf("Sistemnie katalogi: %s\n", infoBuf);

    // 1.3. имя пользователя и компьютера
     char user_name[MAX_LENGTH];
    char pc_name[MAX_LENGTH];
    DWORD sz_user = sizeof(user_name);
    DWORD sz_pc = sizeof(pc_name);
    test_status_print(GetUserName(user_name, &sz_user),
                      " occured during determining username!",
                      "ima polzovatela: ", user_name, '\n');
    std::cout << '\n';
    test_status_print(GetComputerName(pc_name, &sz_pc),
                      " occured during determining PC name!",
                      "ima compuctera: ", pc_name, '\n');
    std::cout << '\n';






    //1.4. Вывод данных о томах
    char buffer[MAX_PATH +1];
    DWORD  CBufLen = MAX_PATH;
    char Names[MAX_PATH +1];
    CHAR VolumeName[MAX_PATH];
    __int64 total, available, free;


    HANDLE search = FindFirstVolume(VolumeName, BUFSIZE);
    printf("\n      %s", VolumeName);

    do {
        printf("\n      %s", Names);
        GetVolumePathNamesForVolumeName(Names, buffer, CBufLen, &CBufLen);
        char* path = buffer;
        printf("\n      First path: %s", path);
        GetDiskFreeSpaceEx(buffer, (PULARGE_INTEGER)&available, (PULARGE_INTEGER)&total, (PULARGE_INTEGER)&free);
        if (GetDiskFreeSpaceEx(buffer, (PULARGE_INTEGER)&available, (PULARGE_INTEGER)&total, (PULARGE_INTEGER)&free)==0) {
            printf("\n      No information available.\n ");
        }
        else{
        printf("\n      Total size: %u  bytes ", total);
        printf("\n      Free space: %u  bytes\n", available);
        }

    } while (FindNextVolume(search, Names, BUFSIZE));
    FindVolumeClose(search);

    //1.5. Вывод списка программ, которые запускаются  при старте системы из реестра Windows
    HKEY hKey;
    DWORD dwIndex = 0;
    DWORD retCode;
    DWORD BufferSize = 8192;
    PPERF_DATA_BLOCK PerfData = (PPERF_DATA_BLOCK)malloc(BufferSize);
    DWORD cbData = BufferSize;
    DWORD dwSize;
    TCHAR ValueName[MAX_KEY_LENGTH];

    if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_READ, &hKey)== ERROR_SUCCESS)
    {
		printf("\n1.5.  STARTOVIE PROGI :\n");
		while (1)
		{
        dwSize = sizeof(ValueName);
        retCode = RegEnumValue(hKey, dwIndex, ValueName, &dwSize, NULL, NULL, NULL, NULL);
        if (retCode == ERROR_SUCCESS)
        {
            RegQueryValueEx(hKey, ValueName, NULL, NULL, (LPBYTE)PerfData, &cbData);
            printf("      %d: %s:  %s\n", dwIndex + 1, ValueName, PerfData);
            dwIndex++;
        }
        else break;
    }
    RegCloseKey(hKey);
    CounterFreq();
    cout << "2.2.  CPU clock count: " << CounterTakt()  << "  us \n";
    return 0;
}

    RegCloseKey(hKey);

    StartCounter();
    cout << "2.2.  CPU clock count: " << GetCounter()  << "  us \n";
    return 0;
