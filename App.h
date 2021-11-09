#pragma once
#include <iostream>
#include <map>
#include <string>
#include <stack>
#include <fstream>
#include <vector>

using std::string;
using std::to_string;
using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::map;
using std::getline;
using std::ifstream;
using std::exception;

inline bool isnumber(const string& s)
{
	for (int cycle = 0; cycle < s.size() - 1; cycle++)
	{
		if (!isdigit(s[cycle]))
		{
			return false;
		}
	}
	return true;
}

enum class VType
{
	NONE = 0,
	INT = 1,
	STR = 2,
	ERROR = 3
};

struct TypeVariable
{
	// ���� �� ����� ��������� (�������������� ���� - INT ��� STR)
	int i;
	string s;

	// ��� ������
	VType type;

	// ������������� � int (���� ��� INT)
	operator int()
	{
		if (type == VType::INT)
			return i;
		return -1;
	}

	// ������������� � string (���� ��� STR)
	operator string()
	{
		if (type == VType::STR)
			return s;
		return "";
	}

	// ����������� �� int
	TypeVariable(int ival)
		: i(ival)
		, s()
		, type(VType::INT)
	{}

	// ����������� �� string
	TypeVariable(const string& sval)
		: i(0)
		, s(sval)
		, type(VType::STR)
	{}

	// ������ ������
	TypeVariable()
		: i(0)
		, s("")
		, type(VType::NONE)
	{}

	// ��������� � ������ (���� ������ - ������� � ��, ���� ����� - ����������)
	string printable()
	{
		if (type == VType::INT)
			return to_string(i);
		return s;
	}
};

inline bool operator==(const TypeVariable& a, const TypeVariable& b)
{
	if (a.type != b.type)
	{
		return false;
	}
	switch (a.type)
	{
	case VType::INT:
		return a.i == b.i;

	case VType::STR:
		return a.s == b.s;

	default:
		return false;
	}
}

inline bool operator!=(const TypeVariable& a, const TypeVariable& b)
{
	return !operator==(a, b);
}

inline bool operator>(const TypeVariable& a, const TypeVariable& b)
{
	if (a.type != VType::INT || b.type != VType::INT)
	{
		return false;
	}
	return a.i > b.i;
}

inline bool operator<(const TypeVariable& a, const TypeVariable& b)
{
	if (a.type != VType::INT || b.type != VType::INT)
	{
		return false;
	}
	return !operator>(a, b);
}

inline bool operator>=(const TypeVariable& a, const TypeVariable& b)
{
	if (a.type != VType::INT || b.type != VType::INT)
	{
		return false;
	}
	return a.i >= b.i;
}

inline bool operator<=(const TypeVariable& a, const TypeVariable& b)
{
	if (a.type != VType::INT || b.type != VType::INT)
	{
		return false;
	}
	return !operator>=(a, b);
}
// �������� (���� INT) ��� ������������ (���� STR)
inline TypeVariable operator+(const TypeVariable& a, const TypeVariable& b)
{
	if (a.type != b.type)
	{
		throw std::exception("TypeError: string can't + integer");
	}

	switch (a.type)
	{
	case VType::INT:
		return a.i + b.i;

	case VType::STR:
		return a.s + b.s;

	default:
		throw std::exception("TypeError: string can't + integer");
	}
}

// �������� (������ ��� INT)
inline TypeVariable operator-(const TypeVariable& a, const TypeVariable& b)
{
	if (a.type != VType::INT || b.type != VType::INT)
	{
		throw std::exception("TypeError: string can't - integer");
	}
	return a.i - b.i;
}

inline TypeVariable operator/(const TypeVariable& a, const TypeVariable& b)
{
	if (a.type != VType::INT || b.type != VType::INT)
	{
		throw std::exception("TypeError: string can't / integer");
	}
	return a.i / b.i;
}

inline TypeVariable operator*(const TypeVariable& a, const TypeVariable& b)
{
	if (a.type != VType::INT || b.type != VType::INT)
	{
		throw std::exception("TypeError: string can't - integer");
	}
	return a.i * b.i;
}

inline TypeVariable operator%(const TypeVariable& a, const TypeVariable& b)
{
	if (a.type != VType::INT || b.type != VType::INT)
	{
		throw std::exception("TypeError: string can't % integer");
	}
	return a.i % b.i;
}

struct FixedStack
{
	vector <TypeVariable> Stack;
	int back = -1;
	FixedStack() : Stack(4096)
	{}

	void push(TypeVariable val)
	{
		back++;
		if (back > Stack.size() - 1)
		{
			throw std::exception("StackError: stack overflow");
		}
		Stack[back] = val;
	}
	void pushN(int val)
	{
		if (back < val)
		{
			throw std::exception("StackError: index out of the range");
		}
		push(Stack[back - val]);
	}
	TypeVariable top()
	{
		if (back > -1)
		{
			return Stack[back];
		}
		else
		{
			return ' ';
		}
	}
	TypeVariable pop()
	{
		if (back > -1)
		{
			TypeVariable poped = Stack[back];
			back--;
			return poped;
		}
		else
		{
			throw std::exception("StackError: stack is empty");
		}
	}
};

class App
{
private:
	// �������� ���
	vector <string> source_code_;

	// �����
	map <string, int> placemarks;

	// ������������ ����
	FixedStack mem_;

	// ������� � ��������� �� ������� ������������
	size_t pos_ = 0;

	// process id
	int pid = 0;

	// ��������� ������ line. ���� ��������� ����������, false.
	bool Step(const string& line);
	void Jump(const string& line, const string& command = "jmp");
public:
	// �����������, ���������������� ���������� ����������
	App(string fname, int proccesid = 0)
	{
		pid = proccesid;
		ifstream SourceFile(fname);
		while (true)
		{
			string buf;
			getline(SourceFile, buf);
			if (buf.empty())
			{
				break;
			}
			source_code_.push_back(buf);
		}
		for (int pos = 0; pos < source_code_.size();pos++)
		{
			string line = source_code_[pos];
			if (line.rfind(":", 0) == 0)
			{
				string placemarkname = "";
				// �������� ������ ���������� �����
				string symbol = ":";
				int g = symbol.size();

				while (true)
				{
					if (g < source_code_[pos_].size())
					{
						placemarkname += source_code_[pos_][g];
					}
					else break;
					g++;
				}
				placemarks[placemarkname] = pos_;
			}
		}
	}
	// ��������� ��������� num_steps ����� ��������� (false, ���� �������� ����������)
	bool Exec(int num_steps);
};


