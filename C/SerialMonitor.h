#pragma once

#include <stdio.h>
#include <windows.h>

HANDLE GetSerialPortHandleByPortNumber(DWORD PortNumber, DWORD BaudRate);
BOOL SetBaudRate(HANDLE SerialPort, DWORD BaudRate);
BOOL SendData(HANDLE SerialPort, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite);
BOOL RecvData(HANDLE SerialPort, LPVOID lpBuffer, DWORD BufferSize);