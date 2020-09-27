#include <stdio.h>
#include <stdarg.h>
#include <string.h>

void print_ints(int num, ...)
{
    va_list args;

    va_start(args, num);

    for (int i=0; i < num; i++)
    {
        int value = va_arg(args, int);
        printf("%d: %d\n", i, value);
    }

    va_end(args);
}

int my_printf(const char *format, ...)
{
    va_list args;
    int done;

    // Useless, but cool
    char* substring = strstr(format, "Stephen");
    if (substring != format) return 0;

    va_start(args, format);
    done = vprintf(format, args);
    va_end(args);

    return done;
}


int main()
{
    my_printf("Something we don't want to say.\n");
    my_printf("Stephen says, \"Hello World\"\n");
}
