#include <math.h>
#include <ios>
#include "TCALC.h"

/*****************************************************************************/

bool TCALC::IsDelim(void)
{
    return (strchr("+-*/%^()[]", expr[pos]) != nullptr);
}

/*---------------------------------------------------------------------------*/

bool TCALC::IsLetter(void)
{
    return ((expr[pos] >= 'a' && expr[pos] <= 'z') ||
            (expr[pos] >= 'A' && expr[pos] <= 'Z'));
}

/*---------------------------------------------------------------------------*/

bool TCALC::IsDigit()
{
    return (expr[pos] >= '0' && expr[pos] <= '9');
}

/*---------------------------------------------------------------------------*/

bool TCALC::IsPoint()
{
    return (expr[pos] == '.');
}

/*****************************************************************************/

TCALC::TCALC()
{
    result = 0.0;
    root = nullptr;
}

/*---------------------------------------------------------------------------*/

TCALC::~TCALC()
{
    DelTree(root);
    root = nullptr;
}

/*---------------------------------------------------------------------------*/

TCALCNode *TCALC::CreateNode(
    double _value,
    TCALCNode *_left,
    TCALCNode *_right
)
{
    TCALCNode *pNode = new TCALCNode(_value, _left, _right);
    return pNode;
}

/*---------------------------------------------------------------------------*/

void TCALC::SendError(int errNum)
{
    if (*curToken == '\0')
    {
        strcpy(curToken, "Пустое выражение");
    }
    else if (errNum == 2)
    {
        throw "Внезапный конец выражения";
    }
    else if (errNum == 3)
    {
        throw "Конец выражения ожидается";
    }
    else if (errNum == 4)
    {
        throw "Пропущеннаи открывающая скобка";
    }
    else if (errNum == 5)
    {
        throw "Пропущенна закрывающая скобка";
    }
    else
    {
        return;
    }
}

/*---------------------------------------------------------------------------*/

bool TCALC::GetToken(void)
{
    *curToken = '\0';
    while (expr[pos] == ' ')
    {
        pos++;
    }
    if (expr[pos] == '\0')
    {
        curToken[0] = '\0';
        typToken = CALC_END;
        return true;
    }
    else if (IsDelim())
    {
        curToken[0] = expr[pos++];
        curToken[1] = '\0';
        switch(*curToken)
        {
            case '+':
                typToken = OP_PLUS;
                return true;
            case '-':
                typToken = OP_MINUS;
                return true;
            case '*':
                typToken = OP_MULTIPLY;
                return true;
            case '/':
                typToken = OP_DIVIDE;
                return true;
            case '%':
                typToken = OP_PERCENT;
                return true;
            case '[':
            case '(':
                typToken = CALC_L_BRACKET;
                return true;
            case ']':
            case ')':
                typToken = CALC_R_BRACKET;
                return true;
        }
    }
    else if (IsLetter())
    {
        int i = 0;
        while (IsLetter())
        {
            curToken[i++] = expr[pos++];
        }
        curToken[i] = '\0';
        int len = static_cast<int>(strlen(curToken));
        for (i = 0; i < len; i++)
        {
            if (curToken[i] >= 'A' && curToken[i] <= 'Z')
            {
                curToken[i] += 'a' - 'A';
            }
        }
        if (!strcmp(curToken, "leet"))
        {
            typToken = CALC_LEET;
            return true;
        }
        else if (!strcmp(curToken, "g"))
        {
            typToken = CALC_G;
            return true;
        }
        else if (!strcmp(curToken, "pi"))
        {
            typToken = CALC_PI;
            return true;
        }
        else if (!strcmp(curToken, "sin"))
        {
            typToken = OP_SIN;
            return true;
        }
        else if (!strcmp(curToken, "cos"))
        {
            typToken = OP_COS;
            return true;
        }
        else if (!strcmp(curToken, "tg"))
        {
            typToken = OP_TG;
            return true;
        }
        else if (!strcmp(curToken, "ctg"))
        {
            typToken = OP_CTG;
            return true;
        }
        else if (!strcmp(curToken, "arcsin"))
        {
            typToken = OP_ARCSIN;
            return true;
        }
        else if (!strcmp(curToken, "arccos"))
        {
            typToken = OP_ARCCOS;
            return true;
        }
        else if (!strcmp(curToken, "arcctg"))
        {
            typToken = OP_ARCCTG;
            return true;
        }
        else if (!strcmp(curToken, "sh"))
        {
            typToken = OP_SH;
            return true;
        }
        else if (!strcmp(curToken, "ch"))
        {
            typToken = OP_CH;
            return true;
        }
        else if (!strcmp(curToken, "th"))
        {
            typToken = OP_TH;
            return true;
        }
        else if (!strcmp(curToken, "cth"))
        {
            typToken = OP_CTH;
            return true;
        }
        else if (!strcmp(curToken, "exp"))
        {
            typToken = OP_EXP;
            return true;
        }
        else if (!strcmp(curToken, "lg"))
        {
            typToken = OP_LG;
            return true;
        }
        else if (!strcmp(curToken, "ln"))
        {
            typToken = OP_LN;
            return true;
        }
        else if (!strcmp(curToken, "sqrt"))
        {
            typToken = OP_SQRT;
            return true;
        }
        else
        {
            SendError(0);
        }
    }
    else if (IsDigit() || IsPoint())
    {
        int i = 0;
        while (IsDigit())
        {
            curToken[i++] = expr[pos++];
        }
        if (IsPoint())
        {
            curToken[i++] = expr[pos++];
            while (IsDigit())
            {
                curToken[i++] = expr[pos++];
            }
        }
        curToken[i] = '\0';
        typToken = CALC_NUMBER;
        return true;
    }
    else
    {
        curToken[0] = expr[pos++];
        curToken[1] = '\0';
        SendError(1);
    }
    return false;
}

