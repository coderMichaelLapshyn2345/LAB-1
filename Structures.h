#pragma once
typedef struct Master
{
	int ID;
	char firstName[50];
	char lastName[50];
	int Age;
	long firstSlaveAddress;
	int slaveCount;
};
// Структура Напій
typedef struct Slave
{
	int ID;
	int masterID;
	int exist;
	long selfAddress;
	long nextAddress;
	char name[30];
	char type[30];
	int character;
};

// Структура Індексер 
typedef struct Indexer
{
	int ID;
	int exist;
	long address;
};