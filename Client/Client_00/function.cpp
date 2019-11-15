#include"Header.h"

#define BUFLEN 300
#define DEFAULT_PORT 3333
#define BUFLEN 700000
#define PATH_SAVE "../store/"

mutex mtx0, mtx1;
static int sta;

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


	
	char recvbuf[512];
	int recvSize = 512;

	iResult = recv(ConnectSocket, recvbuf, (int)recvSize, 0);
	printf("Bytes Received: %d\n", iResult);

	string str;
	for (int i = 0; i < iResult; i++)
	{
		str.push_back(recvbuf[i]);
	}
	//cout << str << endl;


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

int UDP_SendSocket(const char Addr[], unsigned short Port, char* SendBuf, int BufLen)
{
	int iResult;
	//WSADATA wsaData;

	SOCKET SendSocket = INVALID_SOCKET;
	sockaddr_in RecvAddr;
	

	//unsigned short Port = 27015;

	//char SendBuf[1024];
	//int BufLen = 1024;

	//----------------------
	// Initialize Winsock
	/*iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		cout << "WSAStartup failed with error: " << iResult << endl;
		return 1;
	}*/

	//---------------------------------------------
	// Create a socket for sending data
	SendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (SendSocket == INVALID_SOCKET) {
		cout << "socket failed with error: " << WSAGetLastError() << endl;
		//WSACleanup();
		return 1;
	}

	//---------------------------------------------
	// Set up the RecvAddr structure with the IP address of
	RecvAddr.sin_family = AF_INET;
	RecvAddr.sin_port = htons(Port);
	RecvAddr.sin_addr.s_addr = inet_addr(Addr);

	//---------------------------------------------
	// Send a datagram to the receiver
	cout << "Sending a datagram to the receiver...\n";
	iResult = sendto(SendSocket, SendBuf, BufLen, 0, (SOCKADDR*)&RecvAddr, sizeof(RecvAddr));
	if (iResult == SOCKET_ERROR) {
		cout << "sendto failed with error: " << WSAGetLastError() << endl;
		closesocket(SendSocket);
		//WSACleanup();
		return 1;
	}

	//----------------------------
	//RecvAddr.sin_port = htons(5555);
	//int tt = sizeof(RecvAddr);
	////RecvAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	////
	////iResult = bind(SendSocket, (SOCKADDR*)&RecvAddr, sizeof(RecvAddr));
	////if (iResult == SOCKET_ERROR) {
	////	cout << "sendto failed with error: " << WSAGetLastError() << endl;
	////	closesocket(SendSocket);
	////	//WSACleanup();
	////	return 1;
	////}
	//iResult = recvfrom(SendSocket, SendBuf, BufLen, 0, (SOCKADDR*)&RecvAddr, &tt);
	//if (iResult == SOCKET_ERROR) {
	//	cout << "sendto failed with error: " << WSAGetLastError() << endl;
	//	closesocket(SendSocket);
	//	//WSACleanup();
	//	return 1;
	//}
	//cout << iResult << endl;
	//for (int i = 0; i < iResult; i++)
	//	cout << SendBuf[i];
	//cout << endl;

	//---------------------------------------------
	// When the application is finished sending, close the socket.
	cout << "Finish sending, Closing socket.\n" << endl;
	iResult = closesocket(SendSocket);
	if (iResult == SOCKET_ERROR) {
		cout << "closesocket failed with error: " << WSAGetLastError() << endl;
		//WSACleanup();
		return 1;
	}

	//---------------------------------------------
	// Clean up and quit.
	cout << "Exiting.\n" << endl;
	//WSACleanup();
	return 0;
}

