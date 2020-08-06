

#include <iostream>
#include <map>
#include <string>
#include <stack>
#include <fstream>
#include <vector>
using namespace std;
bool stop = false;
bool isnumber(const string& s)
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
enum class Vtype {
    NONE = 0,
    INT = 1,
    STR = 2
};
struct typevariable
{
    // ���� �� ����� ��������� (�������������� ���� - INT ��� STR)
    int i;
    string s;

    // ��� ������
    Vtype type;

    // ������������� � int (���� ��� INT)
    operator int()
    {
        if (type == Vtype::INT)
            return i;
        return -1;
    }

    // ������������� � string (���� ��� STR)
    operator string()
    {
        if (type == Vtype::STR)
            return s;
        return "";
    }

    // ����������� �� int
    typevariable(int ival)
        : i(ival)
        , s()
        , type(Vtype::INT)
    {
    }

    // ����������� �� string
    typevariable(const string& sval)
        : i(0)
        , s(sval)
        , type(Vtype::STR)
    {
    }

    // ������ ������
    typevariable()
        : i(0)
        , s("")
        , type(Vtype::NONE)
    {
    }

    // ��������� � ������ (���� ������ - ������� � ��, ���� ����� - ����������)
    string printable()
    {
        if (type == Vtype::INT)
            return to_string(i);
        return s;
    }
};

// �������� (���� INT) ��� ������������ (���� STR)
typevariable operator+(const typevariable& a, const typevariable& b) {
    if (a.type != b.type)
        return -1;

    switch (a.type)
    {
    case Vtype::INT:
        return a.i + b.i;

    case Vtype::STR:
        return a.s + b.s;

    default:
        return -1;
    }
}

// �������� (������ ��� INT)
typevariable operator-(const typevariable& a, const typevariable& b)
{
    if (a.type != Vtype::INT || b.type != Vtype::INT)
        return -1;
    return a.i - b.i;
}

typevariable operator/(const typevariable& a, const typevariable& b)
{
    if (a.type != Vtype::INT || b.type != Vtype::INT)
        return -1;
    return a.i / b.i;
}
typevariable operator*(const typevariable& a, const typevariable& b)
{
    if (a.type != Vtype::INT || b.type != Vtype::INT)
        return -1;
    return a.i * b.i;
}
typevariable operator%(const typevariable& a, const typevariable& b)
{
    if (a.type != Vtype::INT || b.type != Vtype::INT)
        return -1;
    return a.i % b.i;
}

struct fixed_stack
{
    vector <typevariable> Stack;
    int back = -1;
    fixed_stack() : Stack(4096)
    {


    }

    void push(typevariable val)
    {
        back += 1;
        Stack[back] = val;
    }
    typevariable top()
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
    typevariable pop()
    {
        if (back > -1)
        {
            typevariable PopNum = Stack[back];
            back -= 1;
            return PopNum;

        }
        else
        {
            cout << "StackError: stack is empty" << endl;
            stop = true;
            return 0;
        }
        
        

    }
};
class run {
private:
    // �������� ���
    vector <string> source_code_;

    map <string,int> func_code;

    bool run_func = false;

    bool dont_run_code = false;

    fixed_stack mem_;

    // ������� � ��������� �� ������� ������������
    size_t pos_ = 0;

    int func_pos = -1;

    // ��������� ������ line. ���� ��������� ����������, false.
    bool step(const string& line);

    //map<string, typevariable*> variables;

public:
    // �����������, ���������������� ���������� ����������
    run(string fname)
    {
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

    }

