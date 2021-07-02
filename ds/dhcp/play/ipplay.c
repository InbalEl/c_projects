#include <arpa/inet.h>
#include <stdio.h>

/*int inet_pton(int af, const char *src, void *dst);*/

void PrintBin(int number);

int main(void)
{
    /* |0001-0100|0000-1010|1010-1000|1100-0000| */
    const char ip_Add[] = "192.168.10.20";
    int ip_int = 0;
    inet_pton(AF_INET ,ip_Add, &ip_int);

    printf("int: %d\nhex: %x\n",ip_int, ip_int);
    PrintBin(ip_int);

    return 0;
}

void PrintBin(int number)
{
	unsigned int i;

	printf("%d = ",number);

	for (i = 1 << 31; i > 0; i /= 2)
	{
		number & i ? printf("1") : printf("0");
	}

	puts("");
}