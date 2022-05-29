//name:������
//last time��2022��5��29��09:51:25

#include <bits/stdc++.h>
using namespace std;

//string str = "int$v,$c;$v=41;$c=15;$c=$v+4*6+($v*3)*$c+940*($v*6);if($v*89<$v)then$v=$c*479;elsebegin$c=$c+85;$c=$c+734;end;while($c*6>$v)do$v=$v+9;";
string str = "";
// ��ȡ��Ҫ���дʷ��������ַ���
ifstream fileIn("in.txt");
// ����ʷ�����ʱ��õĶ�Ԫʽ
ofstream fileOut("out.txt", ios_base::out);

// ��ʶ��
struct Identifier
{
	string name;	//����
	string type;	//����
	string value;	//ֵ
	Identifier() {}
	Identifier(string name) {
		this->name = name;
	}
};
// ��Ԫʽ
struct TwoTuple
{
	string type;
	string value;
};
//��Ԫʽ
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
// �м�����
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
//��ʱ������
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

vector<Identifier> identifierTable;	//��ʶ���б�
MiddleCodeTable codeTable; // �м�����ʵ��
TempVarTable tempVarTable; //�м������ʵ��
string inputProgram;	//�������Դ������ַ���
int inputPointer;	//�����ͷ��ָ�������
string temp;	//�������ڶ�ȡ���ʵ��ַ���
int state;	//�����Զ���״̬��ŵ�����

TwoTuple nextInput(); //�ʷ���������
bool existIdentifier(string); //�жϱ�ʶ���Ƿ����
void printIdentifier(); //��ӡ��ʶ��
void printTwoTuple(TwoTuple);//��ӡ��Ԫʽ
TwoTuple returnTwoTuple(string, string);//�����Ԫʽ������
bool match(string type);//ƥ��
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
	// �ж��ļ��Ƿ���������
	if (!fileOut.is_open() || !fileIn.is_open())
	{
		cout << "�ļ���ʧ�ܣ�" << endl;
		return 0;
	}

	//����in.txt�ļ��е��ַ���
	//�ݴ���������ַ�
	char c;
	fileIn >> noskipws;
	while (fileIn.peek() != EOF) {
		fileIn >> c;
		str += c;
	}
	cout << str << endl;

	//��ʼ��
	inputProgram = str + "#";
	inputPointer = 0;

	word = nextInput();
	cout << "ʶ�𵥴ʣ�";
	fileOut << "ʶ�𵥴ʣ�";
	printTwoTuple(word);

	parseProgram();

	cout << "Identifier Table��";
	fileOut << "Identifier Table��";
	printIdentifier();
	cout << "���������������Ԫʽ" << endl;
	fileOut << "���������������Ԫʽ" << endl;
	codeTable.print();


	//�ر��ļ�
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
		//ʶ��ո�,����
		if (symbol == ' ')
		{
			++inputPointer;
			continue;
		}

		//ʶ���ʶ��
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
			//�ҵ���ʶ��
			Identifier iden;
			iden.name = temp;
			if (!existIdentifier(iden.name))
			{
				identifierTable.push_back(iden);
			}
			return returnTwoTuple("��ʶ��", temp);
		}

		//ʶ��ֺ�
		if (state == 1 && symbol == ';')
		{
			state = 4;
			temp += symbol;
			continue;
		}
		if (state == 4)
		{
			//�ҵ��ֺ�
			++inputPointer;
			return returnTwoTuple("�ֺ�", temp);
		}

		//ʶ�𶺺�
		if (state == 1 && symbol == ',')
		{
			state = 5;
			temp += symbol;
			++inputPointer;
			continue;
		}
		if (state == 5)
		{
			//�ҵ�����
			return returnTwoTuple("����", temp);
		}

		//ʶ��=��ֵ�������==��ϵ�����
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
				//�ҵ�==��ϵ�����
				temp += symbol;
				++inputPointer;
				return returnTwoTuple("��ϵ�����", temp);
			}
			else
			{
				//�ҵ���ֵ�����
				return returnTwoTuple("��ֵ�����", temp);
			}
		}

		//ʶ��<��ϵ�������<=��ϵ�����
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
				//�ҵ�<=��ϵ�����
				temp += symbol;
				++inputPointer;
				return returnTwoTuple("��ϵ�����", temp);
			}
			else
			{
				//�ҵ�<��ϵ�����
				return returnTwoTuple("��ϵ�����", temp);
			}
		}

		//ʶ��>��ϵ�������>=��ϵ�����
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
				//�ҵ�>=��ϵ�����
				temp += symbol;
				++inputPointer;
				return returnTwoTuple("��ϵ�����", temp);
			}
			else
			{
				//�ҵ�>��ϵ�����
				return returnTwoTuple("��ϵ�����", temp);
			}
		}

		//ʶ���ϵ�����!=
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
			//�ҵ���ϵ�����!=
			++inputPointer;
			return returnTwoTuple("��ϵ�����", temp);

		}

		//ʶ��ӷ������
		if (state == 1 && symbol == '+')
		{
			state = 14;
			temp += symbol;
			++inputPointer;
			continue;
		}
		if (state == 14)
		{
			//�ҵ��ӷ������
			return returnTwoTuple("�ӷ������", temp);
		}

		//ʶ��˷������
		if (state == 1 && symbol == '*')
		{
			state = 15;
			temp += symbol;
			++inputPointer;
			continue;
		}
		if (state == 15)
		{
			//�ҵ��˷������
			return returnTwoTuple("�˷������", temp);
		}

		//ʶ��������
		if (state == 1 && symbol == '(')
		{
			state = 16;
			temp += symbol;
			++inputPointer;
			continue;
		}
		if (state == 16)
		{
			//�ҵ�������
			return returnTwoTuple("������", temp);
		}

		//ʶ��������
		if (state == 1 && symbol == ')')
		{
			state = 17;
			temp += symbol;
			++inputPointer;
			continue;
		}
		if (state == 17)
		{
			//�ҵ�������
			return returnTwoTuple("������", temp);
		}

		// ʶ��int��if
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
			// ����int
			temp += symbol;
			++inputPointer;
			return returnTwoTuple("int", temp);
		}
		if (state == 18 && symbol == 'f')
		{
			// ����if
			temp += symbol;
			++inputPointer;
			return returnTwoTuple("if", temp);
		}

		//ʶ��then
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
			//�ҵ�then
			++inputPointer;
			return returnTwoTuple("then", temp);
		}

		//ʶ��else��end
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
			//�ҵ�else
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
			//�ҵ�end
			++inputPointer;
			return returnTwoTuple("end", temp);
		}

		//ʶ��while
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
			//�ҵ�while
			++inputPointer;
			return returnTwoTuple("while", temp);
		}

		//ʶ��do
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
			//�ҵ�do
			++inputPointer;
			return returnTwoTuple("do", temp);
		}

		//ʶ��begin
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
			//�ҵ�begin
			++inputPointer;
			return returnTwoTuple("begin", temp);
		}

		//ʶ������
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
			//�ҵ�����
			return returnTwoTuple("����", temp);
		}

		//���ʷ��������ڴ���
		cout << "----------------" << "��" << inputPointer + 1 << "��" << symbol << "���ִʷ�����������������" << "-----------------" << endl;
		fileOut << "----------------" << "��" << inputPointer + 1 << "��" << symbol << "���ִʷ�����������������" << "-----------------" << endl;
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
		cout << "ƥ��ʧ�ܣ�" << endl;
		fileOut << "ƥ��ʧ��" << endl;
		return false;
	}
	// ƥ��ɹ�
	cout << "ƥ��ɹ���" << endl;
	fileOut << "ƥ��ɹ���" << endl;
	word = nextInput();
	cout << "ʶ�𵥴ʣ�";
	fileOut << "ʶ�𵥴ʣ�";
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
		cout << "----------------" << "��" << inputPointer - name.size() << "���ظ������ʶ��" << name << "-----------------" << endl;
		fileOut << "----------------" << "��" << inputPointer - name.size() << "���ظ������ʶ��" << name << "-----------------" << endl;
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
	cout << "(�Ƶ���<����> ��<����˵������>;<��䲿��> )" << endl;
	fileOut << "(�Ƶ���<����> ��<����˵������>;<��䲿��> )" << endl;

	parseVariableDescriptionPart();
	match("�ֺ�");
	parseStatementPart();

	cout << "�������������" << endl;
	fileOut << "�������������" << endl;
	return true;
}

