#pragma once
#include "Structures.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE_OF_MASTER sizeof(struct Master)
#define SIZE_OF_SLAVE sizeof(struct Slave)
#define SIZE_OF_INDEXER sizeof(struct Indexer)
#define MASTER_DATABASE "clients.fl"
#define MASTER_GARBAGE "clients_garbage.txt"
#define SLAVE_DATABASE "drinks.fl"
#define SLAVE_GARBAGE "drinks_garbage.txt"
#define MASTER_INDEXER_DB "indexer.ind"
void RewriteMasterGarbageId(struct Master* master, FILE* masterGarbage, int garbageCount)
{
    int* deletedMasterIds = (int*)malloc(garbageCount * sizeof(int));
    for (int i = 0; i < garbageCount; i++)
    {
        fscanf(masterGarbage, "%d", deletedMasterIds + i);
    }
    master->ID = deletedMasterIds[0];
    fclose(masterGarbage);
    fopen(MASTER_GARBAGE, "wb");
    fprintf(masterGarbage, "%d", garbageCount - 1);
    for (int i = 1; i < garbageCount; i++)
    {
        fprintf(masterGarbage, " %d", deletedMasterIds[i]);
    }
    free(deletedMasterIds);
    fclose(masterGarbage);
}
int InsertMaster(struct Master master)
{
	FILE* master_garbage = fopen(MASTER_GARBAGE, "rb");
	FILE* indexerDatabase;
	FILE* masterDatabase;
	struct Indexer indexer;
	int garbageCount;
	fscanf(master_garbage, "%d", &garbageCount);
	if (garbageCount > 0)
	{
        RewriteMasterGarbageId(&master, master_garbage, garbageCount);
        indexerDatabase = fopen(MASTER_INDEXER_DB, "r+b");
        masterDatabase = fopen(MASTER_DATABASE, "r+b");
        fseek(indexerDatabase, (master.ID - 1) * SIZE_OF_INDEXER, SEEK_SET);
        fread(&indexer, SIZE_OF_INDEXER, 1, indexerDatabase);
        fseek(masterDatabase, indexer.address, SEEK_SET);
	}
    else
    {
        indexerDatabase = fopen(MASTER_INDEXER_DB, "a+b");
        masterDatabase = fopen(MASTER_DATABASE, "a+b");
        long indexerSize = SIZE_OF_INDEXER;
        fseek(indexerDatabase, 0, SEEK_END);
        if (ftell(indexerDatabase))
        {
            fseek(indexerDatabase, -indexerSize, SEEK_END);
            fread(&indexer, SIZE_OF_INDEXER, 1, indexerDatabase);
            master.ID = indexer.ID + 1;
        }
        else
        {
            master.ID = 1;
        }
    }
    master.firstSlaveAddress = -1;
    master.slaveCount = 0;
    fwrite(&master, SIZE_OF_MASTER, 1, masterDatabase);
    indexer.ID = master.ID;
    indexer.address = (master.ID - 1) * SIZE_OF_MASTER;
    indexer.exist = 1;
    printf("Your master\'s id is %d\n", master.ID);
    fseek(indexerDatabase, (master.ID - 1) * SIZE_OF_INDEXER, SEEK_SET);
    fwrite(&indexer, SIZE_OF_INDEXER, 1, indexerDatabase);
    fclose(indexerDatabase);
    fclose(masterDatabase);
    return 1;
}

