#include "common.hpp"

int main() 
{
    cout << "======= Redes - Trabalho A1 =======\n\n" << endl;
    cout << "======= Servidor =======" << endl;

    cout << "\nPasso 1 - Configurando a DLL..." << endl;
    SOCKET serverSocket, acceptSocket;
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
    service.sin_port = htons(PORT);

    if(bind(serverSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR)
    {
        cout << "bind() nao realizado" << endl;
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

    cout << "\nPasso 6 - Aguardando envio de dados do Cliente..." << endl;

    // Crio a estrutura mensagem para receber o nome e o título do arquivo
    mensagem m;
    
    // Aguardo então o cliente me enviar essas duas informações
    int byteCountNome = recv(acceptSocket, m.nome, sizeof(m.nome), 0);
    int byteCountMensagem = recv(acceptSocket, m.mensagem, sizeof(m.mensagem), 0);

    if(byteCountNome > 0 && byteCountMensagem > 0)
    {
        cout << "\nMensagem recebida." << endl;
        cout << "Nome: " << m.nome << endl;
        cout << "Texto: " << m.mensagem << endl;

        // Com os dados recebidos, crio um arquivo com o nome e preencho com a mensagem
        string nome_do_arquivo = m.nome;
        string caminho = "receive/" + nome_do_arquivo + ".txt";
        ofstream arquivo;
        arquivo.open(caminho);
        if(arquivo.is_open())
        {
            arquivo << m.mensagem;
            arquivo.close();
        }
        else
        {
            cout << "Não foi possível criar o arquivo" << m.nome << endl;
        }
    }
    else
    {
        WSACleanup();
    }
    
    

    cout << "\nPasso 7 - Socket encerrado" << endl;

    system("pause");
    WSACleanup();

    return 0;
}

