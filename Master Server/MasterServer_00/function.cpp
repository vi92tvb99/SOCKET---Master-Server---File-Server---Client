#include"Header.h"

#define BUFLEN 512
#define PORT_CONNECT_FILESERVER 1111 //port de file server connect
#define PORT_CONNECT_CLIENT 3333
#define DEFAULT_PORT 2222;


string convertCharStrToStr(char* charStr, int length)
{
	int i = 0;
	string stringConverted;

	while (i < length)
	{
		stringConverted.push_back(charStr[i]);
		i++;
	}
	return stringConverted;
}

void ffile(string& str)
{
	vector<string> c;

	char path[] = "./ServerFile/";
	char a[300];

	strcpy(a, path);
	strcat_s(a, 300, "*");
	WIN32_FIND_DATA data;
	HANDLE hFind = FindFirstFile(a, &data);      // DIRECTORY

	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			c.push_back(data.cFileName);
		} while (FindNextFile(hFind, &data));
		FindClose(hFind);
	}

	c.erase(c.begin());
	c.erase(c.begin());

	string name = path;
	name.append(c[0]);

	ifstream iif(name);
	if (!iif.is_open())
	{
		cout << "Loi!" << endl;
		return;
	}

	char chr;
	while (1)
	{
		iif.get(chr);
		if (iif.eof())
			break;
		str.push_back(chr);
	}

}

char* taoTenFile(char*& fileName, char*& url, int& lengthURL)
{
	int len = lengthURL + 12 + 4;
	//đường dẫn
	int i = 0;
	int position = 0;
	char dir[13] = "ServerFile\\\\";
	for (i = 0; i < 12; i++)
	{
		fileName[position] = dir[i];
		position++;
	
	}
	i = 0;
	while (i < lengthURL-1) //127.0.0.1_1111 co 14 ki tu
	{
		fileName[position] = url[i];
		i++;
		position++;
	}
	fileName[position] = '\0';
	fileName[position] = '.';
	fileName[position + 1] = 't';
	fileName[position + 2] = 'x';
	fileName[position + 3] = 't';
	fileName[position + 4] = '\0';
	
	return fileName;
}

string suaString(string str) //sua thanh ten file co dang ip_port.txt
{
	int len = str.length();
	string temp;
	int viTri = 0;
	for (int i = 0; i < len; i++)
	{
		temp[viTri] = str[i];
		if (str[i] == '\r' && str[i + 1] == '\n')
		{
			temp[viTri] = '_';
			i++;
		}
		++viTri;
	}
	return temp;
}

void luuFileList(char* fileName,char* strList, int lengthStr)
{
	FILE* fileList;
	fileList = fopen(fileName, "w+");
	
	cout << endl;
	if (!fileList)
	{
		cout << "!!! Opening file failed. Cannot save list from file server!!! " << endl;
	}
	else
	{
		cout << "Saving......." << endl;
		fwrite(strList, lengthStr, 1, fileList);
		cout << "Saved successfully !!! " << endl;
		fclose(fileList);
	}
}

int TCP_Recv_FileSer(const char IP[], unsigned short port)
{
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOK = WSAStartup(ver, &wsData);
	if (wsOK != 0)
	{
		cout << "Cannot Initialize winsock!!!" << endl;
		return 1;
	}

	// Create a SOCKET for connecting to server
	SOCKET ListenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (ListenSocket == INVALID_SOCKET)
	{
		printf("socket failed with error: %ld\n", WSAGetLastError());
		return 1;
	}
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	hint.sin_addr.s_addr = inet_addr(IP);

	// Setup the TCP listening socket
	if (bind(ListenSocket, (sockaddr*)&hint, sizeof(hint)) != 0)
	{
		cout << "Binding error!!!" << endl;
		return 1;
	}

	if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		printf("listen failed with error: %d\n", WSAGetLastError());
		return 1;
	}

	while (1)
	{
		cout << "\n============== Waiting for File Server ==============" << endl;
		// Accept a client socket
		SOCKET FileServer;
		sockaddr_in fileServerInfo;
		int fileServerSize = sizeof(fileServerInfo);
		FileServer = accept(ListenSocket, (sockaddr*)&fileServerInfo, &fileServerSize);

		//GET IP
		char* ip = inet_ntoa(fileServerInfo.sin_addr);
		printf("Accepted Connection from File Server's IP:  %s\n", ip);


		if (FileServer == INVALID_SOCKET)
		{
			printf("Accept failed with error: %d\n", WSAGetLastError());
			return 1;
		}
		char recvbuf[512];
		int iResult;
		do
		{
			iResult = recv(FileServer, recvbuf, (int)BUFLEN, 0);
			if (iResult == 0)
			{
				cout << "File Server Disconnected!!!" << endl;
				break;
			}
			if (iResult == SOCKET_ERROR)
			{
				cout << "Recv failed with error!!!" << endl;
				break;
			}

			//sau khi nhan duoc chuoi~, chuoi~ dau tien la ten file;
			//=================		TAO FILE     ==========================

			//========= LUU =============
			char* pch;
			pch = strtok(recvbuf, "\n");
			int lenpch = strlen(pch);

			int len = lenpch + 12 + 4;
			char* fileName = (char*)malloc(len);
			memset(fileName, 0, len - 1);
			char* tenFile = taoTenFile(fileName, pch, lenpch);


			int lenND = iResult - lenpch;
			char* noiDung = (char*)malloc(lenND);
			int j;
			int pos = 0;
			for (j = lenpch; j < iResult; j++)
			{
				noiDung[pos] = recvbuf[j];
				pos++;
			}
			luuFileList(tenFile, noiDung, lenND);
			free(noiDung);
			free(fileName);

		} while (iResult > 0);


		closesocket(FileServer);
	}
	closesocket(ListenSocket);
	WSACleanup();
	return 0;
}

