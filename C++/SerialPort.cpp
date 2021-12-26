#include "SerialPort.hpp"

SerialPort::SerialPort(DWORD Number) : SerialPort(Number, 9600) {}

SerialPort::SerialPort(DWORD Number, DWORD BaudRate)
{
    if (Connect(Number) == FALSE)
        return;

    dcb.BaudRate = BaudRate;

    if (SetState(&dcb) == FALSE)
        return;
}

SerialPort::~SerialPort()
{
    CloseHandle(hComm);
}

BOOL SerialPort::Connect()
{
    WCHAR DeviceName[16];

    swprintf(DeviceName, L"\\\\.\\COM%d", Number);

    if (isConnected())
        CloseHandle(hComm);

    hComm = CreateFileW(DeviceName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hComm == INVALID_HANDLE_VALUE)
        return FALSE;

    if (GetState(&dcb) == FALSE)
        return FALSE;

    if (SetTimeouts(&CommTimeouts) == FALSE)
        return FALSE;

    return TRUE;
}

BOOL SerialPort::Connect(DWORD Number)
{
    this->Number = Number;
    return Connect();
}

VOID SerialPort::Disconnect()
{
    CloseHandle(hComm);
    hComm = NULL;
}

BOOL SerialPort::isConnected()
{
    if (hComm == NULL || hComm == INVALID_HANDLE_VALUE)
        return FALSE;
    return TRUE;
}

VOID SerialPort::SetSerialPortNumber(DWORD Number)
{
    this->Number = Number;

    if (isConnected())
        Disconnect();

    Connect();
}

DWORD SerialPort::GetSerialPortNumber()
{
    return Number;
}

HANDLE SerialPort::GetHandle()
{
    return hComm;
}

BOOL SerialPort::SetState(LPDCB lpDCB)
{
    return SetCommState(hComm, lpDCB);
}

BOOL SerialPort::GetState(LPDCB lpDCB)
{
    return GetCommState(hComm, lpDCB);
}

BOOL SerialPort::SetTimeouts(LPCOMMTIMEOUTS lpCommTimeouts)
{
    return SetCommTimeouts(hComm, lpCommTimeouts);
}

BOOL SerialPort::GetTimeouts(LPCOMMTIMEOUTS lpCommTimeouts)
{
    return GetCommTimeouts(hComm, lpCommTimeouts);
}

BOOL SerialPort::SetBaudRate(DWORD BaudRate)
{
    if (isConnected() == FALSE)
        return FALSE;

    dcb.BaudRate = BaudRate;

    return SetState(&dcb);
}

DWORD SerialPort::GetBaudRate()
{
    return dcb.BaudRate;
}

BOOL SerialPort::SetByteSize(BYTE ByteSize)
{
    if (isConnected() == FALSE)
        return FALSE;

    dcb.ByteSize = ByteSize;

    return SetState(&dcb);
}

BYTE SerialPort::GetByteSize()
{
    return dcb.ByteSize;
}

BOOL SerialPort::SetParity(BYTE Parity)
{
    if (isConnected() == FALSE)
        return FALSE;

    dcb.Parity = Parity;

    return SetState(&dcb);
}

BYTE SerialPort::GetParity()
{
    return dcb.Parity;
}

BOOL SerialPort::SetStopBits(BYTE StopBits)
{
    if (isConnected() == FALSE)
        return FALSE;

    dcb.StopBits = StopBits;

    return SetState(&dcb);
}

BYTE SerialPort::GetStopBits()
{
    return dcb.StopBits;
}

size_t SerialPort::write(BYTE byte)
{
    if (isConnected() == FALSE)
        return 0;

    DWORD NumberOfBytesWritten = 0;

    if (WriteFile(hComm, &byte, 1, &NumberOfBytesWritten, NULL) == FALSE)
        return 0;

    return NumberOfBytesWritten;
}

size_t SerialPort::write(LPCSTR string)
{
    if (isConnected() == FALSE)
        return 0;
    
    DWORD nNumberOfBytesToWrite = strlen(string);
    DWORD NumberOfBytesWritten = 0;

    if (WriteFile(hComm, string, nNumberOfBytesToWrite, &NumberOfBytesWritten, NULL) == FALSE)
        return 0;

    return NumberOfBytesWritten;
}

size_t SerialPort::write(LPCWSTR string)
{
    if (isConnected() == FALSE)
        return 0;
    
    DWORD nNumberOfBytesToWrite = wcslen(string) * 2;
    DWORD NumberOfBytesWritten = 0;

    if (WriteFile(hComm, string, nNumberOfBytesToWrite, &NumberOfBytesWritten, NULL) == FALSE)
        return 0;

    return NumberOfBytesWritten;
}

size_t SerialPort::write(PVOID buffer, DWORD nNumberOfBytesToWrite)
{
    if (isConnected() == FALSE)
        return 0;
    
    DWORD NumberOfBytesWritten = 0;

    if (WriteFile(hComm, buffer, nNumberOfBytesToWrite, &NumberOfBytesWritten, NULL) == FALSE)
        return 0;

    return NumberOfBytesWritten;
}

BYTE SerialPort::read()
{
    if (isConnected() == FALSE)
        return 0;

    BYTE byte;
    DWORD NumberOfBytesRead = 0;

    if (ReadFile(hComm, &byte, 1, &NumberOfBytesRead, NULL) == FALSE)
        return FALSE;

    return byte;
}

size_t SerialPort::read(PVOID buffer, DWORD nNumberOfBytesToRead)
{
    if (isConnected() == FALSE)
        return 0;

    DWORD NumberOfBytesRead = 0;

    if (ReadFile(hComm, buffer, nNumberOfBytesToRead, &NumberOfBytesRead, NULL) == FALSE)
        return FALSE;

    return NumberOfBytesRead;
}