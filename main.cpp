/*2.2.	表达式树的建立和遍历
【问题描述】
表达式树是二叉树的一种应用，叶子是操作数，其余结点为操作符。例如，下图表示的表达式树，用中序遍历得到中序表达式  (a+b*c)+(（d*e+f）*g）
 请编程实现表达式树的建立和遍历
【基本要求】
1)	表达式支持的运算符自行设定，例如，四则运算。
2)	采用某种方式输入表达式，例如后缀表达式形式。将用户输入的表达式创建成如上图所示的表达式树。
3)	遍历该表达式树，分别输出该表达式的中缀表达式和后缀表达式形式。
*/
//规定数字的优先级为0，幂运算的优先级为1，乘除运算的优先级为2，加减运算的优先级为3，如遇括号优先级降为0
//功能：加减乘除，乘方，括号，小数，负数
#include <iostream>
#include <stack>
#include <queue>
#include <string>

using namespace std;

class ExpressionTree {
public:
    ExpressionTree() {      //构造函数
        root = new BTNode;
        root->priority = 3;
        root->type = 3;
        root->lchild = new BTNode;
        root->lchild->priority = 0;
        root->lchild->value = 0;
        root->lchild->lchild = nullptr;
        root->lchild->rchild = nullptr;
        root->rchild = nullptr;
        pt = nullptr;
        p = nullptr;
        CurrentType = 1;
    }

    bool InsertNumber(double v) {       //向表达式树中插入数字
        if (CurrentType == 0 || CurrentType == 3)       //若格式不符则抛出异常
            throw 3;
        else {
            p = new BTNode;
            p->priority = 0;        //设定数字节点的优先级为0
            p->value = v;
            p->lchild = nullptr;
            p->rchild = nullptr;
            pt = root;
            while (pt->rchild != nullptr) {     //插到最右端节点，右子树的右子树的右子树。。。。
                pt = pt->rchild;
            }
            pt->rchild = p;
            CurrentType = 0;
            return true;
        }
    }

    bool InsertOperator(int t) {                //插入运算符
        if (CurrentType == 1 || CurrentType == 2)       //若运算符之前为运算符或左括号则抛出异常
            throw 4;
        else {
            p = new BTNode;
            if (t == 0)                 //设置运算符优先级
                p->priority = 1;
            else if (t == 1 || t == 2)
                p->priority = 2;
            else if (t == 3 || t == 4)
                p->priority = 3;
            else
                return false;
            p->type = t;
            p->lchild = nullptr;
            p->rchild = nullptr;
            pt = root;
            while (pt->rchild->priority > p->priority) {        //找到适当的地方插入运算符
                pt = pt->rchild;
            }
            p->lchild = pt->rchild;
            pt->rchild = p;
            CurrentType = 1;
            return true;
        }
    }

    bool InsertLeftBrace() {        //插入左括号
        if (CurrentType == 0 || CurrentType == 3)       //同上
            throw 5;
        else {
            s.push(root);       //将当前根节点进栈
            while (root->rchild != nullptr) {
                root = root->rchild;
            }
            CurrentType = 2;
            return true;
        }
    }

    bool InsertRightBrace() {       //插入右括号
        if (s.empty())
            throw 1;
        else if (CurrentType == 1 || CurrentType == 2)
            throw 6;
        else {
            if (root->rchild->priority != 0)
                root->rchild->priority = -1;
            root = s.top();     //出栈根节点
            s.pop();
            CurrentType = 3;
            return true;
        }
    }

    double Calculate() {                //计算表达式的值
//        PreOrder(root->rchild);
//        InOrder(root->rchild);
        PostOrder(root->rchild);        //后序遍历成后缀表达式
        double a, b, c = 1;
        while (!postfix.empty()) {
            p = postfix.front();
            postfix.pop();
            if (p->priority == 0)
                Opnd.push(p->value);        //数字压栈
            else {
                b = Opnd.top();     //出栈两个数字
                Opnd.pop();
                a = Opnd.top();
                Opnd.pop();
                switch (p->type) {
                    case 0:
                        c = 1;
                        for (int i = 0; i < b; i++)         //乘方运算
                            c = c * a;
                        break;
                    case 1:
                        c = a * b;          //乘法
                        break;
                    case 2:
                        if (b == 0)     //若除数等于0则抛出异常
                            throw 7;
                        else
                            c = a / b;      //除法
                        break;
                    case 3:
                        c = a + b;      //加法
                        break;
                    case 4:
                        c = a - b;      //减法
                        break;
                    default:
                        break;
                }
                Opnd.push(c);       //将结果压栈
            }
        }
        return Opnd.top();      //返回最终结果
    }

