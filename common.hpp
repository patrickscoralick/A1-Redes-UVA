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

string lerArquivo(string nomeArquivo)
{
    string texto;

    ifstream arquivo(nomeArquivo);

    while (getline (arquivo, texto)) {
        arquivo >> texto;
    }

    arquivo.close();

    return texto;
}