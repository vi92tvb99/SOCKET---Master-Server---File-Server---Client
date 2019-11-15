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

using namespace std;
namespace fs = std::experimental::filesystem;

struct AllFile
{
	int stt;
	string Name;
	string Ip;
	unsigned int Port;
	vector<string> StrVec;
};

int TCP_Send(const char IP[], unsigned short port, char sendbuf[], int buflen);
vector<string> Fun_ListIP();

void NameOfDir(char input[], char out[]);

string Fun_IntToStr(unsigned int a);

int Fun_StrToInt(string a);

int UpLoad(const char input[]);

vector<string> vtorListDir(const char path[], string Ip, int port);

vector<string> vtorListDir(const char path[]);

vector<string> sortListDir(vector<string> ls);

char* chrListDir(vector<string> listdir);

void Thr_ConnectToServer(string Ip);

void Thr_UpLoad();

int UDP_SendSocket(char Addr[], unsigned short Port, char* SendBuf, int BufLen);

int UDP_RecvSocket(unsigned short Port);

void Thr_SendataF(string data, sockaddr_in SenderAddr);

void Check_Sum(string path);
