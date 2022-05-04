#include<iostream>
#include<Windows.h>
#include<time.h>

using namespace std;

#define CHAIRS 5 //椅子数目
#define MAX_COUNT 10 //顾客最多人数
HANDLE Mutex = CreateMutex(NULL,FALSE,L"Mutex");//用来实现进程的互斥(L是指后面的字符使用unicode格式)
HANDLE barbers = CreateSemaphore(NULL, 1, 1, L"barbers");//进行线程间的同步
HANDLE customers = CreateSemaphore(NULL, 0, CHAIRS, L"customers");
int count = 0;//顾客
int waiting = 0;//等待的顾客数
int finish = 0;//理发完毕的顾客数

//随机函数
int random()
{
	srand((int)time(NULL));
	return rand()% 5000;
}

//顾客进程
DWORD WINAPI customer(LPVOID pParm2)
{
	if (ReleaseSemaphore(customers,1,NULL))	//V(customer)
	{
		WaitForSingleObject(Mutex, INFINITE);
		::count++;
		cout << "你是第 " << ::count << " 位顾客，欢迎您的到来^_^" << endl;
		if (waiting!=0)
		{
			cout << "现在有" << waiting << " 位顾客等待理发，请您耐心等待^_^" << endl;
		}
		else
		{
			cout << "没有顾客在理发，我马上为您服务^_^" << endl;//输出有多少人在等待
		}
		waiting++;
		ResumeThread(customers);//唤醒理发师进程
		ReleaseMutex(Mutex);//释放互斥量，以便其他线程使用
		WaitForSingleObject(barbers, INFINITE);//等待理发
	}
	else
	{
		::count++;
		//没有椅子，顾客直接离开
		cout << "对不起，没有空椅子......第" << ::count << "个顾客离开理发店" << endl;
	}
	return 0;
}

//理发师线程
DWORD WINAPI barber(LPVOID pParm1)
{
	while (true)
	{
		WaitForSingleObject(customers, INFINITE);//P(customers)，等待顾客
		WaitForSingleObject(Mutex, INFINITE);//等待互斥量
		ReleaseSemaphore(barbers, 1, NULL);//释放信号量
		ResumeThread(barbers);//唤醒顾客进程
		Sleep(5000);
		finish++;//理发完毕的顾客数
		cout << "第" << finish << "个顾客理发完毕，离开" << endl;
		waiting--;//等待的人数减少1
		ReleaseMutex(Mutex);//V(mutex);
	}
	return 0;
}

int main()
{
	char close_door='n';//初始默认开张
	cout << "***************新店开张，热烈欢迎光大顾客的光临！！***********" << endl;
	cout << "本店中共有" << CHAIRS << "把椅子" << endl;
	HANDLE hThreadCustomer;
	HANDLE hThreadBarder;
	hThreadBarder = CreateThread(NULL, 0, barber, NULL, 0, NULL); //产生一个理发师进程
	while (close_door != 'y')
	{
		Sleep(random());//rand()函数实现顾客随机到来 
		cout<<endl<<"正在营业，请进！"<<endl;
		if (finish >= MAX_COUNT) //如果完成数超过 10 并且没有人等待
		{
			while (waiting != 0)
			{
				Sleep(1000); continue;
			}
			cout << "已经为" << finish << "个顾客理发了，是否停止营业?" << endl << "输入 y 或"
				"n"<<endl; //提示是否关门
			cin >> close_door;
			if (close_door == 'y')
			{
				cout << "暂停营业！欢迎下次光临！" << endl;
				system("pause");
				return 0;
			}

			else
			{
				finish = 0;
				::count = 0;
				cout << "继续营业" << endl;
			}
		} 
		hThreadCustomer = CreateThread(NULL, 0, customer, NULL, 0, NULL);
	}
	return 0;
}


