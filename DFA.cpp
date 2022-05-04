#include <iostream>
#include <fstream>
#include "DFA.h"

using namespace std;

//���ļ���ȡ��������
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

//����ַ����Ƿ����ظ������ظ����ص�һ���ظ����ַ������򷵻�-1
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

//�ж��Ƿ��ڽ���״̬��
bool DFA::inEndStates(char state)
{
	return this->endStates.find(state) != this->endStates.npos;
}

//�ж��Ƿ���״̬��
bool DFA::inStates(char state)
{
	return this->states.find(state) != this->states.npos;
}

//�ж��Ƿ����ַ���
bool DFA::inLetters(char letter)
{
	return this->letters.find(letter) != this->letters.npos;
}

//��鼯��
void DFA::checkSets()
{
	char repetition = -1;
	repetition = this->checkRepetition(this->letters);
	if (repetition != -1)
	{
		cout << "�ַ������ظ���" << repetition << endl;
	}
	repetition = this->checkRepetition(this->states);
	if (repetition != -1)
	{
		cout << "״̬�����ظ���" << repetition << endl;
	}
	if (this->startState.length() > 1)
	{
		cout << "��ʼ״̬��ֹһ��" << endl;
	}
	if (!this->inStates(this->startState[0]))
	{
		cout << "��ʼ״̬����״̬����" << endl;
	}
	for (int i = 0; i < this->endStates.size(); i++)
	{
		if (!this->inStates(this->endStates[i]))
		{
			cout << "����״̬����״̬���У�" << this->endStates[i] << endl;
		}
	}
}

//���ת������
void DFA::checkRules()
{
	for (int i = 0; i < this->transitionRules.size(); i++)
	{
		TransitionRule rule = this->transitionRules[i];
		if (!this->inStates(rule.getCurState()))
		{
			cout << "��ǰ״̬����״̬���У�" << rule.getCurState() << endl;
		}
		if (!this->inLetters(rule.getInputLetter()))
		{
			cout << "�����ַ������ַ����У�" << rule.getInputLetter() << endl;
		}
		if (!this->inStates(rule.getNextState()))
		{
			cout << "��һ״̬����״̬���У�" << rule.getNextState() << endl;
		}
	}
}

//�ƶ�����һ��״̬��δ�ҵ�ƥ��Ĺ��򷵻�-1
char DFA::moveToNextState(char curState, char inputLetter)
{
	for (int i = 0; i < this->transitionRules.size(); i++)//�������й���
	{
		if (this->transitionRules[i].getCurState() == curState && 
			this->transitionRules[i].getInputLetter() == inputLetter)
		{
			return this->transitionRules[i].getNextState();
		}
	}
	return -1;
}

//�ж��Ƿ����ʶ��
bool DFA::identifyLegal(string str)
{
	char curState = this->startState[0];//��ʼʱ��ǰ״̬��Ϊ��ʼ״̬
	for (int i = 0; i < str.length(); i++)
	{
		curState = this->moveToNextState(curState, str[i]);
		if (curState == -1)//����Ҳ�����Ӧ�Ĺ���
		{
			return false;//�����Ϲ���
		}

	}
	if (!this->inEndStates(curState))//�������й���������״̬�����ڽ���״̬��
	{
		return false;//��Ȼ��Ϊ����ʶ��
	}
	return true;
}

//�ݹ��������кϷ����ַ��� num���ַ���ʣ����󳤶ȣ�str����ǰ�ַ�����count�����ɵ��ַ�������
void DFA::generateLegal(int num, string str, int& count)
{
	if (num <= 0)//û��ʣ�೤�ȣ������Ѿ�������󳤶�
	{
		return;
	}
	for (int i = 0; i < this->letters.length(); i++)//�ڵ�ǰ�ַ�����ֱ�����ַ����еĲ�ͬ�ַ�
	{
		str += this->letters[i];//����һ���ַ�
		if (this->identifyLegal(str))//�����ַ������ʶ�������
		{
			cout << str << endl;
			count++;
			this->generateLegal(num - 1, str, count);//ʣ�೤��-1������Ѱ��
		}
		else//�����ַ��󲻿�ʶ��
		{
			this->generateLegal(num - 1, str, count);
		}
		str.pop_back();//ȥ���¼�����ַ�
	}
}

//�����ַ����Ƿ�ɱ�ʶ��
void DFA::analyseLegal()
{
	cout << "������Ҫʶ����ַ�����";
	string str;
	cin >> str;
	if (this->identifyLegal(str))
	{
		cout << str << "���Ա�ʶ��" << endl;
	}
	else
	{
		cout << str << "���ܱ�ʶ��" << endl;
	}
}

//��ʾ���г��Ȳ���������ֵ�Ŀ���ʶ����ַ���
void DFA::showAllLegal()
{
	cout << "��������󳤶ȣ�";
	int num;
	cin >> num;
	int count = 0;
	cout << "��󳤶�Ϊ" << num << "�Ŀ��Ա�ʶ����ַ���Ϊ��" << endl;
	this->generateLegal(num, "", count);
	cout << "��󳤶�Ϊ"<<num<<"�Ŀ��Ա�ʶ����ַ�������Ϊ��" << count << endl;
}

//��ʾ��ǰDFA����Ϣ
void DFA::displayInfo()
{
	cout << "�ַ�����" << this->letters << endl;
	cout << "״̬����" << this->states << endl;
	cout << "��ʼ״̬��" << this->startState << endl;
	cout << "����״̬����" << this->endStates << endl;
	cout << "״̬ת����" << endl;
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