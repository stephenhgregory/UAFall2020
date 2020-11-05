/* Gregory.Stephen */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

#define N 1
#define MAX_LINE_CHARS 10
#define MAX_ADDRESSES 1000
#define FRAME_SIZE 256
#define PAGE_TABLE_SIZE 256
#define TLB_SIZE 16
#define PHYSICAL_MEMORY_SIZE 32768

/**
 * Single entry in a page table
 * 
 * Keeps track of time since entry into page table
 * for LRU replacement.
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

/**
 * Single frame in memory
 */
typedef struct pageFrame
{
    int free;
    int lastUpdate;
} pageFrame;

void simulateVirtualMemory(uint16_t* addressList, int addressListLength, pageTableEntry* pageTable, TLBEntry* TLB, const char* backingStore, char* physicalMemory, pageFrame* freeFrames, const int freeFramesSize, int* numAddressReferences, int* numPageFaults, int* numTLBHits, int* tlbIndex);
void checkPageTable(pageTableEntry* pageTable, TLBEntry* TLB, const char* backingStore, char* physicalMemory, const uint8_t pageNumber, pageFrame* freeFrames, const int freeFramesSize, int* numPageFaults, uint8_t* frameNumber, const int counter);
void storePageInNextFreeFrame(pageTableEntry* pageTable, TLBEntry* TLB, const uint8_t pageNumber, char* physicalMemory, signed char* page, pageFrame* freeFrames, const int freeFramesSize, const int counter);
void storePage(char* physicalMemory, signed char* page, int index);
uint8_t findVictimPage(pageTableEntry* pageTable);
uint8_t findVictimFrame(pageTableEntry* pageTable, pageFrame* freeFrames, int numFreeFrames);
int checkTLB(TLBEntry* TLB, const uint8_t pageNumber, uint8_t* frameNumber, int* numTLBHits);
void addEntryToTLB(TLBEntry* TLB, uint8_t pageNumber, uint8_t frameNumber, int* tlbIndex);
int removeEntryFromTLB(TLBEntry* TLB, const uint8_t pageNumber);
void populateAddressList(uint16_t* addressList, FILE* fp, int* addressListLength);
uint16_t getLower16Bits(int value);
uint16_t combinePageNumberAndOffset(uint8_t pageNumber, uint8_t pageOffset);
uint8_t extractPageNumber(uint16_t virtualAddress);
uint8_t extractPageOffset(uint16_t virtualAddress);
void initializePageTable(pageTableEntry* pageTable, int size);
void initializeTLB(TLBEntry* TLB, int size);
void initializeFreeFramesList(pageFrame* freeFramesList, int size);
void printPageInformation(uint16_t virtualAddress, uint16_t physicalAddress, signed int value);
void printStatistics(const int numAddressReferences, const int numPageFaults, const int numTLBHits);
void readPageFromBackingStore(uint8_t pageNumber, const char* backingStore, signed char* charBuffer);

/**
 * Reads a list of addresses from a file through the passed-in FILE pointer,
 * and adds all of them into the passed-in address_list.
 * 
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
 * Returns the lower 16 bits of a 32 bit integer
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
 * Initializes the free frames list.
 * 
 * The "free" field of each freeFrame is set to 1 (signifying the frame is free).
 * The "lastUpdate" field of each freeFrame is set to INT_MAX
 */
void initializeFreeFramesList(pageFrame* freeFramesList, int size)
{
    int i;
    for (i = 0; i < size; i++)
    {
        freeFramesList[i].free = 1;
        freeFramesList[i].lastUpdate = INT_MAX;
    }
}

/**
 * Checks if a page number is in the TLB, and modifies the passed in frameNumber
 * if it is found in the TLB
 * 
 * Returns 1 if Page Number IS found in the TLB
 * Returns 0 if Page Number IS NOT found in the TLB
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
 * Checks if a page number is in the TLB, and if so, removes that entry from the TLB
 * 
 * Returns 1 if Page Number to be removed WAS found in the TLB
 * Returns 0 if Page Number to be removed WAS NOT found in the TLB
 */
