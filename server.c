#include<WinSock2.h>  //������ ��� ���� ���α׷����� ���� ��� ����
#include<stdio.h>
#include<string.h>
#include<process.h>  //������ ����� ���� ��� ����
#pragma comment(lib,"ws2_32.lib") //������ ��� ���� ���α׷����� ���� ���̺귯�� ����

#define bufsize 1024
#define PORT 9391

int client_socket;
char read_buf[bufsize];

void read(void)  //�ۼ����� ���ÿ� �� �� �ֵ��� ���ſ� �����带 Ȱ��
{
	while (1)
	{
		memset(read_buf, 0, bufsize);
		recv(client_socket, read_buf, sizeof(read_buf), 0);
		printf("[client] : %s\n", read_buf);
		if (strcmp(read_buf, "exit") == 0) //exit ���Ž� ���α׷� ����
			exit(0);
	}
}

int main()
{
	WSADATA wsa;
	char buf[bufsize];

	/*������ ���� ���� ���̺귯���� ����ϱ� ����*/
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)  //2.2 ���� �����ϸ� 0�� �ƴ� �����ڵ� �� ��ȯ
	{
		printf("not equal version\n");
		return 0;
	}

	/*Ŭ���̾�Ʈ�� ������ ��ٸ��� ���� ���� ��ü ����*/
	int server_soket, client_addr_size;
	server_soket = socket(PF_INET, SOCK_STREAM, 0);
	if (SOCKET_ERROR == server_soket)
	{
		printf("Create Socket Fail\n");
		return 0;
	}

	/*server_address�� ������ �ּ� ���� ����*/
	SOCKADDR_IN server_addr, client_addr;
	memset(&server_addr, 0, sizeof(server_addr)); //������ �ּҰ� ����� server_adress�� 0���� �ʱ�ȭ
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	/*���� ���ε�*/
	if (SOCKET_ERROR == bind(server_soket, (struct sockaddr*) & server_addr, sizeof(server_addr)))
	{
		printf("Fail binding\n");
		return 0;
	}

	/*Ŭ���̾�Ʈ�� ������ ��ٸ��� ��� ���� ��ȯ*/
	if (SOCKET_ERROR == listen(server_soket, 5))
	{
		printf("������ ���� ����\n");
		return 0;
	}

	printf("%d�� ��Ʈ�� ���� �����...\n", PORT);

	client_addr_size = sizeof(client_addr);

	/*Ŭ���̾�Ʈ�� ������ �����ϸ� ������ ����*/
	client_socket = accept(server_soket, (struct sockaddr*) & client_addr, &client_addr_size);

	if (SOCKET_ERROR == client_socket)
	{
		printf("Client Connect Fail\n");
		return 0;
	}

	printf("���� �Ϸ�\n\n");

	/*���� ������ ����*/
	_beginthread(read, 0, (void*)1);

	/*��� ä���� �� �� �ֵ��� ���α׷��� ������ �ʰ� ���� loop*/
	while (1) //�۽��� ����
	{
		memset(buf, 0, bufsize);
		gets(buf);
		fflush(stdin);
		send(client_socket, buf, sizeof(buf), 0);
		if (strcmp(buf, "exit") == 0) //exit �۽Ž� ���α׷� ����
			break;
	}
	closesocket(client_socket);
	closesocket(server_soket);
	WSACleanup();
	return 0;
}