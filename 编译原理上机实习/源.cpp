//name:吕忠有
//last time：2022年5月29日09:51:25

#include <bits/stdc++.h>
using namespace std;

//string str = "int$v,$c;$v=41;$c=15;$c=$v+4*6+($v*3)*$c+940*($v*6);if($v*89<$v)then$v=$c*479;elsebegin$c=$c+85;$c=$c+734;end;while($c*6>$v)do$v=$v+9;";
string str = "";
// 读取需要进行词法分析的字符串
ifstream fileIn("in.txt");
// 输出词法分析时获得的二元式
ofstream fileOut("out.txt", ios_base::out);

// 标识符
struct Identifier
{
	string name;	//名称
	string type;	//类型
	string value;	//值
	Identifier() {}
	Identifier(string name) {
		this->name = name;
	}
};
// 二元式
struct TwoTuple
{
	string type;
	string value;
};
//四元式
struct FourTuple
{
	string symbol;
	string operand1;
	string operand2;
	string result;

	FourTuple(string symbol, string operand1, string operand2, string result) {
		this->symbol = symbol;
		this->operand1 = operand1;
		this->operand2 = operand2;
		this->result = result;
	}

	string toString() {
		return "(" + this->symbol + ", " + this->operand1 + ", " + this->operand2 + ", " + this->result + ")";
	}
};
// 中间代码表
struct MiddleCodeTable
{
	vector<FourTuple> table;
	int NXQ = 0;
	bool add(string symbol, string operand1, string operand2, string result) {
		table.push_back(FourTuple(symbol, operand1, operand2, result));
		return true;
	}
	int getNXQ() {
		NXQ = table.size();
		//return atoi(to_string(table.size()).c_str());
		return NXQ;
	}
	bool backPath(int index, string result) {
		int length = table.size();
		if (index < 0 || index > length - 1)
		{
			return false;
		}
		table[index].result = result;
		return true;
	}
	void clear() {
		table.clear();
	}
	void print() {
		for (unsigned i = 0; i < table.size(); i++)
		{
			cout << "(" + to_string(i) + ")" + table[i].toString() << endl;
			fileOut << "(" + to_string(i) + ")" + table[i].toString() + "\n";
		}
	}
};
//临时变量表
struct TempVarTable {
	vector<string> table;
	Identifier getNewTempVar() {
		unsigned index = table.size();
		string name = "T" + to_string(index);
		table.push_back(name);

		return Identifier(name);
	}
	void clear() {
		table.clear();
	}
	void print() {
		string str = "";
		for (unsigned i = 0; i < table.size(); i++)
		{
			str += table[i] + ", ";
		}
	}
};
//
TwoTuple word;

vector<Identifier> identifierTable;	//标识符列表
MiddleCodeTable codeTable; // 中间代码表实例
TempVarTable tempVarTable; //中间变量表实例
string inputProgram;	//保存程序源代码的字符串
int inputPointer;	//保存读头的指针或整数
string temp;	//保存正在读取单词的字符串
int state;	//保存自动机状态编号的整数

TwoTuple nextInput(); //词法分析程序
bool existIdentifier(string); //判断标识符是否存在
void printIdentifier(); //打印标识符
void printTwoTuple(TwoTuple);//打印二元式
TwoTuple returnTwoTuple(string, string);//构造二元式并返回
bool match(string type);//匹配
int getIndexByName(string);
bool updateIdentiferTypeByName(string, string);
bool parseProgram();
bool parseVariableDescriptionPart();
string parseVariableDescription();
bool parseIdentifierList(string);
bool parseIdentifierListPrime(string);
bool parseStatementPart();
bool parseStatementPartPrime();
bool parsePentence();
bool parseAssignmentStatement();
bool parseConditionalStatement();
bool parseCircularStatement();
Identifier parseExpression();
Identifier parseExpressionPrime(Identifier);
Identifier parseTerm();
Identifier parseTermPrime(Identifier);
Identifier parseFactor();
bool parseConstant();
bool parseUnsignedInteger();
bool parseDigitalSequence();
bool parseDigitalSequencePrime();
bool parseAdditionOperator();
bool parseMultiplicationOperator();
string parseRelationalOperator();
Identifier parseCondition();
bool parseCompoundStatement();
bool parseNestedStatement();
int main() {
	// 判断文件是否这正常打开
	if (!fileOut.is_open() || !fileIn.is_open())
	{
		cout << "文件打开失败！" << endl;
		return 0;
	}

	//读入in.txt文件中的字符串
	//暂存读出来的字符
	char c;
	fileIn >> noskipws;
	while (fileIn.peek() != EOF) {
		fileIn >> c;
		str += c;
	}
	cout << str << endl;

	//初始化
	inputProgram = str + "#";
	inputPointer = 0;

	word = nextInput();
	cout << "识别单词：";
	fileOut << "识别单词：";
	printTwoTuple(word);

	parseProgram();

	cout << "Identifier Table：";
	fileOut << "Identifier Table：";
	printIdentifier();
	cout << "语义分析产生的四元式" << endl;
	fileOut << "语义分析产生的四元式" << endl;
	codeTable.print();


	//关闭文件
	fileIn.close();
	fileOut.close();
	system("pause");

	return 0;
}