    void PrintPre() {               //输出前缀表达式
        PreOrder(root->rchild);
        cout << "前缀表达式为：";
        while (!prefix.empty()) {
            p = prefix.front();
            prefix.pop();
            if (p->priority == 0)
                cout << p->value << " ";
            else
                switch (p->type) {
                    case 0:
                        cout << "^ ";
                        break;
                    case 1:
                        cout << "* ";
                        break;
                    case 2:
                        cout << "/ ";
                        break;
                    case 3:
                        cout << "+ ";
                        break;
                    case 4:
                        cout << "- ";
                        break;
                    default:
                        break;
                }
        }
        cout << endl;
    }

    void PrintIn() {            //输出中缀表达式
        InOrder(root->rchild);      //中序遍历
        cout << "中缀表达式为：";
        while (!nifix.empty()) {
            p = nifix.front();
            nifix.pop();
            if (p->priority == 0)
                cout << p->value << " ";
            else
                switch (p->type) {
                    case 0:
                        cout << "^ ";
                        break;
                    case 1:
                        cout << "* ";
                        break;
                    case 2:
                        cout << "/ ";
                        break;
                    case 3:
                        cout << "+ ";
                        break;
                    case 4:
                        cout << "- ";
                        break;
                    default:
                        break;
                }
        }
        cout << endl;
    }

    void PrintPost() {              //输出后缀表达式
        PostOrder(root->rchild);        //后序遍历
        cout << "后缀表达式为：";
        while (!postfix.empty()) {
            p = postfix.front();
            postfix.pop();
            if (p->priority == 0)
                cout << p->value << " ";
            else
                switch (p->type) {
                    case 0:
                        cout << "^ ";
                        break;
                    case 1:
                        cout << "* ";
                        break;
                    case 2:
                        cout << "/ ";
                        break;
                    case 3:
                        cout << "+ ";
                        break;
                    case 4:
                        cout << "- ";
                        break;
                    default:
                        break;
                }
        }
        cout << endl;
    }

    ~ExpressionTree() {         //析构函数
        DestroyBTree(root);
    }

private:
    typedef struct node {
        int priority;       //节点优先级，表达式为0，乘方为1，乘除为2，加减为3
        double value;       //表达式的值
        int type;           //运算符类型，0为‘^’，1为‘*’，2为‘/’，3为‘+’，4为‘-’
        struct node *lchild;
        struct node *rchild;
    } BTNode;               //定义表达式树节点类型

    void DestroyBTree(BTNode *&b) {     //销毁树，释放内存
        if (b != nullptr) {
            DestroyBTree(b->lchild);
            DestroyBTree(b->rchild);
            delete b;
        }
    }

    void PreOrder(BTNode *b) {              //先序遍历
        if (b != nullptr) {
            prefix.push(b);
            PreOrder(b->lchild);
            PreOrder(b->rchild);
        }
    }

    void InOrder(BTNode *b) {           //中序遍历
        if (b != nullptr) {
            InOrder(b->lchild);
            nifix.push(b);
            InOrder(b->rchild);
        }
    }

    void PostOrder(BTNode *b) {             //后序遍历
        if (b != nullptr) {
            PostOrder(b->lchild);
            PostOrder(b->rchild);
            postfix.push(b);
        }
    }

    BTNode *root, *pt, *p;
    stack<BTNode *> s;
    queue<BTNode *> prefix;
    queue<BTNode *> nifix;
    queue<BTNode *> postfix;
    stack<double> Opnd;
    int CurrentType;        //当前类型，0为表达式，1为运算符，2为左括号，3为右括号
};

