#include <iostream>;
#include <conio.h>;
#include <fstream>;
#include <vector>;
#include  <iomanip>;
using namespace std;

// argc ������� ���-�� ����������, argv[0] - *.exe, argv[1] - *.png
int main(int argc, char* argv[1])
{
	setlocale(LC_ALL, "Russian");

	//��������� ������ ��������
	char* path = argv[1];

	//�������� ����� 
	ifstream picture;
	picture.open(path, ios::binary); //������ �� ��������� ����� �� ������..
	if (!picture.is_open()) // �������� ���������� �����
	{
		cout << "������! �������� �� �������.";
		_getch();
		return 0;
	}

	//������� "�������" ��������� � ������, ����� ����� �������� � ���, ��� ������ �� ��������
	int signatura[8] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};

	//������ ��� ��������� ��������
	int SignPic[8] = { 0 };

	//������������ �� �������� ������ 8 ������, ���������� ������� ��� ��������� ��������
	char SignPicRead[8];
	picture.read(SignPicRead, 8);
	for (int i(0); i < 8; i++)
	{
		SignPic[i] = SignPicRead[i];
	}

	//�����  ��������� ��������
	cout << "��������� ��������: ";
	int i = 0;
	while (i != 8)
	{


		//////////////////////////////////////////////////////////����������� ������ (int)(unsigned char)////////////////////////////////


		/*uppercase - ������� �������
		 setfill('0') - ������ ���� (� = 0�)
		 setw(2)  - �� ����, ����� ���, �� ��� ���� ���� ���� �� ������ (���� �������, ��� ����������, ��� ������ 0�, � �� 00� ��� 000�)
		 hex - ������� � 16-� �.�.*/
		cout << uppercase << setfill('0') << setw(2) << hex << (int)(unsigned char)SignPicRead[i];
		i++;
	}
	cout << endl;

	//��������� ����, ��� ����� ��������, � ���������� ����������. ����� ���������� ���������.
	int count(0); 
	for (int i(0); i < 8; i++)
	{
		if (SignPic[i] == signatura[i])
		{
			count++;
		}
	}
	if (count == 7)
		cout << "��������� ������" << endl << endl;
	else
		cout << "��������� �� ������" << endl << endl;

	//���������� ���������� ��������� ���� � ������, ���� ��������� ������. 
	if (count == 7)
	{
		//������ ��� ����� � ���� ������� ����� (Image Header = IHDR) 
		char PicLenghTypeIHDR[8];
		picture.read(PicLenghTypeIHDR, 8);

		//������ ��� ���� 1 �����
		int PicType[3] = { 0 };

		//���������� ������� ��� ���� 1 �����
		int j(0);
		for (int i(4); i < 8; i++)

		{
			PicType[j] = PicLenghTypeIHDR[i];
			j++;
		}

		//������ ��� ����������� �������� 1 ���� �����
		int PicStandartType[4] = {0x49, 0x48, 0x44, 0x52};

		cout << "����� 1 ����� ��������: ";
		int z = 0;
		while (z != 4)
		{
			cout << uppercase << setfill('0') << setw(2) << dec << (int)(unsigned char)PicLenghTypeIHDR[z];
			z++;
		}
		cout << endl;
		cout << "��� 1 ����� ��������: ";
		while (z != 8)
		{
			cout << uppercase  << setfill('0') << setw(2) << hex << (int)(unsigned char)PicLenghTypeIHDR[z];
			z++;
		}
		cout << endl;

		//��������� ����, ��� ����� ��������, � ���������� �����. ����� ���������� ���������.
		int count1(0);
		for (int i(0); i < 4; i++)
		{
			if (PicType[i] == PicStandartType[i])
			{
				count1++;
			}
		}
		if (count1 == 4)
			cout << "��� ������" << endl;
		else
			cout << "��� �� ������" << endl;

		//������� "��������" ������� IDAT: 13 ���� ������ � 4 ����� CRC
		char garbage[10000];
		picture.read(garbage, 17);
		cout << "CRC 1 ����� ��������: ";
		z = 13;
		while (z != 17)
		{
			cout << uppercase << setfill('0') << setw(2) << hex << (int)(unsigned char)garbage[z];
			z++;
		}
		cout << endl << endl;

		//����� IEND
		char PicEND[8];
		picture.seekg(-8, ios::end); //����������� "�������" � ����� ����� - 8
		picture.read(PicEND, 4);
		cout << "��� ���������� ����� ��������: ";
		z = 0;
		while (z != 4)
		{
			cout << uppercase << setfill('0') << setw(2) << hex << (int)(unsigned char)PicEND[z];
			z++;
		}
		cout << endl;

		//������ ��� ����������� �������� ���������� ���� �����
		int PicStandartTypeEND[4] = { 0x49, 0x45, 0x4E, 0x44 };

		//��������� ����, ��� ����� ��������, � ���������� �����. ����� ���������� ���������.
		count1 = 0;
		for (int i(0); i < 4; i++)
		{
			if (PicEND[i] == PicStandartTypeEND[i])
			{
				count1++;
			}
		}
		if (count1 == 4)
			cout << "��� ������" << endl << endl;
		else
			cout << "��� �� ������" << endl << endl;

		//����������� "�������" � ������ ����� �� ��������� +33 (��������� 8 ���� + IHDR 25 ����)
		int position = 33;
		picture.seekg(+position, ios::beg); 

		//�������� �������� � ����� IDAT � ���������� ��� ����� � CRC
		char PicStandartTypeIDAT[4] = {0x49, 0x44, 0x41, 0x54};
		char PicLengthTypeIDAT[8] = { 0 };
		char CRC[4] = { 0 };

		//����� IDAT (Image Data), ����� ���������� � ���
		bool flag = 0;
		while (!picture.eof()) 
		{
			//������ �������� ����� � �����
			picture.read(PicLengthTypeIDAT, 8);

			//��������� ����, ��� ����� ��������, � ���������� �����. ����� ���������� ���������.
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
			if (count == 4) //���� ��� ������������� IDAT, ������� ����������
			{
				flag = 1;
				cout << "IDAT ������ �� �������: " << uppercase << setfill('0') << setw(2) << dec << position << " �� ������ �����." << endl;
				cout << "����� �����: ";
				int q = 0;
				while (q != 4)
				{
					cout << uppercase << setfill('0') << setw(2) << hex << (int)(unsigned char)PicLengthTypeIDAT[q];
					q++;
				}
				cout << endl;
				cout << "��� �����: ";
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
				cout << "CRC ����� IDAT ����� ��������: ";
				z = 0;
				while (z != 4)
				{
					cout << uppercase << setfill('0') << setw(2) << hex << (int)(unsigned char)garbage[Length*16 + z];
					z++;
				}
				position = position + Length*16 + 12; // ����� �� ����� ������ + 4 ����� ����� + 4 ���� + 4 ���
				cout << endl << endl;
			}
		}
		if (flag == 0)
		{
			cout << "������ IDAT ���!";
		}
	}

	//�������� ����� 
	picture.close();

	_getch();
	return(0);
}
