/******************************************************************************
* Project name:					 	dhcp
* Developer: 						Inbal Elmalech
* Project Lauch: 					Aug 13, 2020
* Submitted for review:				20/08/2020
* Reviewer:
******************************************************************************/

/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <stdio.h>
#include "dhcp.h" /* dhcp_t */
/****************************   DECLARATIONS   *******************************/
void TestDhcpCreateDestroy(void);
void TestDhcpGetAddRelease(void);
void TestCaseDhcp(void);
void PrintBin(uint32_t number, int size);
/********************************  MAIN  *************************************/

int main(void)
{
	puts("----------------TestDhcpCreateDestroy-----------------");
	TestDhcpCreateDestroy();

	puts("----------------TestDhcpGetAddRelease-----------------");
	TestDhcpGetAddRelease();
	
	puts("----------------TestCaseDhcp-----------------");
	TestCaseDhcp();

	puts("--------------------THE END----------------------");

	return (0);
}

/*****************************************************************************/

void TestDhcpCreateDestroy(void)
{
	const char net_address[] = "192.186.0.0";
	unsigned int set_bits = 4;

	dhcp_t* new_dhcp = DhcpCreate(net_address, set_bits);

	if (!NULL)
	{
		printf("new dhcp created!\n");
	}
	
	else
	{
		printf("no soup for you\n");
	}
	
	printf("Destroying dhcp\n");
	DhcpDestroy(new_dhcp);
}

/*---------------------------------------------------------------------------*/

void TestDhcpGetAddRelease(void)
{
	char net_address[] = "192.186.10.0";
	unsigned int set_bits = 2;

	uint32_t ip_add1 = 0;
	uint32_t *ip_add_ptr1 = &ip_add1;

	uint32_t ip_add2 = 0;
	uint32_t *ip_add_ptr2 = &ip_add2;

	uint32_t ip_add3 = 0;
	uint32_t *ip_add_ptr3 = &ip_add3;

	uint32_t ip_add4 = 0;
	uint32_t *ip_add_ptr4 = &ip_add4;

	uint32_t ip_add5 = 0;
	uint32_t *ip_add_ptr5 = &ip_add5;

	int res = 0;

	uint32_t forbidden1 = 0x00000000;
	uint32_t forbidden2 = 0xFFFFFFFF;
	uint32_t forbidden3 = 0xFFFFFFFE;

	dhcp_t* new_dhcp = DhcpCreate(net_address, set_bits);



	res = DhcpGetAddress(new_dhcp, ip_add_ptr1);

	if (0 == res)
	{
		printf("new address is:\n");
		PrintBin(ip_add1, sizeof(uint32_t));
	}

	else
	{
		printf("Get address failed!\n");
	}
	
	
	res = DhcpGetAddress(new_dhcp, ip_add_ptr2);

	if (0 == res)
	{
		printf("new address is:\n");
		PrintBin(ip_add2, sizeof(uint32_t));
	}
	
	else
	{
		printf("Get address failed!\n");
	}


	res = DhcpGetAddress(new_dhcp, ip_add_ptr3);

	if (0 == res)
	{
		printf("new address is:\n");
		PrintBin(ip_add3, sizeof(uint32_t));
	}

	else
	{
		printf("Get address failed!\n");
	}


	res = DhcpGetAddress(new_dhcp, ip_add_ptr4);

	if (0 == res)
	{
		printf("new address is:\n");
		PrintBin(ip_add4, sizeof(uint32_t));
	}
	
	else
	{
		printf("Get address failed!\n");
	}


	res = DhcpGetAddress(new_dhcp, ip_add_ptr5);

	if (0 == res)
	{
		printf("new address is : %ud\n", ip_add5);
	}
	
	else
	{
		printf("Get address failed!\n");
	}


	DhcpReleaseAddress(new_dhcp, ip_add1);
	DhcpReleaseAddress(new_dhcp, ip_add2);
	DhcpReleaseAddress(new_dhcp, ip_add3);
	DhcpReleaseAddress(new_dhcp, ip_add4);
	DhcpReleaseAddress(new_dhcp, ip_add5);

	DhcpReleaseAddress(new_dhcp, forbidden1);
	DhcpReleaseAddress(new_dhcp, forbidden2);
	DhcpReleaseAddress(new_dhcp, forbidden3);

	DhcpDestroy(new_dhcp);
}

/*---------------------------------------------------------------------------*/

void TestCaseDhcp(void)
{
	printf("hi I'm a stub\n");
	/* TODO: stub */
}

/***************************** Utilty Functions ******************************/

void PrintBin(uint32_t number, int size)
{
	uint32_t i = 0;

	printf("%u = %x = ",number, number);

	for (i = (1 << ((size * 8) - 1)); i > 0; i /= 2)
	{
		number & i ? printf("1") : printf("0");
	}

	puts("");
}

/*****************************************************************************/