/*---------------------------------------------------------------------------*/

bool TCALC::Compile(char *_expr)
{
    pos = 0;
    expr = _expr;
    *curToken = '\0';
    if (root != nullptr)
    {
        DelTree(root);
        root = nullptr;
    }
    GetToken();
    if (typToken == CALC_END)
    {
        SendError(2);
    }
    root = Expr();
    if (typToken != CALC_END)
    {
        SendError(3);
    }
    return true;
}

/*---------------------------------------------------------------------------*/

TCALCNode *TCALC::Expr(void)
{
    TCALCNode *temp = Expr1();
    while (true)
    {
        if (typToken == OP_PLUS)
        {
            GetToken();
            temp = CreateNode(OP_PLUS, temp, Expr1());
        }
        else if (typToken == OP_MINUS)
        {
            GetToken();
            temp = CreateNode(OP_MINUS, temp, Expr1());
        }
        else { break; }
    }
    return temp;
}

/*---------------------------------------------------------------------------*/

TCALCNode *TCALC::Expr1(void)
{
    TCALCNode *temp = Expr2();
    while (true)
    {
        if (typToken == OP_MULTIPLY)
        {
            GetToken();
            temp = CreateNode(OP_MULTIPLY, temp, Expr2());
        }
        else if (typToken == OP_DIVIDE)
        {
            GetToken();
            temp = CreateNode(OP_DIVIDE, temp, Expr2());
        }
        else if (typToken == OP_PERCENT)
        {
            GetToken();
            temp = CreateNode(OP_PERCENT, temp, Expr2());
        }
        else
        {
            break;
        }
    }
    return temp;
}

/*---------------------------------------------------------------------------*/

TCALCNode *TCALC::Expr2(void)
{
    TCALCNode *temp;
    if (typToken == OP_PLUS)
    {
        GetToken();
        temp = Expr3();
    }
    else if (typToken == OP_MINUS)
    {
        GetToken();
        temp = CreateNode(OP_UMINUS, Expr3());
    }
    else
    {
        temp = Expr3();
    }
    return temp;
}

/*---------------------------------------------------------------------------*/

TCALCNode *TCALC::Expr3(void)
{
    TCALCNode *temp;
    if (typToken >= OP_SIN && typToken <= OP_SQRT + 1)
    {
        temp = CreateNode(OP_SIN - OP_SIN + typToken);
        GetToken();
        if (typToken != CALC_L_BRACKET)
        {
            SendError(4);
        }
        GetToken();
        temp->left = Expr();
        if (typToken != CALC_R_BRACKET)
        {
            SendError(5);
        }
        GetToken();
    }
    else
    {
        temp = Expr4();
    }
    return temp;
}

