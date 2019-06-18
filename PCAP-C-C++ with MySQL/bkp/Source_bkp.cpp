#include "pcap/pcap.h"
#include "string.h"
#include "time.h"
#include "arpa/inet.h"
#include "mysql/my_global.h"
#include "mysql/mysql.h"
#ifndef WIN32
#include "sys/socket.h"
#include "netinet/in.h"
#include "netdb.h"
#else
#include "winsock.h"
#endif
#define LOCALHOST "localhost"
#define USER "root"
#define PASSWORD "password" //security much?
#define DATABASE "PRACTICA_SRI"

//
//HEADERS
//

/* 4 bytes IP address */
typedef struct ip_address
{
	u_char byte1;
	u_char byte2;
	u_char byte3;
	u_char byte4;
} ip_address;

/* IPv4 header */
typedef struct ip_header
{
	u_char ver_ihl;			// Version (4 bits) + Internet header length (4 bits)
	u_char tos;				// Type of service
	u_short tlen;			// Total length
	u_short identification; // Identification
	u_short flags_fo;		// Flags (3 bits) + Fragment offset (13 bits)
	u_char ttl;				// Time to live
	u_char proto;			// Protocol
	u_short crc;			// Header checksum
	ip_address saddr;		// Source address
	ip_address daddr;		// Destination address
	u_int op_pad;			// Option + Padding
} ip_header;

/* UDP header*/
typedef struct udp_header
{
	u_short sport; // Source port
	u_short dport; // Destination port
	u_short len;   // Datagram length
	u_short crc;   // Checksum
} udp_header;

/*TCP header*/
struct tcp_header
{
	u_short srcport;
	u_short destport;
	u_int seqnum;
	u_int acknum;
	u_char reserved : 4, offset : 4;
	u_char flags; //-----
	u_short win;
	u_short chksum;
	u_short urgptr;
	u_int padding;
};
/* total tcp header length: 20 bytes (= 160 bits) */

/*DNS header*/
typedef struct dnshdr
{
	u_short id;
	u_short rd : 1; //random shit I hope I won't use God please NO.
	u_short tc : 1;
	u_short aa : 1;
	u_short opcode : 4;
	u_short qr : 1;
	u_short rcode : 4;
	u_short zero : 3;
	u_short ra : 1;
	u_short qcount;  /* question count */
	u_short ancount; /* Answer record count */
	u_short nscount; /* Name Server (Autority Record) Count */
	u_short adcount; /* Additional Record Count */
} dnshdr;

int i = 0; //no of devs
MYSQL *con = mysql_init(NULL);
char **files = NULL;
int noFile = 0;

//
//PROTOTYPES.
//

void printDev(pcap_if_t *);
char *ip6tos(struct sockaddr *, char *, int);
char *iptos(u_long);
void packet_handler(u_char *, const struct pcap_pkthdr *, const u_char *);
void dispatcher_handler(u_char *, const struct pcap_pkthdr *, const u_char *);
int hostname_to_ip(char *, char *);
int ip_to_hostname(char *, char *);
void upload_ipv4dev(pcap_if_t *, MYSQL *);
void dispatcher_mysql_handler(u_char *temp1, const struct pcap_pkthdr *header, const u_char *pkt_data);
void dispatcher_saveData_handler(u_char *temp1, const struct pcap_pkthdr *header, const u_char *pkt_data);
void parseToDb();
void parsefile(int);
void addToFiles(char *filename);
void deletefiles(int i);
//
//MAIN
//