TwoTuple nextInput() {
	temp = "";
	state = 1;
	char symbol;
	while (inputProgram[inputPointer] != '#')
	{
		symbol = inputProgram[inputPointer];
		//识别空格,跳过
		if (symbol == ' ')
		{
			++inputPointer;
			continue;
		}

		//识别标识符
		if (state == 1 && symbol == '$')
		{
			state = 2;
			temp += symbol;
			++inputPointer;
			continue;
		}
		if (state == 2 && (symbol >= 'a' && symbol <= 'z'))
		{
			state = 3;
			temp += symbol;
			++inputPointer;
			continue;
		}
		if (state == 3 && ((symbol >= 'a' && symbol <= 'z') || (symbol >= '0' && symbol <= '9')))
		{
			temp += symbol;
			++inputPointer;
			continue;
		}
		else if (state == 3)
		{
			//找到标识符
			Identifier iden;
			iden.name = temp;
			if (!existIdentifier(iden.name))
			{
				identifierTable.push_back(iden);
			}
			return returnTwoTuple("标识符", temp);
		}

		//识别分号
		if (state == 1 && symbol == ';')
		{
			state = 4;
			temp += symbol;
			continue;
		}
		if (state == 4)
		{
			//找到分号
			++inputPointer;
			return returnTwoTuple("分号", temp);
		}

		//识别逗号
		if (state == 1 && symbol == ',')
		{
			state = 5;
			temp += symbol;
			++inputPointer;
			continue;
		}
		if (state == 5)
		{
			//找到逗号
			return returnTwoTuple("逗号", temp);
		}

		//识别=赋值运算符和==关系运算符
		if (state == 1 && symbol == '=')
		{
			state = 6;
			temp += symbol;
			++inputPointer;
			continue;
		}
		if (state == 6)
		{
			if (symbol == '=')
			{
				//找到==关系运算符
				temp += symbol;
				++inputPointer;
				return returnTwoTuple("关系运算符", temp);
			}
			else
			{
				//找到赋值运算符
				return returnTwoTuple("赋值运算符", temp);
			}
		}

		//识别<关系运算符和<=关系运算符
		if (state == 1 && symbol == '<')
		{
			state = 8;
			temp += symbol;
			++inputPointer;
			continue;
		}
		if (state == 8)
		{
			if (symbol == '=')
			{
				//找到<=关系运算符
				temp += symbol;
				++inputPointer;
				return returnTwoTuple("关系运算符", temp);
			}
			else
			{
				//找到<关系运算符
				return returnTwoTuple("关系运算符", temp);
			}
		}

		//识别>关系运算符和>=关系运算符
		if (state == 1 && symbol == '>')
		{
			state = 10;
			temp += symbol;
			++inputPointer;
			continue;
		}
		if (state == 10)
		{
			if (symbol == '=')
			{
				//找到>=关系运算符
				temp += symbol;
				++inputPointer;
				return returnTwoTuple("关系运算符", temp);
			}
			else
			{
				//找到>关系运算符
				return returnTwoTuple("关系运算符", temp);
			}
		}

		//识别关系运算符!=
		if (state == 1 && symbol == '!')
		{
			state = 12;
			temp += symbol;
			++inputPointer;
			continue;
		}
		if (state == 12 && symbol == '=')
		{
			state = 13;
			temp += symbol;
			continue;
		}
		if (state == 13)
		{
			//找到关系运算符!=
			++inputPointer;
			return returnTwoTuple("关系运算符", temp);

		}

		//识别加法运算符
		if (state == 1 && symbol == '+')
		{
			state = 14;
			temp += symbol;
			++inputPointer;
			continue;
		}
		if (state == 14)
		{
			//找到加法运算符
			return returnTwoTuple("加法运算符", temp);
		}

		//识别乘法运算符
		if (state == 1 && symbol == '*')
		{
			state = 15;
			temp += symbol;
			++inputPointer;
			continue;
		}
		if (state == 15)
		{
			//找到乘法运算符
			return returnTwoTuple("乘法运算符", temp);
		}

		//识别左括号
		if (state == 1 && symbol == '(')
		{
			state = 16;
			temp += symbol;
			++inputPointer;
			continue;
		}
		if (state == 16)
		{
			//找到左括号
			return returnTwoTuple("左括号", temp);
		}

		//识别右括号
		if (state == 1 && symbol == ')')
		{
			state = 17;
			temp += symbol;
			++inputPointer;
			continue;
		}
		if (state == 17)
		{
			//找到右括号
			return returnTwoTuple("右括号", temp);
		}

		// 识别int和if
		if (state == 1 && symbol == 'i')
		{
			state = 18;
			temp += symbol;
			++inputPointer;
			continue;
		}
		if (state == 18 && symbol == 'n')
		{
			state = 19;
			temp += symbol;
			++inputPointer;
			continue;
		}
		if (state == 19 && symbol == 't')
		{
			// 发现int
			temp += symbol;
			++inputPointer;
			return returnTwoTuple("int", temp);
		}
		if (state == 18 && symbol == 'f')
		{
			// 发现if
			temp += symbol;
			++inputPointer;
			return returnTwoTuple("if", temp);
		}

		//识别then
		if (state == 1 && symbol == 't')
		{
			state = 23;
			temp += symbol;
			++inputPointer;
			continue;
		}
		if (state == 23 && symbol == 'h')
		{
			state = 24;
			temp += symbol;
			++inputPointer;
			continue;
		}
		if (state == 24 && symbol == 'e')
		{
			state = 25;
			temp += symbol;
			++inputPointer;
			continue;
		}
		if (state == 25 && symbol == 'n')
		{
			state = 26;
			temp += symbol;
			continue;
		}
		if (state == 26)
		{
			//找到then
			++inputPointer;
			return returnTwoTuple("then", temp);
		}

		//识别else和end
		if (state == 1 && symbol == 'e')
		{
			state = 27;
			temp += symbol;
			++inputPointer;
			continue;
		}
		if (state == 27 && symbol == 'l')
		{
			state = 28;
			temp += symbol;
			++inputPointer;
			continue;
		}
		if (state == 28 && symbol == 's')
		{
			state = 29;
			temp += symbol;
			++inputPointer;
			continue;
		}
		if (state == 29 && symbol == 'e')
		{
			state = 30;
			temp += symbol;
			continue;
		}
		if (state == 30)
		{
			//找到else
			++inputPointer;
			return returnTwoTuple("else", temp);
		}
		if (state == 27 && symbol == 'n')
		{
			state = 31;
			temp += symbol;
			++inputPointer;
			continue;
		}
		if (state == 31 && symbol == 'd')
		{
			state = 32;
			temp += symbol;
			continue;
		}
		if (state == 32)
		{
			//找到end
			++inputPointer;
			return returnTwoTuple("end", temp);
		}

		//识别while
		if (state == 1 && symbol == 'w')
		{
			state = 33;
			temp += symbol;
			++inputPointer;
			continue;
		}
		if (state == 33 && symbol == 'h')
		{
			state = 34;
			temp += symbol;
			++inputPointer;
			continue;
		}
		if (state == 34 && symbol == 'i')
		{
			state = 35;
			temp += symbol;
			++inputPointer;
			continue;
		}
		if (state == 35 && symbol == 'l')
		{
			state = 36;
			temp += symbol;
			++inputPointer;
			continue;
		}
		if (state == 36 && symbol == 'e')
		{
			state = 37;
			temp += symbol;
			continue;
		}
		if (state == 37)
		{
			//找到while
			++inputPointer;
			return returnTwoTuple("while", temp);
		}

		//识别do
		if (state == 1 && symbol == 'd')
		{
			state = 38;
			temp += symbol;
			++inputPointer;
			continue;
		}
		if (state == 38 && symbol == 'o')
		{
			state = 39;
			temp += symbol;
			continue;
		}
		if (state == 39)
		{
			//找到do
			++inputPointer;
			return returnTwoTuple("do", temp);
		}

		//识别begin
		if (state == 1 && symbol == 'b')
		{
			state = 40;
			temp += symbol;
			++inputPointer;
			continue;
		}
		if (state == 40 && symbol == 'e')
		{
			state = 41;
			temp += symbol;
			++inputPointer;
			continue;
		}
		if (state == 41 && symbol == 'g')
		{
			state = 42;
			temp += symbol;
			++inputPointer;
			continue;
		}
		if (state == 42 && symbol == 'i')
		{
			state = 43;
			temp += symbol;
			++inputPointer;
			continue;
		}
		if (state == 43 && symbol == 'n')
		{
			state = 44;
			temp += symbol;
			continue;
		}
		if (state == 44)
		{
			//找到begin
			++inputPointer;
			return returnTwoTuple("begin", temp);
		}

		//识别数字
		if (state == 1 && (symbol >= '0' && symbol <= '9'))
		{
			state = 45;
			temp += symbol;
			++inputPointer;
			continue;
		}
		if (state == 45)
		{
			if (symbol >= '0' && symbol <= '9')
			{
				temp += symbol;
				++inputPointer;
				continue;
			}
			//找到数字
			return returnTwoTuple("数字", temp);
		}

		//若词法分析存在错误
		cout << "----------------" << "第" << inputPointer + 1 << "处" << symbol << "出现词法分析错误，已跳过！" << "-----------------" << endl;
		fileOut << "----------------" << "第" << inputPointer + 1 << "处" << symbol << "出现词法分析错误，已跳过！" << "-----------------" << endl;
		++inputPointer;
		return nextInput();
	}
	return returnTwoTuple("#", "#");
}

