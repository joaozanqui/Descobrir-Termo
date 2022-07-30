#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

#define esq 75
#define dir 77
#define esc 27
#define enter 13
#define BS 8
#define F1 59
#define F2 60

// Lista Palavras escritas

typedef struct
{
	char letra;
	int cor;
	int pos;
} word;

word text[20][5];
word organizado[20][5];

// -------------------------------------------------------------- //

void gotoxy(int x, int y)
{ // 80 / 24
	COORD pos = {x, y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

int color(char color)
{
	HANDLE h;
	h = GetStdHandle(STD_OUTPUT_HANDLE);
	return SetConsoleTextAttribute(h, color);
}

void texto(int x, int y, char texto[], char cor)
{
	color(cor);
	gotoxy(x, y);
	printf("%s", texto);
}

void selecao(int x, int y, char texto[], char cor)
{
	color(cor);
	gotoxy(x, y);
	printf("%s", texto);
	gotoxy(x + 2, y + 1);
	printf("^");
}

void colocarLetra(int x, int y, char letra, char cor)
{
	color(cor);
	gotoxy(x, y);
	printf("%c", letra);
}

void selecaoLetra(int x, int y, char letra, char cor, word text)
{
	color(text.cor);
	gotoxy(x, y);
	printf("%c", letra);
	color(cor);
	gotoxy(x, y + 1);
	printf("^");
}

// -------------------------------------------------------------- //

void menu(int opc, int n, int cor, int ntext)
{
	int i, j = 0;
	char x[6][1000] = {"Pressione a Letra desejada:", "ENTER - Enviar a palavra", "F1 - Palavras Possiveis", "F2 - Reiniciar", "ESC - SAIR", "_____"};

	gotoxy(0, 3);
	color(15);
	printf("Escolha a cor da letra:\n\n\t1 - Preto");
	color(6);
	printf("\n\t2 - Amarelo");
	color(2);
	printf("\n\t3 - Verde");
	for (i = 0; i < n; i++)
	{
		if (i < 5)
		{
			texto(40, 3 + i, x[i], 15);
		}
		else
		{
			for (j = 0; j <= ntext; j++)
			{
				if (opc + 5 == i && j == ntext)
				{
					if (text[j][i - 5].letra != NULL)
					{
						selecaoLetra(12 + 6 * i, 9 + j * 2, text[j][i - 5].letra, cor, text[j][i - 5]);
					}
					else
					{
						selecao(10 + 6 * i, 9 + j * 2, x[5], cor);
					}
				}
				else
				{
					if (text[j][i - 5].letra == NULL)
						texto(10 + 6 * i, 9 + j * 2, x[5], 4);
					else
						colocarLetra(12 + 6 * i, 9 + j * 2, text[j][i - 5].letra, text[j][i - 5].cor);
				}
			}
		}
	}

	gotoxy(0, 0);
}

// -------------------------------------------------------------- //

// Lista palavras existentes
typedef struct reg *no;

struct reg
{
	char palavra[6];
	no prox, ant;
};

typedef struct
{
	no prim, ult;
	int qte;
} Descritor;

int palavrasPossiveis(no lista)
{
	int num = 0;
	no p = lista;
	while (p != NULL)
	{
		num++;
		p = p->prox;
	}

	return num;
}

int removerPalavra(Descritor *lista, char palavra[])
{
	if ((*lista).qte == 1)
		return 0;

	no p = (*lista).prim;
	while (p != NULL)
	{
		if (strcmp(p->palavra, palavra) == 0)
		{
			if (p->ant == NULL)
			{
				(*lista).prim = p->prox;
				(*lista).prim->ant = NULL;
				if ((*lista).qte == 1)
					(*lista).ult = NULL;
				p = (*lista).prim->ant;
				free(p);
			}
			else if (p->prox == NULL)
			{
				p = p->ant;
				(*lista).ult = p;
				p->prox = NULL;
				free(p->prox);
			}
			else
			{
				p->ant->prox = p->prox;
				p->prox->ant = p->ant;
			}
			(*lista).qte--;
			return 1;
		}
		else
			p = p->prox;
	}
}

void mostrarPalavras(Descritor lista)
{
	if (lista.qte == 0)
	{
		printf("\nLista vazia");
		return;
	}

	gotoxy(0, 10);
	no p = lista.prim;
	printf("\n-> Palavras Possiveis:\n\n");

	do
	{
		printf("%s\n", p->palavra);
		p = p->prox;
	} while (p != NULL);
}

void colocaPalavras(Descritor *lista, char palavra[])
{
	no p;
	p = (no)malloc(sizeof(struct reg));
	strcpy(p->palavra, palavra);
	p->prox = NULL;
	p->ant = NULL;
	if ((*lista).qte == 0)
		(*lista).prim = p;
	else
	{
		p->ant = (*lista).ult;
		(*lista).ult->prox = p;
	}

	(*lista).ult = p;
	(*lista).qte++;
}

Descritor criaLista()
{
	FILE *file;
	char palavra[6];
	int i;

	Descritor lista;

	lista.prim = lista.ult = NULL;
	lista.qte = 0;

	if ((file = fopen("Palavras5.txt", "r")) == NULL)
	{
		printf("erro");
		return lista;
	}

	while (fscanf(file, "%s", palavra) != EOF)
	{ // Coloca as palavras do arquivo na lista
		for (i = 0; i < 5; i++)
			palavra[i] = toupper(palavra[i]);
		colocaPalavras(&lista, palavra);
	}
	fclose(file);

	return lista;
}

void organizaCor(int ntext)
{
	int i, j, k;
	for (i = 0; i < 5; i++)
		organizado[ntext][i] = text[ntext][i];
	word aux;
	for (i = 0; i < 5; i++)
	{
		for (j = i; j < 5; j++)
		{
			if (organizado[ntext][i].cor > organizado[ntext][j].cor)
			{
				aux = organizado[ntext][i];
				organizado[ntext][i] = organizado[ntext][j];
				organizado[ntext][j] = aux;
			}
		}
	}
}

void restart(Descritor *lista, int *opc, int *cor, int *ntext)
{
	int i, j;
	for (i = 0; i < 20; i++)
	{ // Limpa as palavras e cores que ser�o escritas
		for (j = 0; j < 5; j++)
		{
			text[i][j].letra = NULL;
			text[i][j].cor = 15;
		}
	}

	*lista = criaLista();
	*opc = 0;
	*cor = 15;
	*ntext = 0;
}

void achou(Descritor lista, int ntext)
{
	gotoxy(42, 11 + ntext * 2);
	printf("Palavra encontrada: ");
	color(2);
	printf("%s", lista.prim->palavra);
	color(15);
	gotoxy(42, 13 + ntext * 2);
	system("pause");
}

int main()
{
	int ntext = 0, i, j;

	for (i = 0; i < 20; i++)
	{ // Limpa as palavras e cores que ser�o escritas
		for (j = 0; j < 5; j++)
		{
			text[i][j].letra = NULL;
			text[i][j].cor = 15;
		}
	}

	int opc = 0, n = 10, cor = 15, cont = 0, apag = 0, temLetra = 0, repetida = 0, nRep = 0, outraLetra = 0;
	char t;

	Descritor lista = criaLista();	   // Cria uma lista
	Descritor listaFixa = criaLista(); // Cria uma lista fixa para ser usada na valida��o as palavras

	do
	{
		system("cls");
		menu(opc, n, cor, ntext);

		color(15);
		gotoxy(90, 3);
		int nExiste = lista.qte;
		printf("%d Palavras Possiveis", nExiste);
		gotoxy(0, 0);

		t = getch();
		if (t < 0)
		{
			apag = 0;
			t = getch();
			system("cls");

			if (t == dir)
			{
				if (opc < 4)
					opc++;
				menu(opc, n, cor, ntext);
			}

			else if (t == esq)
			{
				if (opc > 0)
					opc--;
				menu(opc, n, cor, ntext);
			}
		}

		else if (t == 49 || t == 50 || t == 51)
		{
			apag = 0;
			if (t == 49)
				cor = 15;
			else if (t == 50)
				cor = 6;
			else
				cor = 2;
			;
			menu(opc, n, cor, ntext);
		}

		else if (toupper(t) >= 65 && toupper(t) <= 90)
		{
			apag = 0;
			int vazio = 0;
			if (text[ntext][opc].letra == NULL)
				vazio = 1;
			text[ntext][opc].letra = toupper(t);
			text[ntext][opc].cor = cor;
			text[ntext][opc].pos = opc;
			if (vazio == 1)
			{
				cont++;
				if (cont < 5)
					opc++;
				if (opc > 4)
					opc = 0;
				while (text[ntext][opc].letra != NULL && cont < 5)
				{
					opc++;
					if (opc > 4)
						opc = 0;
				}
			}
			menu(opc, n, cor, ntext);
		}

		else if (t == enter)
		{
			int incompleto = 0;
			for (i = 0; i < 5; i++)
			{
				if (text[ntext][i].letra == NULL)
				{
					incompleto = 1;
					break;
				}
			}

			if (incompleto == 0)
			{
				int existe = 0;

				// Armazenar a palavra em uma variavel
				char plv[6];
				for (i = 0; i < 5; i++)
					plv[i] = text[ntext][i].letra;
				for (i = 5; i < strlen(plv); i++)
					plv[i] = NULL;
				// Descobrir se a palavra existe

				no q = listaFixa.prim;
				while (strcmp(q->palavra, plv) != 0 && q->prox != NULL)
					q = q->prox;

				if (q->prox == NULL && strcmp(q->palavra, plv) != 0)
					existe = 0;
				else
					existe = 1;

				// -------------------------------------- //
				if (existe == 1)
				{
					no p = lista.prim;

					int pos[5];
					organizaCor(ntext);
					for (i = 0; i < 5; i++)
					{
						plv[i] = organizado[ntext][i].letra;
						pos[i] = organizado[ntext][i].pos;
					}

					for (i = 0; i < 5; i++)
					{
						cor = organizado[ntext][i].cor;
						repetida = 0;
						for (j = i - 1; j >= 0; j--)
						{
							if (plv[i] == plv[j])
							{
								repetida = 1;
								nRep = j;
								break;
							}
						}

						while (p != NULL)
						{
							// printf("p->palavra= %s, %c=plv[%d], nRep: %d\n", p->palavra, plv[i], i, nRep);
							switch (cor)
							{
							case 2:
								temLetra = 0; // Verde
								if (p->palavra[pos[i]] == plv[i])
									temLetra = 1;
								if (temLetra == 0)
								{
									if (removerPalavra(&lista, p->palavra))
										nExiste--;
									else
									{
										gotoxy(42, 11 + nExiste * 2);
										printf("Nao existem palavras com essas configuracoes...");
										gotoxy(42, 13 + nExiste * 2);
										system("pause");
										restart(&lista, &opc, &cor, &ntext);
										ntext--;
										nExiste--;
									}
								}
								break;
							case 6:
								temLetra = 0; // Amarelo
								if (repetida == 1)
								{

									if (organizado[ntext][nRep].cor == 2)
									{
										outraLetra = 0;
										for (j = 0; j < 5; j++)
										{
											if (p->palavra[pos[j]] == plv[i])
												outraLetra++;
										}
										for (j = 0; j < 5; j++)
										{
											if (p->palavra[pos[j]] == plv[i])
											{
												if (outraLetra > 1)
												{
													if (p->palavra[pos[nRep]] == plv[nRep], plv[i] != p->palavra[pos[i]])
														temLetra = 1;
												}
											}
										}
									}

									else if (organizado[ntext][nRep].cor == 6)
									{
										outraLetra = 0;
										for (j = 0; j < 5; j++)
										{
											if (p->palavra[pos[j]] == plv[i])
												outraLetra++;
										}
										for (j = 0; j < 5; j++)
										{
											if (p->palavra[pos[j]] == plv[i])
											{
												if (j != i && outraLetra > 1)
													temLetra = 1;
												break;
											}
										}
									}
								}

								else
									for (j = 0; j < 5; j++)
									{
										if (p->palavra[pos[j]] == plv[i])
										{
											if (j != i)
												temLetra = 1;
											break;
										}
									}

								if (temLetra == 0)
								{
									if (removerPalavra(&lista, p->palavra))
										nExiste--;
									else
									{
										gotoxy(42, 11 + nExiste * 2);
										printf("Nao existem palavras com essas configuracoes...");
										gotoxy(42, 13 + nExiste * 2);
										system("pause");
										restart(&lista, &opc, &cor, &ntext);
										ntext--;
										nExiste--;
									}
								}
								break;

							case 15:
								outraLetra = 0;
								if (repetida == 1)
								{
									for (j = 0; j < 5; j++)
									{
										if (p->palavra[pos[j]] == plv[i])
											outraLetra++;
									}
								}

								if (outraLetra > 1 || repetida == 0)
								{
									for (j = 0; j < 5; j++)
									{ // Preto
										if (p->palavra[pos[j]] == plv[i])
										{
											if (removerPalavra(&lista, p->palavra))
												nExiste--;
											else
											{
												gotoxy(42, 11 + nExiste * 2);
												printf("Nao existem palavras com essas configuracoes...");
												gotoxy(42, 13 + nExiste * 2);
												system("pause");
												restart(&lista, &opc, &cor, &ntext);
												ntext--;
												nExiste--;
											}
											break;
										}
									}
								}
								break;
							}
							p = p->prox;
						}
						p = lista.prim;
					}
					ntext++;
					opc = 0;
					cor = 15;
					cont = 0;
					apag = 0;

					if (nExiste == 1)
					{
						gotoxy(90, 3);
						printf("%d Palavras Possiveis", nExiste);
						achou(lista, ntext);
						restart(&lista, &opc, &cor, &ntext);
					}
				}

				else
				{
					gotoxy(40, 9 + ntext * 2);
					printf("Essa Palavra nao eh aceita...");
					getch();
				}
			}
		}

		else if (t == esc)
		{
			color(15);
			system("cls");
			return 0;
		}

		else if (t == BS)
		{
			if (text[ntext][opc].letra != NULL)
			{
				text[ntext][opc].letra = NULL;
				text[ntext][opc].cor = 15;
				menu(opc, n, cor, ntext);
				cont--;
				if (apag == 1 && opc != 0)
					opc--;
			}
			else if (opc != 0)
				opc--;
			apag = 1;
		}

		else if (t == F1)
		{
			mostrarPalavras(lista);
			printf("\n");
			opc = 0;
			system("pause");
		}

		else if (t == F2)
			restart(&lista, &opc, &cor, &ntext);

		else
			menu(opc, n, cor, ntext);

	} while (t != esc);

	return 0;
}
