#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<netinet/in.h>
#include<string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include<signal.h>
#include<math.h>
#include <arpa/inet.h>
#include<sys/wait.h>

#define LEN 1500 // because lenght will now be 4/3 * lenght of message allowed to the user. For safe side this is taken to be 1500

char or[]="Message Received from client ",dor[]="Decoded Message from client ";
char cl[] = "Terminate connection";
char re[] = "Message received";

void sigchld_handler(int s)
{
    while(waitpid(-1, NULL, WNOHANG) > 0);
}

int findValue(char a)
{
	int k = (int)a;
	if(k>=65&&k<=90)
	{
		return(k-65);
	}
	if(k>=97&&k<=122)
	{
		return(k-71);
	}
	if(k>=48&&k<=57)
	{
		return(k+4);
	}
	if(k==43) /* + */
		return(62);
	if(k==47) /* / */
		return(63);
}

char *convertToBits(char *inp)
{
	char *out = (char*)malloc(24*sizeof(char));
	int i,num,j,ind,k;
	for(i=0; i<4; i++)
	{
		ind = 6*i;
		num = findValue(inp[i]);
		for(j = 1<<5; j>0; j=j/2)
		{
			if(num&j) out[ind++] = '1';
			else out[ind++] = '0'; 
		}
	}
	return(out);
}

char *decode(char *mg,int len)
{
	int k = len - 4,len1,len2,i,j,x,num,m,it;
	char arr[4];
	if(k>0) len1 = (k*3)/4, it=k/4;
	else len1 = 0, it= 0;
	if(mg[len-1]=='='&&mg[len-2]=='=')
		len2 = 1;
	else if(mg[len-1]=='=')
		len2 = 2;
	else
		len2 = 3;
	int outlen=len1 + len2 + 1; // one extra byte is kept for null character
	char *out = (char*)malloc(outlen*sizeof(char));
	out[outlen-1] = '\0';
	
	for(i=0; i<it; i++)
	{
		int y = i*4;
		for(j=y; j< y + 4; j++)
		{
			arr[j-y] = mg[j];
		}
		char *bits= convertToBits(arr);
		for(x=0;x<3;x++)
		{
			num = 0;
			m = x*8;
			for(j = 7+m; j>=m; j--)
			{
				num += (bits[j]-'0')*(pow(2,7+m-j));
			}
			out[3*i+x] = (char)num;
		}
	}
	// now we have to look after the last 4 bytes of the mg which may contain =
	j = it*4;
	for(i=j;i<len;i++)
	{
		arr[i-j] = mg[i];
	}
	char *bits= convertToBits(arr);
	for(x=0;x<len2;x++)
	{
		m = x*8;
		num = 0;
		for(j = 7+m; j>=m; j--)
		{
			num += (bits[j]-'0')*(pow(2,7+m-j));
		}
		char r = num;
		out[it*3+x] = r;
	}
	return(out);
	
}

void handleClient(int sockfd,struct sockaddr_in *client)
{
	char mg[LEN];
	int len;
	char *ip = inet_ntoa(client->sin_addr);
	int port = client->sin_port;
	while(1)
	{
		int i = 0;
		int st = read(sockfd,mg,LEN);
		write(1,or,strlen(or));
		printf("%s:%d - ",ip,port);
		fflush(stdout);
		write(1,mg,st);
		write(1,"\n",1);
		char *dMsg = decode(mg,st);
		
		if(dMsg[0]=='2')
		{
			write(1,dor,strlen(dor));
			printf("%s:%d - ",ip,port);
			fflush(stdout);
			write(1,cl,strlen(cl));
			write(1,"\n",1);
			break;
		}
		else
		{
			write(1,dor,strlen(dor));
			printf("%s:%d - ",ip,port);
			fflush(stdout);
			len = strlen(dMsg);
			dMsg[0] = '\0';
			write(1,dMsg,len);
			write(1,"\n",1);
		}
		write(sockfd,re,strlen(re));
	}
	close(sockfd);
	exit(0);
}

int main(int argc, char *argv[])
{
	struct sigaction sa;
	struct sockaddr_in client, server;
	int sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock==-1)
	{
		perror("socket");
		exit(-1);
	}
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(atoi(argv[1]));
	bzero(&server.sin_zero,8);
	int len = sizeof(struct sockaddr_in);
	if(bind(sock,(struct sockaddr *) &server, len)==-1)
	{
		perror("bind");
		exit(-1);
	}
	if(listen(sock,5)==-1)
	{
		perror("listen");
		exit(-1);
	}
	
	sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1)
	{
		perror("sigaction");
		exit(1);
    	}

	while(1)
	{
		int csock = accept(sock,(struct sockaddr *) &client,&len);
		if(csock==-1)
		{
			perror("accept");
			exit(-1);
		}
		switch(fork())
		{
			case -1:
				printf("error in fork\n");
				break;
			case 0:
				close(sock);
				handleClient(csock,&client);
				break;
			default:
				close(csock);
		}
	}
	return 0;
}