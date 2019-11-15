#include"function.h"

//#define BUFLEN 512
#define PORT 1111
#define PORT_R_CLIENT 4444
#define PATHFILESTORE "../FileStored/"
#define MAXSIZEOFFILE 65535-28-1

mutex mtx, mtx1;

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
	for (int i = 0; i < buflen; i++)
	{
		printf("%c", sendbuf[i]);

	}
	if (iResult == SOCKET_ERROR)
	{
		wprintf(L"send failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		//WSACleanup();
		return 1;
	}

	printf("Bytes Sent: %d\n", iResult);


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

vector<string> vtorListDir(const char path[])
{
	vector<string> c;
	char a[300];

	strcpy(a, path);
	strcat_s(a, 300, "*");
	WIN32_FIND_DATA data;
	HANDLE hFind = FindFirstFile(a, &data);      // DIRECTORY

	if (hFind != INVALID_HANDLE_VALUE) 
	{
		do {
			c.push_back(data.cFileName);
		} while (FindNextFile(hFind, &data));
		FindClose(hFind);
	}

	c.erase(c.begin());
	c.erase(c.begin());

	return c;
} //list file

void Thr_ConnectToServer(string Ip)
{
	vector<string> a;
	char* b;

	a = vtorListDir(PATHFILESTORE, Ip, PORT_R_CLIENT);
	b = chrListDir(a);

	//TCP_Send("127.0.0.1", 1111, b, strlen(b));


	delete[] b;
}

char* chrListDir(vector<string> listdir)
{
	string a;
	//xoa thu muc . va ..
	listdir.erase(listdir.begin() + 1);
	listdir.erase(listdir.begin() + 1);
	for (auto i : listdir)
	{
		a.append(i);
		a.append("\r\n");
	}
	int len = a.length() + 1;
	char* chr = new char[len];
	strcpy_s(chr, len, a.c_str());
	chr[len - 1] = '\0';
	return chr;
}






/*vector<string> Fun_ListIP()
{
	vector<string> lis;
	char szBuffer[1024] = { 0 };

	/*WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 0);
	if (::WSAStartup(wVersionRequested, &wsaData) != 0)
		cout << 1 << endl;

	gethostname(szBuffer, sizeof(szBuffer));

	cout << szBuffer << endl;

	hostent* host = gethostbyname(szBuffer);
	int i = 0;
	while (host->h_addr_list[i] != NULL)
	{
		lis.push_back(inet_ntoa(*((in_addr*)host->h_addr_list[i])));
		i++;
	}

	WSACleanup();
	return lis;
}*/


void NameOfDir(char input[], char out[])
{
	for (int i = strlen(input) - 1; i >= 0; i--)
	{
		if (input[i] == '/')
		{
			int j;
			for (j = i + 1; j < strlen(input); j++)
			{
				out[j - i - 1] = input[j];
			}
			out[j - i - 1] = '\0';
			return;
		}
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

int UpLoad(const char input[])
{
	char namedir[250] = { 0 };
	char output[300] = { 0 };
	char path[300] = { 0 };
	char xox[] = "_xox";
	char temp = 'A';
	char INput[300];
	string so;
	int cou = 0;

	ifstream in(input, ios::binary);
	if (!in.is_open())
	{
		cout << "Can't open file( in)." << endl;
		return 1;
	}

	strcpy_s(INput, 300, input);
	NameOfDir(INput, namedir);
	strcat_s(path, 300, PATHFILESTORE);
	strcat_s(path, 300, namedir);
	if (!_mkdir(path))
		cout << "Create file success" << endl;

	strcat_s(output, 300, namedir);
	strcat_s(output, 300, xox);
	int len_out = strlen(output);
	output[len_out] = '_';

	so = Fun_IntToStr(cou);
	for (int i = 0; i < so.length(); i++)
	{
		output[len_out + 1 + i] = so[i];
	}
	output[len_out + 1 + so.length()] = '\0';

	strcat_s(path, 300, "/");
	strcat_s(path, 300, output);
	ofstream out(path, ios::binary);
	if (!out.is_open())
	{
		cout << "Can't open file( out)." << endl;
		return 1;
	}

	char chr;
	int pawn = 0;
	string str_cou;

	// Tim ten file
	char* poi = strrchr(INput, '/');
	int loc = poi - INput;
	string NameF;
	for (int i = loc + 1; i < strlen(INput); i++)
	{
		NameF.push_back(INput[i]);
	}

	//ghi 5 ky tu nhap
	for (int i = 0; i < 5; i++)
	{
		out.put('0');
		pawn++;
	}
	// Ghi file
	//int gg = 0, ff = 0, count = 0;
	int pawn001 = 0;
	while (1)
	{

		if (pawn == MAXSIZEOFFILE)
		{
			out.close();
			string PPAT = path;
			thread chk(Check_Sum, PPAT);
			chk.detach();
			//gg = gg ^ ff;

			//chuyen con tro file ve dau ghi lai 6 ky tu nhap
			//out.seekp(0,ios_base::beg);
			//out.seekp(0, out._Seekbeg);
			//string t = Fun_IntToStr(gg);
			/*while (t.length() < 5)
			{
				t.insert(t.begin(), '0');
			}*/

			/*for (int i = t.length() - 5; i < t.length(); i++)
			{
				out.put(t[i]);
			}
			out.put('\t');
			t.clear();
			out.close();*/

			// Tim vi tri '_'
			char* vit = strrchr(path, '_');
			int tri = vit - path;

			cou++;
			so = Fun_IntToStr(cou);
			for (int i = 0; i < so.length(); i++)
			{
				path[tri + 1 + i] = so[i];
			}
			path[tri + 1 + so.length()] = '\0';

			out.open(path, ios::binary);
			if (!out.is_open())
			{
				cout << "Can't open file( out)." << endl;
				return 1;
			}

			//ghi 5 ky tu nhap
			for (int i = 0; i < 5; i++)
			{
				out.put('0');
			}

			pawn = 5;
			pawn001 = 0;
			/*gg = 0;
			ff = 0;*/
		}

		if (pawn001 == 0)
		{
			out.put('\t');

			// Ghi ten file
			for (int i = 0; i < NameF.length(); i++)
			{
				out.put(NameF[i]);
				/*ff += (int)NameF[i];
				count++;*/
				pawn++;
			}
			out.put('\t');
			/*ff += (int)'\t';
			count++;*/
			pawn++;

			// Ghi byte cuoi
			out.put('0');
			/*ff += (int)'0';*/
			out.put('\t');
			/*ff += (int)'\t';*/
			/*count = count + 2;*/
			pawn = pawn + 2;

			// Ghi so thu tu
			for (int i = 0; i < so.length(); i++)
			{
				out.put(so[i]);
				/*ff += (int)so[i];
				count++;*/
				pawn++;
			}
			so.clear();
			out.put('\r');
			/*ff += (int)'\r';*/
			out.put('\n');
			/*ff += (int)'\n';
			count = count + 2;*/
			pawn = pawn + 2;

			pawn001 = 1;
		}

		/*if (count == 256)
		{
			count = 0;
			gg = gg ^ ff;
			ff = 0;
		}*/

		// Ghi noi dung
		in.get(chr);
		if (in.eof())
		{
			//ff -= (int)chr;
			break;
		}

		out.put(chr);
		/*ff += (int)chr;*/

		/*count++;*/
		pawn++;
	}

	/*out.seekp(-1, out._Seekcur);
	out.put(EOF);*/

	//ff += 255;	// ky tu eof
	//gg = gg ^ ff;


	//chuyen con tro file ve dau ghi lai 6 ky tu nhap
	//out.seekp(0,ios_base::beg);
	//out.seekp(0, out._Seekbeg);
	/*string t = Fun_IntToStr(gg);
	while (t.length() < 5)
	{
		t.insert(t.begin(), '0');
	}
	for (int i = t.length() - 5; i < t.length(); i++)
	{
		out.put(t[i]);
	}
	out.put('\t');*/

	// Ghi byte cuoi
	out.seekp(NameF.length() + 6 + 1, out._Seekbeg);
	out.put('1');

	in.close();
	out.close();

	Check_Sum(path);

	cout << "Upload success!" << endl;


	return 0;
}

vector<string> sortListDir(vector<string> ls)
{
	vector<string> a;
	int j = 0;
	int size = ls.size();
	while (j != size)
	{
		string dm = Fun_IntToStr(j);
		for (int i = 0; i < ls.size(); i++)
		{
			string test;
			for (int k = ls[i].find_last_of('_') + 1; k < ls[i].length(); k++)
			{
				test.push_back(ls[i][k]);
			}
			if (test.compare(dm) == 0)
			{
				a.push_back(ls[i]);
				ls.erase(ls.begin() + i);
				i--;
				break;
			}
		}
		j++;
	}

	return a;
}

vector<string> vtorListDir(const char path[], string Ip, int port)
{
	vector<string> c;
	char a[300];

	strcpy(a, path);
	strcat_s(a, 300, "*");
	WIN32_FIND_DATA data;
	HANDLE hFind = FindFirstFile(a, &data);      // DIRECTORY

	Ip.append("_");
	string por = Fun_IntToStr(port);
	Ip.append(por);
	c.push_back(Ip);

	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			c.push_back(data.cFileName);
		} while (FindNextFile(hFind, &data));
		FindClose(hFind);
	}

	return c;
}




void Thr_UpLoad()
{
	string path;
	char c;

	while (1)
	{
		cout << "Nhan u de upload file." << endl;
		cin >> c;
		if (c == 'u')
		{
			cout << "Nhap duong dan file: ";
			cin >> path;
			UpLoad(path.c_str());
			path.clear();
			system("pause");
			system("cls");
		}
	}
}

int UDP_SendSocket(char Addr[], unsigned short Port, char* SendBuf, int BufLen)
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

	char RecvBuf[3000];
	int BufLen = 3000;

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
	while (1)
	{
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


		for (int i = 0; i < iResult; i++)
		{
			temp.push_back(RecvBuf[i]);
		}

		thread sen(Thr_SendataF, temp, SenderAddr);
		sen.detach();
		//Thr_SendataF(temp, SenderAddr);
		temp.clear();
	}

	//---------------------------
	//Close the socket when finished receiving datagrams
	cout << "Finished receiving. Closing socket. \n";
	iResult = closesocket(RecvSocket);
	if (iResult == SOCKET_ERROR)
	{
		cout << "closesocket failed with error: " << WSAGetLastError();
		closesocket(RecvSocket);
		WSACleanup();
		return 1;
	}

	//----------------------------
	//Clean up and exit
	cout << "Exiting.\n";
	WSACleanup();
	return 0;
}

