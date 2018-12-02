#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>

#define MYPORT  7000
#define BUFFER_SIZE 1024
int main()
{
    int sock_cli;
    fd_set rfds;//fd_set�����ݽṹ��ʵ������һlong���͵�����
    struct timeval tv;//��ʱ �ṹ��
    int retval, maxfd;

   
    sock_cli = socket(AF_INET,SOCK_STREAM, 0); ///����sockfd
    
    struct sockaddr_in servaddr;///����sockaddr_in ��Ԫ��
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(MYPORT);  ///�������˿�
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");  ///������ip

    //���ӷ��������ɹ�����0�����󷵻�-1
    if (connect(sock_cli, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("connect error\n\n\n");
        exit(1);
    }
	printf("connect is OK\n");
	
	char name[20];char bbuf[100];
	printf("write ur name:  ");
        scanf("%s",&name);
	fgets(bbuf, sizeof(bbuf), stdin);
	printf("\nHello! %s u can chat now\n\n\n",name);
	strcat(name," say : ");
	printf("**********************************************\n");

    while(1){
        
        FD_ZERO(&rfds);/*���������������*/
       
        FD_SET(0, &rfds); /*�ѱ�׼������ļ����������뵽������*/

        maxfd = 0;
        
        FD_SET(sock_cli, &rfds);/*��sock_cli�׽������������뵽������*/
          
        if(maxfd < sock_cli)/*�ҳ��׽��������������������׽���������*/  
            maxfd = sock_cli;
      
        tv.tv_sec = 5;  /*���ó�ʱʱ��*/
        tv.tv_usec = 0;
       
        retval = select(maxfd+1, &rfds, NULL, NULL, &tv); /*�ȴ�����(����׽����Ƿ�ɶ�,������)*/
        if(retval == -1){
            printf("select error\n");//select����
            break;
        }else if(retval == 0){
            //printf("no message��waiting...\n");
            continue;
        }else{ char buf[BUFFER_SIZE];char sendbuf[BUFFER_SIZE];
           
            if(FD_ISSET(sock_cli,&rfds)){ /*��������������Ϣ(����׽��� ,���ɶ��Ļᱻȥ�� ,���sock_cli�Ƿ��ڼ�����)*/
                int len;
                len = recv(sock_cli, buf, sizeof(buf),0);
		printf("----------------------------------\n");
		printf("->>>>     %s\n", buf);
		printf("----------------------------------\n");
                memset(buf, 0, sizeof(buf));
		
            }
            /*�û�������Ϣ��,��ʼ������Ϣ������*/
            if(FD_ISSET(0, &rfds)){
		
                fgets(sendbuf, sizeof(sendbuf), stdin);
		strcpy(bbuf,name);
		strcat(bbuf,sendbuf);
                send(sock_cli, bbuf, strlen(bbuf),0); //����
		memset(bbuf, 0, sizeof(bbuf));//���
		memset(sendbuf, 0, sizeof(sendbuf));
                
            }
        }
    }

    close(sock_cli);
    return 0;
}