/* Gregory.Stephen */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define N 1
#define MAX_LINE_CHARS 10
#define MAX_ADDRESSES 1000
#define FRAME_SIZE 256
#define PAGE_TABLE_SIZE 256
#define TLB_SIZE 16
#define PHYSICAL_MEMORY_SIZE 65536

// // Comment this line of code out when not running from VS Code's debugger
// #define VSCODE_DEBUG 1

/**
 * Single entry in a page table
 */
typedef struct pageTableEntry
{
    uint8_t address;
    int empty;
} pageTableEntry;

/**
 * Single entry in a TLB (Translation Lookaside Buffer)
 */
typedef struct TLBEntry
{
    uint8_t pageNumber;
    uint8_t frameNumber;
    int empty;
} TLBEntry;

void simulateVirtualMemory(uint16_t* addressList, int addressListLength, pageTableEntry* pageTable, TLBEntry* TLB, const char* backingStore, char* physicalMemory, int* numAddressReferences, int* numPageFaults, int* numTLBHits, int* tlbIndex);
void checkPageTable(pageTableEntry* pageTable, const char* backingStore, char* physicalMemory, const uint8_t pageNumber, int* nextFreeFrame, int* numPageFaults, uint8_t* frameNumber);
int checkTLB(TLBEntry* TLB, const uint8_t pageNumber, uint8_t* frameNumber, int* numTLBHits);
void addEntryToTLB(TLBEntry* TLB, uint8_t pageNumber, uint8_t frameNumber, int* tlbIndex);
void populateAddressList(uint16_t* addressList, FILE* fp, int* addressListLength);
double approxRollingAverage(double avg, double new_sample);
uint16_t getLower16Bits(int value);
uint16_t combinePageNumberAndOffset(uint8_t pageNumber, uint8_t pageOffset);
uint8_t extractPageNumber(uint16_t virtualAddress);
uint8_t extractPageOffset(uint16_t virtualAddress);
void printAddressList(uint16_t* addressList, int size);
void initializePageTable(pageTableEntry* pageTable, int size);
void initializeTLB(TLBEntry* TLB, int size);
void printPageInformation(uint16_t virtualAddress, uint16_t physicalAddress, signed int value);
void printStatistics(const int numAddressReferences, const int numPageFaults, const int numTLBHits);
void readPageFromBackingStore(uint8_t pageNumber, const char* backingStore, signed char* charBuffer);

/**
 * Reads a list of addresses from a file through the passed-in FILE pointer,
 * and adds all of them into the passed-in address_list.
 */
void populateAddressList(uint16_t* addressList, FILE* fp, int* addressListLength)
{
    int counter = 0;
    int number = 0;
    char addressLine[MAX_LINE_CHARS];

    // Read a line from the file into "addressLine" until EOF
    while (fgets(addressLine, MAX_LINE_CHARS, fp))
    {
        // Convert the char array in addressLine into an int in number
        sscanf(addressLine, "%d", &number);
        // Add the 16-bit address into the addressList
        addressList[counter] = getLower16Bits(number);
        counter++;
    }

    *addressListLength = counter;
}

/**
 * Calculate the approximate rolling average without
 * storing the total sum of items.
 */
double approxRollingAverage(double avg, double new_sample) 
{
    avg -= avg / N;
    avg += new_sample / N;

    return avg;
}

/**
 * Returns the lower 16 bits of a 32-bit integer
 */
uint16_t getLower16Bits(int value)
{
    return (value & 0x0000FFFF);
}

/**
 * Combines an 8-bit page number and an 8-bit page offset
 * to create a 16-bit address
 */
uint16_t combinePageNumberAndOffset(uint8_t pageNumber, uint8_t pageOffset)
{
    return (pageNumber << 8 ) | (pageOffset & 0xff);
}

/**
 * Gets the Page Number for a virtual address
 */
uint8_t extractPageNumber(uint16_t virtualAddress)
{
    return virtualAddress>>8;
}

/**
 * Gets the Page Offset for a virtual address
 */
uint8_t extractPageOffset(uint16_t virtualAddress)
{
    return (virtualAddress & 0x00FF);
}

