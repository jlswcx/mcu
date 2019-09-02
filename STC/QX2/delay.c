#include "intrins.h"

void Delay1us()		//@24.000MHz
{
	unsigned char i;

	i = 6;
	while (--i);
}


void Delay20us()		//@24.000MHz
{
	unsigned char i;

	i = 158;
	while (--i);
}

void Delay50us()		//@24.000MHz
{
	unsigned char i, j;

	_nop_();
	i = 2;
	j = 140;
	do
	{
		while (--j);
	} while (--i);
}


void Delay200us()		//@24.000MHz
{
	unsigned char i, j;

	i = 7;
	j = 57;
	do
	{
		while (--j);
	} while (--i);
}

void Delay300us()		//@24.000MHz
{
	unsigned char i, j;

	i = 10;
	j = 87;
	do
	{
		while (--j);
	} while (--i);
}



void Delay500us()		//@24.000MHz
{
	unsigned char i, j;

	i = 16;
	j = 147;
	do
	{
		while (--j);
	} while (--i);
}
void Delay1000us()		//@24.000MHz
{
	unsigned char i, j;

	_nop_();
	i = 32;
	j = 40;
	do
	{
		while (--j);
	} while (--i);
}

void Delay2000us()		//@24.000MHz
{
	unsigned char i, j;

	_nop_();
	_nop_();
	i = 63;
	j = 83;
	do
	{
		while (--j);
	} while (--i);
}



void Delay19800us()		//@24.000MHz
{
	unsigned char i, j, k;

	_nop_();
	i = 3;
	j = 106;
	k = 31;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}




void Delay5ms()		//@24.000MHz
{
	unsigned char i, j;

	_nop_();
	_nop_();
	i = 156;
	j = 213;
	do
	{
		while (--j);
	} while (--i);
}
void Delay8ms()		//@24.000MHz
{
	unsigned char i, j;

	i = 250;
	j = 87;
	do
	{
		while (--j);
	} while (--i);
}
void Delay9ms()		//@24.000MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 2;
	j = 25;
	k = 128;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void Delay10ms()		//@24.000MHz
{
	unsigned char i, j, k;

	i = 2;
	j = 56;
	k = 172;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}


void Delay20ms()		//@24.000MHz
{
	unsigned char i, j, k;

	_nop_();
	i = 3;
	j = 112;
	k = 91;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void Delay30ms()		//@24.000MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 4;
	j = 168;
	k = 10;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void Delay50ms()		//@24.000MHz
{
	unsigned char i, j, k;

	_nop_();
	i = 7;
	j = 23;
	k = 105;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void Delay100ms()		//@24.000MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 13;
	j = 45;
	k = 214;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void Delay200ms()		//@24.000MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 25;
	j = 90;
	k = 176;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void Delay1000ms()		//@24.000MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 122;
	j = 193;
	k = 128;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}
