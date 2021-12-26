#pragma once

#include <iostream>
#include <windows.h>

class SerialPort
{
private:
    HANDLE hComm = NULL;
    DWORD Number = 0;
    DCB dcb = { 0, };
    COMMTIMEOUTS CommTimeouts = { 50, 10, 50, 10, 50};

public:
    SerialPort();
    SerialPort(DWORD Number);
    SerialPort(DWORD Number, DWORD BaudRate);
    SerialPort(DWORD Number, LPDCB lpDCB);

    ~SerialPort();

    BOOL Connect();
    BOOL Connect(DWORD Number);
    VOID Disconnect();

    BOOL isConnected();

    VOID SetSerialPortNumber(DWORD Number);
    DWORD GetSerialPortNumber();

    HANDLE GetHandle();

    BOOL SetState(LPDCB lpDCB);
    BOOL GetState(LPDCB lpDCB);

    BOOL SetTimeouts(LPCOMMTIMEOUTS lpCommTimeouts);
    BOOL GetTimeouts(LPCOMMTIMEOUTS lpCommTimeouts);

    BOOL SetBaudRate(DWORD BaudRate);
    DWORD GetBaudRate();

    BOOL SetByteSize(BYTE ByteSize);
    BYTE GetByteSize();

    BOOL SetParity(BYTE Parity);
    BYTE GetParity();

    BOOL SetStopBits(BYTE StopBits);
    BYTE GetStopBits();

    size_t write(BYTE byte);
    size_t write(LPCSTR string);
    size_t write(LPCWSTR string);
    size_t write(PVOID buffer, DWORD nNumberOfBytesToWrite);

    BYTE read();
    size_t read(PVOID buffer, DWORD nNumberOfBytesToRead);
};