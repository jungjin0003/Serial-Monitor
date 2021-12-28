#include "SerialMonitor.h"

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

BOOL SetBaudRate(HANDLE SerialPort, DWORD BaudRate)
{
    DCB state;

    if (GetCommState(SerialPort, &state) == FALSE)
    {
        return FALSE;
    }

    state.BaudRate = BaudRate;

    if (SetCommState(SerialPort, &state) == FALSE)
    {
        return FALSE;
    }

    return TRUE;
}

BOOL SendData(HANDLE SerialPort, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite)
{
    DWORD NumberOfBytesWritten = 0;
    BOOL bSuccess = WriteFile(SerialPort, lpBuffer, nNumberOfBytesToWrite, &NumberOfBytesWritten, NULL);

    if (bSuccess == FALSE || NumberOfBytesWritten != nNumberOfBytesToWrite)
        return FALSE;

    return TRUE;
}

BOOL RecvData(HANDLE SerialPort, LPVOID lpBuffer, DWORD BufferSize)
{
    BOOL bSuccess = ReadFile(SerialPort, lpBuffer, BufferSize, NULL, NULL);

    return bSuccess;
}