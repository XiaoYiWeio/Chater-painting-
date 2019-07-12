#pragma comment(lib,"winmm.lib")
#include "PicReader.h"
#include "FastPrinter.h"
#include <stdio.h>
#include<iostream>

using namespace std;


class Array
{
private:
	BYTE *pic;			//存储像素值、灰度值、映射后的ASCII码
public:
	int size_x;			//矩阵规模
	int size_y;
	int size_z;
	Array(int a)		//构造函数：一维
	{
		pic = new BYTE[a];

		size_x = a;
		size_y = 0;
		size_z = 0;
	}
	Array(int a, int b)		//构造函数：二维
	{
		pic = new BYTE[a*b];

		size_x = a;
		size_y = b;
		size_z = 0;
	}
	Array(int x, int y, int z)	//构造函数：三维
	{
		pic = new BYTE[x*y*z];

		size_x = x;
		size_y = y;
		size_z = z;
	}
	Array(BYTE *, unsigned int x, unsigned int y);		//构造函数：传入图像像素值和图像长宽并直接计算各像素灰度值
	int at(int x, int y)							//二维访问
	{
		int temp = x * size_x + y;
		return temp;
	}
	int at(int x, int y, int z)						//三维访问
	{
		int temp = x * size_x + y * size_y + z;
		return temp;
	}

	void print();									//打印图片
	void print_player(FastPrinter &);				//针对字符画视频的打印图片 

