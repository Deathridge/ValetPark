#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>



void f1(int x)
{
x = 2;
}
int f2(int x)
{
x = 2;
return x;
}
void go(void)
{
int x = 5;
int y = 5;
f1(x);
y = f2(x);
printf("x = %d, y = %d\n", x ,y);
}

int main()
{
	go();
}