bool existIdentifier(string name) {
	for (unsigned int i = 0; i < identifierTable.size(); i++)
	{
		if (identifierTable[i].name == name) { return true; }
	}
	return false;
}

void printIdentifier() {
	string str = "";
	for (unsigned int i = 0; i < identifierTable.size(); i++)
	{
		str += identifierTable[i].name + ',' + identifierTable[i].type + ',' + identifierTable[i].value + ';';
	}
	cout << str << endl;
	fileOut << str << endl;
}

void printTwoTuple(TwoTuple word) {
	cout << "(" + word.type + ", " + word.value + ")" << endl;
	fileOut << "(" + word.type + ", " + word.value + ")" << endl;
}

TwoTuple returnTwoTuple(string type, string value) {
	TwoTuple tt;
	tt.type = type;
	tt.value = value;
	return tt;
}

bool match(string type) {
	if (type != word.type)
	{
		cout << "匹配失败！" << endl;
		fileOut << "匹配失败" << endl;
		return false;
	}
	// 匹配成功
	cout << "匹配成功！" << endl;
	fileOut << "匹配成功！" << endl;
	word = nextInput();
	cout << "识别单词：";
	fileOut << "识别单词：";
	printTwoTuple(word);
	return true;
}

int getIndexByName(string name) {
	for (unsigned i = 0; i < identifierTable.size(); i++)
	{
		if (identifierTable[i].name == name)
		{
			return i;
		}
	}
	return -1;
}