	void to_ascii();					//灰度值转ASCII码			
	void to_ascii_player();				//针对字符画视频的灰度值转ASCII码	
	void add(Array &b);					//矩阵相加
	void sub(Array &b);					//矩阵相减
	void mul(Array &b);					//矩阵点乘
	void div(Array &b);					//矩阵点除
	void add(int a);
	void sub(int b);
	void mul(int a);
	void div(int b);
	void reshape(int a, int b, int c);		//reshape操作
	BYTE* get_content();				//返回指针并指向矩阵元数据
};
void Array::to_ascii()
{
	char asciiStrength[] = { 'M','N','H','Q','$','O','C','?','7','>','!',':','-',';','.' };
	if (size_x <= 100)
	{
		BYTE  *p = pic;
		//BYTE temp;
		for (int i = 0; i < size_x * size_y; i++)
		{
			*(p) = asciiStrength[(*p) / 18];
			p++;
		}
	}
	else  if (size_x < 400)
	{
		/*
		if (x >= 400)
			size_x = x / 2;
		if (y > 400)
			size_y = y / 2;
			*/

		BYTE gray[4];
		BYTE *p2, *head;
		BYTE temp;
		head = new BYTE[size_x*size_y];
		p2 = head;
		for (int i = 0; i < size_y - 1; i += 2)
		{
			for (int j = 0; j < size_x - 1; j += 2)
			{
				gray[0] = pic[this->at(i, j)];
				gray[1] = pic[this->at(i, j + 1)];
				gray[2] = pic[this->at(i + 1, j)];
				gray[3] = pic[this->at(i + 1, j + 1)];
				temp = (gray[0] + gray[1] + gray[2] + gray[3]) / (4);
				*(p2++) = asciiStrength[temp / 18];
			}
		}
		size_x /= 2;
		size_y /= 2;
		delete[]pic;
		pic = head;
	}
	else
	{
		BYTE gray[16];
		BYTE *p2, *head;
		BYTE temp;
		int m = 0;
		head = new BYTE[size_x*size_y];
		p2 = head;
		for (int i = 0; i < size_y - 3; i += 4)
		{
			for (int j = 0; j < size_x - 3; j += 4)
			{
				m = 0;
				gray[0] = pic[this->at(i, j)];
				gray[1] = pic[this->at(i, j + 1)];
				gray[2] = pic[this->at(i, j + 2)];
				gray[3] = pic[this->at(i + 1, j)];
				gray[4] = pic[this->at(i + 1, j + 1)];
				gray[5] = pic[this->at(i + 1, j + 2)];
				gray[6] = pic[this->at(i + 2, j)];
				gray[7] = pic[this->at(i + 2, j + 1)];
				gray[8] = pic[this->at(i + 2, j + 2)];
				gray[9] = pic[this->at(i, j + 3)];
				gray[10] = pic[this->at(i + 1, j + 3)];
				gray[11] = pic[this->at(i + 2, j + 3)];
				gray[12] = pic[this->at(i + 3, j + 3)];
				gray[13] = pic[this->at(i + 3, j)];
				gray[14] = pic[this->at(i + 3, j + 1)];
				gray[15] = pic[this->at(i + 3, j + 2)];
				for (int k = 0; k < 16; k++)
					m += gray[k];
				temp = m / 16;
				*(p2++) = asciiStrength[temp / 18];
			}
		}
		size_x /= 4;
		size_y /= 4;
		delete[]pic;
		pic = head;
	}
}
void Array::to_ascii_player()
{
	char asciiStrength[] = { 'M','N','H','Q','$','O','C','?','7','>','!',':','-',';','.' };
	BYTE gray[4];
	BYTE *p2, *head;
	BYTE temp;
	head = new BYTE[size_x*size_y];
	p2 = head;
	for (int i = 0; i < size_y - 1; i += 2)
	{
		for (int j = 0; j < size_x - 1; j += 2)
		{
			gray[0] = pic[this->at(i, j)];
			gray[1] = pic[this->at(i, j + 1)];
			gray[2] = pic[this->at(i + 1, j)];
			gray[3] = pic[this->at(i + 1, j + 1)];
			temp = (gray[0] + gray[1] + gray[2] + gray[3]) / (4);
			*(p2++) = asciiStrength[temp / 18];
		}
	}
	size_x /= 2;
	size_y /= 2;
	delete[]pic;
	pic = head;
}
Array::Array(BYTE *data, unsigned int x, unsigned int y)			//存储灰度值
{
	size_x = x;
	size_y = y;
	pic = new BYTE[x*y];

	BYTE gray, *p = pic;
	BYTE temp;
	for (DWORD i = 0; i < x * y * 4; i += 4)
	{
		gray = (data[i] * 299 + data[i + 1] * 587 + data[i + 2] * 114 + 500) / 1000;
		temp = gray;
		*(p++) = temp;
	}
	delete[] data;
	data = nullptr;
}
void Array::add(Array &b)
{

	if (size_x != b.size_x || size_y != b.size_y || size_z != b.size_z)
	{
		cout << "不可相加！\n";
		return;
	}
	else
	{
		int dim = 0;
		dim = (size_x != 0) + (size_y != 0) + (size_z != 0);
		switch (dim)
		{
		case 1:
			for (int i = 0; i < size_x; i++)
			{
				pic[i] += b.pic[i];
			}
			break;
		case 2:
			for (int i = 0; i < size_x; i++)
				for (int j = 0; j < size_y; j++)
					pic[this->at(i, j)] += b.pic[b.at(i, j)];
			break;
		case 3:
			for (int i = 0; i < size_x; i++)
				for (int j = 0; j < size_y; j++)
					for (int k = 0; k < size_z; k++)
						pic[this->at(i, j, k)] += b.pic[b.at(i, j, k)];
			break;
		default:break;
		}
	}

}
void Array::add(int n)
{
	int dim = 0;
	dim = (size_x != 0) + (size_y != 0) + (size_z != 0);
	switch (dim)
	{
	case 1:
		for (int i = 0; i < size_x; i++)
		{
			pic[i] += n;
		}
		break;
	case 2:
		for (int i = 0; i < size_x; i++)
			for (int j = 0; j < size_y; j++)
				pic[this->at(i, j)] += n;
		break;
	case 3:
		for (int i = 0; i < size_x; i++)
			for (int j = 0; j < size_y; j++)
				for (int k = 0; k < size_z; k++)
					pic[this->at(i, j, k)] += n;
		break;
	default:break;
	}
}

