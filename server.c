#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdlib.h>

void print_logo();
void print_help();
int create_socket(int port);
int lstn_calc(int s);

#define red "\033[1;31m"
#define green "\033[1;32m"
#define blue "\033[1;34m"
#define ret "\033[00m"

void print_logo()
{
	char *logo = "\n"
	"\t┌───────────────────────────────────────────────────────────┐\n"
	"\t│  ████████ ████████ ███████   ██      ██ ████████ ███████  │\n"
	"\t│ ██░░░░░░ ░██░░░░░ ░██░░░░██ ░██     ░██░██░░░░░ ░██░░░░██ │\n"
	"\t│░██       ░██      ░██   ░██ ░██     ░██░██      ░██   ░██ │\n"
	"\t│░█████████░███████ ░███████  ░░██    ██ ░███████ ░███████  │\n"
	"\t│░░░░░░░░██░██░░░░  ░██░░░██   ░░██  ██  ░██░░░░  ░██░░░██  │\n"
	"\t│       ░██░██      ░██  ░░██   ░░████   ░██      ░██  ░░██ │\n"
	"\t│ ████████ ░████████░██   ░░██   ░░██    ░████████░██   ░░██│\n"
	"\t│░░░░░░░░  ░░░░░░░░ ░░     ░░     ░░     ░░░░░░░░ ░░     ░░ │\n"
	"\t└───────────────────────────────────────────────────────────┘\n";
	
	puts(logo);
}

void print_help()
{
	char *help = "./lab1_server <port>\n"
		"\texample: ./lab1_server 6776\n";
	puts(help);
}

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		printf("%s[-]%sInvalid number of arguments\n", red, green);
		print_help();
		return 1;
	}
	print_logo();
	
	int get_sock = create_socket(atoi(argv[1]));
	lstn_calc(get_sock);
	return 0;
}

int create_socket(int port)
{
	int server_socket = 0;

	printf("%s[*]%sCreating socket...\n", blue, green);
	if((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("[create socket: socket] reason");
		return 1;
	}
	printf("%s[+] Socket created !\n", green);
	struct sockaddr_in addr;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = (INADDR_ANY);

	printf("%s[*]%sBinding...\n", blue, green);
	if((bind(server_socket, (struct sockaddr *)&addr, sizeof(addr)) < 0 ))
	{
		perror("[create socket: bind] reason");
		return 1;
	}
	printf("%s[+] Binded !\n%s[*]%sWaiting for connection...\n", green, blue, green);
	listen(server_socket, 1);
	return server_socket;
}

int lstn_calc(int s)
{
	char buffer[4], result[4];
	char *answer;
	int recv_data = 0, accept_socket = 0, nums[4];
	
	while(1)
	{
	printf("%s[*]%sAccepting data...\n", blue, green);

		if((accept_socket = accept(s, NULL, NULL)) , 0)
		{
			perror("[lstn: accept] reason");
			return 1;
		}

	
	recv(accept_socket, buffer, 4, 0);
	
	printf("%s[+] Accepted !\n%s[*]%sCalculating...\n", green, blue, green);
	
	for(int i = 0; i < 4; ++i)
		nums[i] = buffer[i] - '0';
	
	for(int i = 0; i < 4; ++i)
	{
		if((nums[i] % 2) == 0)
		{
			result[i] = nums[i] + '0';
		}
		else
			result[i] = " " + '0';
	}

		answer = result;
	printf("%s[+] Calculated !\n%s[*]%sSending\n", green, blue, green);
		int snd_status = send(accept_socket, answer, sizeof(answer), 0);
		if((snd_status) < 0)
		{
			perror("[lstn_calc: send] reason");
		}
	}
	close(accept_socket);
}