bool parseVariableDescriptionPart() {
	cout << "(�Ƶ���<����˵������> �� <����˵��><��ʶ���б�> )" << endl;
	fileOut << "(�Ƶ���<����˵������> �� <����˵��><��ʶ���б�> )" << endl;

	string type = parseVariableDescription();
	parseIdentifierList(type);

	return true;
}

string parseVariableDescription() {
	cout << "(�Ƶ���<����˵��> �� int )" << endl;
	fileOut << "(�Ƶ���<����˵��> �� int )" << endl;

	match("int");

	return "int";
}

bool parseIdentifierList(string type) {
	cout << "(�Ƶ���<��ʶ���б�> ��<��ʶ��><��ʶ���б�prime>)" << endl;
	fileOut << "(�Ƶ���<��ʶ���б�> ��<��ʶ��><��ʶ���б�prime>)" << endl;

	if (word.type == "��ʶ��")
	{
		updateIdentiferTypeByName(word.value, "int");
		cout << "���±�ʶ��" + word.value + "����Ϊint" << endl;
		fileOut << "���±�ʶ��" + word.value + "����Ϊint" << endl;
		match("��ʶ��");
	}
	parseIdentifierListPrime(type);
	return true;
}

bool parseIdentifierListPrime(string type) {
	cout << "(�Ƶ���<��ʶ���б�prime> ��,<��ʶ��><��ʶ���б�prime>|��)" << endl;
	fileOut << "(�Ƶ���<��ʶ���б�prime> ��,<��ʶ��><��ʶ���б�prime>|��)" << endl;

	if (word.type == "����")
	{
		match("����");
		if (word.type == "��ʶ��")
		{
			if (updateIdentiferTypeByName(word.value, "int")) {
				cout << "���±�ʶ��" + word.value + "����Ϊint" << endl;
				fileOut << "���±�ʶ��" + word.value + "����Ϊint" << endl;
			}
			match("��ʶ��");
		}
		parseIdentifierListPrime(type);
	}
	else
	{

	}
	return true;
}

bool parseStatementPart() {
	cout << "(�Ƶ���<��䲿��> ��<���>;<��䲿��prime>)" << endl;
	fileOut << "(�Ƶ���<��䲿��> ��<���>;<��䲿��prime>)" << endl;

	parsePentence();
	match("�ֺ�");
	parseStatementPartPrime();

	return true;
}

bool parseStatementPartPrime() {
	cout << "(�Ƶ���<��䲿��prime> ��<���>;<��䲿��prime>|��)" << endl;
	fileOut << "(�Ƶ���<��䲿��prime> ��<���>;<��䲿��prime>|��)" << endl;

	if (word.type == "��ʶ��" || word.type == "if" || word.type == "while")
	{
		parsePentence();
		match("�ֺ�");
		parseStatementPartPrime();
	}
	else
	{

	}

	return true;
}

bool parsePentence() {
	cout << "(�Ƶ���<���> �� <��ֵ���>|<�������>|<ѭ�����> )" << endl;
	fileOut << "(�Ƶ���<���> �� <��ֵ���>|<�������>|<ѭ�����> )" << endl;

	if (word.type == "��ʶ��")
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
	cout << "(�Ƶ���<��ֵ���> �� <��ʶ��>=<���ʽ> )" << endl;
	fileOut << "(�Ƶ���<��ֵ���> �� <��ʶ��>=<���ʽ> )" << endl;

	string identifierName = "";
	if (word.type == "��ʶ��")
	{
		identifierName = word.value;
		match("��ʶ��");
	}
	match("��ֵ�����");

	Identifier E = parseExpression();
	codeTable.add("=", E.name, "null", identifierName);
	cout << "������Ԫʽ" << endl;
	fileOut << "������Ԫʽ" << endl;

	updateIdentiferValueByName(identifierName, E.value);

	return true;
}

