/* Gregory.Stephen */

#include <stdio.h>
#include <stdint.h>

#define N 1
#define MAX_LINE_CHARS 10
#define MAX_ADDRESSES 1000
#define FRAME_SIZE 256
#define PAGE_TABLE_SIZE 256
#define PHYSICAL_MEMORY_SIZE 65536

int main(void)
{
    uint8_t pageNumber = 66;

    int startingPageAddress = pageNumber * 256;
    signed char charBuffer[256];

    // Open the file for reading ("rb" means "read" and "binary", respectively)
    FILE* fp = fopen("BACKING_STORE.bin", "rb");

    // Moving the pointer "pageNumber * PAGE_TABLE_SIZE" places ahead of the start of the file
    fseek(fp, startingPageAddress, SEEK_SET);

    // Read PAGE_TABLE_SIZE bytes from the file
    fread(&charBuffer, 1, PAGE_TABLE_SIZE, fp);

    int i;
    for (i = 0; i < PAGE_TABLE_SIZE; i++)
    {
        printf("charBuffer[%d] = %d\n", i, charBuffer[i]);
    }

    // Reset the pointer to the start of the file
    fseek(fp, 0, SEEK_SET);

    // Close the file
    fclose(fp);
}