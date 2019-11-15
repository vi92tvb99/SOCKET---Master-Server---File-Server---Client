#include<iostream>
#include<WS2tcpip.h>
#include<WinSock2.h>
#include<string.h>
#include<stdio.h>
#include<fstream>
#include<direct.h>
#include<Windows.h>
#include<vector>
#include<string>
#include<thread>
#include<mutex>


#pragma comment(lib, "Ws2_32.lib")

//#define BUFLEN 300
//#define DEFAULT_PORT_Client "3333"

using namespace std;
namespace fs = std::experimental::filesystem;

int TCP_Send(const char IP[], unsigned short port, char sendbuf[], int buflen);

int TCP_Recv_Master(string& str, const char IP[], unsigned short portClient);

int UDP_SendSocket(const char Addr[], unsigned short Port, char* SendBuf, int BufLen);

int UDP_RecvSocket(unsigned short Port);

void Get_Ip_Port_Name(string a,string &Ip,string &Port,string &Name);

string Fun_IntToStr(unsigned int a);

int Fun_StrToInt(string a);

void Bef_Recvfrom(char[], int ,sockaddr_in );