bool updateIdentiferTypeByName(string name, string type) {
	int index = getIndexByName(name);
	if (index == -1)
	{
		return false;
	}
	if (identifierTable[index].type == type)
	{
		cout << "----------------" << "第" << inputPointer - name.size() << "处重复定义标识符" << name << "-----------------" << endl;
		fileOut << "----------------" << "第" << inputPointer - name.size() << "处重复定义标识符" << name << "-----------------" << endl;
		return false;
	}
	else {
		identifierTable[index].type = type;
	}
	return true;
}

bool updateIdentiferValueByName(string name, string value) {
	int index = getIndexByName(name);
	if (index == -1)
	{
		return false;
	}
	identifierTable[index].value = value;
	return true;
}

string getIdentiferValueByName(string name) {
	int index = getIndexByName(name);
	if (index == -1)
	{
		return "error identifier not exist";
	}
	return identifierTable[index].value;
}

int getIdentiferValueByNameAsInt(string name) {
	int index = getIndexByName(name);
	if (index == -1)
	{
		return -1;
	}
	if (identifierTable[index].value == "")
	{
		return -1;
	}
	return atoi(identifierTable[index].value.c_str());
}

bool parseProgram() {
	cout << "(推导：<程序> →<变量说明部分>;<语句部分> )" << endl;
	fileOut << "(推导：<程序> →<变量说明部分>;<语句部分> )" << endl;

	parseVariableDescriptionPart();
	match("分号");
	parseStatementPart();

	cout << "语义分析结束！" << endl;
	fileOut << "语义分析结束！" << endl;
	return true;
}

