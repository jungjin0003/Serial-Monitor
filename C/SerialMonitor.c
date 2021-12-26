#include "SerialMonitor.h"

HANDLE GetSerialPortHandleByPortNumber(DWORD PortNumber, DWORD BaudRate)
{
    WCHAR DeviceName[16];

    swprintf(DeviceName, 16, L"\\\\.\\COM%d", PortNumber);

    HANDLE SerialPort = CreateFileW(DeviceName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (SerialPort == INVALID_HANDLE_VALUE)
    {
        printf("CreateFileW Failed\n");
        printf("GetLastError : %d\n", GetLastError());
        return NULL;
    }

    DCB state;

    if (GetCommState(SerialPort, &state) == FALSE)
    {
        printf("GetCommState Failed\n");
        printf("GetLastError : %d\n", GetLastError());
        CloseHandle(SerialPort);
        return NULL;
    }

    state.DCBlength = sizeof(DCB);
    state.BaudRate = BaudRate;
    state.ByteSize = 8;
    state.Parity = NOPARITY;
    state.StopBits = ONESTOPBIT;

    if (SetCommState(SerialPort, &state) == FALSE)
    {
        printf("SetCommState Failed\n");
        printf("GetLastError : %d\n", GetLastError());
        CloseHandle(SerialPort);
        return NULL;
    }

    COMMTIMEOUTS timeout = { 0 };
    timeout.ReadIntervalTimeout = 50;
    timeout.ReadTotalTimeoutConstant = 50;
    timeout.ReadTotalTimeoutMultiplier = 10;
    timeout.WriteTotalTimeoutConstant = 50;
    timeout.WriteTotalTimeoutMultiplier = 10;

    if (SetCommTimeouts(SerialPort, &timeout) == FALSE)
    {
        printf("SetCommTimeouts Failed\n");
        printf("GetLastError : %d\n", GetLastError());
        CloseHandle(SerialPort);
        return NULL;
    }

    return SerialPort;
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