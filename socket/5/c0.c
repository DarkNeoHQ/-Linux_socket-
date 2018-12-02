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
#include <iostream>
#include <thread>
#define PORT 7000
#define QUEUE 20
int ss;
struct sockaddr_in client_addr;
socklen_t length = sizeof(client_addr);
int conns[2] = {};
int z = 0;
void fo();
void thread_fn() {
    //�ɹ����طǸ������֣�������-1
    int conn = accept(ss, (struct sockaddr*)&client_addr, &length);
    if( conn < 0 ) {
        perror("connect error");
        exit(1);
    }
    //�����ӱ��浽��ʱ������;
    conns[z] = conn;
    z++;

    fd_set rfds;
    struct timeval tv;
    int retval, maxfd;
    printf("\n user is join in !\n");
    while(1) {
        /*�ѿɶ��ļ��������ļ������*/
        FD_ZERO(&rfds);
        /*�ѱ�׼������ļ����������뵽������*/
        FD_SET(0, &rfds);
        maxfd = 0;
        /*�ѵ�ǰ���ӵ��ļ����������뵽������*/
        FD_SET(conn, &rfds);
        /*�ҳ��ļ������������������ļ�������*/    
        if(maxfd < conn)
            maxfd = conn;
        /*���ó�ʱʱ��*/
        tv.tv_sec = 5;
        tv.tv_usec = 0;
        /*�ȴ�����*/
        retval = select(maxfd+1, &rfds, NULL, NULL, &tv);
        if(retval == -1){
            printf("select error\n");
            break;
        }else if(retval == 0){
            //printf("no message��waiting...\n");
            continue;
        }else{
            /*�ͻ��˷�������Ϣ(conn �ڼ�����)*/
            if(FD_ISSET(conn,&rfds))
	    {
                char buffer[1024];    
                memset(buffer, 0 ,sizeof(buffer));
                int len = recv(conn, buffer, sizeof(buffer), 0);
                if(strcmp(buffer, "exit\n") == 0) break;
                //printf("%s", buffer);
                for(int i=0; i<z; i++) 
	        {
                    send(conns[i], buffer, sizeof(buffer), 0);//���͸�ÿ���ͻ���
                }  
		memset(buffer, 0 ,sizeof(buffer));
            }
            
        }
    }
    
    close(conn);
}


void thread_select(int conn) {
}
int main() {
    ss = socket(AF_INET, SOCK_STREAM, 0);//ѡ��tcp��ʽЭ��
    struct sockaddr_in server_sockaddr;  //�����ṹ��
    server_sockaddr.sin_family = AF_INET;//ipv4Э����
    server_sockaddr.sin_port = htons(PORT);//�˿�����
    //printf("%d\n",INADDR_ANY);
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);//���÷�������ַ
    //���׽��ֵ����ص�ַ
    if(bind(ss, (struct sockaddr* ) &server_sockaddr, sizeof(server_sockaddr))==-1) {
        perror("bind");
        exit(1);
    }
    if(listen(ss, QUEUE) == -1) {//���ü���
        perror("listen");
        exit(1);
    }
	else
	{

fo();
printf("sever is ok !~~\n\n\n");
}//��ɷ���������
    std::thread t(thread_fn);//�½��߳�
    std::thread t1(thread_fn);//�½��߳�
    t.join();
    t1.join();
    close(ss);
    return 0;
}










void fo()
{
printf("                   _ooOoo_\n");
printf("                  o8888888o\n");
printf("                  88' . '88\n");
printf("                  (| -_- |)\n");
printf("                  O\\  =  /O\n");
printf("               ____/`---'\\____\n");
printf("             .'  \\\\|     |//  `.\n");
printf("            /  \\\\|||  :  |||//  \\\n");
printf("           /  _||||| -:- |||||-  \\\n");
printf("           |   | \\\\\\  -  /// |   |\n");
printf("           | \\_|  ''\\---/''  |   |\n");
printf("           \\  .-\\__  `-`  ___/-. /\n");
printf("         ___`. .'  /--.--\\  `. . __\n");
printf("      .'' '<  `.___\\_<|>_/___.'  >'''.\n");
printf("     | | :  `- \\`.;`\\ _ /`;.`/ - ` : | |\n");
printf("     \\  \\ `-.   \\_ __\\ /__ _/   .-` /  /\n");
printf("======`-.____`-.___\\_____/___.-`____.-'======\n");
printf("                   `=---='\n");
printf("\n");
printf("\n");

}