bool parseVariableDescriptionPart() {
	cout << "(推导：<变量说明部分> → <变量说明><标识符列表> )" << endl;
	fileOut << "(推导：<变量说明部分> → <变量说明><标识符列表> )" << endl;

	string type = parseVariableDescription();
	parseIdentifierList(type);

	return true;
}

string parseVariableDescription() {
	cout << "(推导：<变量说明> → int )" << endl;
	fileOut << "(推导：<变量说明> → int )" << endl;

	match("int");

	return "int";
}

bool parseIdentifierList(string type) {
	cout << "(推导：<标识符列表> →<标识符><标识符列表prime>)" << endl;
	fileOut << "(推导：<标识符列表> →<标识符><标识符列表prime>)" << endl;

	if (word.type == "标识符")
	{
		updateIdentiferTypeByName(word.value, "int");
		cout << "更新标识符" + word.value + "类型为int" << endl;
		fileOut << "更新标识符" + word.value + "类型为int" << endl;
		match("标识符");
	}
	parseIdentifierListPrime(type);
	return true;
}

bool parseIdentifierListPrime(string type) {
	cout << "(推导：<标识符列表prime> →,<标识符><标识符列表prime>|ε)" << endl;
	fileOut << "(推导：<标识符列表prime> →,<标识符><标识符列表prime>|ε)" << endl;

	if (word.type == "逗号")
	{
		match("逗号");
		if (word.type == "标识符")
		{
			if (updateIdentiferTypeByName(word.value, "int")) {
				cout << "更新标识符" + word.value + "类型为int" << endl;
				fileOut << "更新标识符" + word.value + "类型为int" << endl;
			}
			match("标识符");
		}
		parseIdentifierListPrime(type);
	}
	else
	{

	}
	return true;
}

bool parseStatementPart() {
	cout << "(推导：<语句部分> →<语句>;<语句部分prime>)" << endl;
	fileOut << "(推导：<语句部分> →<语句>;<语句部分prime>)" << endl;

	parsePentence();
	match("分号");
	parseStatementPartPrime();

	return true;
}

bool parseStatementPartPrime() {
	cout << "(推导：<语句部分prime> →<语句>;<语句部分prime>|ε)" << endl;
	fileOut << "(推导：<语句部分prime> →<语句>;<语句部分prime>|ε)" << endl;

	if (word.type == "标识符" || word.type == "if" || word.type == "while")
	{
		parsePentence();
		match("分号");
		parseStatementPartPrime();
	}
	else
	{

	}

	return true;
}

bool parsePentence() {
	cout << "(推导：<语句> → <赋值语句>|<条件语句>|<循环语句> )" << endl;
	fileOut << "(推导：<语句> → <赋值语句>|<条件语句>|<循环语句> )" << endl;

	if (word.type == "标识符")
	{
		parseAssignmentStatement();
	}
	else if (word.type == "if")
	{
		parseConditionalStatement();
	}
	else if (word.type == "while")
	{
		parseCircularStatement();
	}

	return true;
}

bool parseAssignmentStatement() {
	cout << "(推导：<赋值语句> → <标识符>=<表达式> )" << endl;
	fileOut << "(推导：<赋值语句> → <标识符>=<表达式> )" << endl;

	string identifierName = "";
	if (word.type == "标识符")
	{
		identifierName = word.value;
		match("标识符");
	}
	match("赋值运算符");

	Identifier E = parseExpression();
	codeTable.add("=", E.name, "null", identifierName);
	cout << "产生四元式" << endl;
	fileOut << "产生四元式" << endl;

	updateIdentiferValueByName(identifierName, E.value);

	return true;
}

