#pragma once

#include <stdio.h>
#include <windows.h>

HANDLE GetCommHandleByComNumber(DWORD ComNumber);
BOOL SetBaudRate(HANDLE hComm, DWORD BaudRate);
DWORD SerialWrite(HANDLE hComm, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite);
DWORD SerialRead(HANDLE hComm, LPVOID lpBuffer, DWORD nNumberOfBytesToRead);

CRITICAL_SECTION ConsoleLock;