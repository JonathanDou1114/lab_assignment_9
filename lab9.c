#include <stdio.h>

// RecordType
struct RecordType
{
    int    id;
    char   name;
    int    order;
};

// Fill out this structure
struct HashType
{
    struct RecordType* record;  // Pointer to the RecordType data
    int used;                   // Flag to indicate if the index is occupied or not
};

// Compute the hash function
int hash(int x)
{
    return x % HASH_SIZE; // You need to define a suitable value for HASH_SIZE
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
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

// prints the records
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

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
    int i;

    for (i=0;i<hashSz;++i)
    {
        // if index is occupied with any records, print all
        if (pHashArray[i].used == 1) {
            printf("index %d -> %d, %c, %d\n", i, pHashArray[i].record->id, pHashArray[i].record->name, pHashArray[i].record->order);
        }
    }
}
int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;
	int hashSz = 100;  // size of the hash table
	struct HashType *pHashArray;
	int i, j, index;

	// Parse input file and print records
	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);

	// Initialize hash table
	pHashArray = (struct HashType*) malloc(sizeof(struct HashType) * hashSz);
	if (pHashArray == NULL)
	{
		printf("Cannot allocate memory\n");
		exit(-1);
	}
	for (i = 0; i < hashSz; ++i)
	{
		// set the order to -1 to indicate that the slot is free
		pHashArray[i].order = -1;
	}

	// Insert records into the hash table
	for (i = 0; i < recordSz; ++i)
	{
		index = hash(pRecords[i].id) % hashSz;
		while (pHashArray[index].order >= 0)
		{
			index = (index + 1) % hashSz;  // linear probing
		}
		pHashArray[index].id = pRecords[i].id;
		pHashArray[index].name = pRecords[i].name;
		pHashArray[index].order = pRecords[i].order;
	}

	// Display records in the hash table
	displayRecordsInHash(pHashArray, hashSz);

	// Free memory
	free(pRecords);
	free(pHashArray);

	return 0;
}
