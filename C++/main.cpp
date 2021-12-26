#include "SerialPort.hpp"
#include "serialPort.cpp"

int main(int argc, char *argv[])
{
    SerialPort Serial(3, 115200);

    if (Serial.isConnected() == FALSE)
    {
        printf("Connect Failed\n");
        printf("GetLastError : %d\n", GetLastError());
        return -1;
    }

    while (TRUE)
    {
        char buffer[32] = { 0, };
        if (Serial.read(buffer, 32) > 0)
            printf("%s", buffer);
    }
}