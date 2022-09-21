#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main() 
{
    cout << "======= Redes - Trabalho A1 =======\n\n" << endl;
    cout << "======= Cliente =======" << endl;

    cout << "\nPasso 1 - Configurando a DLL..." << endl;
    SOCKET clientSocket;
    int port=55555;
    WSADATA wsaData;
    int wsaerr;
    WORD wVersionRequested = MAKEWORD(2, 2);
    wsaerr = WSAStartup(wVersionRequested, &wsaData);
    if(wsaerr != 0) 
    {
        cout << "DLL do Winsock não encontrada" << endl;
        return 0;
    }
    else 
    {
        cout << "DLL do Winsock encontrada!" << endl;
        cout << "Status: " << wsaData.szSystemStatus << endl;
    }

    cout << "\nPasso 2 - Configurando o Socket..." << endl;
    clientSocket = INVALID_SOCKET;
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (clientSocket==INVALID_SOCKET) 
    {
        cout << "Erro na criação do socket(): " << WSAGetLastError() << endl;
        WSACleanup();
        return 0;
    }
    else
    {
        cout << "socket() Criado com sucesso!" << endl;
    }

    cout << "\nPasso 3 - Realizando conexao com o Server..." << endl;
    sockaddr_in clientService;
    clientService.sin_family = AF_INET;

    InetPton(AF_INET, "127.0.0.1", &clientService.sin_addr.s_addr);
    clientService.sin_port = htons(port);

    if(connect(clientSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR)
    {
        cout << "Client: connect() não realizado" << endl;;
        WSACleanup();
        return 0;
    }
    else 
    {
        cout << "Client: connect() OK!" << endl;
        cout << "Client: Conexao estabelecida, dados podem ser enviados e recebidos." << endl;
    }

    cout << "\nPasso 4 - Conversando com o servidor" << endl;

    while(true)
    {
        char buffer[200] = "";
        cout << "\nClient: Escreva uma mensagem a ser enviada (nao escreva nada para encerrar): ";
        cin.getline(buffer, 200);

        int byteCount = send(clientSocket, buffer, sizeof(buffer), 0);

        if(byteCount > 0)
        {
            cout << "Client: Mensagem enviada: " << byteCount << " bytes." << endl;
            
        }
        
        else
        {
            WSACleanup();
        }

        char bufferReceived[200] = "";
        int rByteCount = recv(clientSocket, bufferReceived, 200, 0);
        if(rByteCount > 0)
        {
            cout << bufferReceived << endl;
        }
        else
        {
            WSACleanup();
        }
    }


    cout << "\nPasso 5 - Socket Encerrado" << endl;

    system("pause");
    WSACleanup();

    return 0;
}