void Array::sub(Array &b)
{

	if (size_x != b.size_x || size_y != b.size_y || size_z != b.size_z)
	{
		cout << "不可相加！\n";
		return;
	}
	else
	{
		int dim = 0;
		dim = (size_x != 0) + (size_y != 0) + (size_z != 0);
		switch (dim)
		{
		case 1:
			for (int i = 0; i < size_x; i++)
			{
				pic[i] -= b.pic[i];
			}
			break;
		case 2:
			for (int i = 0; i < size_x; i++)
				for (int j = 0; j < size_y; j++)
					pic[this->at(i, j)] -= b.pic[b.at(i, j)];
			break;
		case 3:
			for (int i = 0; i < size_x; i++)
				for (int j = 0; j < size_y; j++)
					for (int k = 0; k < size_z; k++)
						pic[this->at(i, j, k)] -= b.pic[b.at(i, j, k)];
			break;
		default:break;
		}
	}

}
void Array::sub(int n)
{
	int dim = 0;
	dim = (size_x != 0) + (size_y != 0) + (size_z != 0);
	switch (dim)
	{
	case 1:
		for (int i = 0; i < size_x; i++)
		{
			pic[i] -= n;
		}
		break;
	case 2:
		for (int i = 0; i < size_x; i++)
			for (int j = 0; j < size_y; j++)
				pic[this->at(i, j)] -= n;
		break;
	case 3:
		for (int i = 0; i < size_x; i++)
			for (int j = 0; j < size_y; j++)
				for (int k = 0; k < size_z; k++)
					pic[this->at(i, j, k)] -= n;
		break;
	default:break;
	}
}

void Array::mul(Array &b)
{

	if (size_x != b.size_x || size_y != b.size_y || size_z != b.size_z)
	{
		cout << "不可相加！\n";
		return;
	}
	else
	{
		int dim = 0;
		dim = (size_x != 0) + (size_y != 0) + (size_z != 0);
		switch (dim)
		{
		case 1:
			for (int i = 0; i < size_x; i++)
			{
				pic[i] *= b.pic[i];
			}
			break;
		case 2:
			for (int i = 0; i < size_x; i++)
				for (int j = 0; j < size_y; j++)
					pic[this->at(i, j)] *= b.pic[b.at(i, j)];
			break;
		case 3:
			for (int i = 0; i < size_x; i++)
				for (int j = 0; j < size_y; j++)
					for (int k = 0; k < size_z; k++)
						pic[this->at(i, j, k)] *= b.pic[b.at(i, j, k)];
			break;
		default:break;
		}
	}

}
void Array::mul(int n)
{
	int dim = 0;
	dim = (size_x != 0) + (size_y != 0) + (size_z != 0);
	switch (dim)
	{
	case 1:
		for (int i = 0; i < size_x; i++)
		{
			pic[i] *= n;
		}
		break;
	case 2:
		for (int i = 0; i < size_x; i++)
			for (int j = 0; j < size_y; j++)
				pic[this->at(i, j)] *= n;
		break;
	case 3:
		for (int i = 0; i < size_x; i++)
			for (int j = 0; j < size_y; j++)
				for (int k = 0; k < size_z; k++)
					pic[this->at(i, j, k)] *= n;
		break;
	default:break;
	}
}

void Array::div(Array &b)
{

	if (size_x != b.size_x || size_y != b.size_y || size_z != b.size_z)
	{
		cout << "不可相加！\n";
		return;
	}
	else
	{
		int dim = 0;
		dim = (size_x != 0) + (size_y != 0) + (size_z != 0);
		switch (dim)
		{
		case 1:
			for (int i = 0; i < size_x; i++)
			{
				pic[i] /= b.pic[i];
			}
			break;
		case 2:
			for (int i = 0; i < size_x; i++)
				for (int j = 0; j < size_y; j++)
					pic[this->at(i, j)] /= b.pic[b.at(i, j)];
			break;
		case 3:
			for (int i = 0; i < size_x; i++)
				for (int j = 0; j < size_y; j++)
					for (int k = 0; k < size_z; k++)
						pic[this->at(i, j, k)] /= b.pic[b.at(i, j, k)];
			break;
		default:break;
		}
	}

}
void Array::div(int n)
{
	int dim = 0;
	dim = (size_x != 0) + (size_y != 0) + (size_z != 0);
	switch (dim)
	{
	case 1:
		for (int i = 0; i < size_x; i++)
		{
			pic[i] /= n;
		}
		break;
	case 2:
		for (int i = 0; i < size_x; i++)
			for (int j = 0; j < size_y; j++)
				pic[this->at(i, j)] /= n;
		break;
	case 3:
		for (int i = 0; i < size_x; i++)
			for (int j = 0; j < size_y; j++)
				for (int k = 0; k < size_z; k++)
					pic[this->at(i, j, k)] /= n;
		break;
	default:break;
	}
}

