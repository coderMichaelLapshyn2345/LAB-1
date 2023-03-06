#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Structures.h"
#define SIZE_OF_CLIENT (sizeof(struct Client))
#define SIZE_OF_BARMEN (sizeof(struct Barmen)
#define SIZE_OF_DRINKS (sizeof(struct Drink)
#define SIZE_OF_INDEXER (sizeof(struct Indexer))
#define CLIENT_DATABASE ("clients.fl")
#define BARMEN_DATABASE ("barmen.fl")
#define DRINKS_DATABASE ("drinks.fl")
#define CLIENT_INDEXER_DATABASE ("indexer.ind")
#define CLIENT_GARBAGE ("clients.txt")
#define BARMEN_GARBAGE ("barmen.txt")
#define DRINKS_GARBAGE ("drinks.txt")
// ������� insert_m, insert_s
void InsertClient(struct Client new_client)
{
	FILE* clients_fl = fopen(CLIENT_DATABASE, "ab");
	fwrite(&new_client, sizeof(struct Client), 1, clients_fl);
	fclose(clients_fl);
	return;
}
void InsertBarmen(struct Barmen barmen)
{
	FILE* barmen_fl = fopen(BARMEN_DATABASE, "ab");
	fwrite(&barmen, sizeof(struct Barmen), 1, barmen_fl);
	fclose(barmen_fl);
	return;
}
void InsertDrink(struct Drink drink)
{
	FILE* drinks_fl = fopen(DRINKS_DATABASE, "ab");
	fwrite(&drink, sizeof(struct Drink), 1, drinks_fl);
	fclose(drinks_fl);
	return;
}
// ������� ut_m, ut_s 
void PrintClient(struct Client client)
{
	printf("�������: %s\n", client.lastName);
	printf("��'�: %s\n", client.firstName);
	printf("³�: %d\n", client.age);
	return;
}
void PrintBarmen(struct Barmen barmen)
{
	printf("�������: %s\n", barmen.lastName);
	printf("��'�: %s\n", barmen.firstName);
	printf("ʳ������ �볺���: %d\n", barmen.clientNo);
	return;
}
void PrintDrink(struct Drink drink)
{
	printf("�����: %s]n", drink.name);
	printf("���: %s\n", drink.type);
	printf("���������: %d\n", drink.isLiquid);
	printf("����� �����: %d\n", drink.noOfDrink);
	return;
}
