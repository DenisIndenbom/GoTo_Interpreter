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
enum class Vtype 
{
    NONE = 0,
    INT = 1,
    STR = 2
};
struct typevariable
{
    // Одно из полей заполнено (соответственно типу - INT или STR)
    int i;
    string s;

    // Тип ячейки
    Vtype type;

    // Преобразовать в int (если тип INT)
    operator int()
    {
        if (type == Vtype::INT)
            return i;
        return -1;
    }

    // Преобразовать в string (если тип STR)
    operator string()
    {
        if (type == Vtype::STR)
            return s;
        return "";
    }

    // Конструктор из int
    typevariable(int ival)
        : i(ival)
        , s()
        , type(Vtype::INT)
    {
    }

    // Конструктор из string
    typevariable(const string& sval)
        : i(0)
        , s(sval)
        , type(Vtype::STR)
    {
    }

    // Пустая клетка
    typevariable()
        : i(0)
        , s("")
        , type(Vtype::NONE)
    {
    }

    // Перевести в строку (если строка - вернуть её же, если число - закавычить)
    string printable()
    {
        if (type == Vtype::INT)
            return to_string(i);
        return s;
    }
};

// Сложение (если INT) или конкатенация (если STR)
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

// Разность (только для INT)
typevariable operator-(const typevariable& a, const typevariable& b)
{
    if (a.type != Vtype::INT || b.type != Vtype::INT)
        cout << "TypeError: string can't - integer" << endl;
        stop = true;
        return -1;
    return a.i - b.i;
}

typevariable operator/(const typevariable& a, const typevariable& b)
{
    if (a.type != Vtype::INT || b.type != Vtype::INT)
        cout << "TypeError: string can't / integer" << endl;
        stop = true;
        return -1;
    return a.i / b.i;
}
typevariable operator*(const typevariable& a, const typevariable& b)
{
    if (a.type != Vtype::INT || b.type != Vtype::INT)
        cout << "TypeError: string can't - integer" << endl;
        stop = true;
        return -1;
    return a.i * b.i;
}
typevariable operator%(const typevariable& a, const typevariable& b)
{
    if (a.type != Vtype::INT || b.type != Vtype::INT)
        cout << "TypeError: string can't % integer" << endl;
        stop = true;
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
        if (back > Stack.size() - 1) 
        {
            cout << "StackError: stack overflow" << endl;
            stop = true;
            return;
        }
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
            return -1;
        }
        
        

    }
};
class run 
{
private:
    // Исходный код
    vector <string> source_code_;

    map <string,int> placemarks;

    fixed_stack mem_;

    // Позиция в исходнике на которой остановились
    size_t pos_ = 0;

    // Исполнить строку line. Если произошла блокировка, false.
    bool step(const string& line);

    //map<string, typevariable*> variables;

public:
    // Конструктор, инициализирующий внутренние переменные
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

    // Выполнить следующие num_steps строк исходника (false, если закончил исполнение)
    bool exec() 
    {
        while (!stop)
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
            
            pos_ += 1;

        }
        return true;
    }
};
    

bool run::step(const string& line)
{
    // Завести переменные
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

    
    // Арифметика
    else if (line.rfind("+", 0) == 0)
    {
        typevariable a = mem_.pop();
        typevariable b = mem_.pop();

        mem_.push(a + b);
    }
    else if (line.rfind("-", 0) == 0)
    {
        typevariable a = mem_.pop();
        typevariable b = mem_.pop();

        mem_.push(a - b);
    }
    else if (line.rfind("%", 0) == 0)
    {
        typevariable a = mem_.pop();
        typevariable b = mem_.pop();

        mem_.push(a % b);
    }
    else if (line.rfind("/", 0) == 0)
    {
        typevariable a = mem_.pop();
        typevariable b = mem_.pop();

        mem_.push(a / b);
    }
    else if (line.rfind("*", 0) == 0)
    {
        typevariable a = mem_.pop();
        typevariable b = mem_.pop();

        mem_.push(a * b);
    }
    else if (line.rfind("inc", 0) == 0)

    {
        typevariable a = mem_.pop();
        typevariable b = 1;
        mem_.push(a + b);
    }
    else if (line.rfind("dec", 0) == 0)
    {
        typevariable a = mem_.pop();
        typevariable b = 1;
        mem_.push(a - b);
    }
    else if (line.rfind("sqrt", 0) == 0)
    {
        typevariable a = mem_.pop();
        mem_.push(pow(a.i, 2));
    }

    // Сервисные функции
    else if (line.rfind("dup", 0) == 0) {
        typevariable a = mem_.top();
        mem_.push(a);
    }
    else if (line.rfind("swap", 0) == 0) {
        typevariable a = mem_.top();
        mem_.push(a);
    }
    else if (line.rfind("pop", 0) == 0)
    {
        mem_.pop();
    }
    else if (line.rfind("printpop",0) == 0)
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
        typevariable a;
        cin >> a.s;
        mem_.push(a);
    }
    else if (line == "stop")
    {
        //pos_ = source_code_.size() + 1;
        return false; 
    }
    else if (line.rfind(":", 0) == 0)
    {
        string markname = "";
        int g = 1;
        while (true)
        {
            if (g < source_code_[pos_].size())
            {
                markname += source_code_[pos_][g];
            }
            else break;
            g++;
        }
        placemarks[markname] = pos_;
    }
    else if (line.rfind("jmp", 0) == 0)     
    {
        string markname = "";
        int g = 4;
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
    else if (line.rfind("jne", 0) == 0) 
    {
        
    }
    else if (line.rfind("jle", 0) == 0) 
    {
        
    }
    else if (line.rfind("jge", 0) == 0) 
    {
      
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

    // Остальное
    else if (line.rfind("   ") || line.rfind(" ") || line.empty()) {}
    else if (line.rfind("pass", 0) == 0) {}
    else 
    {
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
