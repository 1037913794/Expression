/*2.2.	���ʽ���Ľ����ͱ���
������������
���ʽ���Ƕ�������һ��Ӧ�ã�Ҷ���ǲ�������������Ϊ�����������磬��ͼ��ʾ�ı��ʽ��������������õ�������ʽ  (a+b*c)+(��d*e+f��*g��
 ����ʵ�ֱ��ʽ���Ľ����ͱ���
������Ҫ��
1)	���ʽ֧�ֵ�����������趨�����磬�������㡣
2)	����ĳ�ַ�ʽ������ʽ�������׺���ʽ��ʽ�����û�����ı��ʽ����������ͼ��ʾ�ı��ʽ����
3)	�����ñ��ʽ�����ֱ�����ñ��ʽ����׺���ʽ�ͺ�׺���ʽ��ʽ��
*/
//�涨���ֵ����ȼ�Ϊ0������������ȼ�Ϊ1���˳���������ȼ�Ϊ2���Ӽ���������ȼ�Ϊ3�������������ȼ���Ϊ0
//���ܣ��Ӽ��˳����˷������ţ�С��������
#include <iostream>
#include <stack>
#include <queue>
#include <string>

using namespace std;

class ExpressionTree {
public:
    ExpressionTree() {      //���캯��
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

    bool InsertNumber(double v) {       //����ʽ���в�������
        if (CurrentType == 0 || CurrentType == 3)       //����ʽ�������׳��쳣
            throw 3;
        else {
            p = new BTNode;
            p->priority = 0;        //�趨���ֽڵ�����ȼ�Ϊ0
            p->value = v;
            p->lchild = nullptr;
            p->rchild = nullptr;
            pt = root;
            while (pt->rchild != nullptr) {     //�嵽���Ҷ˽ڵ㣬������������������������������
                pt = pt->rchild;
            }
            pt->rchild = p;
            CurrentType = 0;
            return true;
        }
    }

    bool InsertOperator(int t) {                //���������
        if (CurrentType == 1 || CurrentType == 2)       //�������֮ǰΪ����������������׳��쳣
            throw 4;
        else {
            p = new BTNode;
            if (t == 0)                 //������������ȼ�
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
            while (pt->rchild->priority > p->priority) {        //�ҵ��ʵ��ĵط����������
                pt = pt->rchild;
            }
            p->lchild = pt->rchild;
            pt->rchild = p;
            CurrentType = 1;
            return true;
        }
    }

    bool InsertLeftBrace() {        //����������
        if (CurrentType == 0 || CurrentType == 3)       //ͬ��
            throw 5;
        else {
            s.push(root);       //����ǰ���ڵ��ջ
            while (root->rchild != nullptr) {
                root = root->rchild;
            }
            CurrentType = 2;
            return true;
        }
    }

    bool InsertRightBrace() {       //����������
        if (s.empty())
            throw 1;
        else if (CurrentType == 1 || CurrentType == 2)
            throw 6;
        else {
            if (root->rchild->priority != 0)
                root->rchild->priority = -1;
            root = s.top();     //��ջ���ڵ�
            s.pop();
            CurrentType = 3;
            return true;
        }
    }

    double Calculate() {                //������ʽ��ֵ
//        PreOrder(root->rchild);
//        InOrder(root->rchild);
        PostOrder(root->rchild);        //��������ɺ�׺���ʽ
        double a, b, c = 1;
        while (!postfix.empty()) {
            p = postfix.front();
            postfix.pop();
            if (p->priority == 0)
                Opnd.push(p->value);        //����ѹջ
            else {
                b = Opnd.top();     //��ջ��������
                Opnd.pop();
                a = Opnd.top();
                Opnd.pop();
                switch (p->type) {
                    case 0:
                        c = 1;
                        for (int i = 0; i < b; i++)         //�˷�����
                            c = c * a;
                        break;
                    case 1:
                        c = a * b;          //�˷�
                        break;
                    case 2:
                        if (b == 0)     //����������0���׳��쳣
                            throw 7;
                        else
                            c = a / b;      //����
                        break;
                    case 3:
                        c = a + b;      //�ӷ�
                        break;
                    case 4:
                        c = a - b;      //����
                        break;
                    default:
                        break;
                }
                Opnd.push(c);       //�����ѹջ
            }
        }
        return Opnd.top();      //�������ս��
    }

    void PrintPre() {               //���ǰ׺���ʽ
        PreOrder(root->rchild);
        cout << "ǰ׺���ʽΪ��";
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

    void PrintIn() {            //�����׺���ʽ
        InOrder(root->rchild);      //�������
        cout << "��׺���ʽΪ��";
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

    void PrintPost() {              //�����׺���ʽ
        PostOrder(root->rchild);        //�������
        cout << "��׺���ʽΪ��";
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

    ~ExpressionTree() {         //��������
        DestroyBTree(root);
    }

private:
    typedef struct node {
        int priority;       //�ڵ����ȼ������ʽΪ0���˷�Ϊ1���˳�Ϊ2���Ӽ�Ϊ3
        double value;       //���ʽ��ֵ
        int type;           //��������ͣ�0Ϊ��^����1Ϊ��*����2Ϊ��/����3Ϊ��+����4Ϊ��-��
        struct node *lchild;
        struct node *rchild;
    } BTNode;               //������ʽ���ڵ�����

    void DestroyBTree(BTNode *&b) {     //���������ͷ��ڴ�
        if (b != nullptr) {
            DestroyBTree(b->lchild);
            DestroyBTree(b->rchild);
            delete b;
        }
    }

    void PreOrder(BTNode *b) {              //�������
        if (b != nullptr) {
            prefix.push(b);
            PreOrder(b->lchild);
            PreOrder(b->rchild);
        }
    }

    void InOrder(BTNode *b) {           //�������
        if (b != nullptr) {
            InOrder(b->lchild);
            nifix.push(b);
            InOrder(b->rchild);
        }
    }

    void PostOrder(BTNode *b) {             //�������
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
    int CurrentType;        //��ǰ���ͣ�0Ϊ���ʽ��1Ϊ�������2Ϊ�����ţ�3Ϊ������
};

int main() {
    ExpressionTree exp;
    string str;
    int integer = 0, fraction = 0, offset = 0;          //�������֣�С�����֣�ƫ����
    double num = 0.0;
    bool IsInteger = true;          //����ɨ����������
    bool IsNegative = false;        //�Ƿ��Ǹ���
    enum Mode {
        Number, Operator, LeftBrace, RightBrace     //����ɨ��ģʽ
    } mode = LeftBrace;
    int BraceCount = 0;         //���Ų���
    cout << "����������ʽ������֧�֣��Ӽ��˳����˷������ţ�С������������" << endl;
    cin >> str;
    try {
        for (string::const_iterator it = str.begin(); it < str.end(); it++) {       //����������
            switch (*it) {
                case '+':
                    if (mode == Operator || mode == LeftBrace)
                        throw 4;
                    else if (mode == Number) {          //�Ƚ����ִ�����ʽ��
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
                    exp.InsertOperator(3);      //�ٽ������������ʽ��
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
                    else if (mode == LeftBrace) {       //˵���Ǹ���
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
                    exp.InsertLeftBrace();      //�������Ų�����ʽ��
                    break;
                case ')':
                    if (mode == Operator || mode == LeftBrace)
                        throw 6;
                    else if (mode == Number) {      //�����ֲ�����ʽ��
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
                    exp.InsertRightBrace();         //�������Ų�����ʽ��
                    break;
                case '.':
                    mode = Number;
                    IsInteger = false;      //����С��ģʽ
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
        if (BraceCount > 0)     //������������Ŀ��ƥ�����׳��쳣
            throw 2;
        if (mode == Operator)       //������������β
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
    catch (int error) {     //�����쳣
        switch (error) {
            case 0:
                cout << "���ַǷ��ַ���" << endl;
                return 0;
                break;
            case 1:
                cout << "�����Ŷ��������ţ�" << endl;
                return 0;
                break;
            case 2:
                cout << "����������Ŀ��ƥ�䣡" << endl;
                return 0;
                break;
            case 3:
                cout << "����֮ǰӦΪ������������ţ�" << endl;
                return 0;
                break;
            case 4:
                cout << "�����֮ǰӦΪ���ֻ������ţ�" << endl;
                return 0;
                break;
            case 5:
                cout << "������֮ǰӦΪ������������ţ�" << endl;
                return 0;
                break;
            case 6:
                cout << "������֮ǰӦΪ���ֻ������ţ�" << endl;
                return 0;
                break;
            case 7:
                cout << "��������Ϊ0��" << endl;
                return 0;
                break;
            case 8:
                cout << "���ʽ�������������β��" << endl;
                return 0;
                break;
            default:
                break;
        }
    }
    cout << "�ñ��ʽ��ֵΪ��" << exp.Calculate() << endl;
    exp.PrintPre();
    exp.PrintIn();
    exp.PrintPost();
    return 0;
}
