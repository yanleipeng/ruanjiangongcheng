#include <iostream>
#include <fstream>
#include "DFA.h"

using namespace std;

//从文件读取所有数据
DFA::DFA(string fileName)
{
	fstream file(fileName);
	string tmp;
	file >> this->letters;
	file >> this->states;
	file >> this->startState;
	file >> this->endStates;
	this->checkSets();
	while (!file.eof())
	{
		file >> tmp;
		this->transitionRules.push_back(TransitionRule(tmp[0], tmp[1], tmp[2]));
	}
	this->checkRules();
}

//检查字符串是否有重复，有重复返回第一个重复的字符，否则返回-1
char DFA::checkRepetition(string str)
{
	for (int i = 0; i < str.length(); i++)
	{
		for (int j = 0; j < str.length(); j++)
		{
			if (str[i] == str[j] && i != j)
			{
				return str[i];
			}
		}
	}
	return -1;
}

//判断是否在结束状态集
bool DFA::inEndStates(char state)
{
	return this->endStates.find(state) != this->endStates.npos;
}

//判断是否在状态集
bool DFA::inStates(char state)
{
	return this->states.find(state) != this->states.npos;
}

//判断是否在字符集
bool DFA::inLetters(char letter)
{
	return this->letters.find(letter) != this->letters.npos;
}

//检查集合
void DFA::checkSets()
{
	char repetition = -1;
	repetition = this->checkRepetition(this->letters);
	if (repetition != -1)
	{
		cout << "字符集有重复：" << repetition << endl;
	}
	repetition = this->checkRepetition(this->states);
	if (repetition != -1)
	{
		cout << "状态集有重复：" << repetition << endl;
	}
	if (this->startState.length() > 1)
	{
		cout << "开始状态不止一个" << endl;
	}
	if (!this->inStates(this->startState[0]))
	{
		cout << "开始状态不在状态集中" << endl;
	}
	for (int i = 0; i < this->endStates.size(); i++)
	{
		if (!this->inStates(this->endStates[i]))
		{
			cout << "结束状态不在状态集中：" << this->endStates[i] << endl;
		}
	}
}

//检查转换规则
void DFA::checkRules()
{
	for (int i = 0; i < this->transitionRules.size(); i++)
	{
		TransitionRule rule = this->transitionRules[i];
		if (!this->inStates(rule.getCurState()))
		{
			cout << "当前状态不在状态集中：" << rule.getCurState() << endl;
		}
		if (!this->inLetters(rule.getInputLetter()))
		{
			cout << "输入字符不在字符集中：" << rule.getInputLetter() << endl;
		}
		if (!this->inStates(rule.getNextState()))
		{
			cout << "下一状态不在状态集中：" << rule.getNextState() << endl;
		}
	}
}

//移动到下一个状态，未找到匹配的规则返回-1
char DFA::moveToNextState(char curState, char inputLetter)
{
	for (int i = 0; i < this->transitionRules.size(); i++)//遍历所有规则
	{
		if (this->transitionRules[i].getCurState() == curState && 
			this->transitionRules[i].getInputLetter() == inputLetter)
		{
			return this->transitionRules[i].getNextState();
		}
	}
	return -1;
}

//判断是否可以识别
bool DFA::identifyLegal(string str)
{
	char curState = this->startState[0];//初始时当前状态置为开始状态
	for (int i = 0; i < str.length(); i++)
	{
		curState = this->moveToNextState(curState, str[i]);
		if (curState == -1)//如果找不到对应的规则
		{
			return false;//不符合规则
		}

	}
	if (!this->inEndStates(curState))//符合所有规则但是最后的状态不属于结束状态集
	{
		return false;//仍然视为不可识别
	}
	return true;
}

//递归生成所有合法的字符串 num：字符串剩余最大长度，str：当前字符串，count：生成的字符串总数
void DFA::generateLegal(int num, string str, int& count)
{
	if (num <= 0)//没有剩余长度，代表已经到了最大长度
	{
		return;
	}
	for (int i = 0; i < this->letters.length(); i++)//在当前字符串后分别加入字符集中的不同字符
	{
		str += this->letters[i];//加入一个字符
		if (this->identifyLegal(str))//加入字符后可以识别则输出
		{
			cout << str << endl;
			count++;
			this->generateLegal(num - 1, str, count);//剩余长度-1，继续寻找
		}
		else//加入字符后不可识别
		{
			this->generateLegal(num - 1, str, count);
		}
		str.pop_back();//去掉新加入的字符
	}
}

//分析字符串是否可被识别
void DFA::analyseLegal()
{
	cout << "请输入要识别的字符串：";
	string str;
	cin >> str;
	if (this->identifyLegal(str))
	{
		cout << str << "可以被识别" << endl;
	}
	else
	{
		cout << str << "不能被识别" << endl;
	}
}

//显示所有长度不超过给定值的可以识别的字符串
void DFA::showAllLegal()
{
	cout << "请输入最大长度：";
	int num;
	cin >> num;
	int count = 0;
	cout << "最大长度为" << num << "的可以被识别的字符串为：" << endl;
	this->generateLegal(num, "", count);
	cout << "最大长度为"<<num<<"的可以被识别的字符串总数为：" << count << endl;
}

//显示当前DFA的信息
void DFA::displayInfo()
{
	cout << "字符集：" << this->letters << endl;
	cout << "状态集：" << this->states << endl;
	cout << "开始状态：" << this->startState << endl;
	cout << "结束状态集：" << this->endStates << endl;
	cout << "状态转换表：" << endl;
	for (int i = 0; i < this->transitionRules.size(); i++)
	{
		cout << this->transitionRules[i].getCurState() << this->transitionRules[i].getInputLetter() 
			<< this->transitionRules[i].getNextState() << endl;
	}
}
DFA dfa("lab1.dfa");
	dfa.displayInfo();
	dfa.analyseLegal();
	dfa.showAllLegal();