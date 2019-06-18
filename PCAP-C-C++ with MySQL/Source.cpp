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

int dnum = 0; //no of devs
MYSQL *con = mysql_init(NULL);
char **files = NULL;
int noFile = 0;

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
    u_char ver_ihl;         // Version (4 bits) + Internet header length (4 bits)
    u_char tos;             // Type of service
    u_short tlen;           // Total length
    u_short identification; // Identification
    u_short flags_fo;       // Flags (3 bits) + Fragment offset (13 bits)
    u_char ttl;             // Time to live
    u_char proto;           // Protocol
    u_short crc;            // Header checksum
    ip_address saddr;       // Source address
    ip_address daddr;       // Destination address
    u_int op_pad;           // Option + Padding
} ip_header;

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

void printDev(pcap_if_t *);
char *iptos(u_long);
void listenLive_packet_handler(u_char *, const struct pcap_pkthdr *, const u_char *);
void listenFile_packet_handler(u_char *, const struct pcap_pkthdr *, const u_char *);
void addToFiles(char *);
void parseRemains();
void deletefiles(int);
void parsefile(int);
int menu();
int listenLive();
int listenFile();
int returnNumCaptures();
char *getPwd();

int main()
{
    return menu();
}
void parseRemains()
{
    for (int i = 0; i < noFile; i++)
    {
        printf("Parsing %s...", files[i]);
        FILE *raw = fopen(files[i], "rb");
        size_t size = 200; //"\r\n"
        char *buffer = (char *)malloc(size);
        char *infoTxt = (char *)malloc(2000);
        strcpy(infoTxt, "");
        do
        {
            getline(&buffer, &size, raw);
            sprintf(infoTxt, "%s%s", infoTxt, buffer);
        } while (strcmp(buffer, "\r\n") != 0);

        //insert info to db
        char querry[3000];
        char *pwd = getPwd();
        char filePath[strlen(pwd) + 25];
        sprintf(filePath, "%s/%s", pwd, files[i]);
        free(pwd);
        char acknum[9];
        sprintf(acknum, "%s", files[i] + 8);
        sprintf(querry, "insert into html_files(html_info, payload_path, tcp_acknum_bin) values('%s','%s','%s')",
                infoTxt, filePath, acknum);
        if (mysql_query(con, querry))
        {
            fprintf(stderr, "%s\n", mysql_error(con));
            printf("\n%s", querry);
        }

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
        free(infoTxt);
        fclose(file);
        printf("Done!\n");
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
            clona[i] = (char *)malloc(strlen(files[i]) + 1);
            strcpy(clona[i], files[i]);
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
void listenLive_packet_handler(u_char *dumpfile, const struct pcap_pkthdr *header, const u_char *pkt_data)
{
    //index of capture
    static int index = 0;
    index++;

    //saving to dump files
    pcap_dump(dumpfile, header, pkt_data);

    //structuring the bytes
    ip_header *ih = (ip_header *)(pkt_data + 14);
    u_int ip_len = (ih->ver_ihl & 0xf) * 4;
    tcp_header *tc = (tcp_header *)((u_char *)ih + ip_len);

    //working with structures data
    u_short sport = ntohs(tc->srcport);
    u_short dport = ntohs(tc->destport);

    //time structure
    struct tm *ltime;
    char timestr[20];
    time_t local_tv_sec;
    local_tv_sec = header->ts.tv_sec;
    ltime = localtime(&local_tv_sec);
    strftime(timestr, sizeof timestr, "%Y-%m-%d %H:%M:%S", ltime);
    //print timestamp and length of the packet
    printf("%d. %s len:%d \n", index, timestr, header->len);
    for (u_int i = 1; i <= header->len; i++)
    {
        printf("%.2x ", pkt_data[i]);
        if (i % 16 == 0)
            printf("\n");
        else if (i % 4 == 0)
            printf(" ");
    }
    printf("\n\n");
    if (((tc->flags) >> 1) % 2 == 0) //no syn
    {
        if (sport == 80 && header->len > 66) //is tcp, with a payload.
        {
            //geting data
            char dmac[18];
            snprintf(dmac, 18, "%.2x:%.2x:%.2x:%.2x:%.2x:%.2x", pkt_data[0], pkt_data[1], pkt_data[2], pkt_data[3], pkt_data[4], pkt_data[5]);
            char smac[18];
            snprintf(smac, 18, "%.2x:%.2x:%.2x:%.2x:%.2x:%.2x", pkt_data[6], pkt_data[7], pkt_data[8], pkt_data[9], pkt_data[10], pkt_data[11]);
            char daddr[16];
            snprintf(daddr, 16, "%d.%d.%d.%d", ih->daddr.byte1, ih->daddr.byte2, ih->daddr.byte3, ih->daddr.byte4);
            char saddr[16];
            snprintf(saddr, 16, "%d.%d.%d.%d", ih->saddr.byte1, ih->saddr.byte2, ih->saddr.byte3, ih->saddr.byte4);

            //forming the querry
            char querry[300];
            snprintf(querry, 300, "insert into tcp_received(no, smac, dmac, acknum, sip, dip, dport, sport, time) values(%d, '%s', '%s', '%.2x', '%s', '%s', %d, %d,str_to_date('%s','%%Y-%%m-%%d %%H:%%i:%%s'));",
                     index, smac, dmac, tc->acknum, saddr, daddr, dport, sport, timestr);
            if (mysql_query(con, querry))
            {
                if(mysql_error(con)[0] != 'D')
                fprintf(stderr, "%s\n", mysql_error(con));
            }

            //data parsing
            char filename[40];
            snprintf(filename, 39, "data/raw%.2x", tc->acknum);
            FILE *file = fopen(filename, "ab+");
            for (u_int i = 66; i < header->len; i++)
            {
                putc(pkt_data[i], file);
            }
            fclose(file);
            bool toAdd = true;
            for (int i = 0; i < noFile; i++)
            {
                if (strcmp(filename, files[i]) == 0)
                {
                    toAdd = false;
                    break;
                }
            }
            if (toAdd)
            {
                addToFiles(filename);
            }
            if (noFile > 10)
            {
                parsefile(0);
            }
        }
    }
}
void listenFile_packet_handler(u_char *dumpfile, const struct pcap_pkthdr *header, const u_char *pkt_data)
{
    //index of capture
    static int index = 0;
    index++;

    //structuring the bytes
    ip_header *ih = (ip_header *)(pkt_data + 14);
    u_int ip_len = (ih->ver_ihl & 0xf) * 4;
    tcp_header *tc = (tcp_header *)((u_char *)ih + ip_len);

    //working with structures data
    u_short sport = ntohs(tc->srcport);
    u_short dport = ntohs(tc->destport);

    //time structure
    struct tm *ltime;
    char timestr[20];
    time_t local_tv_sec;
    local_tv_sec = header->ts.tv_sec;
    ltime = localtime(&local_tv_sec);
    strftime(timestr, sizeof timestr, "%Y-%m-%d %H:%M:%S", ltime);

    //data for print
    char daddr[16];
    snprintf(daddr, 16, "%d.%d.%d.%d", ih->daddr.byte1, ih->daddr.byte2, ih->daddr.byte3, ih->daddr.byte4);
    char saddr[16];
    snprintf(saddr, 16, "%d.%d.%d.%d", ih->saddr.byte1, ih->saddr.byte2, ih->saddr.byte3, ih->saddr.byte4);
    printf("%.5d. %s %u\t%s %u\t%d \n", index, saddr, sport, daddr, dport, header->len);

    if (((tc->flags) >> 1) % 2 == 0) //no syn
    {
        if (sport == 80 && header->len > 66) //is tcp, with a payload.
        {
            //geting data
            char dmac[18];
            snprintf(dmac, 18, "%.2x:%.2x:%.2x:%.2x:%.2x:%.2x", pkt_data[0], pkt_data[1], pkt_data[2], pkt_data[3], pkt_data[4], pkt_data[5]);
            char smac[18];
            snprintf(smac, 18, "%.2x:%.2x:%.2x:%.2x:%.2x:%.2x", pkt_data[6], pkt_data[7], pkt_data[8], pkt_data[9], pkt_data[10], pkt_data[11]);

            //forming the querry
            char querry[300];
            snprintf(querry, 300, "insert into tcp_received(no, smac, dmac, acknum, sip, dip, dport, sport, time) values(%d, '%s', '%s', '%.2x', '%s', '%s', %d, %d,str_to_date('%s','%%Y-%%m-%%d %%H:%%i:%%s'));",
                     index, smac, dmac, tc->acknum, saddr, daddr, dport, sport, timestr);
            if (mysql_query(con, querry))
            {
                if(mysql_error(con)[0] != 'D')
                fprintf(stderr, "%s\n", mysql_error(con));
            }

            //data parsing
            char filename[40];
            snprintf(filename, 39, "data/raw%.2x", tc->acknum);
            FILE *file = fopen(filename, "ab+");
            for (u_int i = 66; i < header->len; i++)
            {
                putc(pkt_data[i], file);
            }
            fclose(file);
            bool toAdd = true;
            for (int i = 0; i < noFile; i++)
            {
                if (strcmp(filename, files[i]) == 0)
                {
                    toAdd = false;
                    break;
                }
            }
            if (toAdd)
            {
                addToFiles(filename);
            }
            if (noFile > 10)
            {
                parsefile(0);
            }
        }
    }
}
void parsefile(int i)
{
    printf("Parsing %s...", files[i]);
    FILE *raw = fopen(files[i], "rb");
    size_t size = 200; //"\r\n"
    char *buffer = (char *)malloc(size);
    char *infoTxt = (char *)malloc(2000);
    strcpy(infoTxt, "");
    do
    {
        getline(&buffer, &size, raw);
        sprintf(infoTxt, "%s%s", infoTxt, buffer);
    } while (strcmp(buffer, "\r\n") != 0);

    //insert info to db
    char querry[3000];
    char *pwd = getPwd();
    char filePath[strlen(pwd) + 25];
    sprintf(filePath, "%s/%s", pwd, files[i]);
    free(pwd);
    char acknum[9];
    sprintf(acknum, "%s", files[i] + 8);
    sprintf(querry, "insert into html_files(html_info, payload_path, tcp_acknum_bin) values('%s','%s','%s')",
            infoTxt, filePath, acknum);
    if (mysql_query(con, querry))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
    }

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
    fclose(file);
    free(infoTxt);
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
char *iptos(u_long in)
{
    static char output[16];
    u_char *p = (u_char *)(&in);
    sprintf(output, "%d.%d.%d.%d", p[0], p[1], p[2], p[3]);
    return output;
}
void printDev(pcap_if_t *d)
{
    pcap_addr_t *a;
    //Name
    printf("%d.%s\n", dnum, d->name);
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
        if (a->addr->sa_family == PF_INET)
        {
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
        }
    }
    printf("\n");
}
int menu()
{
    system("clear");
    int choice;
    do
    {
        printf("+-----------------------------------------------+\n");
        printf("IP traffic data analyzer app:\n");
        printf("\t1. Listen to a device\n");
        printf("\t2. Listen to a saved .dmp / .pcapng file\n");
        printf("\t3. Exit\n");
        printf("+-----------------------------------------------+\n");
        printf("Choice:");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            return listenLive();
            break;
        case 2:
            return listenFile();
            break;
        case 3:
            system("clear");
            printf("App closed!\n");
            break;
        default:
            system("clear");
            printf("Inexistent choice. Enter again\n");
            break;
        }
    } while (choice != 3);
    return 0;
}
int listenFile()
{
    system("clear");
    //Open the previous captured file
    pcap_t *fisHandle;
    char filename[30];
    int noCapturi = returnNumCaptures();
    printf("Fisiere .pcapng disponibile: %d\n", noCapturi);
    if (noCapturi > 0)
    {
        system("ls -la | grep -e \"dmp\" -e \"pcapng\"");
        printf("Nume fisier:");
        getchar();
        fgets(filename, 29, stdin);
        filename[strlen(filename) - 1] = '\0';
        char errbuf[PCAP_ERRBUF_SIZE];
        if ((fisHandle = pcap_open_offline(filename, errbuf)) == NULL)
        {
            fprintf(stderr, "\nUnable to open the file %s.\n", filename);
            return -1;
        }

        //connecting to db and parsing data
        mysql_real_connect(
            con, //connection
            LOCALHOST,
            USER,
            PASSWORD,
            DATABASE, //database
            0,        //port?
            NULL,     //unix socket?
            0);
        //Looping
        system("mkdir data");
        system("chmod 777 data");
        system("clear");
        printf("\nlistening on %s...\n", filename);
        pcap_loop(fisHandle, 0, listenFile_packet_handler, NULL);
        //split remaining files into html and data
        parseRemains();
        //FREE
        if (files)
        {
            free(files);
            files = NULL;
        }
        mysql_close(con);
    }
    else
    {
        printf("Nu exista capturi!\n");
    }
    return 0;
}
int listenLive()
{
    system("clear");
    //find devices
    pcap_if_t *alldevs = NULL;
    char errbuf[PCAP_ERRBUF_SIZE];
    if (pcap_findalldevs(&alldevs, errbuf) == -1)
    {
        fprintf(stderr, "pcap_findalldevs failure: %s\n", errbuf);
        return -1;
    }
    //print devices
    pcap_if_t *d;
    for (d = alldevs; d != NULL; d = d->next)
    {
        dnum++;
        printDev(d);
    }
    //user select device
    printf("Select capture device no.:");
    int inum, i;
    scanf("%d", &inum);
    if (inum > dnum || inum < 1)
    {
        pcap_freealldevs(alldevs);
        printf("Inexistent device.");
        return -1;
    }
    //jump to the selected adapter
    for (d = alldevs, i = 0; i < inum - 1; d = d->next, i++)
        ;
    //open the adapter
    pcap_t *adhandle;
    if ((adhandle = pcap_open_live(d->name, 65536, 1, 1000, errbuf)) == NULL)
    {
        fprintf(stderr, "\nUnable to open the adapter. %s is not supported.\nerrbuf:%s\n", d->name, errbuf);
        pcap_freealldevs(alldevs);
        return -1;
    }
    //connecting to db and parsing data
    mysql_real_connect(
        con, //connection
        LOCALHOST,
        USER,
        PASSWORD,
        DATABASE, //database
        0,        //port?
        NULL,     //unix socket?
        0);
    //Dumpfile to save captures
    pcap_dumper_t *dumpfile = pcap_dump_open(adhandle, "dump_data.pcapng");
    system("chmod 777 dump_data.pcapng");
    int captures;
    printf("Number of captures: (0 = until interruption):");
    scanf("%i", &captures);
    system("mkdir data");
    system("chmod 777 data");
    system("clear");
    //Looping
    printf("\nlistening on %s...\n", (d->description ? d->description : d->name));
    pcap_loop(adhandle, captures, listenLive_packet_handler, (u_char *)dumpfile); //0 for unlimited
    //split remaining files into html and data
    parseRemains();
    //FREE
    if (files)
    {
        free(files);
        files = NULL;
    }
    int save = 0;
    do
    {
        printf("\nDo you want to save the captures in a \"dump_data.pcapng\" file? [y-1/n-0]:");
        scanf("%i", &save);
        if (!save)
        {
            system("rm dump_data.pcapng");
        }
    } while (save > 1);

    pcap_freealldevs(alldevs);
    mysql_close(con);
    return 0;
}
int returnNumCaptures()
{
    int n;
    system("ls | grep -e \"dmp\" -e \"pcapng\" | wc -l > temp.txt");
    FILE *f = fopen("temp.txt", "r");
    fscanf(f, "%d", &n);
    fclose(f);
    system("rm temp.txt");
    return n;
}
char *getPwd()
{
    char buffer[200];
    system("pwd > temp.txt");
    FILE *f = fopen("temp.txt", "r");
    fgets(buffer, 199, f);
    strtok(buffer, "\n");
    fclose(f);
    system("rm temp.txt");
    char *pwd = (char *)malloc(strlen(buffer) + 1);
    strcpy(pwd, buffer);
    return pwd;
}
