#pragma comment(lib,"ws2_32.lib")
#include<WinSock2.h>
#include<iostream>
#include<WS2tcpip.h>
#include<stdio.h>
#include<string>
#include <fstream>
#include "md51.h"
#include <stdlib.h>
#include <Windows.h>
#include <tchar.h>

using namespace std;
using namespace System;
using namespace System::IO;
using namespace System::Security::Cryptography;
SOCKET Connect;
SOCKET *Connections;
SOCKET ListenSocket;
#define PORT "631"
#ifdef _UNICODE
#  define _tcout wcout
#else
#  define _tcout cout
#endif // _UNICODE

int ClientCount = 0;



void PrintByteArray(cli::array<Byte>^array, unsigned int* bb, unsigned int* check1 , int check)
{
	int i;
	long int b;
	unsigned int Target;
	//Target = (unsigned int)check1;
	//std::cout << Target << endl;
	//Target = (unsigned int)bb;
	//std::cout << Target << endl;
	for (i = 0; i < array->Length; i++)
 
	{
		//Console::Write((array[i]));
		b = b + array[i];

	}
	Console::WriteLine(b);
	if (check == 1) {
		Console::WriteLine(b);
		*bb = b;
		//std::cout << b << endl;
		//std::cout << bb << endl;
		//Target = (unsigned int)bb;
		//std::cout << Target << endl;
		Console::WriteLine();
	}
	else
	{
		if (b != *check1) {
			system("del file.txt");
			exit(1);
		}
	}
}

void SendMessageToClient(int ID)
{
	char *buffer = new char[1024];
	for (;; Sleep(75))
	{
		memset(buffer, 0, sizeof(buffer));
		if (recv(Connections[ID], buffer, 1024, 0))
		{
			for (int i = 0; i <= ClientCount; i++)
				send(Connections[i], buffer, strlen(buffer), 0);
		}
	}
	delete buffer;
}
int main()
{
	
	bool Check_Debug = IsDebuggerPresent(); //IsDebuggerPresent отключаетс€ всем , зачем его вообще использовать ? в том же  ollydbg он вообще отключаетс€ мышкой
	if (Check_Debug) { // господи боже eax в 0 и все обойдет
		exit(1);       
	}
	
 	_tsetlocale(LC_ALL, _T("Russain"));

	// —трока которую будем писать в реестр
	DWORD szTestString ;
	DWORD szTestString1 ;
	//  люч который будем создавать
	_TCHAR szPath[] = _T("Software\\RegistrySample\\");

	HKEY hKey;



	DWORD szBuf ;
	DWORD dwBufLen = MAX_PATH;
	unsigned int temp_chek1 , temp_check2;
	system("CertUtil -hashfile server.exe  > file.txt");
	//system("cls");
	SHA256 ^ mySHA256 = SHA256Managed::Create();
	cli::array<Byte>^hashValue;
	//std::ifstream ifs("server.exe", std::ios::binary);
	FileInfo^ fi1 = gcnew FileInfo("file.txt");
	FileStream^ fileStream = fi1->Open(FileMode::Open);
	hashValue = mySHA256->ComputeHash(fileStream);




	if (RegGetValue(HKEY_CURRENT_USER, szPath, _T("Test String"), RRF_RT_REG_DWORD, NULL, (BYTE*)&szBuf, &dwBufLen) != ERROR_SUCCESS) {
		    RegCreateKeyEx(HKEY_CURRENT_USER, szPath, 0, NULL, REG_OPTION_VOLATILE, KEY_WRITE, NULL, &hKey, NULL);
			
			PrintByteArray(hashValue, &temp_chek1, &temp_check2, 1);
			
			szTestString1 = (DWORD)temp_chek1;
			RegSetValueEx(hKey, _T("Test string"), 0, REG_DWORD, (const BYTE*)&szTestString1, sizeof(szTestString1));
			RegCloseKey(hKey);
	}
	else
	{
		RegGetValue(HKEY_CURRENT_USER, szPath, _T("Test String"), RRF_RT_REG_DWORD, NULL, (BYTE*)&szBuf, &dwBufLen);
		temp_check2 = (unsigned int)szBuf;
		
		
		PrintByteArray(hashValue, &temp_chek1, &temp_check2, 0);
	}


	fileStream->Close();
	system("del file.txt");
	string s = "";
	printf("Welcome to the chat server, Please enter the license key\n");
	getline(cin, s);
	cout << s+"\n";
	char *cstr = new char[s.length() + 33];
	strcpy(cstr, s.c_str());
	//printf("md5sum : %s\n", md5sum(cstr, strlen(cstr)));
	//printf("md5sum : %s\n", md5sum(cstr + '23', strlen(cstr)));
	std::cout << cstr << endl;
	s = md5sum(cstr, strlen(cstr));
	//std::cout << s;
	if (s == "5b721ff432d088cb67c8ac0a60753dc1") {

		cout << "key correct\n";

		setlocale(LC_ALL, "russian");
		WSAData Wsadata;
		int res = WSAStartup(MAKEWORD(2, 2), &Wsadata);
		if (res != 0) { return 0; }

		struct addrinfo hints;
		struct addrinfo * result;

		Connections = (SOCKET*)calloc(64, sizeof(SOCKET));
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_flags = AI_PASSIVE;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		int iResult;
		iResult = getaddrinfo(NULL, PORT, &hints, &result);
		if (iResult != 0) {
			printf("getaddrinfo failed: %d\n", iResult);
			WSACleanup();
			return 1;
		}
		ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (ListenSocket == INVALID_SOCKET) {
			printf("Error at socket(): %ld\n", WSAGetLastError());
			freeaddrinfo(result);
			WSACleanup();
			return 1;
		}
		iResult = bind(ListenSocket, result->ai_addr, result->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			printf("bind failed with error: %d\n", WSAGetLastError());
			freeaddrinfo(result);
			closesocket(ListenSocket);
			WSACleanup();
			return 1;
		}
		listen(ListenSocket, SOMAXCONN);
		freeaddrinfo(result);

		printf("ready\n");
		char m_connect[] = "Connecting";

		for (;; Sleep(75))
		{
			if (Connect = accept(ListenSocket, NULL, NULL))
			{
				printf("Client connecting...\n");
				char b[1024];
				send(Connect, m_connect, strlen(m_connect), NULL);
				recv(Connect, b, 1024, 0);
				printf("%s\n", b);//выводим пароль
				printf("Line %d\n", strlen(b));

				string ss = md5sum(b, strlen(b));
				string temp_ss = "5b721ff432d088cb67c8ac0a60753dc1";
				std::cout << ss << endl;
				std::cout << temp_ss;
				if (ss == temp_ss) {
					printf("yes\n");
					send(Connect, "key correct", 12, NULL);
					Connections[ClientCount] = Connect;
				}
				else {
					printf("no\n");
					send(Connect, "wrong  key ", 12, NULL);
				}

				ClientCount++;
				CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)SendMessageToClient, (PVOID)(ClientCount - 1), NULL, NULL);
			}
		}
	}
	else {
		cout << "no\n";
		cout << "key wrong!!!\n";
		system("pause");
	}

	return 1;
}

