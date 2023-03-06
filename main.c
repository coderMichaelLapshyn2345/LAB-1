#define _CRT_SECURE_NO_WARNINGS
#include "Structures.h"
#include <stdio.h>
#include <stdlib.h>
#include "IOtput.h"
#include "functions.h"
void ShowInfo()
{
    struct Indexer indexer;
    struct Master master;
    FILE* indexerDatabase = fopen(MASTER_INDEXER_DB, "rb");
    FILE* masterDatabase = fopen(MASTER_DATABASE, "rb");
    fseek(indexerDatabase, 0, SEEK_END);
    int count = ftell(indexerDatabase) / SIZE_OF_INDEXER;
    fseek(indexerDatabase, 0, SEEK_SET);
    for (int i = 0; i < count; i++)
    {
        fread(&indexer, SIZE_OF_INDEXER, 1, indexerDatabase);
        if (indexer.exist != 0)
        {
            fseek(masterDatabase, indexer.address, SEEK_SET);
            fread(&master, SIZE_OF_MASTER, 1, masterDatabase);
            printf("Master with id %d has %d slaves\n", master.ID, master.slaveCount);
        }
    }
    fclose(masterDatabase);
    fclose(indexerDatabase);
}
void GetMasterId(int* id)
{
    printf("Enter master id:");
    scanf("%d", id);
}
void GetSlaveId(int* id)
{
    printf("Enter slave id:");
    scanf("%d", id);
}


int main()
{
    int masterId;
    int slaveId;
    int choice;
    struct Master master;
    struct Slave slave;
    do
    {
        printf("Choose option:\n0 - Quit\n1 - Insert Master\n2 - Get Master\n3 - Update Master\n4 - Delete Master\n5 - Insert Slave\n6 - Get Slave\n7 - Update Slave\n8 - Delete Slave\n9 - Show Info\nEnter choice:");
        if (scanf("%d", &choice) == EOF) return 0;

        switch (choice)
        {
        case 0:
            printf("%s", "bye bye...");
            return 0;
        case 1:
            if (ReadMaster(&master) == 0) printf("too long string...\n");
            else InsertMaster(master);
            break;
        case 2:
            GetMasterId(&masterId);
            if (GetMaster(&master, masterId) == 0) printf("Something went wong...\n");
            else PrintMaster(master);
            break;
        case 3:
            GetMasterId(&masterId);
            if (ReadMaster(&master) == 0) printf("too long string...\n");
            else
            {
                master.ID = masterId;
                if (UpdateMaster(master) == 0) printf("Something went wong...\n");
            }
            break;
        case 4:
            GetMasterId(&masterId);
            if (GetMaster(&master, masterId))
            {
                DeleteMaster(masterId);
            }
            else
            {
                printf("Something went wong...\n");
            }
            break;
        case 5:
            GetMasterId(&masterId);
            if (GetMaster(&master, masterId) == 1)
            {
                GetSlaveId(&slaveId);
                slave.ID = slaveId;
                if (ReadSlave(&slave) == 0) printf("too long string...\n");
                else
                {
                    if (InsertSlave(master, slave) == 0) printf("Something went wong...\n");
                }
            }
            else printf("Something went wong...\n");
            break;
        case 6:
            GetMasterId(&masterId);
            if (GetMaster(&master, masterId) == 1)
            {
                GetSlaveId(&slaveId);
                if (GetSlave(master, &slave, slaveId) != 0)
                {
                    PrintSlave(slave);
                }
                else printf("Something went wong...\n");
            }
            else printf("Something went wong...\n");
            break;
        case 7:
            GetMasterId(&masterId);
            if (GetMaster(&master, masterId) == 1)
            {
                GetSlaveId(&slaveId);
                if (GetSlave(master, &slave, slaveId) != 0)
                {
                    if (ReadSlave(&slave) == 0) printf("too long string...\n");
                    else UpdateSlave(slave, slaveId);
                }
                else printf("Something went wong...\n");
            }
            else printf("Something went wong...\n");
            break;
        case 8:
            GetMasterId(&masterId);
            if (GetMaster(&master, masterId) == 1)
            {
                GetSlaveId(&slaveId);
                if (GetSlave(master, &slave, slaveId) != 0)
                {
                    DeleteSlave(master, slave, slaveId);
                }
                else printf("Something went wong...\n");
            }
            else printf("Something went wong...\n");
            break;
        case 9:
            ShowInfo();
            break;
        default:
            break;
        }
        printf("////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////\n");
    } while (1);
    return 0;
}