void printAddressList(uint16_t* addressList, const int size)
{
    int i;
    for (i = 0; i < size; i++)
    {
        printf("addressList[%d] = %hu, ", i, addressList[i]);
        printf("Page Number: %d, ", extractPageNumber(addressList[i]));
        printf("Page Offset: %d\n", extractPageOffset(addressList[i]));
    }
}

/**
 * Initializes the page table with addresses set to 0 and "empty" set to 1 (true)
 */
void initializePageTable(pageTableEntry* pageTable, int size)
{
    int i;
    for (i = 0; i < size; i++)
    {
        pageTable[i].address = 0;
        pageTable[i].empty = 1;
    }
}

/**
 * Initializes the TLB with pageNumber and frameNumber set to 0 and "empty" set to 1 (true)
 */
void initializeTLB(TLBEntry* TLB, int size)
{
    int i;
    for (i = 0; i < size; i++)
    {
        TLB[i].pageNumber = 0;
        TLB[i].frameNumber = 0;
        TLB[i].empty = 1;
    }
}

/**
 * Checks if a page number is in the TLB, and modifies the passed in frameNumber
 * if it is found in the TLB
 * 
 * Returns 1 if Page Number is found in the TLB
 */
int checkTLB(TLBEntry* TLB, const uint8_t pageNumber, uint8_t* frameNumber, int* numTLBHits)
{
    // TODO: Possibly parallelize the following loop
    int i;
    for (i = 0; i < TLB_SIZE; i++)
    {
        if ((!TLB[i].empty) && (TLB[i].pageNumber == pageNumber))
        {
            *frameNumber = TLB[i].frameNumber;
            (*numTLBHits)++;
            return 1;
        } 
    }
    return 0;
}

/**
 * Adds an entry to the TLB (Translation Lookaside Buffer)
 * 
 * The replacement strategy for TLB entries is the FIFO 
 * (First-In-First-Out) strategy.
 */
void addEntryToTLB(TLBEntry* TLB, uint8_t pageNumber, uint8_t frameNumber, int* tlbIndex)
{
    TLBEntry *entry = &TLB[*tlbIndex % TLB_SIZE];
    (*tlbIndex)++;
    entry->pageNumber = pageNumber;
    entry->frameNumber = frameNumber;
    entry->empty = 0;
}

/**
 * Simulates virtual memory operation
 */
void simulateVirtualMemory(uint16_t* addressList, int addressListLength, pageTableEntry* pageTable, TLBEntry* TLB, const char* backingStore, char* physicalMemory, int* numAddressReferences, int* numPageFaults, int* numTLBHits, int* tlbIndex)
{
    int i;
    int nextFreeFrame = 0;
    for (i = 0; i < addressListLength; i++)
    {
        uint8_t pageNumber = extractPageNumber(addressList[i]);
        uint8_t pageOffset = extractPageOffset(addressList[i]);
        uint8_t frameNumber;

        // Check if page number is in the TLB
        if (!checkTLB(TLB, pageNumber, &frameNumber, numTLBHits))
        {
            // Get the frame number from the page table
            checkPageTable(pageTable, backingStore, physicalMemory, pageNumber, &nextFreeFrame, numPageFaults, &frameNumber);

            // Add entry to TLB
            addEntryToTLB(TLB, pageNumber, frameNumber, tlbIndex);
        }

        // Combine MSB (frame number) and LSB (offset) to get physicalAddress
        uint16_t physicalAddress = combinePageNumberAndOffset(frameNumber, pageOffset);

        // Get value from physical memory
        signed int value = physicalMemory[physicalAddress];

        // Print the page information
        printPageInformation(addressList[i], physicalAddress, value);

        // Update numAddressReferences
        (*numAddressReferences)++;
    }
}

/**
 * Looks at page table to find mapping from virtual
 * address to physical address.
 * 
 * This is done after any TLB miss.
 */