    // ��������� ��������� num_steps ����� ��������� (false, ���� �������� ����������)
    bool exec() 
    {
        while (!stop)
        {
            
            if (run_func) 
            {
                exec_func();
            }
            else if (dont_run_code) 
            {
                if (source_code_[pos_].rfind("end",0) == 0) 
                {
                    dont_run_code = false;
                }
            }
            else if (pos_ < source_code_.size() && source_code_[pos_].rfind("def", 0) == 0  )
            {
                string funcname = "";
                int g = 4;
                while (true)
                {
                    if (g < source_code_[pos_].size()-1)
                    {
                        funcname += source_code_[pos_][g];
                    }
                    else if (source_code_[pos_][g] == ':')
                    {
                        break;
                    }
                    else
                    {
                        cout << "SyntexError: line don't have ':' in line " << source_code_[pos_];
                    }
                    g++;
                }
                func_code[funcname] = pos_;
                dont_run_code = true;
            }
            else
            {
                if (pos_ < source_code_.size())
                {
                    if (!step(source_code_[pos_]))
                    {
                        break;
                    }
                }
                else
                {
                    return false;
                }
            }
            if (!run_func)
            {
                pos_ += 1;
            }

        }
    return true;
    }
    void exec_func() 
    {
        func_pos += 1;
        if (source_code_[func_pos].rfind("end",0) == 0)
        {
            func_pos = -1;
            run_func = false;
            
        }
        else 
        {
            step(source_code_[func_pos]);
        }

    }
};
    

bool run::step(const string& line)
{
    // ������� ����������
    if (line.rfind("int", 0) == 0) 
    {
        int ch = 4;
        string number;
        while (true)
        {
            if (ch > line.size()) { break; }
            if (line[ch] == ' ') { break; }
            number += line[ch];
            ch++;
        }

        if (isnumber(number))
        {
            typevariable pushed = stoi(number);
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
        int ch = 4;
        string str;
        while (true)
        {
            if (ch > line.size()) { break; }
            else if (line[ch] == ' ') { break; }
            str += line[ch];
            ch++;
        }
        mem_.push(str);
    }

    
    // ����������
    else if (line.rfind("+",0) == 0)
    {
        typevariable a = mem_.pop();
        typevariable b = mem_.pop();

        mem_.push(a + b);
    }
    else if (line == "-")
    {
        typevariable a = mem_.pop();
        typevariable b = mem_.pop();

        mem_.push(a - b);
    }
    else if (line == "%")
    {
        typevariable a = mem_.pop();
        typevariable b = mem_.pop();

        mem_.push(a % b);
    }
    else if (line == "/")
    {
        typevariable a = mem_.pop();
        typevariable b = mem_.pop();

        mem_.push(a / b);
    }
    else if (line == "*")
    {
        typevariable a = mem_.pop();
        typevariable b = mem_.pop();

        mem_.push(a * b);
    }
    else if (line == "inc")

    {
        typevariable a = mem_.pop();
        mem_.push(a.i + 1);
    }
    else if (line == "dec")
    {
        typevariable a = mem_.pop();
        mem_.push(a.i - 1);
    }
    else if (line == "sqrt")
    {
        typevariable a = mem_.pop();
        mem_.push(pow(a.i, 2));
    }

    // ��������� �������
    else if (line == "dup") {
        typevariable a = mem_.top();
        mem_.push(a);
    }
    else if (line == "swap") {
        typevariable a = mem_.top();
        mem_.push(a);
    }
    else if (line == "pop")
    {
        mem_.pop();
    }
    else if (line == "print")
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
    else if (line == "printpop")
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
    else if (line == "stop")
    {
        //pos_ = source_code_.size() + 1;
        return false; 
    }

    else if (line.rfind("jmp", 0) == 0)     
    {
        
        string name = "";
        int g = 4;
        while (true)
        {
            if (g < line.size())
            {
                name += source_code_[pos_][g];
                g++;
            }
            else { break; }
        }
        func_pos = func_code[name];
        run_func = true;

    }
    else if (line.rfind("jne", 0) == 0) 
    {
        
    }
    else if (line.rfind("jle", 0) == 0) 
    {
        
    }
    else if (line.rfind("jge", 0) == 0) 
    {
        // Step 1
        // Fill me in
    }
    else if (line.rfind("jl", 0) == 0) 
    {
 
    }
    else if (line.rfind("jg", 0) == 0) 
    {

    }
    else if (line.rfind("je", 0) == 0) 
    {

    }

    // ���������
    else if (line.rfind("   ") || line.rfind("  ")) {}
    else if (line.rfind("pass", 0) == 0) {}

    else {
        cout << "InvalidSyntex: " << line << " line:" << pos_ << endl;
        return false;
    }
    return true;
}
int main() 
{
    run Run("code.txt");
    Run.exec();
}