/*---------------------------------------------------------------------------*/

TCALCNode *TCALC::Expr4(void)
{
    TCALCNode *temp = nullptr;
    if (typToken == CALC_NUMBER)
    {
        temp = CreateNode(static_cast<double>(atof(curToken)));
        GetToken();
    }
    else if (typToken == CALC_PI)
    {
        temp = CreateNode(static_cast<double>(M_PI));
        GetToken();
    }
    else if (typToken == CALC_G)
    {
        temp = CreateNode(static_cast<double>(F_G));
        GetToken();
    }
    else if (typToken == CALC_L_BRACKET)
    {
        GetToken();
        temp = Expr();
        if (typToken != CALC_R_BRACKET)
        {
            SendError(5);
        }
        GetToken();
    }
    else if (typToken == CALC_LEET)
    {
        temp = CreateNode(static_cast<double>(H_LEET));
        GetToken();
    }
    else
    {
        SendError(6);
    }
    return temp;
}

/*---------------------------------------------------------------------------*/

double TCALC::Evaluate(void)
{
    result = CalcTree(root);
    return result;
}

/*---------------------------------------------------------------------------*/

double TCALC::GetResult()
{
    return result;
}

/*---------------------------------------------------------------------------*/

double TCALC::CalcTree(TCALCNode *tree)
{
    static double temp;
    if (tree->left == nullptr && tree->right == nullptr)
    {
        return tree->value;
    }
    else
    {
        switch( static_cast<int>(tree->value))
        {
            case OP_PLUS:
                return CalcTree(tree->left) + CalcTree(tree->right);
            case OP_MINUS:
                return CalcTree(tree->left) - CalcTree(tree->right);
            case OP_MULTIPLY:
                return CalcTree(tree->left) * CalcTree(tree->right);
            case OP_DIVIDE:
                return CalcTree(tree->left) / CalcTree(tree->right);
            case OP_PERCENT:
                return static_cast<int>(CalcTree(tree->left))
                       % static_cast<int>(CalcTree(tree->right));
            case OP_POWER:
                return static_cast<double>(
                           pow(CalcTree(tree->left), CalcTree(tree->right))
                       );
            case OP_UMINUS:
                return -CalcTree(tree->left);
            case OP_SIN:
                return sin(CalcTree(tree->left));
            case OP_COS:
                return cos(CalcTree(tree->left));
            case OP_TG:
                return tan(CalcTree(tree->left));
            case OP_CTG:
                return 1.0 / tan(CalcTree(tree->left));
            case OP_ARCSIN:
                return asin(CalcTree(tree->left));
            case OP_ARCCOS:
                return acos(CalcTree(tree->left));
            case OP_ARCTG:
                return atan(CalcTree(tree->left));
            case OP_ARCCTG:
                return M_PI / 2.0 - atan(CalcTree(tree->left));
            case OP_SH:
                temp = CalcTree(tree->left);
                return (exp(temp) - exp(-temp)) / 2.0;
            case OP_CH:
                temp = CalcTree(tree->left);
                return (exp(temp) + exp(-temp)) / 2.0;
            case OP_TH:
                temp = CalcTree(tree->left);
                return (exp(temp) - exp(-temp)) / (exp(temp) + exp(-temp));
            case OP_CTH:
                temp = CalcTree(tree->left);
                return (exp(temp) + exp(-temp)) / (exp(temp) - exp(-temp));
            case OP_EXP:
                return exp(CalcTree(tree->left));
            case OP_LG:
                return log10(CalcTree(tree->left));
            case OP_LN:
                return log(CalcTree(tree->left));
            case OP_SQRT:
                return sqrt(CalcTree(tree->left));
            case OP_IN:
                return 1;
        }
    }
    return 0;
}

/*---------------------------------------------------------------------------*/

void TCALC::DelTree(TCALCNode *tree)
{
    if (tree == nullptr)
    {
        return;
    }
    DelTree(tree->left);
    DelTree(tree->right);
    delete tree;
    return;
}

/*****************************************************************************/
