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
        // Cria uma estrutura do tipo mensagem para armazenar o nome e o título do arquivo a ser enviado
        mensagem m;
        cout << "\nClient: Escreva o nome do arquivo .txt para ser enviado: ";
        cin.getline(m.nome, sizeof(m.nome));

        string send_texto, hold, nome_arquivo, caminho_arquivo;

        // Passo o nome do arquivo para string, pra facilitar a concatenação
        nome_arquivo = m.nome;
        caminho_arquivo = "send/" + nome_arquivo + ".txt";

        // Tenta então abrir o arquivo com o nome passado pelo usuário (deve estar na pasta 'send')
        ifstream arquivo(caminho_arquivo);
        if (arquivo.is_open())
        {
            // Primeiro, leio o arquivo linha por linha e salvo em 'hold'
            int count_letras=0;
            while(!arquivo.eof())
            {
                getline(arquivo, hold);
                send_texto += hold;
            }

            // Depois, leio todo o conteúdo de 'hold' e salvo na estrutura mensagem
            for (int i = 0; i < send_texto.length(); i++)
            {
                m.mensagem[i] = send_texto[i];
            }
            
            // Início do relógio que conta o tempo da requisição
            auto start = chrono::system_clock::now();

            // Envio então o nome e o texto separadamente para o servidor
            int byteCountNome = send(clientSocket, m.nome, sizeof(m.nome), 0);
            int byteCountMensagem = send(clientSocket, m.mensagem, sizeof(m.mensagem), 0);

            if(byteCountNome > 0 && byteCountMensagem > 0)
            {
                cout << "\nClient: Mensagem enviada: " << byteCountNome+byteCountMensagem << " bytes.\n" << endl;
            }
            
            else
            {
                WSACleanup();
            }

            // Por fim aguardo o servidor enviar uma resposta de confirmação de que a mensagem chegou ao destino
            char bufferReceived[200] = "";
            int rByteCount = recv(clientSocket, bufferReceived, 200, 0);

            // Contabilização do tempo de processamento da requisição
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
            // Tentei fazer uma criação autmática de arquivo, caso ele não existisse. Mas não deu mt certo.
            cout << "Arquivo '" << caminho_arquivo << "' nao encontrado. Criando novo arquivo..." << endl;
            
            // ofstream arquivo(caminho_arquivo);
            // cout << "Digite o texto do arquivo " << caminho_arquivo << endl;
            // cin >> m.mensagem;
            // if(arquivo.is_open()){
            //     arquivo <<"isso é um teste" << endl;
            //     arquivo.close();
            //     cout << "Arquivo criado com sucesso." << endl;
            // }
            // else
            // {
            //     cout << "Nao foi possível criar o arquivo "<< caminho_arquivo <<"." << endl;
            // }
        }
    }


    cout << "\nPasso 5 - Socket Encerrado" << endl;

    system("pause");
    WSACleanup();

    return 0;
}