bool parseConditionalStatement() {
	cout << "(推导：<条件语句> → if （<条件>） then <嵌套语句>; else <嵌套语句> )" << endl;
	fileOut << "(推导：<条件语句> → if （<条件>） then <嵌套语句>; else <嵌套语句> )" << endl;

	match("if");
	match("左括号");
	Identifier E = parseCondition();
	match("右括号");
	match("then");

	codeTable.add("jnz", E.name, "null", to_string(codeTable.getNXQ() + 2));
	int falseExitIndex = codeTable.getNXQ();
	codeTable.add("j", "null", "null", "0"); //需要进行回填操作

	parseNestedStatement();

	int exitIndex = codeTable.getNXQ();
	codeTable.add("j", "null", "null", "0"); //需要进行回填操作

	codeTable.backPath(falseExitIndex, to_string(codeTable.getNXQ()));

	match("分号");
	match("else");
	parseNestedStatement();

	codeTable.backPath(exitIndex, to_string(codeTable.getNXQ()));

	return true;
}

bool parseCircularStatement() {
	cout << "(推导：<循环语句> → while （<条件>） do <嵌套语句> )" << endl;
	fileOut << "(推导：<循环语句> → while （<条件>） do <嵌套语句> )" << endl;

	match("while");
	match("左括号");
	int next = codeTable.getNXQ();
	Identifier E = parseCondition();
	match("右括号");
	match("do");

	codeTable.add("jnz", E.name, "null", to_string(codeTable.getNXQ() + 2));
	int falseExitIndex = codeTable.getNXQ();
	codeTable.add("j", "null", "null", "0"); //需要进行回填操作

	parseNestedStatement();

	int exitIndex = codeTable.getNXQ();
	codeTable.add("j", "null", "null", to_string(next));

	codeTable.backPath(falseExitIndex, to_string(codeTable.getNXQ()));

	return true;
}

Identifier parseExpression() {
	cout << "(推导：<表达式> → <项><表达式prime>)" << endl;
	fileOut << "(推导：<表达式> → <项><表达式prime>)" << endl;

	Identifier E1 = parseTerm();
	Identifier E2 = parseExpressionPrime(E1);

	return E2;
}

Identifier parseExpressionPrime(Identifier E1) {
	cout << "(推导：<表达式prime> → <加法运算符><项><表达式prime>|ε)" << endl;
	fileOut << "(推导：<表达式prime> → <加法运算符><项><表达式prime>|ε)" << endl;

	if (word.type == "加法运算符")
	{
		match("加法运算符");
		Identifier E2 = parseTerm();
		Identifier T = tempVarTable.getNewTempVar();

		codeTable.add("+", E1.name, E2.name, T.name);
		cout << "产生加法四元式" << endl;
		fileOut << "产生加法四元式" << endl;

		T.value = to_string(atoi(E1.value.c_str()) + atoi(E2.value.c_str()));

		Identifier E = parseExpressionPrime(T);

		return E;
	}
	else
	{
		return E1;
	}

}

Identifier parseTerm() {
	cout << "(推导：<项> → <因子><项prime>)" << endl;
	fileOut << "(推导：<项> → <因子><项prime>)" << endl;

	Identifier E1 = parseFactor();
	Identifier E2 = parseTermPrime(E1);

	return E2;
}

Identifier parseTermPrime(Identifier E1) {
	cout << "(推导：<项prime> → <乘法运算符><因子> <项prime>|ε)" << endl;
	fileOut << "(推导：<项prime> → <乘法运算符><因子> <项prime>|ε)" << endl;

	if (word.type == "乘法运算符")
	{
		match("乘法运算符");
		Identifier E2 = parseFactor();
		Identifier T = tempVarTable.getNewTempVar();

		codeTable.add("*", E1.name, E2.name, T.name);
		cout << "产生乘法四元式" << endl;
		fileOut << "产生乘法四元式" << endl;

		T.value = to_string(atoi(E1.value.c_str()) * atoi(E2.value.c_str()));
		Identifier E = parseTermPrime(T);

		return E;
	}
	else
	{
		return E1;
	}
}

Identifier parseFactor() {
	cout << "(推导：<因子> → <标识符>|<常量>|(<表达式>))" << endl;
	fileOut << "(推导：<因子> → <标识符>|<常量>|(<表达式>))" << endl;

	Identifier E;
	if (word.type == "标识符")
	{
		if (identifierTable[getIndexByName(word.value)].type == "")
		{
			cout << "----------------" << word.value << "未定义！-----------------" << endl;
			fileOut << "----------------" <<word.value << "未定义！-----------------" << endl;
		}

		E = Identifier(word.value);
		E.value = getIdentiferValueByName(word.value);
		match("标识符");
	}
	else if (word.type == "数字")
	{
		E = Identifier(word.value);
		E.value = word.value;
		parseConstant();
	}
	else
	{
		match("左括号");
		E = parseExpression();
		match("右括号");
	}

	return E;
}

