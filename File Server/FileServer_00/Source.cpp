#include"function.h"
#define PATHFILESTORE "../FileStored/"



void main()
{
	/*

	//----------------------
	// Write here
	//----------------------

	//----------------------
	//Chon IP
	int choose;
	string Ip;
	vector<string> lsIpLocal;

	cout << "Card mang: " << endl;
	lsIpLocal = Fun_ListIP();
	for (int i = 0; i < lsIpLocal.size(); i++)
	{
		cout << i + 1 << ". " << lsIpLocal[i] << endl;
	}
	cout << "Chon card mang: ";
	cin >> choose;

	Ip = lsIpLocal[choose - 1];

	cout << Ip << endl;
	system("pause");
	system("cls");

	//---------------------
	//

	/*thread thr1(Thr_ConnectToServer, Ip);
	thr1.detach();*/

	//Thr_UpLoad();


	/*UDP_RecvSocket(4444);

	WSACleanup();
	system("pause");
	return;*/

	/*vector <string> x = vtorListDir(PATHFILESTORE);

	for (int i = 0; i < x.size(); i++)
	{
		cout << x[i] << " " << endl;
	}*/

	vector<string> a;
	char* b;

	a = vtorListDir(PATHFILESTORE, "127.0.0.1", 4444);
	b = chrListDir(a);

	TCP_Send("127.0.0.1", 1111, b, strlen(b));


	delete[] b;


}
