#include "DomainToIP.h"

BOOL DomainToIP(const WCHAR* domain, IN_ADDR* addr)
{
	ADDRINFOW* addrInfo;
	SOCKADDR_IN* sockAddr;

	if (GetAddrInfo(domain, L"0", nullptr, &addrInfo) != 0)
	{
		return FALSE;
	}
	sockAddr = (SOCKADDR_IN*)addrInfo->ai_addr;
	*addr = sockAddr->sin_addr;
	FreeAddrInfo(addrInfo);

	return TRUE;
}

void PrintIP()
{
	SOCKADDR_IN SockAddr;
	IN_ADDR Addr;
	memset(&SockAddr, 0, sizeof(SockAddr));
	DomainToIP(L"google.com", &Addr);

	WCHAR clientIP[16] = { 0 };
	InetNtop(AF_INET, &Addr, clientIP, 16);
	wprintf(L"%s\n", clientIP);

	// DNS Flush
	system("ipconfig /flushdns");
	DomainToIP(L"google.com", &Addr);
	InetNtop(AF_INET, &Addr, clientIP, 16);
	wprintf(L"%s\n", clientIP);

	system("ipconfig /flushdns");
	DomainToIP(L"google.com", &Addr);
	InetNtop(AF_INET, &Addr, clientIP, 16);
	wprintf(L"%s\n", clientIP);

	system("ipconfig /flushdns");
	DomainToIP(L"google.com", &Addr);
	InetNtop(AF_INET, &Addr, clientIP, 16);
	wprintf(L"%s\n", clientIP);
}