int removeEntryFromTLB(TLBEntry* TLB, const uint8_t pageNumber)
{
    // TODO: Possibly parallelize the following loop
    int i;
    for (i = 0; i < TLB_SIZE; i++)
    {
        if ((!TLB[i].empty) && (TLB[i].pageNumber == pageNumber))
        {
            TLB[i].empty = 1;
            TLB[i].pageNumber = -1;
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
void simulateVirtualMemory(uint16_t* addressList, int addressListLength, pageTableEntry* pageTable, TLBEntry* TLB, const char* backingStore, char* physicalMemory, pageFrame* freeFrames, const int freeFramesSize, int* numAddressReferences, int* numPageFaults, int* numTLBHits, int* tlbIndex)
{
    int i;
    for (i = 0; i < addressListLength; i++)
    {
        uint8_t pageNumber = extractPageNumber(addressList[i]);
        uint8_t pageOffset = extractPageOffset(addressList[i]);
        uint8_t frameNumber;

        // Check if page number is in the TLB
        if (!checkTLB(TLB, pageNumber, &frameNumber, numTLBHits))
        {
            // Get the frame number from the page table
            checkPageTable(pageTable, TLB, backingStore, physicalMemory, pageNumber, freeFrames, freeFramesSize, numPageFaults, &frameNumber, i);

            // Add entry to TLB
            addEntryToTLB(TLB, pageNumber, frameNumber, tlbIndex);
        }

        // Combine MSB (frame number) and LSB (offset) to get physicalAddress
        uint16_t physicalAddress = combinePageNumberAndOffset(frameNumber, pageOffset);

        // Get value from physical memory
        signed int value = physicalMemory[physicalAddress];

        // Print the page information
        printPageInformation(addressList[i], physicalAddress, value);

        // Update the free frames list
        freeFrames[frameNumber].lastUpdate = i;

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
void checkPageTable(pageTableEntry* pageTable, TLBEntry* TLB, const char* backingStore, char* physicalMemory, const uint8_t pageNumber, pageFrame* freeFrames, const int freeFramesSize, int* numPageFaults, uint8_t* frameNumber, const int counter)
{
    // If we have a page fault...
    if (pageTable[pageNumber].empty == 1)
    {
        // Get the page from the backing store
        signed char page[PAGE_TABLE_SIZE+1];
        readPageFromBackingStore(pageNumber, backingStore, page);

        // Store the page in the next free frame
        storePageInNextFreeFrame(pageTable, TLB, pageNumber, physicalMemory, page, freeFrames, freeFramesSize, counter);

        (*numPageFaults)++;
    }

    // Set the frame number equal to the frame number from the page table
    *frameNumber = pageTable[pageNumber].address;
}

/**
 * Stores a page in the next free frame in physical memory
 */
void storePageInNextFreeFrame(pageTableEntry* pageTable, TLBEntry* TLB, const uint8_t pageNumber, char* physicalMemory, signed char* page, pageFrame* freeFrames, const int freeFramesSize, const int counter)
{
    // Loop over the entire freeFrames list to try and find a free frame
    int i;
    for (i = 0; i < freeFramesSize; i++)
    {
        // If you've found a free frame...
        if (freeFrames[i].free)
        {
            // Store the page at frame "i"
            storePage(physicalMemory, page, i);

            // Update the freeFramesList
            freeFrames[i].free = 0;
            freeFrames[i].lastUpdate = counter;

            // Update the pageTable
            pageTable[pageNumber].address = i;
            pageTable[pageNumber].empty = 0;

            return;
        }
    }

    
    // If there are no free frames, find victimFrame to be replaced
    int victimFrameAddress = findVictimFrame(pageTable, freeFrames, freeFramesSize);

    // Update the victim page(s)
    for (i = 0; i < PAGE_TABLE_SIZE; i++)
    {
        if (pageTable[i].address == victimFrameAddress)
        {
            pageTable[i].address = -1;
            pageTable[i].empty = 1;
        }
    }
    
    // Store the page at that victim frame address
    storePage(physicalMemory, page, victimFrameAddress);

    // Update the TLB
    removeEntryFromTLB(TLB, pageNumber);

    // Update the free frames list
    freeFrames[victimFrameAddress].lastUpdate = counter;

    // Update the NEW entry into the page table (the one we're slotting in)
    pageTable[pageNumber].address = victimFrameAddress;
    pageTable[pageNumber].empty = 0;
}

/**
 * Finds a victim frame to be replaced 
 * with new page from backing store
 */
uint8_t findVictimFrame(pageTableEntry* pageTable, pageFrame* freeFrames, int numFrames)
{
    int minimumLastUpdateTime = INT_MAX;
    int minimumLastUpdateIndex = 0;
    int i;
    // Loop through the free frames list table, finding the lowest "last update" time
    for (i = 0; i < numFrames; i++)
    {
        if (freeFrames[i].lastUpdate < minimumLastUpdateTime)
        {
            minimumLastUpdateTime = freeFrames[i].lastUpdate;
            minimumLastUpdateIndex = i;
        }
    }
    // Return the address of the victim frame
    return minimumLastUpdateIndex;
} 

/**
 * Stores a page into a frame of memory
 */
void storePage(char* physicalMemory, signed char* page, int index)
{
    int firstAddress = index * FRAME_SIZE;
    int lastAddress = firstAddress + FRAME_SIZE;
    int i;
    int j = 0;
    for (i = firstAddress; i < lastAddress; i++)
    {
        physicalMemory[i] = page[j];
        j++;
    }
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

/**
 * Prints aggregate statistics about virtual memory operation
 */
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
    int numFrames = PHYSICAL_MEMORY_SIZE / PAGE_TABLE_SIZE;     // Initialize free frames list
    pageFrame freeFrames[numFrames];                            // ...
    initializeFreeFramesList(freeFrames, numFrames);            // ...

    // Get the filename from the command line
    char* filename = argv[1];

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
    simulateVirtualMemory(addressList, addressListLength, pageTable, TLB, backingStore, physicalMemory, freeFrames, numFrames, &numAddressReferences, &numPageFaults, &numTLBHits, &tlbIndex);

    // Print final aggregate statistics about virtual memory operation
    printStatistics(numAddressReferences, numPageFaults, numTLBHits);

    return 0;
}
