#pragma once
#include "Structures.h"
#include <stdio.h>
#include <string.h>
#define str_size 50
int CheckForInputStr(char* str)
{
	char buffer[255];
	buffer[0] = '\0';
	scanf("%s", buffer);
	int size = 0;
	for (int i = 0; i < str_size; i++)
	{
		if (buffer[i] == '\0') break;
		size++;
	}
	if (size > str_size) return 0;
	else
	{
		strcpy(str, buffer);
		return 1;
	}
}
int ReadMaster(struct Master* master)
{
	char name[50];
	char surname[50];
	int age;
	name[0] = '\0';
	surname[0] = '\0';
	age = 0;
	printf("Enter client's name: ");
	if (CheckForInputStr(name) == 0) return 0;
	strcpy(master->firstName, name);
	printf("Enter client's surname: ");
	if (CheckForInputStr(surname) == 0) return 0;
	strcpy(master->lastName, surname);
	printf("Enter client's age: ");
	scanf("%d", &age);
	master->Age = age;
	return 1;
}
int ReadSlave(struct Slave* slave)
{
	char name[50];
	char type[50];
	int character;
	printf("Enter drink's name: ");
	if (CheckForInputStr(name) == 0) return 0;
	strcpy(slave->name, name);
	printf("Enter drink's type(juice, water, etc): ");
	if (CheckForInputStr(type) == 0) return 0;
	strcpy(slave->type, type);
	printf("Enter drink's characterictics(1-alkoholic, 0-non-alkoholic: ");
	scanf("%d", &character);
	if (character < 0 || character > 1) return 0;
	slave->character = character;
	return 1;
}
void PrintMaster(struct Master master)
{
	printf("Client's name: %s\n", master.firstName);
	printf("Client's surname: %s\n", master.lastName);
	printf("Client's age: %d\n", master.Age);
}
void PrintSlave(struct Slave slave)
{
	printf("Drink's name: %s\n", slave.name);
	printf("Drink's type: %s\n", slave.type);
	printf("Drink's characteristics(1-alkoholic, 0-non-alkoholic: %d\n", slave.character);
}