void checkPageTable(pageTableEntry* pageTable, const char* backingStore, char* physicalMemory, const uint8_t pageNumber, int* nextFreeFrame, int* numPageFaults, uint8_t* frameNumber)
{
    // If we have a page fault...
    if (pageTable[pageNumber].empty == 1)
    {
        // Get the page from the backing store
        signed char page[PAGE_TABLE_SIZE+1];
        readPageFromBackingStore(pageNumber, backingStore, page);

        // Store the page in the next free frame
        int i;
        int j = 0;
        for (i = *nextFreeFrame * FRAME_SIZE; i < (*nextFreeFrame * FRAME_SIZE) + FRAME_SIZE; i++)
        {
            physicalMemory[i] = page[j];
            j++;
        }

        // Update pageTable, nextFreeFrame, and numPageFaults
        pageTable[pageNumber].address = *nextFreeFrame;
        pageTable[pageNumber].empty = 0;
        (*nextFreeFrame)++;
        (*numPageFaults)++;
    }

    // Set the frame number equal to the frame number from the page table
    *frameNumber = pageTable[pageNumber].address;
}

/**
 * Prints the necessary information for a particular
 * virtual address, corresponding physical address, and 
 * the value stored at that physical address
 */
void printPageInformation(uint16_t virtualAddress, uint16_t physicalAddress, signed int value)
{
    printf("Virtual address: %d Physical address: %d, Value: %d\n", virtualAddress, physicalAddress, value);
}

void printStatistics(const int numAddressReferences, const int numPageFaults, const int numTLBHits)
{
    double pageFaultRate = (double)numPageFaults / numAddressReferences;
    double TLBHitRate = (double)numTLBHits / numAddressReferences;

    printf("Number of Translated Addresses = %d\n", numAddressReferences);
    printf("Page Faults = %d\n", numPageFaults);
    printf("Page Fault Rate = %.3f\n", pageFaultRate);
    printf("TLB Hits = %d\n", numTLBHits);
    printf("TLB Hit Rate = %.3f\n", TLBHitRate);
}

/**
 * Reads a page (PAGE_TABLE_SIZE bytes) from the backing store
 */
void readPageFromBackingStore(uint8_t pageNumber, const char* backingStore, signed char* charBuffer)
{
    // Open the file for reading ("rb" means "read" and "binary", respectively)
    FILE* fp = fopen(backingStore, "rb");

    // Moving the pointer "pageNumber * PAGE_TABLE_SIZE" places ahead of the start of the file
    fseek(fp, pageNumber * PAGE_TABLE_SIZE, SEEK_SET);

    // Read PAGE_TABLE_SIZE bytes from the file
    fread(charBuffer, 1, PAGE_TABLE_SIZE, fp);

    // Reset the pointer to the start of the file
    fseek(fp, 0, SEEK_SET);

    // Close the file
    fclose(fp);
}

int main(int argc, char** argv)
{
    FILE* fp;                                                   // Initialize file handler
    uint16_t addressList[MAX_ADDRESSES];                        // Initialize list of addresses
    int addressListLength = 0;                                  // Initialize length of address list
    pageTableEntry pageTable[PAGE_TABLE_SIZE];                  // Initialize page table
    initializePageTable(pageTable, PAGE_TABLE_SIZE);            // ...
    TLBEntry TLB[TLB_SIZE];                                     // Initialize TLB
    initializeTLB(TLB, TLB_SIZE);                               // ...
    int tlbIndex = 0;                                           // ...
    static const char backingStore[] = "./BACKING_STORE.bin";   // Initialize path to backing store
    char physicalMemory[PHYSICAL_MEMORY_SIZE];                  // Initialize (simulated) physical memory
    int numAddressReferences = 0;                               // Initialize statistics
    int numPageFaults = 0;                                      // ...
    int numTLBHits = 0;                                         // ...

    // Get the filename from the command line
#ifdef VSCODE_DEBUG
    char* filename = "addresses.txt";
#else
    char* filename = argv[1];
#endif

    // Open the file containing addresses
    fp = fopen(filename, "r");
    // Exit if the file could not be opened
    if (fp == NULL)
    {
        fprintf(stderr, "Could not open file \"%s\". Exiting now...\n", filename);
        return 1;
    }

    // Compile all of the addresses into an array
    populateAddressList(addressList, fp, &addressListLength);

    // Simulate the virtual memory operation
    simulateVirtualMemory(addressList, addressListLength, pageTable, TLB, backingStore, physicalMemory, &numAddressReferences, &numPageFaults, &numTLBHits, &tlbIndex);

    printStatistics(numAddressReferences, numPageFaults, numTLBHits);

    return 0;
}
