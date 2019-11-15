#include"Header.h"
#define DEFAULT_PORT 3333

void main()
{
	int iResult;
	WSADATA wsaData;

	//----------------------
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		wprintf(L"WSAStartup failed with error: %d\n", iResult);
		return;
	}

	//---------------------------
	// Write here
	//---------------------------

	//string recv;
	//string ip, port, name;
	char send[4] = "yes";
	//cin >> send;

	TCP_Send("127.0.0.1", 2222, send, sizeof(send));

	//string test;
	//TCP_Recv_Master(test, "127.0.0.1", 2222);
	//TCP_Recv_Master(recv);
	//cout << recv << endl;

	//int ke = 0, ka = 0;
	//vector<string> lis;
	//for (int i = 0; i < recv.length(); i++)
	//{
	//	if (ke == 0)
	//	{
	//		if (recv[i] == '\n')
	//		{
	//			ke++;
	//			continue;
	//		}
	//		if (recv[i] == '_')
	//		{
	//			ka++;
	//			continue;
	//		}
	//		if (ka == 0)
	//			ip.push_back(recv[i]);
	//		if (ka == 1)
	//			port.push_back(recv[i]);
	//	}
	//	else
	//	{
	//		for (int j = i; j < recv.length(); j++)
	//		{
	//			if (recv[j] == '\n')
	//			{
	//				lis.push_back(name);
	//				name.clear();
	//				continue;
	//			}
	//			name.push_back(recv[j]);
	//		}
	//		break;
	//	}
	//}

	//for (int i = 0; i < lis.size(); i++)
	//{
	//	cout << i + 1 << ". " << lis[i] << endl;
	//}

	//cout << "Chon ten file: " << endl;
	//int choo;
	//cin >> choo;

	//name = lis[choo - 1];

	//// Do may local nen chinh ip "127.0.0.1"
	//ip.clear();
	//ip = "127.0.0.1";


	//string str, ip, port, name;
	/*cout << "Nhap ip, port, ten file can tai (ip_port_name): ";
	cin >> str;*/
	/*str = "127.0.0.1_4444_123.pdf";
	Get_Ip_Port_Name(str, ip, port, name);
	int por = Fun_StrToInt(port);
	UDP_SendSocket(ip.c_str(), por, (char*)name.c_str(), name.length());

	UDP_RecvSocket(5555);

	/*Test("../store/test.docx");*/



	//WSACleanup();*/
	//string s;
	//TCP_Recv_Master(s, "127.0.0.1", DEFAULT_PORT);


}