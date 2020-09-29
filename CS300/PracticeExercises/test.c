#include <stdio.h>

void infinite_recursion(int x)
{
    printf("x = %d\n", x);
    infinite_recursion(x+1);
}

int main() 
{
    int x = 10;

    infinite_recursion(x);
}