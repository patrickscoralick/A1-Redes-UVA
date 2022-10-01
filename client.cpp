#include "common.hpp"

int main() 
{
    cout << "======= Redes - Trabalho A1 =======\n\n" << endl;
    cout << "======= Cliente =======" << endl;

    cout << "\nPasso 1 - Configurando a DLL..." << endl;
    SOCKET clientSocket;
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
    clientService.sin_port = htons(PORT);

    if(connect(clientSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR)
    {
        cout << "Client: connect() nao realizado" << endl;;
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
        cout << "\nClient: Escreva o nome do arquivo .txt para ser enviado: ";
        cin.getline(buffer, 200);

        string send_texto, hold;
        string nome_arquivo = buffer;
        string caminho_arquivo = nome_arquivo + ".txt";

        ifstream arquivo(caminho_arquivo);

        if (arquivo.is_open())
        {
            int count_letras=0;
            while(!arquivo.eof())
            {
                getline(arquivo, hold);
                count_letras += hold.size();
                send_texto += hold;
            }

            const char *str = send_texto.c_str();

            auto start = chrono::system_clock::now();

            int byteCount = send(clientSocket, str, count_letras, 0);

            if(byteCount > 0)
            {
                cout << "\nClient: Mensagem enviada: " << byteCount << " bytes.\n" << endl;
                cout << "Texto:" << str << endl;
            }
            
            else
            {
                WSACleanup();
            }

            char bufferReceived[200] = "";
            int rByteCount = recv(clientSocket, bufferReceived, 200, 0);

            auto end = chrono::system_clock::now();

            chrono::duration<double> elapsed_time = end - start;

            if(rByteCount > 0)
            {
                cout << bufferReceived << endl;
                cout << "Tempo: " << elapsed_time.count() << "s" << endl;
            }
            else
            {
                WSACleanup();
            }
        }
        else
        {
            cout << "Arquivo '" << caminho_arquivo << "' nao encontrado. Criando " << caminho_arquivo << "...\n" << endl;
            string create_texto;
            ofstream arquivo;
            arquivo.open(caminho_arquivo);
            if(arquivo.is_open()){
                cout << "Digite um texto para ser adicionado a " << caminho_arquivo << ":" << endl;
                cin >> create_texto;
                arquivo << create_texto;
                cout << "Arquivo criado!" << endl;
                arquivo.close();
            }
            else
            {
                cout << "Nao foi possivel criar o arquivo" << endl;
            }
        }
    }


    cout << "\nPasso 5 - Socket Encerrado" << endl;

    system("pause");
    WSACleanup();

    return 0;
}