int UDP_RecvSocket(unsigned short Port)
{
	/*WSADATA wsaData;*/

	int iResult;

	SOCKET RecvSocket;
	sockaddr_in RecvAddr;

	char RecvBuf[BUFLEN];
	int BufLen = BUFLEN;

	sockaddr_in SenderAddr;
	int SenderAddrSize = sizeof(SenderAddr);

	//------------------
	//Initialize Winsock
	/*iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR)
	{
		cout << "WSAStartup failed with error" << iResult << endl;
		return 1;
	}*/

	//---------------------
	//Creating receiver socket
	RecvSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (RecvSocket == INVALID_SOCKET)
	{
		cout << "Socket failed with error: " << WSAGetLastError() << endl;
		return 1;
	}

	//----------------------
	//Bind the socket to any address and the specified port.
	RecvAddr.sin_family = AF_INET;
	RecvAddr.sin_port = htons(Port);
	RecvAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	iResult = bind(RecvSocket, (SOCKADDR*)&RecvAddr, sizeof(RecvAddr));
	if (iResult != 0)
	{
		cout << "Bind failed with error: " << WSAGetLastError() << endl;
		closesocket(RecvSocket);
		//WSACleanup();
		return 1;
	}

	string temp;
	int dem = 0, dea = 0;
	sta = 0;
	
	while (1)
	{
		char* sao;
		//memset(RecvBuf, 0, BUFLEN);
		//-------------------------
		//recvfrom function to receive datagrams
		cout << "Receiving datagrams...\n";
		iResult = recvfrom(RecvSocket, RecvBuf, BufLen, 0, (SOCKADDR*)&SenderAddr, &SenderAddrSize);
		if (iResult == SOCKET_ERROR)
		{
			cout << "recvfrom failed with error: " << WSAGetLastError() << endl;
			closesocket(RecvSocket);
			//WSACleanup();
			return 1;
		}
		cout << iResult << endl;


		sao = new char[iResult];
		memcpy(sao, RecvBuf, iResult);
		thread a(Bef_Recvfrom, sao, iResult, SenderAddr);
		a.detach();
		//Bef_Recvfrom(sao, iResult, tt, SenderAddr);



		
	
		//string nameFile;

		//int pos;
		//int l = 0, tt = 0;
		//for (l = 0;; l++)
		//{
		//	if (RecvBuf[l] == '\t')
		//	{
		//		l++;
		//		tt++;
		//	}
		//	if (tt == 2)
		//		break;

		//	if (tt == 1)
		//	{
		//		nameFile.push_back(RecvBuf[l]);
		//	}
		//}

		//if (RecvBuf[l] == '1')
		//	dea = 1;

		//for (;; l++)
		//{
		//	if (RecvBuf[l] == '\r' && RecvBuf[l + 1] == '\n')
		//	{
		//		pos = l + 2;
		//		break;
		//	}
		//}

		//// Ghi file
		//string ppa = PATH_SAVE;
		//ppa.append(nameFile);
		///*string dd = Fun_IntToStr(dem);
		//ppa.append(dd);*/
		//ofstream oof(ppa, ios::app | ios::binary);
		//if (!oof.is_open())
		//{
		//	cout << "Mo file loi!" << endl;
		//	return 1;
		//}

		//for (int i = pos; i < iResult-1; i++)
		//{
		//	oof.put(RecvBuf[i]);
		//}

		//dem++;
		//oof.close();

		//if (dea == 1)
		//	break;

		
	}

	//---------------------------
	//Close the socket when finished receiving datagrams
	cout << "Finished receiving. Closing socket. \n";
	iResult = closesocket(RecvSocket);
	if (iResult == SOCKET_ERROR)
	{
		cout << "closesocket failed with error: " << WSAGetLastError();
		closesocket(RecvSocket);
		//WSACleanup();
		return 1;
	}

	//----------------------------
	//Clean up and exit
	cout << "Exiting.\n";
	//WSACleanup();
	return 0;
}

void Get_Ip_Port_Name(string a, string& Ip, string& Port, string& Name)
{
	int pawn = 0;
	for (int i = 0; i < a.length(); i++)
	{
		if (a[i] == '_')
		{
			pawn++;
			continue;
		}
		if (pawn == 0)
		{
			Ip.push_back(a[i]);
		}
		else if (pawn == 1)
		{
			Port.push_back(a[i]);
		}
		else
			Name.push_back(a[i]);
	}
}

string Fun_IntToStr(unsigned int a)
{
	string b;
	if (a == 0)
	{
		b.push_back('0');
		return b;
	}
	while (a != 0)
	{
		b.insert(b.begin(), (char)(a % 10) + 48);
		a = a / 10;
	}
	return b;
}

int Fun_StrToInt(string a)
{
	int it = 0, at;
	for (int i = 0; i < a.length(); i++)
	{
		at = (int)a[i] - 48;
		it = (it + at) * 10;
	}
	it = it / 10;
	return it;
}

