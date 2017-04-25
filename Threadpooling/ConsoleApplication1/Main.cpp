#include "stdafx.h"
#include <iostream>
#include "ThreadPoolMgr.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{

	CThreadPoolMgr MGR;

	int Count = 0;
	int input;

	cout << "************************************************************\n\n\nWelcome to ThreadPool!!!\n\nEnter number of thread that u want to start(U can only create 5 threads):)\n";
	cin >> Count;


	cout << "Initializing " << Count << "threads...\n";
	MGR.Initialize(Count);

	MGR.AssignTask("1 task");
	MGR.AssignTask("2 task");

	MGR.AssignTask("3 task");

	MGR.AssignTask("4 task");
	
	MGR.AssignTask("5 task");

	MGR.AssignTask("6 task");
	MGR.AssignTask("7 task");

	MGR.AssignTask("8 task");

	Sleep(5000);
	MGR.ShutDown();
	cin >> input;

	return 0;
}