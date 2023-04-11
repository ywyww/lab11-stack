#pragma warning (disable: 4996)
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <string>
using namespace std;

struct Student {
	char name[256];
	Student* ptrNext;
};

Student* head = NULL;
char filenameTmp[] = "tmpFile";
char filename[] = "myFile";

int n = 0;
bool flag1 = false;										// ���� ������

void create();											// ������� ������ ����
void fill(Student*);									// ��������� ���� ����������
void push(char*);										// ���������� �������� �� ���������
void pop(char*);										// �������� ���������� �������� � ������
void pop();												// �������� ���������� ��������
void print(Student*);									// ������ �����
void destroy();											// ����������� �����
void add(const int, const int, FILE*);					// ���������� �� K ��������� ����� � ������
void pluralPush(const int);								// ������������� ���������� ��������� (��� ������� add)
void writePart(FILE*, const int, const int);			// ������ ����� ����� � ���� � ��������� (��� ������� add);
void chop(const int, FILE*, char* data);				// �������� �������� � ������� N, ���� ������� ����������, data = NULL
void menu(const int);									// ����
void restore(FILE*);									// �������������� ����� �� �����
void write(FILE*, Student*);							// ������ ����� � ���� (��� �������� ���������

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	
	while (true) {
		system("pause");
		system("cls");
		cout << "������������, ��� �� ������ �������?" << endl;
		cout << "1. ������� ����" << endl;
		cout << "2. �������� �� K ��������� � �������� �� ������� pos ����� � ������" << endl;
		cout << "3. ������� ������� �� �����" << endl;
		cout << "4. ���������� ����" << endl;
		cout << "5. �������� ���� � ����" << endl;
		cout << "6. ���������� ����" << endl;
		cout << "7. ������������ ���� �� �����" << endl; cout << endl;
		cout << "������� c" << endl;
		int c;
		cin >> c;
		system("cls");
		menu(c);
	}
	return 0;
}
void create() {
	Student* temp = NULL;
	for (int i = 0; i < n; i++) {
		Student* newElem = new(Student);
		head = newElem;
		head->ptrNext = temp;
		temp = head;
	}
}
void fill(Student* ptrHead) {
	Student* ptr = ptrHead;
	if (ptr != NULL) {
		fill(ptr->ptrNext);
		char nameT[255 + 1];
		cin.getline(nameT, 256);
		strcpy_s(ptr->name, nameT);
	}
}
void push(char* data) {
	Student* newElem = new(Student);
	Student* ptr = head;
	head = newElem;
	head->ptrNext = ptr;
	strcpy(head->name, data);
	n++;
}
void pop(char* data) {
	Student* popper = head;
	head = head->ptrNext;
	strcpy(data, popper->name);
	delete popper;
	n--;
}
void pop() {
	Student* popper = head;
	head = head->ptrNext;
	delete popper;
	n--;
}
void print(Student* point) {		// ���������� ����� ��� �� �����
	Student* ptr = point;
	if (point != NULL) {
		print(ptr->ptrNext);
		cout << ptr->name << endl;
	}
}
void destroy() {
	Student* ptr = head;
	while (head != NULL) {
		head = head->ptrNext;
		delete ptr;
		ptr = head;
	}
	head = NULL;
	n = 0;
}
void pluralPush(const int K) {
	char tmp[256];
	int i = 0;
	while (i < K) {
		cout << "�������������? " << endl;
		gets_s(tmp, 256);
		push(tmp);
		i++;
	}
}		
void writePart(FILE* file, const int num, const int pos) {		// �������� ����� pop
	int a = num;
	if (a >= pos) {
		char buf[256];
		pop(buf);
		writePart(file, a - 1, pos);
		fwrite(&buf, sizeof(buf), 1, file);
	}
}	// ������ ����� ����� � ���� (��� ������� add)
void add(const int pos, const int K, FILE* file) {
	char tmp[256];
	if ((file = fopen(filenameTmp, "wb+")) == NULL) exit(1);
	writePart(file, n, pos);
	fclose(file);

	pluralPush(K);
	if ((file = fopen(filenameTmp, "rb+")) == NULL) exit(1);
	fread(&tmp, sizeof(tmp), 1, file);
	push(tmp);

	pluralPush(K);
	restore(file);	// ������������� ����
	fclose(file);
	remove(filenameTmp);
}
void chop(const int pos, FILE* file, char* data = NULL) {
	if ((file = fopen(filenameTmp, "wb+")) == NULL) exit(3);
	writePart(file, n, pos + 1);
	fclose(file);

	if (data != NULL) {
		char tmp[256];
		pop(tmp);
		strcpy(data, tmp);
	}
	else pop();

	if ((file = fopen(filenameTmp, "rb+")) == NULL) exit(1);
	restore(file);
	fclose(file);
	remove(filenameTmp);
}

