// Bibliotecas de Socket
#include <WinSock2.h>
#include <WS2tcpip.h>
// Bibliotecas para inputs do usuário e tratamento de texto
#include <iostream>
#include <string>
// Bibliotecas para controle de arquivos
#include <fstream>
#include <cstdint>
// Bibliotecas para controle de tempo
#include <ctime>
#include <chrono>
// Defino um tamanho mínino para dividir o arquivo em pequenos e definidos blocos
#define TAMANHO_BLOCO 8
int total_blocos = 0;
int blocos_concluidos = 0;
// Sockets que serão sobrescritos com novas informações a cada iteração
WSAOVERLAPPED *sobrescrever_envio = NULL;
int bytes_enviados = 0;
// Toda vez que um novo bloco for enviado com sucesso, esta função irá rodar e adicionar ás variáveis definidas
// a quantidade de blocos concluídos e de bytes enviados
void CALLBACK SalvaEnvioCallback(DWORD dwError, DWORD cbTransferred, LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags)
{
    blocos_concluidos++;
    bytes_enviados += cbTransferred;
}

#define PORT 8080

#pragma comment(lib, "ws2_32.lib")

using namespace std;

struct mensagem {
    char mensagem[999999];
    char nome[50];
};

int GetFileSize(string path)
{
    ifstream in_file(path, ios::binary);
    in_file.seekg(0, ios::end);
    int file_size = in_file.tellg();
    return file_size;
}