#include <stdio.h>
#include <stdlib.h>
//Константы и функции, необходимые для работы с сокетами в файловом пространстве имен, объявлены в файле <sys/socket.h>.
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/ioctl.h>


#define SERVER_PORT 12345
#define BUFFER_SIZE 80

int main (int argc, char *argv[])
{
	int len, error;
	int sockfd; 
	char send_buf[BUFFER_SIZE];
	struct sockaddr_in addr;

	if (argc != 3) {
		perror("wrong args");
		return -1;
	}
	//AF_INET - домен сокета, обозначает тип соединения
	//SOCK_STREAM - тип сокета, потоковым сокетам, реализующим соединения «точка-точка» с надежной передачей данных
	//0 - протокол, используемый для передачи данных 
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)	{
		perror("socket");
		return -1;
	}

	memset(&addr, 0, sizeof(addr));
	
	addr.sin_family      = AF_INET; //формат адреса (набор протоколов), в нашем случае (для TCP/IP) оно должно иметь значение AF_INET
	addr.sin_addr.s_addr = inet_addr(argv[1]); //адрес  узла сети
	addr.sin_port        = htons(SERVER_PORT); //номер порта на узле сети


	error = connect(sockfd, // дескриптор сокета, через который программа обращается к серверу с запросом на соединение
					(struct sockaddr *)&addr, //указывает на структуру данных, содержащую адрес, приписанный сокету программы-сервера
					sizeof(struct sockaddr_in)); // размер (в байтах) структуры данных, указываемой аргументом addr
	if (error < 0) {
		perror("connect");
		close(sockfd);
		return -1;
	}

	len = send(sockfd, argv[2], strlen(argv[2]) + 1, 0);
	if (len != strlen(argv[2]) + 1)	{
		perror("send");
		close(sockfd);
		return -1;
	}

	char buffer[BUFFER_SIZE];
	size_t size;
	int bytes_read;
	int bytes_written;
	char dest_filename[100];
	strcpy (dest_filename, argv[2]);
	strcat (dest_filename, "_copy");
	FILE* file = fopen(dest_filename, "wb");

	// retrieve file name
	read(sockfd, &size, sizeof(size_t));

	size_t remains = size;
	while (remains > 0) {
		size_t buff_size = BUFFER_SIZE;
		if (remains < BUFFER_SIZE) {
			buff_size = remains;
		}

		bytes_read = read(sockfd, buffer, buff_size);
		bytes_written = fwrite(buffer, 1, bytes_read, file);
		remains -= buff_size;
	}

	fclose(file);
	close(sockfd);
	return 0;
}
