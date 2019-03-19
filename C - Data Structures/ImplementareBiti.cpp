#include "stdio.h"
#include "malloc.h"

int main() {
	//Fie o matrice 5x3 la intamplare:
	short mat[5][3] = { {2,3,0}, {4,6,0}, {7,8,0}, {1,5,9}, {0,1,2} };
	short rez[5][3];//Matrice rezultata in urma decodificarii.

	//CODIFICARE MATRICE:
	short index = 0;
	short n = 0; //numar de elemente diferite de 0. Am nevoie pentru a aloca mai tarziu.
	unsigned short b = 0; //short tratat ca un "vector" de biti;
	while (index < 15) {
		//La fiecare iteratie shifteaza la stanga
		//Si adauga 1 daca valoarea este diferita de 0
		short i = index / 3;
		short j = index % 3;
		if (mat[i][j] != 0) {
			b++; 
			n++;
		}
		b = b << 1; 
		index++;
	}
	//Ultimul shift este in plus
	b = b >> 1;

	//Aloc un vector pentru a stoca aceste n valori
	short* valori = (short*)malloc(n * sizeof(short));
	//Vectorul primeste valorile:
	short k = 0;//index vector
	index = 0;
	while (index < 15) {
		short i = index / 3;
		short j = index % 3;
		if (mat[i][j] != 0) {
			valori[k] = mat[i][j];
			k++;
		}
		index++;
	}

	//DECODIFICARE:
	index = 14; 
	k = n - 1;
	//Procesul are loc in sens invers codificarii
	//Deoarece "vectorul" de biti este asemanator stivei
	//First in - Last out
	while (index >= 0) {
		short i = index / 3;
		short j = index % 3;
		if (b % 2 != 0) {
			//intotdeauna b o sa fie impar daca exista un element pe respectiva pozitie
			rez[i][j] = valori[k];
			k--;
		}
		else
			//sau par in caz contrar
			rez[i][j] = 0;
		//Acum shiftarea se face la dreapta
		b = b >> 1;
		index--;
	}

	//Afisarea matricii initiale:
	printf("Matricea initiala:\n");
	index = 0;
	while (index < 12) {
		short i = index / (sizeof(mat[0]) / sizeof(short));
		short j = index % (sizeof(mat[0]) / sizeof(short));
		printf("%d   ", mat[i][j]);
		if (j == (sizeof(mat[0]) / sizeof(short)) - 1)
			printf("\n");
		index++;
	}

	//Afisarea matricii rezultat:
	printf("\nMatricea rezultat:\n");
	index = 0;
	while (index < 12) {
		short i = index / (sizeof(rez[0]) / sizeof(short));
		short j = index % (sizeof(rez[0]) / sizeof(short));
		printf("%d   ", rez[i][j]);
		if (j == (sizeof(rez[0]) / sizeof(short)) - 1)
			printf("\n");
		index++;
	}
	printf("\nMemorie matrice necodificata %d B",(int)sizeof(mat));
	printf("\nMemorie matrice codificata %d B\n",(int)(n*sizeof(short) + 1));//n valori, +1 v de b

	//Eliberare memorie vector de valori;
	free(valori);
	return 0;
}



//CAZ GENERALIZAT - MATRICE M * N CU DIMENSIUNI PESTE SHORT



//Algoritm pentru calcularea dimensiunii necesare
//"vectorului" de biti. O matrice de 18 elemente de exemplu
//are nevoie de un short valori[2];
// short calcDimensiune(short dimMatrice) {
// 	short dim;
// 	if (dimMatrice % 15 == 0)
// 		dim = dimMatrice / 15;
// 	else
// 		dim = dimMatrice / 15 + 1;
// 	return dim;
// }

// int main() {
// 	//Matrice data pentru testare:
// 	short mat[8][4] = { {1,0,2,0},{0,0,0,3},{0,0,4,0},{0,0,5,0},{0,6,0,0},{0,7,0,0},{0,0,8,0},{0,9,0,0} };
// 	//Matrice rezultata in urma decodificarii.
// 	short matRezultat[8][4];

// 	//Alocarea "vectorului" de biti:
// 	unsigned short *b = (unsigned short*)malloc((calcDimensiune(sizeof(mat) / sizeof(short))) * sizeof(short));
// 	for (short i = 0; i < calcDimensiune(sizeof(mat) / sizeof(short)); i++)
// 		b[i] = 2;//fiecare b[i] este un "vector". La codificare explic de ce 2 si nu 0.

