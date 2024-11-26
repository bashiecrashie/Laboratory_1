#include<sys/socket.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>

#define red "\033[1;31m"
#define green "\033[1;32m"
#define blue "\033[1;34m"
#define ret "\033[00m"

void print_logo();
void print_help();
int create_client(int port);
int snd_rcv(int s, char *str);

void print_logo()
{
	char *logo = "\n"
"\t┌──────────────────────────────────────────────────────┐\n"
"\t│   ██████  ██       ██ ████████ ████     ██ ██████████│\n"
"\t│  ██░░░░██░██      ░██░██░░░░░ ░██░██   ░██░░░░░██░░░ │\n"
"\t│ ██    ░░ ░██      ░██░██      ░██░░██  ░██    ░██    │\n"
"\t│░██       ░██      ░██░███████ ░██ ░░██ ░██    ░██    │\n"
"\t│░██       ░██      ░██░██░░░░  ░██  ░░██░██    ░██    │\n"
"\t│░░██    ██░██      ░██░██      ░██   ░░████    ░██    │\n"
"\t│ ░░██████ ░████████░██░████████░██    ░░███    ░██    │\n"
"\t│  ░░░░░░  ░░░░░░░░ ░░ ░░░░░░░░ ░░      ░░░     ░░     │\n"
"\t└──────────────────────────────────────────────────────┘\n\n";
	puts(logo);
}

void print_help()
{
	char *help = "\n"
		"\t./client <port> <four numbers>\n"
		"\texample: ./client 7667 1234\n";
	puts(help);
}

int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		printf("%s[-]%sInvalid number of arguments\n", red, green);
		print_help();
		return 1;
	}

	print_logo();

	int get_sock = create_client(atoi(argv[1]));
	snd_rcv(get_sock, argv[2]);

	return 0;
}

int create_client(int port)
{

	struct sockaddr_in addr;

	printf("%s[*]%sCreating client socket\n", blue, green);
	int client_socket = socket(AF_INET, SOCK_STREAM, 0);
	
	if((client_socket) < 0)
	{
		perror("[create_client: socket] reason");
		return 1;
	}
	printf("%s[+] Socket created !\n", green);
	
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	
	printf("%s[*]%sConnecting to 127.0.0.1:%d\n", blue, green, port);
	int conn_status = connect(client_socket, (struct sockaddr *)&addr, sizeof(addr));
	if((conn_status) < 0)
	{
		perror("[create client: connect] reason");
		return 1;
	}
	printf("%s[+] Connected !\n", green);

	return client_socket;
}

int snd_rcv(int s, char *str)
{
	char buffer[4];
	int bytes = 0;
	printf("%s[*]%sSending [%s]\n", blue, green, str);
	int send_status = send(s, str, sizeof(str), 0);
	if(send_status < 0)
	{
		perror("[snd rcv: send] reason");
		return 1;
	}
	printf("%s[+] Sended !\n%s[*]%sWaiting for response...\n", green, blue, green);
	
	recv(s, buffer, 4, 0);
		
	printf("%s[+] Response:%s\n", green, buffer);
	
	close(s);
}
