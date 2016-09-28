// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	MemLen = (len + 31) >> 5;
	pMem = new unsigned int[MemLen];
	for (int i = 0; i < MemLen; ++i)
		pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new unsigned int[MemLen];
	for (int i = 0; i<MemLen; ++i)
		pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
	delete[]pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return n << 5;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	return 1 << (n & 31);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if ((n > -1) && (n < BitLen))
		pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if ((n > -1) && (n < BitLen))
		pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if ((n > -1) && (n < BitLen))
		return pMem[GetMemIndex(n)] & GetMemMask(n);
	return 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	BitLen = bf.BitLen;
	if (MemLen != bf.MemLen){
		MemLen = bf.MemLen;
		if (pMem != NULL)
			delete[]pMem;
		pMem = new unsigned int[MemLen];
	}
	for (int i = 0; i < MemLen; ++i)
		pMem[i] = bf.pMem[i];
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	bool flag = true;
	int min = MemLen > bf.MemLen ? bf.MemLen : MemLen;
	for (int i = 0; i < min; ++i)
	if (pMem[i] != bf.pMem[i]){
		flag = false;
		break;
	}
	if (MemLen > bf.MemLen){
		for (int i = bf.MemLen; i < MemLen; ++i)
		if (bf.pMem[i] != 0){
			flag = false;
			break;
		}
	}
	else
	for (int i = MemLen; i < bf.MemLen; ++i)
	if (bf.pMem[i] != 0){
		flag = false;
		break;
	}
	return flag;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	bool flag = true;
	int min = MemLen > bf.MemLen ? bf.MemLen : MemLen;
	for (int i = 0; i < min; ++i)
	if (pMem[i] != bf.pMem[i]){
		flag = true;
		break;
	}
	return flag;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int len = BitLen > bf.BitLen ? BitLen : bf.BitLen;
	TBitField tmp(len);
	for (int i = 0; i < tmp.MemLen; ++i)
		tmp.pMem[i] = bf.pMem[i];
	int min = MemLen > tmp.MemLen ? tmp.MemLen : MemLen;
	for (int i = 0; i < min; ++i)
		tmp.pMem[i] |= pMem[i];
	return tmp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int len = BitLen > bf.BitLen ? BitLen : bf.BitLen;
	TBitField tmp(len);
	int min = MemLen > tmp.MemLen ? tmp.MemLen : MemLen;
	for (int i = 0; i < min; ++i)
		tmp.pMem[i] = bf.pMem[i];
	for (int i = 0; i < min; ++i)
		tmp.pMem[i] &= pMem[i];
	for (int i = min; i < tmp.MemLen; ++i)
		tmp.pMem[i] = 0;
	return tmp;
}

TBitField TBitField::operator~(void) // отрицание
{
	int len = BitLen;
	TBitField tmp(len);
	for (int i = 0; i < MemLen; ++i)
		tmp.pMem[i] = ~pMem[i];
	return tmp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	int i = 0;
	char ch;
	do	istream >> ch;
	while (ch == ' ');// !=
	while (1)
	if (ch == '0')
		bf.ClrBit(++i);
	else
	if (ch == '1')
		bf.SetBit(++i);
	else
		break;
	return istream;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	int len = bf.BitLen;
	for (int i = 0; i < len; ++i){
		if (!(i % 8))
			ostream << endl;
		if (bf.GetBit(i))
			ostream << '1';
		else
			ostream << '0';
	}
	return ostream;
}
