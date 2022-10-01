#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <chrono>


#define PORT 8080

#pragma comment(lib, "ws2_32.lib")

using namespace std;

struct mensagem {
    char mensagem[1000];
    char nome[50];
};