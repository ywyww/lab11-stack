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
bool flag1 = false;										// Стек создан

void create();											// Создает пустой стек
void fill(Student*);										// Заполняет стек значениями
void push(char*);										// Добавление элемента на последний
void pop(char*);										// Удаление последнего элемента и запись
void pop();											// Удаление последнего элемента
void print(Student*);										// Печать стека
void destroy();											// Уничтожение стека
void add(const int, const int, FILE*);								// Добавление по K элементов слева и справа
void pluralPush(const int);									// Множественное добавление элементов (для функции add)
void writePart(FILE*, const int, const int);							// Запись части стека в файл с удалением (для функции add);
void chop(const int, FILE*, char* data);							// Удаление элемента с позиции N, если незачем записывать, data = NULL
void menu(const int);										// Меню
void restore(FILE*);										// Восстановление стека из файла
void write(FILE*, Student*);									// Запись стека в файл (без удаления элементов

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	
	while (true) {
		system("pause");
		system("cls");
		cout << "Здравствуйте, что вы хотите сделать?" << endl;
		cout << "1. Создать стек" << endl;
		cout << "2. Добавить по K элементов к элементу на позиции pos слева и справа" << endl;
		cout << "3. Удалить элемент из стека" << endl;
		cout << "4. Напечатать стек" << endl;
		cout << "5. Записать стек в файл" << endl;
		cout << "6. Уничтожить стек" << endl;
		cout << "7. Восстановить стек из файла" << endl; cout << endl;
		cout << "Введите c" << endl;
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
void print(Student* point) {		// Рекурсивно пишет все из стека
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
		cout << "Идентификатор? " << endl;
		gets_s(tmp, 256);
		push(tmp);
		i++;
	}
}		
void writePart(FILE* file, const int num, const int pos) {		// работает через pop
	int a = num;
	if (a >= pos) {
		char buf[256];
		pop(buf);
		writePart(file, a - 1, pos);
		fwrite(&buf, sizeof(buf), 1, file);
	}
}	// Запись части стека в файл (для функции add)
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
	restore(file);	// доресторивает файл
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

void write(FILE* file, Student* ptrHead) {			// работает для меню
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

	if ((c == 1 || c == 7) && flag1) {					// для случая, когда список уже создан
		cout << "Вы уже создали стек" << endl;
		return;
	}
	else if (c > 1 && c < 7 && !flag1) {				// для случая, когда список еще не создан
		cout << "Создайте/ восстановите стек" << endl;
		return;
	}
	else if (c >= 2 && c <= 5 && n == 0) { cout << "Стэк пуст." << endl; return; }
	switch (c) {
	case 1:
	{
		flag1 = true;
		cout << "Введите количество узлов стека: ";
		cin >> n;
		while (n <= 0) {
			cout << "Нельзя ввести столько узлов" << endl;
			cin >> n;
		}
		create();
		cout << "Введите индетификатор для " << n << " пользователей: " << endl; cin.ignore();
		fill(head);
		break;
	}
	case 2:
	{
		cout << "Введите позицию для добавления и количество элементов для добавления" << endl;
		int pos, K;
		cin >> pos >> K;
		cin.get();									// нормализация буфера для работы без перебоев
		while (pos > n) { cout << "В стэке всего " << n << "позиций" << endl; cin >> pos; cin.get(); }
		add(pos, K, file);			// добавление элементов с двух сторон
		cout << endl;
		cout << "Ваш стек: " << endl;
		print(head);
		break;
	}
	case 3:
	{
		int pos;
		cout << "Введите позицию для удаления" << endl;
		cin >> pos;
		while (pos > n) { cout << "В списке всего " << n << " позиций" << endl; cin >> pos; }
		chop(pos, file, NULL);
		cout << endl;
		cout << "Ваш стек: " << endl;
		print(head);
		break;
	}
	case 4:
	{
		cout << "Ваш стек: " << endl;
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
		cout << "Стэк удален" << endl;
		flag1 = false;
		break;
	}

	case 7:
	{
		cout << "Восстановление стека из файла" << endl;
		int n1 = n;
		if ((file = fopen(filename, "rb+")) == NULL) {
			if ((file = fopen(filename, "wb+")) == NULL) exit(6);
		}
		restore(file);
		fclose(file);
		if (n - n1 == 0) cout << "Нечего восстанавливать" << endl;
		else flag1 = true;
		break;
	}
	default:
	{	
		cout << "Выход из меню" << endl;
		destroy();
		exit(777);
	}
	}
}
