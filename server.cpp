#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main() 
{
    cout << "======= Redes - Trabalho A1 =======\n\n" << endl;
    cout << "======= Servidor =======" << endl;

    cout << "\nPasso 1 - Configurando a DLL..." << endl;
    SOCKET serverSocket, acceptSocket;
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
    serverSocket = INVALID_SOCKET;
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (serverSocket==INVALID_SOCKET) 
    {
        cout << "Erro na criação do socket(): " << WSAGetLastError() << endl;
        WSACleanup();
        return 0;
    }
    else
    {
        cout << "socket() Criado com sucesso!" << endl;
    }

    cout << "\nPasso 3 - Realizando o bind do socket() a um IP/PORTA..." << endl;
    sockaddr_in service;
    service.sin_family = AF_INET;

    InetPton(AF_INET, "127.0.0.1", &service.sin_addr.s_addr);
    service.sin_port = htons(port);

    if(bind(serverSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR)
    {
        cout << "bind() não realizado" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 0;
    }
    else 
    {
        cout << "bind() realizado com sucesso!" << endl;
    }

    cout << "\nPasso 4 - Escutando por requisicoes de um cliente..." << endl;
    if(listen(serverSocket, 1) == SOCKET_ERROR)
    {
        cout << "listen(): Erro ao escutar requisicoes no socket " << WSAGetLastError() << endl;
    }
    else
    {
        cout << "listen() OK e aguardando por requisicoes..." << endl;
    }

    cout << "\nPasso 5 - Aguardando conexao do cliente..." << endl;
    acceptSocket = accept(serverSocket, NULL, NULL);
    if(acceptSocket == INVALID_SOCKET)
    {
        cout << "accept() falhou: " << WSAGetLastError() << endl;
        WSACleanup();
        return -1;
    }

    cout << "Server: Conexao realizada" << endl;

    cout << "\nPasso 6 - Conversando com o cliente" << endl;

    
    char receiveBuffer[200] = "";
    while(true){
        int byteCount = recv(acceptSocket, receiveBuffer, 200, 0);

        if(byteCount > 0)
        {
            cout << "\nClient message: " << receiveBuffer << endl;
        }
        else
        {
            WSACleanup();
        }

        char confirmationBuffer[200] = "Server: Mensagem recebida.";
        int confirmationCount = send(acceptSocket, confirmationBuffer, 200, 0);
        if(confirmationCount > 0)
        {
            cout << "Server message: Confirmacao enviada." << endl;
                   
        }
        else
        {
            WSACleanup(); 
        }
    }

    cout << "\nPasso 7 - Socket encerrado" << endl;

    system("pause");
    WSACleanup();

    return 0;
}