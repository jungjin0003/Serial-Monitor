#include "SerialMonitor.h"

VOID Sender(HANDLE hComm)
{

}

VOID Receiver(HANDLE hComm)
{
    while (TRUE)
    {
        CHAR Buffer[128] = { 0, };
        if (SerialRead(hComm, Buffer, 128) > 0)
        {
            EnterCriticalSection(&ConsoleLock);
            printf("%s", Buffer);
            LeaveCriticalSection(&ConsoleLock);
        }
    }
}

VOID SerialMonitor(DWORD ComNumber)
{
    HANDLE hComm = GetCommHandleByComNumber(ComNumber);
    InitializeCriticalSection(&ConsoleLock);
}

HANDLE GetCommHandleByComNumber(DWORD ComNumber)
{
    WCHAR CommName[16];

    swprintf(CommName, 16, L"\\\\.\\COM%d", ComNumber);

    HANDLE hComm = CreateFileW(CommName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hComm == INVALID_HANDLE_VALUE)
    {
        printf("CreateFileW Failed\n");
        printf("GetLastError : %d\n", GetLastError());
        return INVALID_HANDLE_VALUE;
    }

    DCB state;

    if (GetCommState(hComm, &state) == FALSE)
    {
        printf("GetCommState Failed\n");
        printf("GetLastError : %d\n", GetLastError());
        CloseHandle(hComm);
        return INVALID_HANDLE_VALUE;
    }

    state.DCBlength = sizeof(DCB);
    state.BaudRate = CBR_115200;
    state.ByteSize = 8;
    state.Parity = NOPARITY;
    state.StopBits = ONESTOPBIT;

    if (SetCommState(hComm, &state) == FALSE)
    {
        printf("SetCommState Failed\n");
        printf("GetLastError : %d\n", GetLastError());
        CloseHandle(hComm);
        return INVALID_HANDLE_VALUE;
    }

    COMMTIMEOUTS timeout = { 0 };
    timeout.ReadIntervalTimeout = 50;
    timeout.ReadTotalTimeoutConstant = 50;
    timeout.ReadTotalTimeoutMultiplier = 10;
    timeout.WriteTotalTimeoutConstant = 50;
    timeout.WriteTotalTimeoutMultiplier = 10;

    if (SetCommTimeouts(hComm, &timeout) == FALSE)
    {
        printf("SetCommTimeouts Failed\n");
        printf("GetLastError : %d\n", GetLastError());
        CloseHandle(hComm);
        return INVALID_HANDLE_VALUE;
    }

    return hComm;
}

BOOL SetBaudRate(HANDLE hComm, DWORD BaudRate)
{
    DCB state;

    if (GetCommState(hComm, &state) == FALSE)
    {
        return FALSE;
    }

    state.BaudRate = BaudRate;

    if (SetCommState(hComm, &state) == FALSE)
    {
        return FALSE;
    }

    return TRUE;
}

DWORD SerialWrite(HANDLE hComm, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite)
{
    DWORD NumberOfBytesWritten;

    if (WriteFile(hComm, lpBuffer, nNumberOfBytesToWrite, &NumberOfBytesWritten, NULL) == FALSE)
    {
        return 0;
    }
    
    return NumberOfBytesWritten;
}

DWORD SerialRead(HANDLE hComm, LPVOID lpBuffer, DWORD nNumberOfBytesToRead)
{
    DWORD NumberOfBytesRead;

    if (ReadFile(hComm, lpBuffer, nNumberOfBytesToRead, &NumberOfBytesRead, NULL) == FALSE)
    {
        return 0;
    }

    return NumberOfBytesRead;
}