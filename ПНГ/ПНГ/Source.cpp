#include <iostream>;
#include <conio.h>;
#include <fstream>;
#include <vector>;
#include  <iomanip>;
using namespace std;

// argc считает кол-во аргументов, argv[0] - *.exe, argv[1] - *.png
int main(int argc, char* argv[1])
{
	setlocale(LC_ALL, "Russian");

	//Получение адреса картинки
	char* path = argv[1];

	//Открытие файла 
	ifstream picture;
	picture.open(path, ios::binary); //чтение из бинарного файла по адресу..
	if (!picture.is_open()) // проверка открытости файла
	{
		cout << "Ошибка! Картинка не открыта.";
		_getch();
		return 0;
	}

	//Забиваю "хорошую" сигнатуру в массив, чтобы потом сравнить с тем, что получу от картинки
	int signatura[8] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};

	//Массив для сигнатуры картинки
	int SignPic[8] = { 0 };

	//Вытаскивание из картинки первых 8 байтов, заполнение массива для сигнатуры картинки
	char SignPicRead[8];
	picture.read(SignPicRead, 8);
	for (int i(0); i < 8; i++)
	{
		SignPic[i] = SignPicRead[i];
	}

	//Вывод  сигнатуры картинки
	cout << "Сигнатура картинки: ";
	int i = 0;
	while (i != 8)
	{


		//////////////////////////////////////////////////////////Разобраться почему (int)(unsigned char)////////////////////////////////


		/*uppercase - верхний регистр
		 setfill('0') - ставит нули (А = 0А)
		 setw(2)  - не знаю, зачем оно, но без него тоже нули не рисует (есть догадка, что показывает, что именно 0А, а не 00А или 000А)
		 hex - перевод в 16-ю с.с.*/
		cout << uppercase << setfill('0') << setw(2) << hex << (int)(unsigned char)SignPicRead[i];
		i++;
	}
	cout << endl;

	//Сравнение того, что имеет картинка, с правильной сигнатурой. Вывод результата сравнения.
	int count(0); 
	for (int i(0); i < 8; i++)
	{
		if (SignPic[i] == signatura[i])
		{
			count++;
		}
	}
	if (count == 7)
		cout << "Сигнатура верная" << endl << endl;
	else
		cout << "Сигнатура не верная" << endl << endl;

	//Дальнейшее выполнение программы лишь в случае, если сигнатура верная. 
	if (count == 7)
	{
		//Массив для длины и типа первого блока (Image Header = IHDR) 
		char PicLenghTypeIHDR[8];
		picture.read(PicLenghTypeIHDR, 8);

		//Массив для типа 1 блока
		int PicType[3] = { 0 };

		//Заполнение массива для типа 1 блока
		int j(0);
		for (int i(4); i < 8; i++)

		{
			PicType[j] = PicLenghTypeIHDR[i];
			j++;
		}

		//Массив для правильного названия 1 типа блока
		int PicStandartType[4] = {0x49, 0x48, 0x44, 0x52};

		cout << "Длина 1 блока картинки: ";
		int z = 0;
		while (z != 4)
		{
			cout << uppercase << setfill('0') << setw(2) << dec << (int)(unsigned char)PicLenghTypeIHDR[z];
			z++;
		}
		cout << endl;
		cout << "Тип 1 блока картинки: ";
		while (z != 8)
		{
			cout << uppercase  << setfill('0') << setw(2) << hex << (int)(unsigned char)PicLenghTypeIHDR[z];
			z++;
		}
		cout << endl;

		//Сравнение того, что имеет картинка, с правильным типом. Вывод результата сравнения.
		int count1(0);
		for (int i(0); i < 4; i++)
		{
			if (PicType[i] == PicStandartType[i])
			{
				count1++;
			}
		}
		if (count1 == 4)
			cout << "Тип верный" << endl;
		else
			cout << "Тип не верный" << endl;

		//Съедаем "курсором" остатки IDAT: 13 байт данных и 4 байта CRC
		char garbage[10000];
		picture.read(garbage, 17);
		cout << "CRC 1 блока картинки: ";
		z = 13;
		while (z != 17)
		{
			cout << uppercase << setfill('0') << setw(2) << hex << (int)(unsigned char)garbage[z];
			z++;
		}
		cout << endl << endl;

		//Поиск IEND
		char PicEND[8];
		picture.seekg(-8, ios::end); //Перемещение "курсора" в конец файла - 8
		picture.read(PicEND, 4);
		cout << "Тип последнего блока картинки: ";
		z = 0;
		while (z != 4)
		{
			cout << uppercase << setfill('0') << setw(2) << hex << (int)(unsigned char)PicEND[z];
			z++;
		}
		cout << endl;

		//Массив для правильного названия последнего типа блока
		int PicStandartTypeEND[4] = { 0x49, 0x45, 0x4E, 0x44 };

		//Сравнение того, что имеет картинка, с правильным типом. Вывод результата сравнения.
		count1 = 0;
		for (int i(0); i < 4; i++)
		{
			if (PicEND[i] == PicStandartTypeEND[i])
			{
				count1++;
			}
		}
		if (count1 == 4)
			cout << "Тип верный" << endl << endl;
		else
			cout << "Тип не верный" << endl << endl;

		//Перемещение "курсора" в начало файла со смещением +33 (Сигнатура 8 байт + IHDR 25 байт)
		int position = 33;
		picture.seekg(+position, ios::beg); 

		//Создание массивов с типом IDAT и переменных для длины и CRC
		char PicStandartTypeIDAT[4] = {0x49, 0x44, 0x41, 0x54};
		char PicLengthTypeIDAT[8] = { 0 };
		char CRC[4] = { 0 };

		//Поиск IDAT (Image Data), вывод информации о нем
		bool flag = 0;
		while (!picture.eof()) 
		{
			//Чтение названия блока и длины
			picture.read(PicLengthTypeIDAT, 8);

			//Сравнение того, что имеет картинка, с правильным типом. Вывод результата сравнения.
			count = 0;
			int j = 4;
			for (int i(0); i < 4; i++)
			{
				if (PicLengthTypeIDAT[j] == PicStandartTypeIDAT[i])
				{
					count++;
				}
				j++;
			}
			if (count == 4) //Если это действительно IDAT, выводим информацию
			{
				flag = 1;
				cout << "IDAT найден на позиции: " << uppercase << setfill('0') << setw(2) << dec << position << " от начала файла." << endl;
				cout << "Длина чанка: ";
				int q = 0;
				while (q != 4)
				{
					cout << uppercase << setfill('0') << setw(2) << hex << (int)(unsigned char)PicLengthTypeIDAT[q];
					q++;
				}
				cout << endl;
				cout << "Тип чанка: ";
				for (int i(4); i < 8; i++)
				{
					cout << uppercase << setfill('0') << setw(2) << hex << (int)(unsigned char) PicLengthTypeIDAT[i];
				}
				cout << endl;
				int Length = 0;
				for (int i(0); i < 4; i++)
				{
					Length = Length*16 + PicLengthTypeIDAT[i];
				}
				picture.read(garbage, Length*16+4);
				cout << "CRC этого IDAT блока картинки: ";
				z = 0;
				while (z != 4)
				{
					cout << uppercase << setfill('0') << setw(2) << hex << (int)(unsigned char)garbage[Length*16 + z];
					z++;
				}
				position = position + Length*16 + 12; // Сдвиг на длину данных + 4 байта длины + 4 типа + 4 СРС
				cout << endl << endl;
			}
		}
		if (flag == 0)
		{
			cout << "Блоков IDAT нет!";
		}
	}

	//Закрытие файла 
	picture.close();

	_getch();
	return(0);
}
