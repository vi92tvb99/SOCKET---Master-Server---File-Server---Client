#include<WinSock2.h>
#include<WS2tcpip.h>
#include<stdio.h>
#include<vector>
#include<thread>
#include<fstream>
#include <string>
#pragma comment(lib, "Ws2_32.lib")

#include<iostream>
using namespace std;

void docFile(string& str);

char* taoTenFile(char*& fileName, char*& url, int& lengthURL);

void luuFileList(char* fileName,char* strList, int lengthStr);

int TCP_Recv_FileSer(const char IP[], unsigned short port);

int TCP_Send(const char IP[], unsigned short port, char sendbuf[], int buflen);

string convertCharStrToStr(char* charStr, int length);

void ffile(string& str);

int TCP_Recv_Client(const char IP[], unsigned short portClient);