int GetMaster(struct Master* master, int id)
{
    struct Indexer indexer;
    FILE* indexerDatabase = fopen(MASTER_INDEXER_DB, "rb");
    FILE* masterDatabase = fopen(MASTER_DATABASE, "rb");
    if (indexerDatabase == 0 || masterDatabase == 0) return 0;
    fseek(indexerDatabase, (id - 1) * SIZE_OF_INDEXER, SEEK_SET);
    fread(&indexer, SIZE_OF_INDEXER, 1, indexerDatabase);
    if (indexer.exist == 0) return 0;
    fseek(masterDatabase, indexer.address, SEEK_SET);
    fread(master, sizeof(struct Master), 1, masterDatabase);
    fclose(indexerDatabase);
    fclose(masterDatabase);
    return 1;
}
int UpdateMaster(struct Master master)
{
    FILE* indexerDatabase = fopen(MASTER_INDEXER_DB, "r+b");
    FILE* masterDatabase = fopen(MASTER_DATABASE, "r+b");
    if (indexerDatabase == 0 || masterDatabase == 0) return 0;
    struct Indexer indexer;
    int id = master.ID;
    fseek(indexerDatabase, (id - 1) * SIZE_OF_INDEXER, SEEK_SET);
    fread(&indexer, SIZE_OF_INDEXER, 1, indexerDatabase);
    if (indexer.exist == 0) return 0;
    fseek(masterDatabase, indexer.address, SEEK_SET);
    fwrite(&master, SIZE_OF_MASTER, 1, masterDatabase);
    fclose(indexerDatabase);
    fclose(masterDatabase);
    return 1;
}
void MarkDeletedMaster(int id)
{
    FILE* masterGarbage = fopen(MASTER_GARBAGE, "rb");
    int garbageCount;
    fscanf(masterGarbage, "%d", &garbageCount);
    int* deletedMasterIds = (int*)malloc(garbageCount * sizeof(int));
    for (int i = 0; i < garbageCount; i++)
    {
        fscanf(masterGarbage, "%d", deletedMasterIds + i);
    }
    fclose(masterGarbage);
    masterGarbage = fopen(MASTER_GARBAGE, "wb");
    fprintf(masterGarbage, "%d", garbageCount + 1);
    for (int i = 0; i < garbageCount; i++)
    {
        fprintf(masterGarbage, " %d", deletedMasterIds[i]);
    }
    fprintf(masterGarbage, " %d", id);
    free(deletedMasterIds);
    fclose(masterGarbage);
}
int DeleteMaster(int id)
{
    FILE* indexerDatabase = fopen(MASTER_INDEXER_DB, "r+b");
    // для читання та запису
    if (indexerDatabase == NULL)
    {
        return 0;
    }
    struct Master master;
    GetMaster(&master, id);

    struct Indexer indexer;

    fseek(indexerDatabase, (id - 1) * SIZE_OF_INDEXER, SEEK_SET);
    fread(&indexer, SIZE_OF_INDEXER, 1, indexerDatabase);
    if (indexer.exist == 0) return 0;
    indexer.exist = 0;

    fseek(indexerDatabase, (id - 1) * SIZE_OF_INDEXER, SEEK_SET);

    fwrite(&indexer, SIZE_OF_INDEXER, 1, indexerDatabase);
    fclose(indexerDatabase);

    MarkDeletedMaster(id);


    if (master.slaveCount)
    {
        FILE* slavesDatabase = fopen(SLAVE_DATABASE, "r+b");
        struct Slave slave;

        fseek(slavesDatabase, master.firstSlaveAddress, SEEK_SET);

        for (int i = 0; i < master.slaveCount; i++)
        {
            fread(&slave, SIZE_OF_SLAVE, 1, slavesDatabase);
            fclose(slavesDatabase);
            DeleteSlave(master, slave, slave.ID);

            slavesDatabase = fopen(SLAVE_DATABASE, "r+b");
            fseek(slavesDatabase, slave.nextAddress, SEEK_SET);
        }

        fclose(slavesDatabase);
    }
    return 1;
}
void MarkDeletedSlave(long address)
{
    FILE* garbageZone = fopen(SLAVE_GARBAGE, "rb");

    int garbageCount;
    fscanf(garbageZone, "%d", &garbageCount);

    long* delAddresses = (long*)malloc(garbageCount * sizeof(long));

    for (int i = 0; i < garbageCount; i++)
    {
        fscanf(garbageZone, "%ld", delAddresses + i);
    }

    fclose(garbageZone);
    garbageZone = fopen(SLAVE_GARBAGE, "wb");
    fprintf(garbageZone, "%ld", garbageCount + 1);

    for (int i = 0; i < garbageCount; i++)
    {
        fprintf(garbageZone, " %ld", delAddresses[i]);
    }

    fprintf(garbageZone, " %d", address);
    free(delAddresses);
    fclose(garbageZone);
}
void RewriteSlaveGarbageId(int garbageCount, FILE* slaveGarbage, struct Slave* master)
{
    long* deletedSlaveIds = (long*)malloc(garbageCount * sizeof(long));
    for (int i = 0; i < garbageCount; i++)
    {
        fscanf(slaveGarbage, "%d", deletedSlaveIds + i);
    }
    master->selfAddress = deletedSlaveIds[0];
    master->nextAddress = deletedSlaveIds[0];

    fclose(slaveGarbage);
    fopen(SLAVE_GARBAGE, "wb");
    fprintf(slaveGarbage, "%d", garbageCount - 1);

    for (int i = 1; i < garbageCount; i++)
    {
        fprintf(slaveGarbage, " %d", deletedSlaveIds[i]);
    }

    free(deletedSlaveIds);
    fclose(slaveGarbage);
}
int InsertSlave(struct Master master, struct Slave slave)
{
    if (master.slaveCount > 0)
    {
        FILE* slavesDatabase = fopen(SLAVE_DATABASE, "r+b");
        struct Slave current;
        fseek(slavesDatabase, master.firstSlaveAddress, SEEK_SET);
        for (int i = 0; i < master.slaveCount; i++) {
            fread(&current, SIZE_OF_SLAVE, 1, slavesDatabase);
            fclose(slavesDatabase);
            if (current.ID == slave.ID) {
                return 0;
            }
            slavesDatabase = fopen(SLAVE_DATABASE, "r+b");
            fseek(slavesDatabase, current.nextAddress, SEEK_SET);
        }
        fclose(slavesDatabase);
    }
    FILE* slaveDatabase = fopen(SLAVE_DATABASE, "a+b");
    FILE* slaveGarbage = fopen(SLAVE_GARBAGE, "rb");
    int garbageCount;
    fscanf(slaveGarbage, "%d", &garbageCount);
    slave.exist = 1;
    if (garbageCount > 0)
    {
        RewriteSlaveGarbageId(garbageCount, slaveGarbage, &slave);
        fclose(slaveDatabase);
        slaveDatabase = fopen(SLAVE_DATABASE, "r+b");
        fseek(slaveDatabase, slave.selfAddress, SEEK_SET);
    }
    else
    {
        fseek(slaveDatabase, 0, SEEK_END);
        long slaveDatabaseSize = ftell(slaveDatabase);
        slave.selfAddress = slaveDatabaseSize;
        slave.nextAddress = slaveDatabaseSize;
    }

    fwrite(&slave, SIZE_OF_SLAVE, 1, slaveDatabase);

    if (!master.slaveCount)
    {
        master.firstSlaveAddress = slave.selfAddress;
    }
    else
    {
        fclose(slaveDatabase);
        slaveDatabase = fopen(SLAVE_DATABASE, "r+b");
        struct Slave previous;
        fseek(slaveDatabase, master.firstSlaveAddress, SEEK_SET);

        for (int i = 0; i < master.slaveCount; i++)
        {
            fread(&previous, SIZE_OF_SLAVE, 1, slaveDatabase);
            fseek(slaveDatabase, previous.nextAddress, SEEK_SET);
        }
        previous.nextAddress = slave.selfAddress;
        fwrite(&previous, SIZE_OF_SLAVE, 1, slaveDatabase);
    }

    fclose(slaveDatabase);
    master.slaveCount++;
    UpdateMaster(master);
    return 1;
}
int GetSlave(struct Master master, struct Slave* slave, int productId)
{
    if (master.slaveCount <= 0)
    {
        return 0;
    }
    FILE* slaveDatabase = fopen(SLAVE_DATABASE, "rb");
    fseek(slaveDatabase, master.firstSlaveAddress, SEEK_SET);
    fread(slave, SIZE_OF_SLAVE, 1, slaveDatabase);
    for (int i = 0; i < master.slaveCount; i++)
    {
        if (slave->ID == productId)
        {
            fclose(slaveDatabase);
            return 1;
        }
        fseek(slaveDatabase, slave->nextAddress, SEEK_SET);
        fread(slave, SIZE_OF_SLAVE, 1, slaveDatabase);
    }
    fclose(slaveDatabase);
    return 0;
}
int UpdateSlave(struct Slave slave, int productId)
{
    FILE* slaveDatabase = fopen(SLAVE_DATABASE, "r+b");
    fseek(slaveDatabase, slave.selfAddress, SEEK_SET);
    fwrite(&slave, SIZE_OF_SLAVE, 1, slaveDatabase);
    fclose(slaveDatabase);
    return 1;
}
int DeleteSlave(struct Master master, struct Slave slave, int id)
{
    FILE* slaveDatabase = fopen(SLAVE_DATABASE, "r+b");
    struct Slave previous;
    fseek(slaveDatabase, master.firstSlaveAddress, SEEK_SET);

    do
    {
        fread(&previous, SIZE_OF_SLAVE, 1, slaveDatabase);
        fseek(slaveDatabase, previous.nextAddress, SEEK_SET);
    } while (previous.nextAddress != slave.selfAddress && slave.selfAddress != master.firstSlaveAddress);

    if (slave.selfAddress == master.firstSlaveAddress)
    {
        if (slave.selfAddress == slave.nextAddress)
        {
            master.firstSlaveAddress = -1;
        }
        else
        {
            master.firstSlaveAddress = slave.nextAddress;
        }
    }
    else
    {
        if (slave.selfAddress == slave.nextAddress)
        {
            previous.nextAddress = previous.selfAddress;
        }
        else
        {
            previous.nextAddress = slave.nextAddress;
        }
        fseek(slaveDatabase, previous.selfAddress, SEEK_SET);
        fwrite(&previous, SIZE_OF_SLAVE, 1, slaveDatabase);
    }
    MarkDeletedSlave(slave.selfAddress);
    slave.exist = 0;
    fseek(slaveDatabase, slave.selfAddress, SEEK_SET);
    fwrite(&slave, SIZE_OF_SLAVE, 1, slaveDatabase);
    fclose(slaveDatabase);
    master.slaveCount--;
    UpdateMaster(master);
}


