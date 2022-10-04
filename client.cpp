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
    clientSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

    int nSndBuf = 0;

    setsockopt(clientSocket, SOL_SOCKET, SO_SNDBUF, (char*)&nSndBuf, sizeof(nSndBuf));

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

        int tamanho_arquivo = GetFileSize(caminho_arquivo);
        
        // Calcula a quantidade de pacotes que serão enviados.
        total_blocos = ceil(tamanho_arquivo / float(TAMANHO_BLOCO));
        // Lista que será sobrescrita durante o envio, deve ser deletada no final
        sobrescrever_envio = new WSAOVERLAPPED[total_blocos];
        memset(sobrescrever_envio, 0, sizeof(WSAOVERLAPPED) * total_blocos);

        // Armazenará um ponteiro para o arquivo e o seu tamanho na hora de realizar o envio
        WSABUF dataBuf;
        
        // Controlador da quantidade de bytes enviados
        DWORD dwBytesSent = 0;

        int err;
        int i, j;

        // Tenta então abrir o arquivo com o nome passado pelo usuário (deve estar na pasta 'send')
        ifstream arquivo(caminho_arquivo);
        if (arquivo.is_open())
        {
            // Primeiro, leio o arquivo linha por linha e salvo em 'hold'
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

            int byteCountNome = send(clientSocket, m.nome, sizeof(m.nome), 0);
            int byteCountMensagem = 0;

            for(i = 0, j = 0; i < tamanho_arquivo; i += TAMANHO_BLOCO, j++)
            {
                int nTransferBytes = min(tamanho_arquivo - i, TAMANHO_BLOCO);
                dataBuf.buf = &m.mensagem[i];
                dataBuf.len = nTransferBytes;
                // Envia os pacotes assincronamente, um de cada vez
                int byteCountMensagem = WSASend(clientSocket, &dataBuf, 1, &dwBytesSent, 0, &sobrescrever_envio[j], SalvaEnvioCallback);

                if ((byteCountMensagem == SOCKET_ERROR) && (WSA_IO_PENDING != (err = WSAGetLastError())))
                {
                    fprintf(stderr, "WSASend Falhou: %d\n", err);
                    exit(EXIT_FAILURE);
                }
                
            }       

            printf("(0 blocos de %d) Taxa de Upload: ???? KB/sec", bytes_enviados);    

            // Continua verificando até a conclusão de todos os blocos
            while(blocos_concluidos < total_blocos)
            {
                SleepEx(10, TRUE);
                printf("\r(%d blocos de %d) ", blocos_concluidos, total_blocos);
                float taxa_envio_kbytes = bytes_enviados/1024.0f;
                printf("Taxa de Upload: %.2f KB/sec", taxa_envio_kbytes);
                // Reseta a taxa de bytes
                bytes_enviados = 0;
            }

            // Deleta todos os dados sobrescritos
            

            cout << "\nTransferencia concluida!\n" << endl;
            if(bytes_enviados > 0)
            {
                cout << "Nome: " << caminho_arquivo << endl;
                cout << "Texto: " << m.mensagem << "\n" << endl;
            }
            
            else
            {
                WSACleanup();
            }

            delete [] sobrescrever_envio;
            break;
        }
        else
        {
            cout << "Arquivo '" << caminho_arquivo << "' nao encontrado." << endl;
        }
    }


    cout << "\nPasso 5 - Socket Encerrado" << endl;

    system("pause");
    WSACleanup();

    return 0;
}