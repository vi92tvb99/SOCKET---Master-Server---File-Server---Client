#include"Header.h"
#define PORT_CONNECT_FILESERVER 1111 //port de file server connect
#define DEFAULT_PORT 2222;
#define PORT_CONNECT_CLIENT 3333

int main()
{
	/*string a;

	TCP_Recv(a);

	cout << a << endl;


	WSADATA wsaData;
	int iResult;
	string a;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	//-------------------------
	// Write here
	//-------------------------
	*/

	//thread thr1(TCP_Recv_FileSer);

	//thr1.join();

	//system("pause");
	//WSACleanup();
	/*string str;
	cout << "NHAP = ";
	fflush(stdin);
	getline(std::cin, str);
	int x = str.size();
	char str1[30] = "ServerFile\\\\17.25_7777.txt";
	luuFileList(str1, "le truong vi \n tran chi vi" , 100);*/

	//TCP_Recv_FileSer("127.0.0.1",PORT_CONNECT_FILESERVER); 
	TCP_Recv_Client("127.0.0.1", 2222);
	return 0;

}

