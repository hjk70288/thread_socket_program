#include<WinSock2.h>  //윈도우 기반 서버 프로그래밍을 위한 헤더 파일
#include<stdio.h>
#include<string.h>
#include<process.h>  //스레드 사용을 위한 헤더 파일
#pragma comment(lib,"ws2_32.lib") //윈도우 기반 서버 프로그래밍을 위한 라이브러리 파일

#define bufsize 1024
#define PORT 9391

int client_socket;
char read_buf[bufsize];

void read(void)  //송수신을 동시에 할 수 있도록 수신에 스레드를 활용
{
	while (1)
	{
		memset(read_buf, 0, bufsize);
		recv(client_socket, read_buf, sizeof(read_buf), 0);
		printf("[client] : %s\n", read_buf);
		if (strcmp(read_buf, "exit") == 0) //exit 수신시 프로그램 종료
			exit(0);
	}
}

int main()
{
	WSADATA wsa;
	char buf[bufsize];

	/*윈도우 소켓 관련 라이브러리를 사용하기 위해*/
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)  //2.2 버전 실패하면 0이 아닌 에러코드 값 반환
	{
		printf("not equal version\n");
		return 0;
	}

	/*클라이언트의 접속을 기다리는 서버 소켓 객체 생성*/
	int server_soket, client_addr_size;
	server_soket = socket(PF_INET, SOCK_STREAM, 0);
	if (SOCKET_ERROR == server_soket)
	{
		printf("Create Socket Fail\n");
		return 0;
	}

	/*server_address에 서버의 주소 정보 대입*/
	SOCKADDR_IN server_addr, client_addr;
	memset(&server_addr, 0, sizeof(server_addr)); //서버의 주소가 저장될 server_adress를 0으로 초기화
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	/*소켓 바인딩*/
	if (SOCKET_ERROR == bind(server_soket, (struct sockaddr*) & server_addr, sizeof(server_addr)))
	{
		printf("Fail binding\n");
		return 0;
	}

	/*클라이언트의 접속을 기다리는 대기 모드로 전환*/
	if (SOCKET_ERROR == listen(server_soket, 5))
	{
		printf("대기상태 설정 실패\n");
		return 0;
	}

	printf("%d번 포트로 접속 대기중...\n", PORT);

	client_addr_size = sizeof(client_addr);

	/*클라이언트가 서버에 접속하면 접속을 수락*/
	client_socket = accept(server_soket, (struct sockaddr*) & client_addr, &client_addr_size);

	if (SOCKET_ERROR == client_socket)
	{
		printf("Client Connect Fail\n");
		return 0;
	}

	printf("접속 완료\n\n");

	/*수신 스레드 시작*/
	_beginthread(read, 0, (void*)1);

	/*계속 채팅을 할 수 있도록 프로그램이 끝나지 않게 무한 loop*/
	while (1) //송신의 역할
	{
		memset(buf, 0, bufsize);
		gets(buf);
		fflush(stdin);
		send(client_socket, buf, sizeof(buf), 0);
		if (strcmp(buf, "exit") == 0) //exit 송신시 프로그램 종료
			break;
	}
	closesocket(client_socket);
	closesocket(server_soket);
	WSACleanup();
	return 0;
}