void Array::reshape(int a = 0, int b = 0, int c = 0)
{
	size_x = a;
	size_y = b;
	size_z = c;
}

BYTE* Array::get_content()
{
	return pic;
}
void Array::print() {
	WORD t = 1;
	FastPrinter printer(size_x * 2, size_y, t);
	COORD textXY;
	char ch[3];

	printer.cleanSrceen();
	for (int i = 0; i < size_y; i++)
	{
		for (int j = 0; j < size_x; j += 1)
		{
			ch[0] = pic[this->at(i, j)];
			ch[1] = ch[0];
			ch[2] = '\0';
			textXY.X = 2 * j;
			textXY.Y = i;
			printer.setText(textXY, ch, (fp_color::f_black | fp_color::b_l_white), 2);
		}
	}
	printer.draw(true);
	getchar();
}

void Array::print_player(FastPrinter &printer) {

	char ch[3];
	COORD textXY;
	printer.cleanSrceen();
	for (int i = 0; i < size_y; i++)
	{
		for (int j = 0; j < size_x; j += 1)
		{
			ch[0] = pic[this->at(i, j)];
			ch[1] = ch[0];
			ch[2] = '\0';
			textXY.X = 2 * j;
			textXY.Y = i;
			printer.setText(textXY, ch, (fp_color::f_black | fp_color::b_l_white), 2);
			//cout << ch<<ch ;
		}
	}
	printer.draw(true);
}
void getnum(int num, char temp[])
{

	temp[2] = num % 10 + '0';
	num /= 10;
	temp[1] = num % 10 + '0';
	num /= 10;
	temp[0] = num % 10 + '0';

}

void changename(char str[])
{
	char temp[3];
	int k = 0, num = 0, p = 0;
	while (str[k] != '\0')
	{
		if (str[k] <= '9'&&str[k] >= '0')
		{
			num *= 10;
			num += str[k] - '0';
		}
		k++;
	}						//已读取当前值
	num++;
	getnum(num, temp);
	k = 0;
	while (str[k] != '\0')
	{
		if (str[k] <= '9'&&str[k] >= '0')
		{
			str[k] = temp[p++];
		}
		k++;
	}

}

void player()
{

	PicReader imread;
	char picname[30] = "kunkun\\kunkun 001.jpg";
	BYTE *data = nullptr;
	UINT x, y;


	imread.readPic(picname);
	imread.testReader(data, x, y);


	Array ima(data, x, y);

	ima.to_ascii_player();

	WORD t = 1;
	FastPrinter printer(ima.size_x * 2, ima.size_y, t);
	PlaySoundA("kunkun\\2.wav", NULL, SND_FILENAME | SND_ASYNC);
	ima.print_player(printer);
	changename(picname);

	//Sleep(3);
	for (int i = 1; i < 152; i++)
	{
		imread.readPic(picname);
		imread.testReader(data, x, y);


		Array ima(data, x, y);

		ima.to_ascii_player();

		ima.print_player(printer);
		changename(picname);

	}
}
int main()
{
	
	PicReader imread;
	BYTE *data = nullptr;
	UINT x, y;
	
	imread.readPic("classic_picture\\1.png");
	imread.testReader(data, x, y);

	Array ima(data, x, y);
	ima.to_ascii();
	ima.print();
	getchar();
	
	player();			//5秒舞蹈欣赏
	return 0;
}


