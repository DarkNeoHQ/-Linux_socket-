/* ����TIME�����UDP������ */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>


int	passiveUDP(const char *service);
int	errexit(const char *format, ...);

#define	UNIXEPOCH	2208988800UL	/*UNIX��Ԫ��UCT��	*/

/*------------------------------------------------------------------------
 * main - Iterative UDP server for TIME service
 *------------------------------------------------------------------------
 */
int
main(int argc, char *argv[])
{
	struct sockaddr_in fsin;	/* ���Կͻ��ĵ�ַ*/
	char	*service = "time";	/*�������ƻ�˿ں�*/
	char	buf[1];			/* �����롱������any size > 0*/
	int	sock;			/*�������׽���*/
	time_t	now;			/*��ǰʱ��*/
	unsigned int	alen;		/*�ӵ�ַ����*/

	switch (argc) {
	case	1:
		break;
	case	2:
		service = argv[1];
		break;
	default:
		errexit("usage: UDPtimed [port]\n");
	}

	sock = passiveUDP(service);

	while (1) {
		alen = sizeof(fsin);
		if (recvfrom(sock, buf, sizeof(buf), 0,(struct sockaddr *)&fsin, &alen) < 0)
			errexit("recvfrom: %s\n", strerror(errno));
		(void) time(&now);
		now = htonl((unsigned long)(now + UNIXEPOCH));//����ʱ��
		(void) sendto(sock, (char *)&now, sizeof(now), 0,(struct sockaddr *)&fsin, sizeof(fsin));
		//���͵��ͻ���
	}
}
/* passiveUDP.c - passiveUDP */

int	passivesock(const char *service, const char *transport,
		int qlen);

/*------------------------------------------------------------------------
 * passiveUDP -����һ����UDP��������ʹ�õı����׽���
 *------------------------------------------------------------------------
 */
int passiveUDP(const char *service)
/*
 * Arguments:
 *      service - ������˿ڹ����ķ���
 */
{
	return passivesock(service, "udp", 0);
}


/* errexit.c - errexit */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

/*------------------------------------------------------------------------
 * errexit - print an error message and exit
 *------------------------------------------------------------------------
 */
int errexit(const char *format, ...)
{
	va_list	args;
	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
	exit(1);
}



int	errexit(const char *format, ...);

unsigned short	portbase = 0;	/* port base, for non-root servers	*/

/*------------------------------------------------------------------------
 * passivesock -ʹ��TCP��UDP����Ͱ󶨷������׽���
 *------------------------------------------------------------------------
 */
int passivesock(const char *service, const char *transport, int qlen)
/*
 * Arguments:
 *      service   - ������˿ڹ����ķ���
 *      transport - Ҫʹ�õĴ���Э�飨��tcp����udp����
 *      qlen      - ��������������г���
 */
{
	struct servent	*pse;	/*ָ�������Ϣ��Ŀ��ָ��*/
	struct protoent *ppe;	/*ָ��Э����Ϣ��Ŀ��ָ��*/
	struct sockaddr_in sin;	/*Internet�˵��ַ*/
	int	s, type;	/*�׽������������׽�������*/

	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;

    /*����������ӳ�䵽�˿ں�*/
	if ( pse = getservbyname(service, transport) )
		sin.sin_port = htons(ntohs((unsigned short)pse->s_port)
			+ portbase);
	else if ((sin.sin_port=htons((unsigned short)atoi(service))) == 0)
		errexit("can't get \"%s\" service entry\n", service);

    /*��Э������ӳ�䵽Э���*/
	if ( (ppe = getprotobyname(transport)) == 0)
		errexit("can't get \"%s\" protocol entry\n", transport);

    /* ʹ��Э��ѡ���׽�������*/
	if (strcmp(transport, "udp") == 0)
		type = SOCK_DGRAM;
	else
		type = SOCK_STREAM;

    /*����һ���׽���*/
	s = socket(PF_INET, type, ppe->p_proto);
	if (s < 0)
		errexit("can't create socket: %s\n", strerror(errno));

    /*���׽���*/
	if (bind(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
		errexit("can't bind to %s port: %s\n", service,strerror(errno));
	if (type == SOCK_STREAM && listen(s, qlen) < 0)
		errexit("can't listen on %s port: %s\n", service,strerror(errno));
	return s;
}