void write(FILE* file, Student* ptrHead) {			// �������� ��� ����
	Student* ptr = ptrHead;
	if (ptr != NULL) {
		char buf[256];
		strcpy_s(buf, ptr->name);
		write(file, ptr->ptrNext);
		fwrite(&buf, sizeof(buf), 1, file);
	}
}
void restore(FILE* file) {
	while (!feof(file)) {
		char tmp[256];
		fread(tmp, sizeof(tmp), 1, file);
		if (!feof(file)) { push(tmp); }
	}
}

void menu(const int c) {
	FILE* file = NULL;

	if ((c == 1 || c == 7) && flag1) {					// ��� ������, ����� ������ ��� ������
		cout << "�� ��� ������� ����" << endl;
		return;
	}
	else if (c > 1 && c < 7 && !flag1) {				// ��� ������, ����� ������ ��� �� ������
		cout << "��������/ ������������ ����" << endl;
		return;
	}
	else if (c >= 2 && c <= 5 && n == 0) { cout << "���� ����." << endl; return; }
	switch (c) {
	case 1:
	{
		flag1 = true;
		cout << "������� ���������� ����� �����: ";
		cin >> n;
		while (n <= 0) {
			cout << "������ ������ ������� �����" << endl;
			cin >> n;
		}
		create();
		cout << "������� ������������� ��� " << n << " �������������: " << endl; cin.ignore();
		fill(head);
		break;
	}
	case 2:
	{
		cout << "������� ������� ��� ���������� � ���������� ��������� ��� ����������" << endl;
		int pos, K;
		cin >> pos >> K;
		cin.get();									// ������������ ������ ��� ������ ��� ��������
		while (pos > n) { cout << "� ����� ����� " << n << "�������" << endl; cin >> pos; cin.get(); }
		add(pos, K, file);			// ���������� ��������� � ���� ������
		cout << endl;
		cout << "��� ����: " << endl;
		print(head);
		break;
	}
	case 3:
	{
		int pos;
		cout << "������� ������� ��� ��������" << endl;
		cin >> pos;
		while (pos > n) { cout << "� ������ ����� " << n << " �������" << endl; cin >> pos; }
		chop(pos, file, NULL);
		cout << endl;
		cout << "��� ����: " << endl;
		print(head);
		break;
	}
	case 4:
	{
		cout << "��� ����: " << endl;
		print(head);
		cout << endl;
		break;
	}
	case 5:
	{
		if ((file = fopen(filename, "wb+")) == NULL) exit(5);
		write(file, head);
		fclose(file);

		break;
	}
	case 6:
	{
		destroy();
		cout << "���� ������" << endl;
		flag1 = false;
		break;
	}

	case 7:
	{
		cout << "�������������� ����� �� �����" << endl;
		int n1 = n;
		if ((file = fopen(filename, "rb+")) == NULL) {
			if ((file = fopen(filename, "wb+")) == NULL) exit(6);
		}
		restore(file);
		fclose(file);
		if (n - n1 == 0) cout << "������ ���������������" << endl;
		else flag1 = true;
		break;
	}
	default:
	{	
		cout << "����� �� ����" << endl;
		destroy();
		exit(777);
	}
	}
}