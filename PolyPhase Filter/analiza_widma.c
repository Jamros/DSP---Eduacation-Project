#include <std.h>
#include "aNaliza_widmacfg.h"
#include "utility.c"
#include "utility.h"
#include <math.h>
#include "filtr_5khz.h"

#define BufSize 1024

short index =0;

typedef union{
	int Probka;
	short LSB;
}SAMPLE;
SAMPLE MySample;


short bufferInput[BufSize];
short bufferOutput[BufSize];
float output=0;
short i;

float buffor;

int main()
{
// zerowaNie tablicy
for(i=0;i<BufSize;i++)	
{
	bufferInput[i]=0;
	bufferOutput[i]=0;
}

//wlaczeNie RX i TX
MCBSP_enableRcv(hMcbsp1);
MCBSP_enableXmt(hMcbsp1);

//wlaczeNie przerwaN
IER |= 0x2000;
}


void interrupt_rcv()
{
static int N_Int = 0;
MySample.Probka=MCBSP_read(hMcbsp1);	
bufferInput[index]=MySample.LSB;


if (!(N_Int%(D+1)))
{	
	for(i=0;i<Fitler_Lenght;i = i + D)
	{
		buffor+=bufferInput[(index-i)&(BufSize-1)] * Corr_Filter[i];
	}
	bufferOutput[index] = buffor * D;
	N_Int = 1;	
}
else
{
	bufferOutput[index] = 0;
}


MCBSP_write(hMcbsp1,bufferOutput[index]);		//wyslaNie Na przetworNik DAC wartosci pobraNej
buffor = 0;
index = ++index & BufSize-1;
N_Int++;
}
