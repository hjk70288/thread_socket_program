#include<WinSock2.h>  //������ ��� ���� ���α׷����� ���� ��� ����
#include<stdio.h>
#include<process.h> //������ ����� ���� ��� ����
#include<string.h>
#pragma comment(lib,"ws2_32.lib") //������ ��� ���� ���α׷����� ���� ���̺귯�� ����

#define bufsize 1024
#define PORT 9391

int server_socket;
char read_buf[bufsize];

void read(void)  //�ۼ����� ���ÿ� �� �� �ֵ��� ���ſ� �����带 Ȱ��
{
	while (1)
	{
		memset(read_buf, 0, bufsize);
		recv(server_socket, read_buf, bufsize, 0);
		printf("[server] : %s\n", read_buf);
		if (strcmp(read_buf, "exit") == 0)  //exit ���Ž� ���α׷� ����
			exit(0);
	}
}

int main()
{
	WSADATA wsa;
	char buf[bufsize];

	/*������ ���� ���� ���̺귯���� ����ϱ� ����*/
	if (WSAStartup(MAKEWORD(2, 2), &wsa)!=0) //2.2 ���� �����ϸ� 0�� �ƴ� �����ڵ� �� ��ȯ
	{
		printf("not equal version\n");
		return 0;
	}

	/*������ ������ Ŭ���̾�Ʈ ���� ��ü ����*/
	server_socket = socket(PF_INET, SOCK_STREAM, 0);
	if (SOCKET_ERROR == server_socket)
	{
		printf("Socket Create Error\n");
		return 0;
	}

	/*������ �ּ� ���� ����*/
	SOCKADDR_IN server_addr;
	memset(&server_addr, 0, sizeof(server_addr)); //������ �ּҰ� ����� server_adress�� 0���� �ʱ�ȭ
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.S_un.S_addr = htonl(0x7f000001);//127.0.0.1 locallhost

	/*������ ����*/
	if (SOCKET_ERROR == connect(server_socket, (struct sockaddr*)& server_addr, sizeof(server_addr)))
	{
		printf("not connect\n");
		return 0;
	}

	printf("���� �Ϸ�\n\n");

	/*���� ������ ����*/
	_beginthread(read, 0, (void*)1);

	/*��� ä���� �� �� �ֵ��� ���α׷��� ������ �ʰ� ���� loop*/
	while (1)//�۽��� ����
	{
		memset(buf, 0, bufsize);
		gets(buf);
		fflush(stdin);
		send(server_socket, buf, sizeof(buf), 0);
		if (strcmp(buf, "exit") == 0)  //exit �۽Ž� ���α׷� ����
			break;
	}
	closesocket(server_socket);
	WSACleanup();
	return 0;
}