bool parseConstant() {
	cout << "(推导：<常量> → <无符号整数> )" << endl;
	fileOut << "(推导：<常量> → <无符号整数> )" << endl;

	parseUnsignedInteger();

	return true;
}

bool parseUnsignedInteger() {
	cout << "(推导：<无符号整数> → <数字序列> )" << endl;
	fileOut << "(推导：<无符号整数> → <数字序列> )" << endl;

	parseDigitalSequence();

	return true;
}

bool parseDigitalSequence() {
	cout << "(推导：<数字序列> →<数字><数字序列prime>" << endl;
	fileOut << "(推导：<数字序列> →<数字><数字序列prime>" << endl;

	match("数字");
	parseDigitalSequencePrime();

	return true;
}

bool parseDigitalSequencePrime() {
	cout << "(推导：<数字序列prime> →<数字><数字序列prime>|ε)" << endl;
	fileOut << "(推导：<数字序列prime> →<数字><数字序列prime>|ε)" << endl;

	if (word.type == "数字")
	{
		match("数字");
		parseDigitalSequencePrime();
	}
	else
	{

	}

	return true;
}

bool parseAdditionOperator() {
	cout << "(推导：<加法运算符> → + )" << endl;
	fileOut << "(推导：<加法运算符> → + )" << endl;

	match("加法运算符");

	return true;
}

bool parseMultiplicationOperator() {
	cout << "(推导：<乘法运算符> → * )" << endl;
	fileOut << "(推导：<乘法运算符> → * )" << endl;

	match("乘法运算符");

	return true;
}

string parseRelationalOperator() {
	cout << "(推导：<关系运算符> → <|>|!= |>=|<= |== )" << endl;
	fileOut << "(推导：<关系运算符> → <|>|!= |>=|<= |== )" << endl;

	string logicOperator = word.value;
	match("关系运算符");

	return logicOperator;
}

Identifier parseCondition() {
	cout << "(推导：<条件> → <表达式><关系运算符><表达式> )" << endl;
	fileOut << "(推导：<条件> → <表达式><关系运算符><表达式> )" << endl;

	Identifier E1 = parseExpression();
	string logicOperator = parseRelationalOperator();
	Identifier E2 = parseExpression();

	Identifier T = tempVarTable.getNewTempVar();
	codeTable.add(logicOperator, E1.name, E2.name, T.name);
	cout << "产生逻辑计算四元式" << endl;
	fileOut << "产生逻辑计算四元式" << endl;
	T.type = "bool";

	int value1 = atoi(E1.value.c_str());
	int value2 = atoi(E2.value.c_str());

	if (logicOperator == "<")
	{
		if (value1 < value2)
		{
			T.value = "true";
		}
		else
		{
			T.value = "false";
		}
	}
	if (logicOperator == ">")
	{
		if (value1 > value2)
		{
			T.value = "true";
		}
		else
		{
			T.value = "false";
		}
	}
	if (logicOperator == "!=")
	{
		if (value1 != value2)
		{
			T.value = "true";
		}
		else
		{
			T.value = "false";
		}
	}
	if (logicOperator == "<=")
	{
		if (value1 <= value2)
		{
			T.value = "true";
		}
		else
		{
			T.value = "false";
		}
	}
	if (logicOperator == ">=")
	{
		if (value1 >= value2)
		{
			T.value = "true";
		}
		else
		{
			T.value = "false";
		}
	}
	if (logicOperator == "==")
	{
		if (value1 == value2)
		{
			T.value = "true";
		}
		else
		{
			T.value = "false";
		}
	}

	return T;
}

bool parseCompoundStatement() {
	cout << "(推导：<复合语句> → begin <语句部分> end )" << endl;
	fileOut << "(推导：<复合语句> → begin <语句部分> end )" << endl;

	match("begin");
	parseStatementPart();
	match("end");

	return true;
}

bool parseNestedStatement() {
	cout << "(推导：<嵌套语句> → <语句>|<复合语句> )" << endl;
	fileOut << "(推导：<嵌套语句> → <语句>|<复合语句> )" << endl;

	if (word.type == "标识符" || word.type == "if" || word.type == "while")
	{
		parsePentence();
	}
	else
	{
		parseCompoundStatement();
	}

	return true;
}