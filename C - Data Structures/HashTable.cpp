#include "stdio.h"
#include "stdlib.h"
#include "string.h"

typedef struct Node {
	int id;
	float value;
	struct Node* pNext;
}Node;

typedef struct HashTable {
	struct Node** table;
	int size;
}HashTable;

Node* newNode(int id, float value);
HashTable* newHashTable(int size);
HashTable* insertNode(HashTable* hTable, Node* node);
HashTable* deleteNode(HashTable* hTable, int key);
void printList(Node* node);
int main() {
	HashTable* hTable = newHashTable(7);
	FILE* pFile = fopen("Data.txt", "r");
	int key;
	fscanf(pFile, "%i", &key);
	while (!feof(pFile)) {
		float value;
		fscanf(pFile, "%f", &value);
		Node* nod = newNode(key, value);
		hTable = insertNode(hTable, nod);
		fscanf(pFile, "%i", &key);
	}
	for (int i = 0; i < hTable->size; i++) {
		printf("[%i] :: ",i);
		printList(hTable->table[i]);
		printf("\n");
	}
	printf("\n==========Dupa stergere 21==========\n");
	hTable = deleteNode(hTable, 21);
	for (int i = 0; i < hTable->size; i++) {
		printf("[%i] :: ", i);
		printList(hTable->table[i]);
		printf("\n");
	}
	return 0;
}
Node* newNode(int id, float value) {
	Node* nod = (Node*)malloc(sizeof(Node));
	nod->id = id;
	nod->value = value;
	nod->pNext = NULL;
	return nod;
}
HashTable* newHashTable(int size) {
	HashTable* HT = (HashTable*)malloc(sizeof(HashTable));
	HT->table = (Node**)malloc(size * sizeof(Node*));
	for (int i = 0; i < size; i++) {
		HT->table[i] = NULL;
	}
	HT->size = size;
	return HT;
}
int getHash(int key, int size) {
	return key % size;
}
HashTable* insertNode(HashTable* hTable, Node* node) {
	int index = getHash(node->id, hTable->size);
	if (hTable->table[index] == NULL) {
		hTable->table[index] = node;
	}
	else {
		node->pNext = hTable->table[index];
		hTable->table[index] = node;
	}
	return hTable;
}
void printList(Node* node) {
	if (node) {
		while (node) {
			printf("[%i-%.2f]->", node->id, node->value);
			node = node->pNext;
			if (node == NULL) break;
		}
	}
}
HashTable* deleteNode(HashTable* hTable, int key) {
	int index = getHash(key, hTable->size);
	Node* lista = hTable->table[index];
	if (lista->id == key) {
		hTable->table[index] = hTable->table[index]->pNext;
	}
	else {


		while (lista->pNext != NULL && lista->pNext->id != key) {
			lista = lista->pNext;
		}
		if (lista->pNext != NULL) {
			lista->pNext = lista->pNext->pNext;
		}
		else {
			printf("negasit!");
		}
	}
	return hTable;
}