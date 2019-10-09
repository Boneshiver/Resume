#include "stdio.h"
#include "stdlib.h"
#include "string.h"


typedef struct AVL {
	int value;
	int balanceFactor;
	struct AVL* left;
	struct AVL* right;
}AVL;


AVL* initAVL(int value);
AVL* insertNode(AVL* root, AVL* node);
int getHeight(AVL* node);
AVL* rebalanceNode(AVL* node);
void printAVL(AVL* tree);
AVL* leftRotation(AVL* node);
AVL* rightRotation(AVL* node);
AVL* deleteNode(AVL* root, int key);
AVL* findMin(AVL* node);
AVL* freeAVL(AVL* root);


int main() {

	FILE* pFile = fopen("Data.txt", "r");//Check file and play with it.
	AVL* root = NULL;
	if (pFile) {
		int value;
		fscanf(pFile, "%i",&value);
		while (!feof(pFile)) {
			AVL* node = initAVL(value);
			root = insertNode(root,node);
			fscanf(pFile, "%i", &value);
		}
	}

	printAVL(root,0);
	root = freeAVL(root);
	fclose(pFile);
	return 0;
}

AVL* initAVL(int value) {
	AVL* node = (AVL*)malloc(sizeof(AVL));
	node->left = NULL;
	node->right = NULL;
	node->value = value;
	node->balanceFactor = 0;
	return node;
}
AVL* insertNode(AVL* root, AVL* node) {

	if (root) {
		if (node->value > root->value) {
			root->right = insertNode(root->right, node);
		}
		else {
			root->left = insertNode(root->left, node);
		}
	}
	else {
		root = node;
	}
	root = rebalanceNode(root);
	return root;
}
int getHeight(AVL* node) {
	if (node) {
		return 1 + (getHeight(node->left) > getHeight(node->right) ? getHeight(node->left) : getHeight(node->right));
	}
	else {
		return 0;
	}
}
AVL* leftRotation(AVL* node) {
	AVL* rNode = node->right;
	node->right = rNode->left;
	rNode->left = node;
	//Parte in plus ce e mai jos, pentru a recalcula balance factor. Nu e necesar. Fac asta ca sa fie la afisare corect.
	rNode->balanceFactor = getHeight(rNode->left) - getHeight(rNode->right);
	node->balanceFactor = getHeight(node->left) - getHeight(node->right);
	return rNode;
}
AVL* rightRotation(AVL* node) {
	AVL* lNode = node->left;
	node->left = lNode->right;
	lNode->right = node;
	//Parte in plus ce e mai jos, pentru a recalcula balance factor. Nu e necesar. Fac asta ca sa fie la afisare corect.
	lNode->balanceFactor = getHeight(lNode->left) - getHeight(lNode->right);
	node->balanceFactor = getHeight(node->left) - getHeight(node->right);
	return lNode;
}
AVL* rebalanceNode(AVL* node) {
	if (node) {
		node->balanceFactor = getHeight(node->left) - getHeight(node->right);
		if (node->balanceFactor == -2) {
			if (node->right->balanceFactor == 1) {
				node->right = rightRotation(node->right);
			}
			node = leftRotation(node);
		}
		else if (node->balanceFactor == 2) {
			if (node->left->balanceFactor == -1) {
				node->left = leftRotation(node->left);
			}
			node = rightRotation(node);
		}
	}
	return node;
}
void printAVL(AVL* root, int zero) {
	if (root) {
		for (int i = 0; i < zero; i++)
			printf("\t");
		printf("%i [bf:%i]\n", root->value,root->balanceFactor);
		printAVL(root->left, zero + 1);
		printAVL(root->right, zero + 1);
	}
	else {
		for (int i = 0; i < zero; i++)
			printf("\t");
		printf("NULL\n");
	}
}
AVL* findMin(AVL* node) {
	if (node) {
		while (node->left) {
			node = node->left;
		}
	}
	return node;
}
AVL* deleteNode(AVL* root, int key) {
	if (root->value == key) {
		if (root->left == NULL && root->right == NULL) {
			AVL* tmp = root;
			root = NULL;
			free(tmp);
		}
		else if (root->left == NULL && root->right != NULL) {
			AVL* tmp = root;
			root = root->right;
			free(tmp);
		}
		else if (root->left != NULL && root->right == NULL) {
			AVL* tmp = root;
			root = root->left;
			free(tmp);
		}
		else {//both != null
			AVL* minRight = findMin(root->right);
			root->value = minRight->value;
			root->right = deleteNode(root->right, root->value);
			root = rebalanceNode(root);
		}
	}
	else {
		if (root->value > key) {
			root->left = deleteNode(root->left, key);
		}
		else if (root->value < key) {
			root->right = deleteNode(root->right, key);
		}
	}
	return root;
}
AVL* freeAVL(AVL* root) {
	if (root) {
		AVL* temp = root;
		root->left = freeAVL(root->left);
		root->right = freeAVL(root->right);
		free(temp);
	}
	return NULL;
}