int main(int argc, char **argv)
{
	mysql_real_connect(
		con, //connection
		LOCALHOST,
		USER,
		PASSWORD,
		DATABASE, //database
		0,		  //port?
		NULL,	 //unix socket?
		0);

	//Find devs.
	pcap_if_t *alldevs = NULL;
	char errbuf[PCAP_ERRBUF_SIZE];
	if (pcap_findalldevs(&alldevs, errbuf) == -1)
	{
		fprintf(stderr, "pcap_findalldevs failure: %s\n", errbuf);
		return -1;
	}

	//List devs.
	pcap_if_t *d;
	for (d = alldevs; d != NULL; d = d->next)
	{
		printDev(d);
	}

	//Menu
	int raspuns = 0;
	printf("Raspuns:");
	scanf("%i", &raspuns);
	if (raspuns == 1) //captura
	{
		//Choose capturing device
		printf("Enter the interface number (1-%d):", i);
		int inum;
		scanf("%d", &inum);
		if (inum < 1 || inum > i)
		{
			printf("\nInterface number out of range.\n");
			pcap_freealldevs(alldevs);
			return -1;
		}

		//Jump to inum dev
		for (d = alldevs, i = 0; i < inum - 1; d = d->next, i++)
			;
		//Open the adaptor
		pcap_t *adhandle;
		if ((adhandle = pcap_open_live(d->name, 65536, 1, 1000, errbuf)) == NULL)
		{
			fprintf(stderr, "\nUnable to open the adapter. %s is not supported.\nerrbuf:%s\n", d->name, errbuf);
			pcap_freealldevs(alldevs);
			return -1;
		}

		//Dumpfile to save captures
		pcap_dumper_t *dumpfile = pcap_dump_open(adhandle, "dump_data.dmp");
		printf("\nlistening on %s...\n", (d->description ? d->description : d->name));
		pcap_loop(adhandle, 0, packet_handler, (u_char *)dumpfile); //0 for unlimited
		pcap_freealldevs(alldevs);
	}
	else if (raspuns == 2) //listeaza din captura
	{

		/* Open the previous captured file */
		pcap_t *fisHandle;
		char filename[20];
		strcpy(filename, "65000.pcapng");
		if ((fisHandle = pcap_open_offline(filename, errbuf)) == NULL)
		{
			fprintf(stderr, "\nUnable to open the file %s.\n", filename);
			return -1;
		}
		// read and dispatch packets until EOF is reached
		pcap_loop(fisHandle, 0, dispatcher_handler, NULL);
	}
	else if (raspuns == 3) //introduce device in bd
	{
		printf("MySQL client version: %s\n", mysql_get_client_info()); //client info;
		if (!con)
		{
			fprintf(stderr, "%s\n", mysql_error(con));
			mysql_close(con);
		}
		else
		{
			printf("%s@%s connected to %s.\n\n\n", USER, LOCALHOST, DATABASE);
		}
		for (d = alldevs; d != NULL; d = d->next)
		{
			upload_ipv4dev(d, con);
		}
	}
	else if (raspuns == 4) //introduce dns
	{
		/* Open the previous captured file */
		pcap_t *fisHandle;
		char filename[20];
		strcpy(filename, "65000.pcapng");
		if ((fisHandle = pcap_open_offline(filename, errbuf)) == NULL)
		{
			fprintf(stderr, "\nUnable to open the file %s.\n", filename);
			return -1;
		}
		// read and dispatch packets until EOF is reached
		printf("\nUploading to %s...", DATABASE);
		pcap_loop(fisHandle, 0, dispatcher_mysql_handler, NULL);
	}
	else if (raspuns == 5)
	{
		/* Open the previous captured file */
		pcap_t *fisHandle;
		char filename[30];
		strcpy(filename, "captures/trentapizza.pcapng");
		if ((fisHandle = pcap_open_offline(filename, errbuf)) == NULL)
		{
			fprintf(stderr, "\nUnable to open the file %s.\n", filename);
			return -1;
		}
		// read and dispatch packets until EOF is reached
		pcap_loop(fisHandle, 0, dispatcher_saveData_handler, NULL);
		printf("Spliting remainig data...\n");
		parseToDb();
	}

	if(files!=NULL)
	{
		free(files);
	}
	mysql_close(con);
	return 0;
}
void parsefile(int i)
{
	printf("Parsing %s...", files[i]);
	//INFO
	char infoname[50];
	sprintf(infoname, "%sINFO.txt", files[i]);
	FILE *raw = fopen(files[i], "rb");
	FILE *info = fopen(infoname, "wt+");
	size_t size = 2000;
	char *buffer = (char *)malloc(size);
	do
	{
		getline(&buffer, &size, raw);
		fprintf(info, "%s", buffer);
	} while (strcmp(buffer, "\r\n") != 0);

	//DATA
	char dataname[50];
	sprintf(dataname, "%s.data", files[i]);

	FILE *file = fopen(dataname, "wb+");
	while (!feof(raw))
	{
		char ch = fgetc(raw);
		putc(ch, file);
	}
	fclose(raw);
	remove(files[i]);
	deletefiles(i);
	fclose(info);
	fclose(file);
	printf("Done!\n");
}
void deletefiles(int i)
{
	char **clona;
	noFile--;
	if (noFile > 0)
	{
		clona = (char **)malloc(noFile * sizeof(char *));
		for (int j = 0; j < i; j++)
		{
			clona[j] = (char *)malloc(strlen(files[j]) + 1);
			strcpy(clona[j], files[j]);
			free(files[j]);
		}
		if (i < noFile)
		{
			for (int j = i; j < noFile; j++)
			{
				clona[j] = (char *)malloc(strlen(files[j + 1]) + 1);
				strcpy(clona[j], files[j + 1]);
				free(files[j + 1]);
			}
		}
		free(files[i]);
		files = clona;
	}
	else
	{
		free(files);
		files = NULL;
	}
}
void parseToDb()
{
	for (int i = 0; i < noFile; i++)
	{
		printf("Parsing %s...\n", files[i]);
		//INFO
		char infoname[50];
		sprintf(infoname, "%sINFO.txt", files[i]);
		FILE *raw = fopen(files[i], "rb");
		FILE *info = fopen(infoname, "wt+");
		size_t size = 2000; //"\r\n"
		char *buffer = (char *)malloc(size);
		do
		{
			getline(&buffer, &size, raw);
			fprintf(info, "%s", buffer);
		} while (strcmp(buffer, "\r\n") != 0);

		//DATA
		char dataname[50];
		sprintf(dataname, "%s.data", files[i]);

		FILE *file = fopen(dataname, "wb+");
		while (!feof(raw))
		{
			char ch = fgetc(raw);
			putc(ch, file);
		}
		fclose(raw);
		remove(files[i]);
		free(files[i]);
		fclose(info);
		fclose(file);
	}
}
void dispatcher_saveData_handler(u_char *temp1, const struct pcap_pkthdr *header, const u_char *pkt_data)
{
	static int index = 0;
	index++;

	ip_header *ih;
	tcp_header *tc;
	u_int ip_len;
	ih = (ip_header *)(pkt_data + 14);
	ip_len = (ih->ver_ihl & 0xf) * 4;
	tc = (tcp_header *)((u_char *)ih + ip_len);

	u_short sport = ntohs(tc->srcport);
	u_short dport = ntohs(tc->destport);

	if (((tc->flags) >> 1) % 2 == 0) //fara syn
	{
		if (sport == 80) //is tcp, with a payload.
		{
			//TCP_UPLOAD
			struct tm *ltime;
			char timestr[20];
			time_t local_tv_sec;
			local_tv_sec = header->ts.tv_sec;
			ltime = localtime(&local_tv_sec);
			strftime(timestr, sizeof timestr, "%Y-%m-%d %H:%M:%S", ltime);
			char dmac[18];
			snprintf(dmac, 18, "%.2x:%.2x:%.2x:%.2x:%.2x:%.2x", pkt_data[0], pkt_data[1], pkt_data[2], pkt_data[3], pkt_data[4], pkt_data[5]);
			char smac[18];
			snprintf(smac, 18, "%.2x:%.2x:%.2x:%.2x:%.2x:%.2x", pkt_data[6], pkt_data[7], pkt_data[8], pkt_data[9], pkt_data[10], pkt_data[11]);
			char daddr[16];
			snprintf(daddr, 16, "%d.%d.%d.%d", ih->daddr.byte1, ih->daddr.byte2, ih->daddr.byte3, ih->daddr.byte4);
			char saddr[16];
			snprintf(saddr, 16, "%d.%d.%d.%d", ih->saddr.byte1, ih->saddr.byte2, ih->saddr.byte3, ih->saddr.byte4);
			char querry[300];
			snprintf(querry, 300, "insert into tcp_received(no, smac, dmac, acknum, sip, dip, dport, sport, time) values(%d, '%s', '%s', '%u', '%s', '%s', %d, %d,str_to_date('%s','%%Y-%%m-%%d %%H:%%i:%%s'));",
					 index, smac, dmac, tc->acknum, saddr, daddr, dport, sport, timestr);
			if (mysql_query(con, querry))
			{
				fprintf(stderr, "%s\n", mysql_error(con));
			}

			//DATA
			char filename[40];
			snprintf(filename, 39, "data/raw%.2x", tc->acknum);
			if (header->len > 66) //has data
			{
				FILE *file = fopen(filename, "ab+");
				for (u_int i = 66; i < header->len; i++)
				{
					putc(pkt_data[i], file);
				}
				fclose(file);

				bool toAdd = true;
				for (int i = 0; i < noFile; i++)
					if (strcmp(filename, files[i]) == 0)
					{
						toAdd = false;
						break;
					}
				if (toAdd)
					addToFiles(filename);
				if(noFile > 3)
					parsefile(0);
			}
		}
	}
}
void addToFiles(char *filename)
{
	if (files != NULL)
	{
		noFile++;
		char **clona = (char **)malloc(noFile * sizeof(char *));
		for (int i = 0; i < noFile - 1; i++)
		{
			clona[i] = (char*)malloc(strlen(files[i])+1);
			strcpy(clona[i],files[i]);
			free(files[i]);
		}
		free(files);
		clona[noFile - 1] = (char *)malloc((strlen(filename) + 1));
		strcpy(clona[noFile - 1], filename);
		files = clona;
	}
	else
	{
		noFile++;
		files = (char **)malloc(noFile * sizeof(char *));
		files[0] = (char *)malloc(strlen(filename) + 1);
		strcpy(files[0], filename);
	}
}
void dispatcher_mysql_handler(u_char *temp1, const struct pcap_pkthdr *header, const u_char *pkt_data)
{
	char querry[300];
	if (!con)
	{
		fprintf(stderr, "%s\n", mysql_error(con));
	}
	else
	{
		struct tm *ltime;
		char timestr[16];
		time_t local_tv_sec;
		local_tv_sec = header->ts.tv_sec;
		ltime = localtime(&local_tv_sec);
		strftime(timestr, sizeof timestr, "%H:%M:%S", ltime);

		ip_header *ih;
		udp_header *uh;
		u_int ip_len;
		ih = (ip_header *)(pkt_data + 14); //length of ethernet header
		/* retireve the position of the udp header */
		ip_len = (ih->ver_ihl & 0xf) * 4;
		uh = (udp_header *)((u_char *)ih + ip_len);
		u_short sport = ntohs(uh->sport);
		u_short dport = ntohs(uh->dport);
		char srcAddr[16];
		strcpy(srcAddr, "");
		char dstAddr[16];
		strcpy(dstAddr, "");
		char resAddr[16];
		strcpy(resAddr, "");
		ip_address *response = (ip_address *)&pkt_data[header->len - 4];
		if (sport == 53) //is dns
		{
			sprintf(srcAddr, "%d.%d.%d.%d",
					ih->saddr.byte1,
					ih->saddr.byte2,
					ih->saddr.byte3,
					ih->saddr.byte4);
			sprintf(dstAddr, "%d.%d.%d.%d",
					ih->daddr.byte1,
					ih->daddr.byte2,
					ih->daddr.byte3,
					ih->daddr.byte4);
			sprintf(resAddr, "%d.%d.%d.%d",
					response->byte1,
					response->byte2,
					response->byte3,
					response->byte4);
			sprintf(querry, "insert into DNS_ResCaptures (IdCapture, Time, Length, SrcAddr, SrcPort, DstAddr, DstPort, ResponseAddr) values(null,'%s',%d,'%s','%d','%s','%d','%s')",
					timestr,
					header->len,
					srcAddr,
					sport,
					dstAddr,
					dport,
					resAddr);
			if (mysql_query(con, querry))
			{
				printf("%s\n", querry);
				fprintf(stderr, "%s\n", mysql_error(con));
			}
		}
	}
}
void upload_ipv4dev(pcap_if_t *d, MYSQL *con)
{
	pcap_addr_t *a;
	char name[45];
	char addr[3][16];
	strcpy(name, d->name);
	for (a = d->addresses; a; a = a->next)
	{
		if (a->addr->sa_family == PF_INET) //ipv4 strict
		{
			strcpy(addr[0], iptos(((struct sockaddr_in *)a->addr)->sin_addr.s_addr));
			if (a->broadaddr)
			{
				strcpy(addr[1], iptos(((struct sockaddr_in *)a->broadaddr)->sin_addr.s_addr));
			}
			if (a->netmask)
			{
				strcpy(addr[2], iptos(((struct sockaddr_in *)a->netmask)->sin_addr.s_addr));
			}
			char querry[300];
			sprintf(querry, "insert into devices(name, inet, broadcast, netmask)values('%s', '%s', '%s', '%s');",
					name, addr[0], addr[1], addr[2]);
			if (mysql_query(con, querry))
			{
				printf("%s\n", querry);
				fprintf(stderr, "%s\n", mysql_error(con));
				break;
			}
		}
	}
}
void dispatcher_handler(u_char *temp1, const struct pcap_pkthdr *header, const u_char *pkt_data)
{
	temp1 = NULL;
	struct tm *ltime;
	char timestr[16];
	time_t local_tv_sec;
	local_tv_sec = header->ts.tv_sec;
	ltime = localtime(&local_tv_sec);
	strftime(timestr, sizeof timestr, "%H:%M:%S", ltime);

	ip_header *ih;
	udp_header *uh;
	u_int ip_len;
	ih = (ip_header *)(pkt_data + 14); //length of ethernet header
	/* retireve the position of the udp header */
	ip_len = (ih->ver_ihl & 0xf) * 4;
	uh = (udp_header *)((u_char *)ih + ip_len);
	u_short sport = ntohs(uh->sport);
	u_short dport = ntohs(uh->dport);

	if (sport == 53) //is dns
	{
		printf("%s.%.6lu len:%d ", timestr, header->ts.tv_usec, header->len);
		ip_address *response = (ip_address *)&pkt_data[header->len - 4];
		printf("%d.%d.%d.%d.%d -> %d.%d.%d.%d.%d resp. %d.%d.%d.%d \n",
			   ih->saddr.byte1,
			   ih->saddr.byte2,
			   ih->saddr.byte3,
			   ih->saddr.byte4,
			   sport,
			   ih->daddr.byte1,
			   ih->daddr.byte2,
			   ih->daddr.byte3,
			   ih->daddr.byte4,
			   dport,
			   response->byte1,
			   response->byte2,
			   response->byte3,
			   response->byte4);
	}
}
void packet_handler(u_char *dumpfile, const struct pcap_pkthdr *header, const u_char *pkt_data)
{
	struct tm *ltime;
	char timestr[16];
	time_t local_tv_sec;

	//saving to dump files
	pcap_dump(dumpfile, header, pkt_data);

	/* convert the timestamp to readable format */
	local_tv_sec = header->ts.tv_sec;
	ltime = localtime(&local_tv_sec);
	strftime(timestr, sizeof timestr, "%H:%M:%S", ltime);

	/* print timestamp and length of the packet */
	printf("%s.%.6lu len:%d \n", timestr, header->ts.tv_usec, header->len);
	for (u_int i = 1; i <= header->len; i++)
	{
		printf("%.2x ", pkt_data[i]);
		if (i % 16 == 0)
			printf("\n");
		else if (i % 4 == 0)
			printf(" ");
	}
	printf("\n\n");
}
int hostname_to_ip(char *hostname, char *ip)
{
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_in *h;
	int rv;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo(hostname, "http", &hints, &servinfo)) != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return -1;
	}

	for (p = servinfo; p != NULL; p = p->ai_next)
	{
		h = (struct sockaddr_in *)p->ai_addr;
		strcpy(ip, inet_ntoa(h->sin_addr));
	}

	freeaddrinfo(servinfo);
	return 0;
}
char *iptos(u_long in)
{
	static char output[16];
	u_char *p = (u_char *)(&in);
	sprintf(output, "%d.%d.%d.%d", p[0], p[1], p[2], p[3]);
	return output;
}
char *ip6tos(struct sockaddr *sockaddr, char *address, int addrlen)
{
	socklen_t sockaddrlen;
#ifdef WIN32
	sockaddrlen = sizeof(struct sockaddr_in6);
#else
	sockaddrlen = sizeof(struct sockaddr_storage);
#endif

	if (getnameinfo(sockaddr, sockaddrlen, address, addrlen, NULL, 0, NI_NUMERICHOST) != 0)
	{
		address = NULL;
	}
	return address;
}
void printDev(pcap_if_t *d)
{
	pcap_addr_t *a;
	char ip6str[128];

	if (d->flags & PCAP_IF_RUNNING)
	{
		++i;
		//Name
		printf("%s\n", d->name);
		//Description
		if (d->description)
		{
			printf("\tDescription: %s\n", d->description);
		}
		//Flags
		printf("\tLoopback: %s\n", (d->flags & PCAP_IF_LOOPBACK) ? "yes" : "no");
		//IP Addresses
		for (a = d->addresses; a; a = a->next)
		{
			switch (a->addr->sa_family)
			{
			case PF_INET:
				printf("\tFamily Name: \tPF_INET\n");
				if (a->addr)
				{
					printf("\tAdress: \t%s\n", iptos(((struct sockaddr_in *)a->addr)->sin_addr.s_addr));
				}
				if (a->netmask)
				{
					printf("\tNetmask: \t%s\n", iptos(((struct sockaddr_in *)a->netmask)->sin_addr.s_addr));
				}
				if (a->broadaddr)
				{
					printf("\tBroadcast: \t%s\n", iptos(((struct sockaddr_in *)a->broadaddr)->sin_addr.s_addr));
				}
				if (a->dstaddr)
				{
					printf("\tDestination: \t%s\n", iptos(((struct sockaddr_in *)a->dstaddr)->sin_addr.s_addr));
				}
				break;
			case PF_INET6:
				printf("\tFamily Name: \tAF_INET6\n");
				if (a->addr)
				{
					printf("\tAddress: \t%s\n", ip6tos(a->addr, ip6str, sizeof(ip6str)));
				}
				break;
			}
		}
		printf("\n");
	}
}