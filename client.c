#include<WinSock2.h>  //윈도우 기반 서버 프로그래밍을 위한 헤더 파일
#include<stdio.h>
#include<process.h> //스레드 사용을 위한 헤더 파일
#include<string.h>
#pragma comment(lib,"ws2_32.lib") //윈도우 기반 서버 프로그래밍을 위한 라이브러리 파일

#define bufsize 1024
#define PORT 9391

int server_socket;
char read_buf[bufsize];

void read(void)  //송수신을 동시에 할 수 있도록 수신에 스레드를 활용
{
	while (1)
	{
		memset(read_buf, 0, bufsize);
		recv(server_socket, read_buf, bufsize, 0);
		printf("[server] : %s\n", read_buf);
		if (strcmp(read_buf, "exit") == 0)  //exit 수신시 프로그램 종료
			exit(0);
	}
}

int main()
{
	WSADATA wsa;
	char buf[bufsize];

	/*윈도우 소켓 관련 라이브러리를 사용하기 위해*/
	if (WSAStartup(MAKEWORD(2, 2), &wsa)!=0) //2.2 버전 실패하면 0이 아닌 에러코드 값 반환
	{
		printf("not equal version\n");
		return 0;
	}

	/*서버에 접속할 클라이언트 소켓 객체 생성*/
	server_socket = socket(PF_INET, SOCK_STREAM, 0);
	if (SOCKET_ERROR == server_socket)
	{
		printf("Socket Create Error\n");
		return 0;
	}

	/*서버의 주소 정보 대입*/
	SOCKADDR_IN server_addr;
	memset(&server_addr, 0, sizeof(server_addr)); //서버의 주소가 저장될 server_adress를 0으로 초기화
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.S_un.S_addr = htonl(0x7f000001);//127.0.0.1 locallhost

	/*서버에 접속*/
	if (SOCKET_ERROR == connect(server_socket, (struct sockaddr*)& server_addr, sizeof(server_addr)))
	{
		printf("not connect\n");
		return 0;
	}

	printf("접속 완료\n\n");

	/*수신 스레드 시작*/
	_beginthread(read, 0, (void*)1);

	/*계속 채팅을 할 수 있도록 프로그램이 끝나지 않게 무한 loop*/
	while (1)//송신의 역할
	{
		memset(buf, 0, bufsize);
		gets(buf);
		fflush(stdin);
		send(server_socket, buf, sizeof(buf), 0);
		if (strcmp(buf, "exit") == 0)  //exit 송신시 프로그램 종료
			break;
	}
	closesocket(server_socket);
	WSACleanup();
	return 0;
}