#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <regex.h>
#include <arpa/inet.h>

#include "Functionality.h"

#define MAX_LABEL_LENGTH 63
#define MAX_DOMAIN_LENGTH 255

/*
The hostent structure is defined in <netdb.h> as follows:
struct hostent {
    char  *h_name;             official name of host
    char **h_aliases;          alias list
    int    h_addrtype;         host address type
    int    h_length;           length of address
    char **h_addr_list;        list of addresses
}
*/

bool IsIPAddress(char* str)
{
    //IPv4 addresses are represented by four numbers (a.b.c.d), where each number is between 0 and 255.
    int a, b, c, d;

    if(!sscanf(str, "%d.%d.%d.%d", &a, &b, &c, &d))
        return false;
    
    if(0 >= a || 255 <= a
    || 0 >= b || 255 <= b
    || 0 >= c || 255 <= c
    || 0 >= d || 255 <= d)
        return false;
    
    return true;
}

/*
A domain name is a sequence of labels separated by dots (periods), and it is used to identify resources on the Internet.
Each label in a domain name must be at most 63 characters long and can contain letters (a-z, A-Z), digits (0-9), and hyphens (-). 
The first and last character of a label cannot be a hyphen.
*/

bool IsDomain(char *domain) 
{
    regex_t regex;
    int ret;

    // create pattern regex by regcomp() function
    ret = regcomp(&regex, "^[a-z0-9]+([\\-\\.]{1}[a-z0-9]+)*\\.[a-z]{2,6}$", REG_EXTENDED | REG_ICASE);
    // check if the compilation was successful
    if (ret != 0) {
        printf("Error compiling regex\n");
        return false;
    }

    // regexec() function used to compare a give string(input) to a give pattern(&regex)
    ret = regexec(&regex, domain, 0, NULL, 0);
    regfree(&regex);

    return ret == 0;
}

void PrintInfoByHostName(char* hostName)
{
    struct hostent *hostent;
    char **ip;

    hostent = gethostbyname(hostName);

    if(hostent == NULL)
    {
        printf("Not found information\n");
        return;
    }

    for (ip = hostent->h_addr_list; *ip != NULL; ip++) 
    {
        struct in_addr ipAddress;
        memcpy(&ipAddress, *ip, sizeof(struct in_addr));

        if(ip == hostent->h_addr_list)
            printf("Official IP: %s\nAlias IP: \n", inet_ntoa(ipAddress));

        printf("%s\n", inet_ntoa(ipAddress));
    }
}

void PrintInfoByIPAddress(char* ipAddress)
{
    struct in_addr addr;
    struct hostent *hostent;
    char **alias;

    inet_aton(ipAddress, &addr);
    hostent = gethostbyaddr(&addr, sizeof(addr), AF_INET);

    if(hostent == NULL)
    {
        printf("Not found information\n");
        return;
    }

    printf("Official name: %s\n", hostent->h_name);

    printf("Alias names: \n");
    for (alias = hostent->h_aliases; *alias != NULL; alias++)
        printf("%s\n", *alias);
}