// 	//CODIFICARE MATRICE:
// 	short n = 0;
// 	short index = 0;
// 	short bit = 0; //intexul vectorului de biti!
// 	while (index < sizeof(mat) / sizeof(short)) {
// 		//Principiul este urmatorul:
// 		//Element diferit de 0?
// 			//marchez cu un bit (adun 1).
// 			//shiftez la stanga
// 		short i = index / (sizeof(mat[0]) / sizeof(short));
// 		short j = index % (sizeof(mat[0]) / sizeof(short));
// 		if (mat[i][j] != 0) {
// 			b[bit]++;
// 			n++;//numar si elementele diferite de 0, pentru alocarea vectorului de valori.
// 		}

// 		//short are 16 biti
// 			//-1 pentru ca avem numarul 1 ca prag = 15
// 		//de ce 2? daca ar fi 0 si la trecerea pe urmatorul bit, ar urma x cifre de 0 din matrice, atunci
// 		//la decodificare nu o sa se stie valoarea lui x. Avand cifra 2 ca prag, o sa se stie.
// 		//plus ca ea imi ofera un bit gol -> 0010
// 		if ((b[bit]) >= 32768) { // 1000 0000 (short) 0000 a ajuns "2"-u ala in final
// 			bit++;
// 			b[bit] = b[bit] >> 1;
// 		}
// 		b[bit] = b[bit] << 1;
// 		index++;
// 	}
// 	//Din nou, exista o shiftare in plus, care se corecteaza:
// 	b[bit] = b[bit] >> 1;

// 	//Aloc un vector pentru a stoca aceste n valori
// 	short* valori = (short*)malloc(n * sizeof(short));//(n * sizeof(short));
// 	//Vectorul primeste valorile:
// 	short k = 0;//index vector
// 	index = 0;
// 	while (index < sizeof(mat) / sizeof(short)) {
// 		short i = index / (sizeof(mat[0]) / sizeof(short));
// 		short j = index % (sizeof(mat[0]) / sizeof(short));
// 		if (mat[i][j] != 0) {
// 			valori[k] = mat[i][j];
// 			k++;
// 		}
// 		index++;
// 	}

// 	//DECODIFICARE:
// 	index = sizeof(matRezultat) / sizeof(short) - 1;//n elemente matrice - 1
// 	bit = calcDimensiune(sizeof(mat) / sizeof(short)) - 1;//same, pt b.
// 	k = n - 1;
// 	//Procesul are loc in sens invers codificarii
// 	//Deoarece "vectorul" de biti este asemanator stivei
// 	//First in - Last out
// 	while (index >= 0) {
// 		short i = index / (sizeof(matRezultat[0]) / sizeof(short));
// 		short j = index % (sizeof(matRezultat[0]) / sizeof(short));
// 		//Aici se pune problema semnului. Cand b[bit] este 0 inseamna
// 		//ca nu mai are informatii. De asemenea, trebuie verificat ca bit>0
// 		//(pentru cazul unei matrici care intra intr-un singur short)
// 		if (b[bit] == 1 && bit > 0) {
// 			bit--;
// 		}

// 		if (b[bit] % 2 != 0) {
// 			//intotdeauna b o sa fie impar daca exista un element pe respectiva pozitie
// 			matRezultat[i][j] = valori[k];
// 			k--;
// 		}
// 		else
// 			//sau par in caz contrar
// 			matRezultat[i][j] = 0;
// 		//Acum shiftarea se face la dreapta
// 		b[bit] = b[bit] >> 1;
// 		index--;
// 	}
// 	index = 0;


	
// 	//Afisarea matricii initiale:
// 	printf("Matricea initiala:\n");
// 	while (index < sizeof(mat) / sizeof(short)) {
// 		short i = index / (sizeof(mat[0]) / sizeof(short));
// 		short j = index % (sizeof(mat[0]) / sizeof(short));
// 		printf("%d   ", mat[i][j]);
// 		if (j == (sizeof(mat[0]) / sizeof(short)) - 1)
// 			printf("\n");
// 		index++;
// 	}

// 	//Afisarea matricii rezultat:
// 	printf("\nMatricea rezultat:\n");
// 	index = 0;
// 	while (index < sizeof(matRezultat) / sizeof(short)) {
// 		short i = index / (sizeof(matRezultat[0]) / sizeof(short));
// 		short j = index % (sizeof(matRezultat[0]) / sizeof(short));
// 		printf("%d   ", matRezultat[i][j]);
// 		if (j == (sizeof(matRezultat[0]) / sizeof(short)) - 1)
// 			printf("\n");
// 		index++;
// 	}
// 	printf("\nMemorie matrice necodificata %d B", (int)sizeof(mat));
// 	printf("\nMemorie matrice codificata %d B\n", (int)(n * sizeof(short) + calcDimensiune(sizeof(mat) / sizeof(short)) * 2 + 2));//n valori, +1 v[bit] de *b
// 	free(b);
// 	free(valori);
// 	return 0;
// }