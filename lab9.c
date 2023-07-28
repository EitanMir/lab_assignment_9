#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
    int id;
    char name;
    int order;
};

// Global variable for the size of the records array
int recordSz = 15;

// Fill out this structure
struct HashType
{
    struct RecordType* data;
    int size;
};

// Compute the hash function
int hash(int x, int tableSize)
{
	return x % tableSize;
}

// Parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
   FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType* pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType*)malloc(sizeof(struct RecordType) * dataSz);
        // Implement parse data block
        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i)
        {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
        }

        fclose(inFile);
    }

    return dataSz;
}

// Prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i)
    {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

// Display records in the hash structure
void displayRecordsInHash(struct HashType* pHashArray)
{
    printf("The Records in the Hash Structure:\n");
    for (int i = 0; i < pHashArray->size; ++i)
    {
        if (pHashArray->data[i].id != -1)
        {
            printf("Index %d -> ID: %d, Name: %c, Order: %d\n", i, pHashArray->data[i].id, pHashArray->data[i].name, pHashArray->data[i].order);
        }
    }
}

int main(void)
{
    struct RecordType* pRecords;

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    // Hash table creation and insertion with open addressing (linear probing)
    int hashSize = recordSz;
    struct HashType hashTable;
    hashTable.data = (struct RecordType*)malloc(sizeof(struct RecordType) * hashSize);
    hashTable.size = hashSize;

    // Initialize hash table slots to -1 to represent an empty slot
    for (int i = 0; i < hashSize; ++i)
    {
        hashTable.data[i].id = -1;
    }

    for (int i = 0; i < recordSz; ++i)
    {
        int index = hash(pRecords[i].id, hashSize);

        // Linear probing to resolve collisions
        while (hashTable.data[index].id != -1)
        {
            index = (index + 1) % hashSize;
        }

        hashTable.data[index] = pRecords[i];
    }

    // Display records in the hash structure
    displayRecordsInHash(&hashTable);

    // Don't forget to free memory
    free(pRecords);
    free(hashTable.data);

    return 0;
}
