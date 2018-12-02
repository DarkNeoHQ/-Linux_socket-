//udpʱ��ͻ���
#include <sys/types.h>

#include <unistd.h>

#include <stdlib.h>

#include <string.h>

#include <stdio.h>

#include <time.h>

#include <errno.h>

#include <stdarg.h>

#include <stdlib.h>

#include <sys/socket.h>

#include <netinet/in.h>

#include <arpa/inet.h>

#include <netdb.h>




#ifndef	INADDR_NONE
#define	INADDR_NONE	0xffffffff
#endif	/* INADDR_NONE */

#define	BUFSIZE 64

#define	UNIXEPOCH	2208988800UL	/* UNIX epoch, in UCT secs*/

#define	MSG		"what time is it?\n"
int	connectUDP(const char *host, const char *service);

int	errexit(const char *format, ...);
/*------------------------------------------------------------------------

* main - UDP client for TIME service that prints the resulting time

*------------------------------------------------------------------------

*/

int

main(int argc, char *argv[])

{

	char	*host = "QiHongCheng";	/* ���û���ṩ��ַ����ʹ��������*/

	char	*service = "daytime";	/* Ĭ�Ϸ�������*/

	time_t	now;			/* ����ʱ���32λ����(ʵ�����ǳ�����)*/

	int	s, n;			/* �׽�������������ȡ����*/



	switch (argc) {

	case 1:

		host = "QiHongCheng";

		break;

	case 3:

		service = argv[2];

		/* FALL THROUGH */

	case 2:

		host = argv[1];

		break;

	default:

		fprintf(stderr, "usage: UDPtime [host [port]]\n");

		exit(1);

	}



	s = connectUDP(host, service);



	(void)write(s, MSG, strlen(MSG));//��time����������msg��ѯʱ��



	/* Read the time */



	n = read(s, (char *)&now, sizeof(now));//��ȡʱ��

	if (n < 0)

		errexit("read failed: %s\n", strerror(errno));

	now = ntohl((unsigned long)now);	/* put in host orderת���ֽ���	*/

	now -= UNIXEPOCH;		/* ��UCTת��ΪUNIX��Ԫ	*/

	printf("%s", ctime(&now));

	exit(0);

}

/* errexit.c - errexit */


/*------------------------------------------------------------------------

* errexit - ��ӡ������Ϣ���˳�

*------------------------------------------------------------------------

*/

int

errexit(const char *format, ...)

{

	va_list	args;



	va_start(args, format);

	vfprintf(stderr, format, args);

	va_end(args);

	exit(1);

}


int	connectsock(const char *host, const char *service,const char *transport);
/*------------------------------------------------------------------------
 * connectUDP - ���ӵ�ָ�������ϵ�ָ��UDP����
 *------------------------------------------------------------------------
 */
int connectUDP(const char *host, const char *service )
/*
 * Arguments:
 *      host    -��Ҫ���ӵ�����������
 *      service - ������˿ڹ����ķ���
 */
{
	return connectsock(host, service, "udp");
}


int
connectsock(const char *host, const char *service, const char *transport )
/*
 * Arguments:
 *      host      - name of host to which connection is desired
 *      service   - service associated with the desired port
 *      transport - name of transport protocol to use ("tcp" or "udp")
 */
{
	struct hostent	*phe;	/*ָ��������Ϣ��Ŀ��ָ��*/
	struct servent	*pse;	/* ָ�������Ϣ��Ŀ��ָ��*/
	struct protoent *ppe;	/* ָ��Э����Ϣ��Ŀ��ָ��*/
	struct sockaddr_in sin;	/*Internet�˵��ַ*/
	int	s, type;	/*�׽������������׽�������*/


	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;

    /* ����������ӳ�䵽�˿ں� */
	if ( pse = getservbyname(service, transport) )
		sin.sin_port = pse->s_port;
	else if ((sin.sin_port=htons((unsigned short)atoi(service))) == 0)
		errexit("can't get \"%s\" service entry\n", service);

    /* ��������ӳ�䵽IP��ַ��������ʮ����*/
	if ( phe = gethostbyname(host) )
		memcpy(&sin.sin_addr, phe->h_addr, phe->h_length);
	else if ( (sin.sin_addr.s_addr = inet_addr(host)) == INADDR_NONE )
		errexit("can't get \"%s\" host entry\n", host);

    /*������Э������ӳ�䵽Э���*/
	if ( (ppe = getprotobyname(transport)) == 0)
		errexit("can't get \"%s\" protocol entry\n", transport);

    /*ʹ��Э��ѡ���׽�������*/
	if (strcmp(transport, "udp") == 0)
		type = SOCK_DGRAM;
	else
		type = SOCK_STREAM;

    /*����һ���׽���*/
	s = socket(PF_INET, type, ppe->p_proto);
	if (s < 0)
		errexit("can't create socket: %s\n", strerror(errno));

    /* Connect the socket */
	if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
		errexit("can't connect to %s.%s: %s\n", host, service,
			strerror(errno));
	return s;
}