bool parseConditionalStatement() {
	cout << "(�Ƶ���<�������> �� if ��<����>�� then <Ƕ�����>; else <Ƕ�����> )" << endl;
	fileOut << "(�Ƶ���<�������> �� if ��<����>�� then <Ƕ�����>; else <Ƕ�����> )" << endl;

	match("if");
	match("������");
	Identifier E = parseCondition();
	match("������");
	match("then");

	codeTable.add("jnz", E.name, "null", to_string(codeTable.getNXQ() + 2));
	int falseExitIndex = codeTable.getNXQ();
	codeTable.add("j", "null", "null", "0"); //��Ҫ���л������

	parseNestedStatement();

	int exitIndex = codeTable.getNXQ();
	codeTable.add("j", "null", "null", "0"); //��Ҫ���л������

	codeTable.backPath(falseExitIndex, to_string(codeTable.getNXQ()));

	match("�ֺ�");
	match("else");
	parseNestedStatement();

	codeTable.backPath(exitIndex, to_string(codeTable.getNXQ()));

	return true;
}

bool parseCircularStatement() {
	cout << "(�Ƶ���<ѭ�����> �� while ��<����>�� do <Ƕ�����> )" << endl;
	fileOut << "(�Ƶ���<ѭ�����> �� while ��<����>�� do <Ƕ�����> )" << endl;

	match("while");
	match("������");
	int next = codeTable.getNXQ();
	Identifier E = parseCondition();
	match("������");
	match("do");

	codeTable.add("jnz", E.name, "null", to_string(codeTable.getNXQ() + 2));
	int falseExitIndex = codeTable.getNXQ();
	codeTable.add("j", "null", "null", "0"); //��Ҫ���л������

	parseNestedStatement();

	int exitIndex = codeTable.getNXQ();
	codeTable.add("j", "null", "null", to_string(next));

	codeTable.backPath(falseExitIndex, to_string(codeTable.getNXQ()));

	return true;
}

Identifier parseExpression() {
	cout << "(�Ƶ���<���ʽ> �� <��><���ʽprime>)" << endl;
	fileOut << "(�Ƶ���<���ʽ> �� <��><���ʽprime>)" << endl;

	Identifier E1 = parseTerm();
	Identifier E2 = parseExpressionPrime(E1);

	return E2;
}