int TCP_Send(const char IP[], unsigned short port, char sendbuf[], int buflen)
{
	//----------------------
	// Declare and initialize variables.
	int iResult;
	/*WSADATA wsaData;*/

	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);
	int wsOK = WSAStartup(ver, &wsData);
	if (wsOK != 0)
	{
		cout << "Cannot Initialize winsock!!!" << endl;
		return 1;
	}
	//----------------------
	// Create a SOCKET for connecting to server
	SOCKET ConnectSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (ConnectSocket == INVALID_SOCKET)
	{
		wprintf(L"socket failed with error: %ld\n", WSAGetLastError());
		/*WSACleanup();*/
		return 1;
	}

	//----------------------
	// The sockaddr_in structure specifies the address family,
	// IP address, and port of the server to be connected to.
	struct sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = inet_addr(IP);
	clientService.sin_port = htons(port);

	//----------------------
	// Connect to server.
	iResult = connect(ConnectSocket, (SOCKADDR*)&clientService, sizeof(clientService));
	if (iResult == SOCKET_ERROR)
	{
		wprintf(L"connect failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		/*WSACleanup();*/
		return 1;
	}
	else cout << "DA KET NOI!!!" << endl;

	//----------------------
	//Send an initial buffer
	iResult = send(ConnectSocket, sendbuf, buflen, 0);

	if (iResult == SOCKET_ERROR)
	{
		wprintf(L"send failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		//WSACleanup();
		return 1;
	}

	printf("Bytes Sent: %d\n", iResult);



	string test;
	for (int i = 0; i < iResult; i++)
	{
		test.push_back(sendbuf[i]);
	}
	cout << test << endl;

	// close the socket
	iResult = closesocket(ConnectSocket);
	if (iResult == SOCKET_ERROR)
	{
		wprintf(L"close failed with error: %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	//WSACleanup();
	return 0;
}

int TCP_Recv_Client(const char IP[], unsigned short portClient)
{
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);
	string str;
	int wsOK = WSAStartup(ver, &wsData);
	if (wsOK != 0)
	{
		cout << "Cannot Initialize winsock!!!" << endl;
		return 1;
	}

	// Create a SOCKET for connecting to server
	SOCKET ListenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (ListenSocket == INVALID_SOCKET)
	{
		printf("socket failed with error: %ld\n", WSAGetLastError());
		return 1;
	}
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(portClient);
	hint.sin_addr.s_addr = inet_addr(IP);

	// Setup the TCP listening socket
	if (bind(ListenSocket, (sockaddr*)&hint, sizeof(hint)) != 0)
	{
		cout << "Binding error!!!" << endl;
		return 1;
	}

	if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		printf("listen failed with error: %d\n", WSAGetLastError());
		return 1;
	}

	while (1)
	{
		cout << "\n============== Waiting for CLIENT ==============" << endl;
		// Accept a client socket
		SOCKET clientSocket;
		sockaddr_in clientInfo;
		int clientSize = sizeof(clientInfo);
		clientSocket = accept(ListenSocket, (sockaddr*)&clientInfo, &clientSize);

		//GET IP
		char* ip = inet_ntoa(clientInfo.sin_addr);
		printf("Accepted Connection from CLIENT's IP:  %s\n", ip);


		if (clientSocket == INVALID_SOCKET)
		{
			printf("Accept failed with error: %d\n", WSAGetLastError());
			return 1;
		}

		char recvbuf[512];
		int iResult;
		do
		{
			iResult = recv(clientSocket, recvbuf, (int)BUFLEN, 0);
			if (iResult == 0)
			{
				cout << "Client Disconnected!!!" << endl;
				break;
			}

			for (int i = 0; i < iResult; i++)
			{
				str.push_back(recvbuf[i]);
			}

			string gg;
			char trc[512] = { 0 };
			ffile(gg);
			strcpy(trc, gg.c_str());
			
			for (int i = 0; i < strlen(trc); i++)
			{
				cout << trc[i];
			}
			if (str.compare("yes"))
			{
				//TCP_Send("127.0.0.1", PORT_CONNECT_CLIENT, trc, sizeof(trc));
				iResult = send(clientSocket, trc, sizeof(trc), 0);
			}
			str.clear();
		} while (iResult > 0);

		closesocket(clientSocket);
	}
	closesocket(ListenSocket);
	WSACleanup();
	return 0;
}
