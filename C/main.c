#include "SerialMonitor.h"

int main()
{
    HANDLE SerialPort = GetSerialPortHandleByPortNumber(3, CBR_115200);

    if (SerialPort == NULL)
    {
        printf("Error");
        return 0;
    }

    while (TRUE)
    {
        char Buffer[32] = { 0, };
        if (RecvData(SerialPort, Buffer, 32) == FALSE)
        {
            printf("False\n");
        }
        else
        {
            printf("%s", Buffer);
        }
    }

    /* const char *Device = "\\\\.\\COM3";

    HANDLE SerialPort = CreateFileA(Device, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (SerialPort == INVALID_HANDLE_VALUE)
    {
        printf("CreateFileA Failed!\n");
        printf("GetLastError : %d\n", GetLastError());
        return -1;
    }

    DCB state = { 0, };

    GetCommState(SerialPort, &state);

    state.DCBlength = sizeof(DCB);
    state.BaudRate = 115200;
    state.ByteSize = 8;
    state.Parity = NOPARITY;
    state.StopBits = ONESTOPBIT;

    if (SetCommState(SerialPort, &state) == FALSE)
    {
        printf("SetCommState Failed!\n");
        printf("GetLastError : %d\n", GetLastError());
        return -1;
    }

    // Set timeouts
    COMMTIMEOUTS timeout = { 0 };
    timeout.ReadIntervalTimeout = 50;
    timeout.ReadTotalTimeoutConstant = 50;
    timeout.ReadTotalTimeoutMultiplier = 50;
    timeout.WriteTotalTimeoutConstant = 50;
    timeout.WriteTotalTimeoutMultiplier = 10;

    SetCommTimeouts(SerialPort, &timeout);

    while (TRUE)
    {
        char buffer[32] = { 0, };
        DWORD NumberOfBytesRead;
        if (ReadFile(SerialPort, buffer, 32, &NumberOfBytesRead, NULL))
            printf("%s", buffer);
    } */
}