int main() {
    ExpressionTree exp;
    string str;
    int integer = 0, fraction = 0, offset = 0;          //整数部分，小数部分，偏移量
    double num = 0.0;
    bool IsInteger = true;          //正在扫描整数部分
    bool IsNegative = false;        //是否是负数
    enum Mode {
        Number, Operator, LeftBrace, RightBrace     //定义扫描模式
    } mode = LeftBrace;
    int BraceCount = 0;         //括号层数
    cout << "请输入运算式（功能支持：加减乘除，乘方，括号，小数，负数）：" << endl;
    cin >> str;
    try {
        for (string::const_iterator it = str.begin(); it < str.end(); it++) {       //迭代器遍历
            switch (*it) {
                case '+':
                    if (mode == Operator || mode == LeftBrace)
                        throw 4;
                    else if (mode == Number) {          //先将数字存入表达式树
                        num = (double) fraction;
                        for (int i = 0; i < offset; i++)
                            num /= 10;
                        num += (double) integer;
                        if (IsNegative)
                            num = -num;
                        exp.InsertNumber(num);
                        integer = 0;
                        fraction = 0;
                        offset = 0;
                        num = 0.0;
                        IsInteger = true;
                        IsNegative = false;
                    }
                    mode = Operator;
                    exp.InsertOperator(3);      //再将运算符存入表达式树
                    break;
                case '-':
                    if (mode == Number) {
                        num = (double) fraction;
                        for (int i = 0; i < offset; i++)
                            num /= 10;
                        num += (double) integer;
                        if (IsNegative)
                            num = -num;
                        exp.InsertNumber(num);
                        integer = 0;
                        fraction = 0;
                        offset = 0;
                        num = 0.0;
                        IsInteger = true;
                        IsNegative = false;
                        mode = Operator;
                        exp.InsertOperator(4);
                    } else if (mode == Operator)
                        throw 4;
                    else if (mode == LeftBrace) {       //说明是负数
                        IsNegative = true;
                        mode = Operator;
                    } else {
                        mode = Operator;
                        exp.InsertOperator(4);
                    }
                    break;
                case '*':
                    if (mode == Operator || mode == LeftBrace)
                        throw 4;
                    else if (mode == Number) {
                        num = (double) fraction;
                        for (int i = 0; i < offset; i++)
                            num /= 10;
                        num += (double) integer;
                        if (IsNegative)
                            num = -num;
                        exp.InsertNumber(num);
                        integer = 0;
                        fraction = 0;
                        offset = 0;
                        num = 0.0;
                        IsInteger = true;
                        IsNegative = false;
                    }
                    mode = Operator;
                    exp.InsertOperator(1);
                    break;
                case '/':
                    if (mode == Operator || mode == LeftBrace)
                        throw 4;
                    else if (mode == Number) {
                        num = (double) fraction;
                        for (int i = 0; i < offset; i++)
                            num /= 10;
                        num += (double) integer;
                        if (IsNegative)
                            num = -num;
                        exp.InsertNumber(num);
                        integer = 0;
                        fraction = 0;
                        offset = 0;
                        num = 0.0;
                        IsInteger = true;
                        IsNegative = false;
                    }
                    mode = Operator;
                    exp.InsertOperator(2);
                    break;
                case '^':
                    if (mode == Operator || mode == LeftBrace)
                        throw 4;
                    else if (mode == Number) {
                        num = (double) fraction;
                        for (int i = 0; i < offset; i++)
                            num /= 10;
                        num += (double) integer;
                        if (IsNegative)
                            num = -num;
                        exp.InsertNumber(num);
                        integer = 0;
                        fraction = 0;
                        offset = 0;
                        num = 0.0;
                        IsInteger = true;
                        IsNegative = false;
                    }
                    mode = Operator;
                    exp.InsertOperator(0);
                    break;
                case '(':
                    if (mode == Number || mode == RightBrace)
                        throw 5;
                    mode = LeftBrace;
                    BraceCount++;
                    exp.InsertLeftBrace();      //将左括号插入表达式树
                    break;
                case ')':
                    if (mode == Operator || mode == LeftBrace)
                        throw 6;
                    else if (mode == Number) {      //将数字插入表达式树
                        num = (double) fraction;
                        for (int i = 0; i < offset; i++)
                            num /= 10;
                        num += (double) integer;
                        if (IsNegative)
                            num = -num;
                        exp.InsertNumber(num);
                        integer = 0;
                        fraction = 0;
                        offset = 0;
                        num = 0.0;
                        IsInteger = true;
                        IsNegative = false;
                    }
                    mode = RightBrace;
                    BraceCount--;
                    exp.InsertRightBrace();         //将右括号插入表达式树
                    break;
                case '.':
                    mode = Number;
                    IsInteger = false;      //进入小数模式
                    offset = 0;
                    break;
                case '0':
                    mode = Number;
                    if (IsInteger) {
                        integer *= 10;
                    } else {
                        fraction *= 10;
                        offset++;
                    }
                    break;
                case '1':
                    mode = Number;
                    if (IsInteger) {
                        integer *= 10;
                        integer += 1;
                    } else {
                        fraction *= 10;
                        fraction += 1;
                        offset++;
                    }
                    break;
                case '2':
                    mode = Number;
                    if (IsInteger) {
                        integer *= 10;
                        integer += 2;
                    } else {
                        fraction *= 10;
                        fraction += 2;
                        offset++;
                    }
                    break;
                case '3':
                    mode = Number;
                    if (IsInteger) {
                        integer *= 10;
                        integer += 3;
                    } else {
                        fraction *= 10;
                        fraction += 3;
                        offset++;
                    }
                    break;
                case '4':
                    mode = Number;
                    if (IsInteger) {
                        integer *= 10;
                        integer += 4;
                    } else {
                        fraction *= 10;
                        fraction += 4;
                        offset++;
                    }
                    break;
                case '5':
                    mode = Number;
                    if (IsInteger) {
                        integer *= 10;
                        integer += 5;
                    } else {
                        fraction *= 10;
                        fraction += 5;
                        offset++;
                    }
                    break;
                case '6':
                    mode = Number;
                    if (IsInteger) {
                        integer *= 10;
                        integer += 6;
                    } else {
                        fraction *= 10;
                        fraction += 6;
                        offset++;
                    }
                    break;
                case '7':
                    mode = Number;
                    if (IsInteger) {
                        integer *= 10;
                        integer += 7;
                    } else {
                        fraction *= 10;
                        fraction += 7;
                        offset++;
                    }
                    break;
                case '8':
                    mode = Number;
                    if (IsInteger) {
                        integer *= 10;
                        integer += 8;
                    } else {
                        fraction *= 10;
                        fraction += 8;
                        offset++;
                    }
                    break;
                case '9':
                    mode = Number;
                    if (IsInteger) {
                        integer *= 10;
                        integer += 9;
                    } else {
                        fraction *= 10;
                        fraction += 9;
                        offset++;
                    }
                    break;
                default:
                    throw 0;
            }
        }
        if (BraceCount > 0)     //若左右括号数目不匹配则抛出异常
            throw 2;
        if (mode == Operator)       //如果以运算符结尾
            throw 8;
        if (mode == Number) {
            num = (double) fraction;
            for (int i = 0; i < offset; i++)
                num /= 10;
            num += (double) integer;
            if (IsNegative)
                num = -num;
            exp.InsertNumber(num);
            integer = 0;
            fraction = 0;
            offset = 0;
            num = 0.0;
            IsInteger = true;
            IsNegative = false;
        }
    }
    catch (int error) {     //处理异常
        switch (error) {
            case 0:
                cout << "出现非法字符！" << endl;
                return 0;
                break;
            case 1:
                cout << "右括号多于左括号！" << endl;
                return 0;
                break;
            case 2:
                cout << "左右括号数目不匹配！" << endl;
                return 0;
                break;
            case 3:
                cout << "数字之前应为运算符或左括号！" << endl;
                return 0;
                break;
            case 4:
                cout << "运算符之前应为数字或右括号！" << endl;
                return 0;
                break;
            case 5:
                cout << "左括号之前应为运算符或左括号！" << endl;
                return 0;
                break;
            case 6:
                cout << "右括号之前应为数字或右括号！" << endl;
                return 0;
                break;
            case 7:
                cout << "除数不得为0！" << endl;
                return 0;
                break;
            case 8:
                cout << "表达式不得以运算符结尾！" << endl;
                return 0;
                break;
            default:
                break;
        }
    }
    cout << "该表达式的值为：" << exp.Calculate() << endl;
    exp.PrintPre();
    exp.PrintIn();
    exp.PrintPost();
    return 0;
}
