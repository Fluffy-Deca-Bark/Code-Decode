#include <stdio.h>
#define LONG long long

void dump (void* p, int n)
{
	unsigned char* p1 = p;
	while (n--)
	{
		printf("%02x\n", *p1);
		p1++;
	};
};

int code (char* desc, void* v, FILE* f)
{
	if (f == NULL)
	{
		printf ("Erro de I/O.\n\n");
		return -1;
	};

	printf ("I/O ok.\n\n");
	//fputc (0xFF, f);
	fprintf (f, "%c", 0xFF);	// marca de início de uma estrutura

	int pos = 0;
	for (int i=0; desc[i] != '\0'; i++)
	{
		unsigned char chave = 0;
		
		if (desc[i+1] != '\0')
		{
			chave |= 0x80;		// bit 7 (cont) setado se há outros valores
		};

		if (desc[i] == 'i')
		{
			chave |= 0x01;		// tipo = 01 se int (32)
		}
		else
		{
			chave |= 0x02;		// tipo = 10 se long (64)
		};

		printf ("desc[%d] = %c  ---> chave = ", i, desc[i]);
		dump (&chave, 1);

		fprintf (f, "%c", chave);

		// Operações de zig-zag

		int n;
	   
		if (desc[i] == 'i')
			n = (int) *((int*)v+pos);
		else
			n = (LONG) *((LONG *)v+pos);

		printf ("v = \t\t\t%p\n", v);
		printf ("pos = \t\t\t%8x\n", pos);
		printf ("v+pos = \t%p\n", (v+pos));
		if (desc[i] == 'i')
			printf ("*((int*)v+pos)=\t%4d\n", (int) *((int*)v+pos));
		else
			printf ("*((long*)v+pos)=\t%4lld\n", (LONG) *((LONG*)v+pos));	// %4ld
		printf ("n = %x (oct)\t%d (dec)\n", n, n);

		unsigned int z = n << 1;
		if (n < 0)
			z = ~z;

		printf ("z = %x (oct)\t%d (dec)\n\n", z, z);


		if (desc[i] == 'i')
		{
			pos += 1;		
			// obs.: lidar com padding mais tarde
		}
		else
		{
			pos += 1;	
		};
	
		//for (int j=0; j<((desc[i]=='i')?4:8); j++)
		{
			
		};


	}; // for

	return 0;
}; // code

typedef struct MyStruct
{
	int i1;
	int i2;
	LONG I1;
	LONG I2;
	LONG I3;
} myStruct;

int main ()
{
	FILE* file;
	file = fopen ("/Users/Vinny/GitHub/Code-Decode/output.txt", "wb");

	char* str = "iiIII";
	myStruct m = { 1, -1, 256, -256 , 4294967299 };
	
	code (str, &m, file);

	printf ("sizeof(long) = %d\nsizeof(int) =  %d\n\n\n", (int) sizeof(LONG), (int) sizeof(int));
	fclose (file);

	return 0;
};