void Thr_SendataF(string temp, sockaddr_in SenderAddr)
{
	SenderAddr.sin_port = htons(5555);

	int iResult;
	static vector<AllFile> veve_str;

	// Gui lai gom phan \r\n fia dau data
	if (temp[0] == '\r' && temp[1] == '\n')
	{

		string  NameF, FiNa, stt;
		int loibit = 0, loi = 0;

		// Cau truc data \r\n...\r\n 
		for (int i = 2; i < temp.length() - 2; i++)
		{

			if (temp[i] == '\t')
			{
				// Neu bit danh dau file cuoi =2 thi la yeu cau gui lai
				if (temp[i + 1] == '2')
					loibit = 1;
				i = i + 3;
				NameF.append("_xox_");
				loi++;
			}
			NameF.push_back(temp[i]);
			if (loi == 0)
				FiNa.push_back(temp[i]);
			if (loi == 1)
				stt.push_back(temp[i]);



		}
		if (loibit == 1)
		{
			FiNa.insert(0, PATHFILESTORE);
			FiNa.push_back('/');
			FiNa.append(NameF);

			ifstream iif(FiNa, ios::binary);
			if (!iif.is_open())
			{
				cout << "Loi mo file (iif)." << endl;
				return;
			}

			string data;
			char c;

			while (1)
			{
				iif.get(c);
				if (iif.eof())
					break;
				data.push_back(c);
			}

			SOCKET SendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
			if (SendSocket == INVALID_SOCKET) {
				cout << "socket failed with error: " << WSAGetLastError() << endl;
				//WSACleanup();
				return;
			}

			iResult = sendto(SendSocket, data.c_str(), data.length(), 0, (SOCKADDR*)&SenderAddr, sizeof(SenderAddr));
			if (iResult == SOCKET_ERROR)
			{
				cout << "sendto failed with error: " << WSAGetLastError() << endl;
				closesocket(SendSocket);
				//WSACleanup();
				return;
			}
		}
		else
		{
			string fac_ip = inet_ntoa(SenderAddr.sin_addr);
			unsigned int fac_port = ntohs(SenderAddr.sin_port);

			string dun;
			unsigned int Stt = Fun_StrToInt(stt);

			mtx1.lock();
			for (int i = 0; i < veve_str.size(); i++)
			{
				if (veve_str[i].Name.compare(FiNa) == 0 && veve_str[i].Ip.compare(fac_ip) == 0 && veve_str[i].Port == fac_port)
				{
					for (int j = 0; j < veve_str[i].StrVec.size(); j++)
					{
						if (veve_str[i].StrVec[j].compare(NameF) == 0)
						{
							cout << "Da gui thanh cong " << NameF << endl;
							veve_str[i].StrVec.erase(veve_str[i].StrVec.begin() + j);
							//cout << veve_str[i].StrVec.size() << endl;

							// Luu thu tu cua file dau tien con sot lai trong kho
							if (veve_str[i].StrVec.size() != 0)
							{
								for (int k = veve_str[i].StrVec[0].find_last_of('_') + 1; k < veve_str[i].StrVec[0].length(); k++)
								{
									dun.push_back(veve_str[i].StrVec[0][k]);
								}
								veve_str[i].stt = Fun_StrToInt(dun);
								//cout << " dun " << dun << endl;
							}

							// Gui lai nhanh
							if (Stt - veve_str[i].stt + 1 >= 3)
							{
								cout << "Gui lai nhanh" << endl;
								string sss = Fun_IntToStr(veve_str[i].stt);
								string cpy;
								for (int x = 0; x <= NameF.find_last_of('_'); x++)
								{
									cpy.push_back(NameF[x]);
								}
								cpy.append(sss);

								cpy.insert(0, "/");
								cpy.insert(0, PATHFILESTORE);

								ifstream iif(cpy, ios::binary);
								if (!iif.is_open())
								{
									cout << "Loi mo file (iif)." << endl;
									return;
								}

								string data;
								char c;

								while (1)
								{
									iif.get(c);
									if (iif.eof())
										break;
									data.push_back(c);
								}

								SOCKET SendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
								if (SendSocket == INVALID_SOCKET) {
									cout << "socket failed with error: " << WSAGetLastError() << endl;
									//WSACleanup();
									return;
								}

								iResult = sendto(SendSocket, data.c_str(), data.length(), 0, (SOCKADDR*)&SenderAddr, sizeof(SenderAddr));
								if (iResult == SOCKET_ERROR)
								{
									cout << "sendto failed with error: " << WSAGetLastError() << endl;
									closesocket(SendSocket);
									//WSACleanup();
									return;
								}
							}

							break;
						}
					}

					if (veve_str[i].StrVec.size() == 0)
					{
						veve_str.erase(veve_str.begin() + i);
					}

					break;
				}
			}
			mtx1.unlock();
		}
	}
	else
	{
		AllFile A;

		// Luu dinh danh may gui
		A.Name = temp;
		A.Ip = inet_ntoa(SenderAddr.sin_addr);
		A.Port = ntohs(SenderAddr.sin_port);
		A.stt = 0;


		temp.insert(0, PATHFILESTORE);
		temp.push_back('/');
		vector<string> vlis = vtorListDir(temp.c_str());
		vlis = sortListDir(vlis);
		for (int i = 0; i < vlis.size(); i++)
		{
			cout << vlis[i] << endl;
		}

		A.StrVec = vlis;
		veve_str.push_back(A);

		//-------------------------------


		Sleep(300);
		int dme = 0;
		int j, dex;

		for (int i = 0; i < veve_str.size(); i++)
		{
			if (veve_str[i].Name.compare(A.Name) == 0 && veve_str[i].Ip.compare(A.Ip) == 0 && veve_str[i].Port == A.Port)
			{
				dex = i;
				break;
			}
		}

		for (int i = 0; i < vlis.size(); i++)
		{
			string sdm = Fun_IntToStr(dme);
			string path = temp;
			string str;

			path.append(vlis[i]);

			ifstream iif(path, ios::binary);
			if (!iif.is_open())
			{
				cout << "Open file error!" << endl;
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
			cout << str.length() << endl;



			while (1)
			{
				if (dme - veve_str[dex].stt < 10)
					break;
			}

			SOCKET SendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
			if (SendSocket == INVALID_SOCKET) {
				cout << "socket failed with error: " << WSAGetLastError() << endl;
				//WSACleanup();
				return;
			}

			iResult = sendto(SendSocket, str.c_str(), str.length(), 0, (SOCKADDR*)&SenderAddr, sizeof(SenderAddr));
			if (iResult == SOCKET_ERROR)
			{
				cout << "sendto failed with error: " << WSAGetLastError() << endl;
				closesocket(SendSocket);
				//WSACleanup();
				return;
			}
			cout << "da gui" << endl;

			closesocket(SendSocket);
			cout << dme << endl;
			dme++;

		}

		cout << temp << endl;
		temp.clear();
	}
}

void Check_Sum(string PATH)
{
	string path = PATH;
	//mtx.lock();
	ifstream iif(path, ios::binary);
	if (!iif.is_open())
	{
		cout << "loi mo file(iif)" << endl;
		return;
	}

	iif.seekg(5);
	char chr;
	string tank;
	int ff = 0, gg = 0, count = 0;
	while (1)
	{
		iif.get(chr);
		if (iif.eof())
			break;
		if ((count % 256) == 0)
		{
			gg = gg ^ ff;
			ff = 0;
		}

		ff += (int)chr;
		count++;
		tank.push_back(chr);
	}
	gg = gg ^ ff;
	iif.close();


	string so = Fun_IntToStr(gg);

	ofstream oof(path, ios::binary | ios::trunc);
	if (!oof.is_open())
	{
		cout << "loi mo file (oof)" << endl;
		return;
	}

	while (so.length() < 5)
		so.insert(so.begin(), '0');

	for (int i = so.length() - 5; i < so.length(); i++)
	{
		oof << so[i];
	}

	for (int i = 0; i < tank.length(); i++)
		oof << tank[i];

	oof.close();
	//mtx.unlock();
}