Identifier parseExpressionPrime(Identifier E1) {
	cout << "(�Ƶ���<���ʽprime> �� <�ӷ������><��><���ʽprime>|��)" << endl;
	fileOut << "(�Ƶ���<���ʽprime> �� <�ӷ������><��><���ʽprime>|��)" << endl;

	if (word.type == "�ӷ������")
	{
		match("�ӷ������");
		Identifier E2 = parseTerm();
		Identifier T = tempVarTable.getNewTempVar();

		codeTable.add("+", E1.name, E2.name, T.name);
		cout << "�����ӷ���Ԫʽ" << endl;
		fileOut << "�����ӷ���Ԫʽ" << endl;

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
	cout << "(�Ƶ���<��> �� <����><��prime>)" << endl;
	fileOut << "(�Ƶ���<��> �� <����><��prime>)" << endl;

	Identifier E1 = parseFactor();
	Identifier E2 = parseTermPrime(E1);

	return E2;
}

Identifier parseTermPrime(Identifier E1) {
	cout << "(�Ƶ���<��prime> �� <�˷������><����> <��prime>|��)" << endl;
	fileOut << "(�Ƶ���<��prime> �� <�˷������><����> <��prime>|��)" << endl;

	if (word.type == "�˷������")
	{
		match("�˷������");
		Identifier E2 = parseFactor();
		Identifier T = tempVarTable.getNewTempVar();

		codeTable.add("*", E1.name, E2.name, T.name);
		cout << "�����˷���Ԫʽ" << endl;
		fileOut << "�����˷���Ԫʽ" << endl;

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
	cout << "(�Ƶ���<����> �� <��ʶ��>|<����>|(<���ʽ>))" << endl;
	fileOut << "(�Ƶ���<����> �� <��ʶ��>|<����>|(<���ʽ>))" << endl;

	Identifier E;
	if (word.type == "��ʶ��")
	{
		if (identifierTable[getIndexByName(word.value)].type == "")
		{
			cout << "----------------" << word.value << "δ���壡-----------------" << endl;
			fileOut << "----------------" <<word.value << "δ���壡-----------------" << endl;
		}

		E = Identifier(word.value);
		E.value = getIdentiferValueByName(word.value);
		match("��ʶ��");
	}
	else if (word.type == "����")
	{
		E = Identifier(word.value);
		E.value = word.value;
		parseConstant();
	}
	else
	{
		match("������");
		E = parseExpression();
		match("������");
	}

	return E;
}

bool parseConstant() {
	cout << "(�Ƶ���<����> �� <�޷�������> )" << endl;
	fileOut << "(�Ƶ���<����> �� <�޷�������> )" << endl;

	parseUnsignedInteger();

	return true;
}

bool parseUnsignedInteger() {
	cout << "(�Ƶ���<�޷�������> �� <��������> )" << endl;
	fileOut << "(�Ƶ���<�޷�������> �� <��������> )" << endl;

	parseDigitalSequence();

	return true;
}

bool parseDigitalSequence() {
	cout << "(�Ƶ���<��������> ��<����><��������prime>" << endl;
	fileOut << "(�Ƶ���<��������> ��<����><��������prime>" << endl;

	match("����");
	parseDigitalSequencePrime();

	return true;
}

bool parseDigitalSequencePrime() {
	cout << "(�Ƶ���<��������prime> ��<����><��������prime>|��)" << endl;
	fileOut << "(�Ƶ���<��������prime> ��<����><��������prime>|��)" << endl;

	if (word.type == "����")
	{
		match("����");
		parseDigitalSequencePrime();
	}
	else
	{

	}

	return true;
}

bool parseAdditionOperator() {
	cout << "(�Ƶ���<�ӷ������> �� + )" << endl;
	fileOut << "(�Ƶ���<�ӷ������> �� + )" << endl;

	match("�ӷ������");

	return true;
}

bool parseMultiplicationOperator() {
	cout << "(�Ƶ���<�˷������> �� * )" << endl;
	fileOut << "(�Ƶ���<�˷������> �� * )" << endl;

	match("�˷������");

	return true;
}

string parseRelationalOperator() {
	cout << "(�Ƶ���<��ϵ�����> �� <|>|!= |>=|<= |== )" << endl;
	fileOut << "(�Ƶ���<��ϵ�����> �� <|>|!= |>=|<= |== )" << endl;

	string logicOperator = word.value;
	match("��ϵ�����");

	return logicOperator;
}

Identifier parseCondition() {
	cout << "(�Ƶ���<����> �� <���ʽ><��ϵ�����><���ʽ> )" << endl;
	fileOut << "(�Ƶ���<����> �� <���ʽ><��ϵ�����><���ʽ> )" << endl;

	Identifier E1 = parseExpression();
	string logicOperator = parseRelationalOperator();
	Identifier E2 = parseExpression();

	Identifier T = tempVarTable.getNewTempVar();
	codeTable.add(logicOperator, E1.name, E2.name, T.name);
	cout << "�����߼�������Ԫʽ" << endl;
	fileOut << "�����߼�������Ԫʽ" << endl;
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
	cout << "(�Ƶ���<�������> �� begin <��䲿��> end )" << endl;
	fileOut << "(�Ƶ���<�������> �� begin <��䲿��> end )" << endl;

	match("begin");
	parseStatementPart();
	match("end");

	return true;
}

bool parseNestedStatement() {
	cout << "(�Ƶ���<Ƕ�����> �� <���>|<�������> )" << endl;
	fileOut << "(�Ƶ���<Ƕ�����> �� <���>|<�������> )" << endl;

	if (word.type == "��ʶ��" || word.type == "if" || word.type == "while")
	{
		parsePentence();
	}
	else
	{
		parseCompoundStatement();
	}

	return true;
}