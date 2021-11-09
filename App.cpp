#include "App.h"

// Выполнить следующие num_steps строк исходника (false, если закончил исполнение)
bool App::Exec(int num_steps = 1)
{
	try
	{
		for (int steps = 0;steps < num_steps; steps++)
		{
			if (pos_ < source_code_.size())
			{
				if (!Step(source_code_[pos_]))
				{
					break;
				}
			}
			else
			{
				//программа исполнена
				return false;
			}

			pos_ += 1;
		}
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		return false;
	}
	// программа не исполнена
	return true;
}

void App::Jump(const string& line, const string& command)
{
	string markname = "";
	// Отсекаем команду jmp
	int g = command.size() + 1;
	while (true)
	{
		if (g < line.size())
		{
			markname += source_code_[pos_][g];
			g++;
		}
		else { break; }
	}
	pos_ = placemarks[markname];
}

bool App::Step(const string& line)
{
	// Завести переменные
	if (line.rfind("int", 0) == 0)
	{
		string number;
		string command = "int";
		// Отсекаем команду int
		int ch = command.size() + 1;
		while (true)
		{
			if (ch > line.size()) { break; }
			if (line[ch] == ' ') { break; }
			number += line[ch];
			ch++;
		}

		if (isnumber(number))
		{
			TypeVariable pushed = stoi(number);
			mem_.push(pushed);
		}
		else
		{
			cout << "TypeError: it's integer not string in line " << pos_ + 1 << endl;
			return false;
		}
	}
	else if (line.rfind("str", 0) == 0)
	{
		string str;
		string command = "str";
		// Отсекаем команду str
		int ch = command.size() + 1;
		while (true)
		{
			if (ch > line.size()) { break; }
			else if (line[ch] == ' ') { break; }
			str += line[ch];
			ch++;
		}
		TypeVariable pushed = str;
		mem_.push(pushed);
	}
	// Арифметика
	else if (line.rfind("+", 0) == 0)
	{
		TypeVariable a = mem_.pop();
		TypeVariable b = mem_.pop();
		TypeVariable c = a + b;
		mem_.push(c);

	}
	else if (line.rfind("-", 0) == 0)
	{
		TypeVariable a = mem_.pop();
		TypeVariable b = mem_.pop();
		TypeVariable c = a - b;
		mem_.push(c);
	}
	else if (line.rfind("%", 0) == 0)
	{
		TypeVariable a = mem_.pop();
		TypeVariable b = mem_.pop();
		TypeVariable c = a % b;
		mem_.push(c);
	}
	else if (line.rfind("/", 0) == 0)
	{
		TypeVariable a = mem_.pop();
		TypeVariable b = mem_.pop();
		TypeVariable c = a / b;
		mem_.push(c);
	}
	else if (line.rfind("*", 0) == 0)
	{
		TypeVariable a = mem_.pop();
		TypeVariable b = mem_.pop();
		TypeVariable c = a * b;
		mem_.push(c);
	}
	else if (line.rfind("inc", 0) == 0)

	{
		TypeVariable a = mem_.pop();
		TypeVariable b = 1;
		TypeVariable c = a + b;
		mem_.push(c);
	}
	else if (line.rfind("dec", 0) == 0)
	{
		TypeVariable a = mem_.pop();
		TypeVariable b = 1;
		TypeVariable c = a - b;
		mem_.push(c);
	}
	else if (line.rfind("sqrt", 0) == 0)
	{
		TypeVariable a = mem_.pop();
		mem_.push(pow(a.i, 2));
	}
	else if (line.rfind("dup", 0) == 0)
	{
		TypeVariable a = mem_.top();
		mem_.push(a);
	}
	else if (line.rfind("swap", 0) == 0)
	{
		TypeVariable a = mem_.top();
		TypeVariable b = mem_.top();
		mem_.push(a);
		mem_.push(b);
	}
	else if (line.rfind("pop", 0) == 0)
	{
		mem_.pop().type;
	}
	else if (line.rfind("pushN", 0) == 0)
	{
		// Отсекаем команду
		string val;
		string command = "pushN";
		int g = command.size() + 1;

		while (true)
		{
			if (line[g] == ' ') { break; }
			if (g < source_code_[pos_].size())
			{
				val += source_code_[pos_][g];
			}
			else break;
			g++;
		}
		if (isnumber(val))
		{
			mem_.pushN(stoi(val));
		}
		else
		{
			cout << "TypeError: it's integer not string in line " << pos_ + 1 << endl;
			return false;
		}
	}
	else if (line.rfind("printpop", 0) == 0)
	{
		if (mem_.top().s.empty())
		{
			cout << mem_.pop().i << endl;
		}
		else
		{
			cout << mem_.pop().s << endl;
		}
	}
	else if (line.rfind("print", 0) == 0)
	{
		if (mem_.top().s.empty())
		{
			cout << mem_.top().i << endl;
		}
		else
		{
			cout << mem_.top().s << endl;
		}
	}
	else if (line.rfind("recv", 0) == 0)
	{
		TypeVariable a;
		cin >> a.s;
		mem_.push(a);
	}
	else if (line == "stop")
	{
		return false;
	}
	else if (line.rfind("jmp", 0) == 0)
	{
		Jump(line);
	}
	else if (line.rfind("jne", 0) == 0)
	{
		TypeVariable a = mem_.pop();
		TypeVariable b = mem_.pop();
		if (a != b)
		{
			Jump(line, "jne");
		}
	}
	else if (line.rfind("jle", 0) == 0)
	{
		TypeVariable a = mem_.pop();
		TypeVariable b = mem_.pop();
		if (a <= b)
		{
			Jump(line, "jle");
		}
	}
	else if (line.rfind("jge", 0) == 0)
	{
		TypeVariable a = mem_.pop();
		TypeVariable b = mem_.pop();
		if (a >= b)
		{
			Jump(line, "jge");
		}
	}
	else if (line.rfind("jl", 0) == 0)
	{
		TypeVariable a = mem_.pop();
		TypeVariable b = mem_.pop();
		if (a < b)
		{
			Jump(line, "jl");
		}
	}
	else if (line.rfind("jg", 0) == 0)
	{
		TypeVariable a = mem_.pop();
		TypeVariable b = mem_.pop();
		if (a > b)
		{
			Jump(line, "jg");
		}
	}
	else if (line.rfind("je", 0) == 0)
	{
		TypeVariable a = mem_.pop();
		TypeVariable b = mem_.pop();
		if (a == b)
		{
			Jump(line, "je");
		}
	}
	else if (line.rfind(":") == 0) {}
	// Остальное
	else if (line.empty()) {}
	else if (line.rfind("pass", 0) == 0) {}
	else
	{
		cout << "InvalidSyntax: " << line << " line:" << pos_ << endl;
		return false;
	}
	return true;
}