void Bef_Recvfrom(char* RecvBuf, int iResult, sockaddr_in SenderAddr)
{
	
	string nameFile;
	string checksum;
	string stt;
	string byc;

	int pos;
	int l = 0, tt = 0;
	for (l = 0;; l++)
	{

		if (RecvBuf[l] == '\t')
		{
			l++;
			tt++;
		}
		if (RecvBuf[l] == '\r')
		{
			pos = l + 2;
			break;
		}

		if (tt == 1)
		{
			nameFile.push_back(RecvBuf[l]);
		}
		if (tt == 0)
		{
			checksum.push_back(RecvBuf[l]);
		}
		if (tt == 2)
		{
			byc.push_back(RecvBuf[l]);
		}
		if (tt == 3)
		{
			stt.push_back(RecvBuf[l]);
		}
	}

	//----------------------
	// Kiem tra loi
	int gg = 0, ff = 0;
	for (int i = 5; i < iResult; i++)
	{
		if ((i - 5) % 256 == 0)
		{
			gg = gg ^ ff;
			ff = 0;
		}
		ff += (int)RecvBuf[i];
	}
	//ff -= (int)RecvBuf[iResult - 1];
	gg = gg ^ ff;
	string strgg = Fun_IntToStr(gg);
	int llen = strgg.length();

	while (strgg.length() < 5)
		strgg.insert(strgg.begin(), '0');

	if (strgg.length() > 5)
	{
		for (int i = 0; i < llen - 5; i++)
			strgg.erase(strgg.begin());
	}
	
	SenderAddr.sin_port = htons(4444);

	// Kiem tra loi va gui lai
	if (strgg.compare(checksum) != 0)
	{
		cout << "loi bit" << endl;
		string abc = "\r\n";
		abc.append(nameFile);
		abc.push_back('\t');
		abc.append("2\t");
		abc.append(stt);
		abc.append("\r\n");

		SOCKET SendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (SendSocket == INVALID_SOCKET) {
			cout << "socket failed with error: " << WSAGetLastError() << endl;
			//WSACleanup();
			return;
		}

		int RR;
		cout << "Sending a datagram to the receiver...\n";
		RR = sendto(SendSocket, abc.c_str(), abc.length(), 0, (SOCKADDR*)&SenderAddr, sizeof(SenderAddr));
		if (RR == SOCKET_ERROR) {
			cout << "sendto failed with error: " << WSAGetLastError() << endl;
			closesocket(SendSocket);
			//WSACleanup();
			return;
		}

		cout << "Gui bao loi" << endl;

		return;
	}
	else
	{
		string a;
		for (int i = 6; i < iResult; i++)
		{
			if (RecvBuf[i] == '\n')
				break;
			a.push_back(RecvBuf[i]);
		}
		a.push_back('\n');
		a.insert(0, "\r\n");

		
		SOCKET SendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (SendSocket == INVALID_SOCKET) {
			cout << "socket failed with error: " << WSAGetLastError() << endl;
			//WSACleanup();
			return ;
		}

		int RR;
		cout << "Sending a datagram to the receiver...\n";
		RR = sendto(SendSocket, a.c_str(), a.length(), 0, (SOCKADDR*)&SenderAddr, sizeof(SenderAddr));
		if (RR == SOCKET_ERROR) {
			cout << "sendto failed with error: " << WSAGetLastError() << endl;
			closesocket(SendSocket);
			//WSACleanup();
			return ;
		}
		cout << "Da gui" << endl;
	}
	
	// Ghi file
	//mtx1.lock();
	int sst = Fun_StrToInt(stt);
	while (1)
	{
		if (sst == sta)
		{
			break;
		}
	}
	mtx0.lock();
	
	
	string ppa = PATH_SAVE;
	ppa.append(nameFile);
	/*string dd = Fun_IntToStr(dem);
	ppa.append(dd);*/
	ofstream oof(ppa, ios::app | ios::binary);
	if (!oof.is_open())
	{
		cout << "Mo file loi!" << endl;
		return;
	}

	for (int i = pos; i < iResult; i++)
	{
		oof.put(RecvBuf[i]);
	}

	oof.close();
	
	sta++;
	delete[] RecvBuf;
	mtx0.unlock();
	//mtx1.unlock();

	if (byc == "1")
	{
		exit(0);
	}
}




