//Peticila Alexandru 311 CB

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#pragma pack(1)

//structura fileheader-ului

typedef struct
{
    unsigned char  marker1; 
    unsigned char  marker2; 
    unsigned int   fsize; 
    unsigned short unused1;
    unsigned short unused2;
    unsigned int   offset; 
} fheader;

//structura infoheader-ului

typedef struct
{
    unsigned int   isize; 
    signed int     latime; 
    signed int     lungime; 
    unsigned short planes ;
    unsigned short bitPix ;
    unsigned int   biCompression ;
    unsigned int   imgsize; 
    int            biXPelsPerMeter ;
    int            biYPelsPerMeter ;
    unsigned int   biClrUsed ;
    unsigned int   biClrImportant ;
} iheader;

#pragma pack()

// functie care schimba culorile cifrelor si creeaza noua imagine

void task1( fheader f, iheader i, unsigned char * imagine, int size, int pad, char *s, int a, int v, int r) 
{
	FILE *fp;
	int j,l=0;

	//parcurgem vectorul cu pixeli sarind peste bitii de padding

	for(j=0; j< size; j+=3)
	{
		if((j-l)%(3*i.latime)==0 && j!=0)
		{
			j=j+pad;
			l=l+pad;
		}

		//daca nu avem pixel colorat ii modificam culoarea

		if(imagine[j] != 255 || imagine[j+1] != 255 || imagine[j+2] != 255 )
		{
			imagine[j]=a;
			imagine[j+1]=v;
			imagine[j+2]=r;
		}
	}

	//cream noua imagine

	fp=fopen(s, "wb");
	
	fwrite(&f,sizeof(fheader),1,fp);
	
	fwrite(&i,sizeof(iheader),1,fp);
	
	fwrite(imagine,size,1,fp);

	fclose(fp);
}

//functie care recunoaste numerele in functie de numarul de pixeli colorati

int numar( int ** matrice, int l, int c, int size) 
{
	int i,j,nr=0, ** aux;

	aux=calloc(5, sizeof( int * ) );
	if( !aux )
	{
		return -1;
	}

	for( i=0; i<5; i++ )
	{
		aux[i]= calloc( 5, sizeof ( int ) );
		
		if( !aux[i] )
		{
			for( j=0; j < i; j++ )
			{
				free(aux[j]);
			}
			free(aux);
		}
	}
	
	// in auxiliar salvam matricea 5x5 a cifrei respective 

	for( i=0; i < 5 ; i++)
		for ( j=0 ; j< 5 ; j++ )
		{
			aux[i][j]=matrice[l+i][j+c];

			if( matrice[l+i][j+c]!= size+1)
			{
				// calculam cati pixeli colorati are cifra respectiva

				nr++;
			}
		}

	// in functie de valoarea contorului nr si de valorile anumitor pozitii din
	// aux, returnam cifra corespunzatoare si eliberam memoria inainte	

	if( nr == 5 ) 
	{
		for( i=0; i<5; i++ )
		{
			free(aux[i]);
		}

		free(aux);
		return 1;
	}

	if( nr == 19 )
	{
		for( i=0; i<5; i++ )
		{
			free(aux[i]);
		}

		free(aux);
		return 8;
	}

	if( nr == 9 )
	{	
		for( i=0; i<5; i++ )
		{
			free(aux[i]);
		}

		free(aux);
		return 7;
	}
	
	if( nr == 16 )
	{
		for( i=0; i<5; i++ )
		{
			free(aux[i]);
		}

		free(aux);
		return 0;
	}

	if( nr == 11 )
	{
		for( i=0; i<5; i++ )
		{
			free(aux[i]);
		}

		free(aux);	
		return 4;
	}

	if( nr == 18 )
	{
		if( aux[1][0] != size+1)
		{
			for( i=0; i<5; i++ )
			{
				free(aux[i]);
			}

			free(aux);
			return 9;
		}
		
		else
		{
			for( i=0; i<5; i++ )
			{
				free(aux[i]);
			}

			free(aux);
			return 6;
		}
	}
	
	if ( nr == 17 )
	{
		if( aux[1][4] != size+1)
		{
			for( i=0; i<5; i++ )
			{
				free(aux[i]);
			}

			free(aux);
			return 5;
		}

		else
			if( aux[3][0] != size+1)
			{
				for( i=0; i<5; i++ )
				{
					free(aux[i]);
				}

				free(aux);
				return 3;
			}
	}
	
	for( i=0; i<5; i++ )
	{
		free(aux[i]);
	}

	free(aux);
	return 2;
}	

// functie care inlocuieste o cifra cu o alta sau cu pixeli albi in matrice

void inlocuire( int ** matrice, int l, int c, int **aux) //task 3
{
	int i,j,z;
	
	for(i=0; i< 5; i++)
	{
		for(j=0 ; j < 5; j++)
		{
			z=matrice[l+i][j+c];

			matrice[l+i][j+c]=aux[i][j];

			//salvez in auxiliar cifra pe care am inlocuit-o 
			//pentru a o pune la stanga

			aux[i][j]=z;
		}
	}
}

// functie care muta toate numerele de la dreapta numarului cautat 
// cu o pozitie la stanga

void eliminare( int ** matrice, int j, int lungime, int ** aux,int size) 
{

	int l,c;

	// parcurgem matricea pe coloane pana la pozitia
	// unde am gasit numarul ce trebuie eliminat

	for( c=0; c <= j; c++)
	{
		for( l=0; l < lungime; l ++)
		{
			// daca am gasit un pixel colorat

			if(matrice[l][c]!= size+1)
			{
				// si matricea 5x5 corespunzatoare e o cifra

				if(matrice[l][c-1]== size+1 && matrice[l+1][c-1]==size+1 )
				{
					if(matrice[l+2][c-1]==size+1 && matrice[l+3][c-1]==size+1)
					{

					// inlocuiesc cifra cu ce se afla in auxiliar si trec pe coloana urmatoare

						inlocuire(matrice,l,c,aux);				
						l=lungime;
					}
				}
			}
		}
	}
	
	// dupa ce am facut toata inlocuirile 
	//reintializam auxiliarul cu pixeli albi

	for(c=0; c<5; c++)
	{
		for(l=0; l<5; l++)
		{
			aux[c][l]=size+1;
		}
	}
}

int main()
{
	fheader f;
	iheader i;
	unsigned char * imagine, * imagine1;

	char *s;

	FILE *fp;

	int pad=0,a,v,r,n=0,e,size;

	int j,k,l=0,c=0,nr=0;

	int ** matrice, **aux, * numere, * vect;

	// alocam memorie pentru vectorii si sirurile ajutatoare

	vect = calloc( 100, sizeof ( int ) );
	if( !vect )
	{
		printf("Nu s-a reusit alocarea lui vect");
	}

	numere = calloc( 100, sizeof( int ) );
	if( !numere )
	{
		printf("Nu s-a reusit alocarea lui numere");
	}

	s=calloc( 100, sizeof( char ) );
	if( !s )
	{
		printf(" Nu s-a reusit alocarea lui s");
	}

	aux=calloc(5,sizeof(int*));
	if(!aux)
		printf("Nu s-a reusit alocarea");

	for(j=0; j< 5; j++)
	{
		aux[j]=calloc(5,sizeof(int));
		if(! aux[j])
		{
			for(k=0 ; k<j; k++)
			{
				free(aux[k]);
			}
			free(aux);
		}
	}

	//citirea datelor din input

	fp=fopen("input.txt","r");

	fscanf(fp,"%s%d%d%d",s,&a,&v,&r);

	while( fscanf(fp,"%d",&numere[n]) == 1)
	{
		n++;
	}
	
	fclose(fp);

	//citirea imaginii 

	fp=fopen(s, "rb");

	fread(&f,sizeof(fheader),1,fp);

	fread(&i,sizeof(iheader),1,fp);

	// calculam valoarea padding-ului si dimensiunea ce ia in calcul padding-ul

	while( (i.latime*3 +pad) %4 != 0)
	{
		pad++;
	}
	
	size=i.lungime*(i.latime*3+pad);

	// alocam memorie pentru vectorii de pixeli
	
	imagine=calloc(size+1, sizeof( unsigned char ) ) ;
	
	if(!imagine)
	{
		return -1;
	}
	
	
	imagine1=calloc(size+1, sizeof( unsigned char ) ) ;
	
	if(!imagine1)
	{
		return -1;
	}

	//citim matricea de pixeli

	fread(imagine,size,1,fp);
	
	fclose(fp);

	// alocam memorie pentru matricea auxiliara

	matrice=calloc(i.lungime,sizeof (int *));
	if(! matrice)
		printf("Nu s-a reusit alocarea");

	for(j=0; j< i.lungime; j++)
	{
		matrice[j]=calloc(i.latime,sizeof(int));
		if(! matrice[j])
		{
			for(k=0 ; k<j; k++)
			{
				free(matrice[k]);
			}
			free(matrice);
		}
	}

	//Task-ul 2

	// punem in matricea auxiliara pozitia pixelului din 
	//vectorul imagine cand avem pixel colorat si size+1 daca pixelul e alb si
	//retinem in matrice imaginea de la dreapta la stanga

	for(j=i.lungime -1 ; j >= 0; j--)
	{
		for(k=i.latime-1; k >= 0; k--)
		{
			if( (l-c)% (3*i.latime)==0 && l!=0)
			{
				// sarim peste padding 

				l=l+pad;
				c=c+pad;
			}

			if(imagine[l] == 255 && imagine[l+1] == 255 && imagine[l+2] == 255 )
			{
				matrice[j][k]=size+1;
			}
			else
			{
				matrice[j][k]=l;
			}			
			l=l+3;
		}
	}
	
	// cream fisierul corespunzator task-ului
	
	s[ strlen(s) -4 ]= '\0';
	strcat(s,"_task2.txt");

	fp=fopen(s,"w");

	l=0;

	// parcurgem matricea pe coloane

	for(j=0;j<i.latime;j++)
	{
		for(k=0; k<i.lungime;k++)
		{
			//daca gasim un pixel colorat
	
			if(matrice[k][j]!= size+1)
			{
				// daca matricea de 5x5 corespunde unei cifre

				if( matrice[k][j-1]== size+1 && matrice[k+1][j-1]==size+1 )
				{
					if( matrice[k+2][j-1]==size+1 && matrice[k+3][j-1]==size+1 )
					{
					
					//scriem cifra recunoscuta in vect si trecem la coloana urmatoare

						vect[l]=numar(matrice,k,j,size);
						l++;
						k=i.lungime;
					}
				}
			}
		}
	}
	
	// scriem in fisier cifrele recunoscute in ordine inversa
	
	for(j=l-1;j>=0;j--)
	{
		fprintf(fp,"%d",vect[j]);
	}
	fclose(fp);

	//Task 3

	// pentru fiecare numar citit din input
	for(e=0; e<n ;e++)
	{

		//initiliazam auxiliarul cu pixeli albi
		
		for(j=0; j<5; j++)
		{
			for(k=0; k<5; k++)
			{
			aux[j][k]=size+1;
			}
		}
		
		// parcurgem matricea pe coloane 

		for( j=0; j < i.latime-4 ; j++ )
		{
			for( k=0; k < i.lungime -4; k++ )
			{
				//daca am gasit un pixel colorat

				if(matrice[k][j]!= size+1)
				{
					//si matricea 5x5 corespunde unei cifre

					if(matrice[k][j-1]== size+1 && matrice[k+1][j-1]==size+1 )
					{
						if( matrice[k+2][j-1]==size+1 && matrice[k+3][j-1]==size+1)
						{
							//daca cifra e chiar o cifra ce trebuie eliminata

							if( numar(matrice,k,j,size) == numere[e])
							{
								// apelam functia eliminare

								eliminare(matrice,j,i.lungime,aux,size);
							}
							
							//trecem pe coloana urmatoare

							k=i.lungime;
						}
					}
				}			
			}	
		}
	}
	
	l=0;
	c=0;

	// cream noul vector imagine cu cifrele mutate

	for(j=i.lungime -1 ; j >= 0; j--)
	{
		for(k=i.latime-1; k >= 0; k--)
		{
			if( (l-c)% (3*i.latime)==0 && l!=0)
			{
				//daca am dat de bitii de padding punem 0 

				for(nr=0;nr<pad;nr++)
				{
					imagine1[l+nr]=0;
				}

				// si trecem peste

				l=l+pad;
				c=c+pad;
			}
			
			// daca in matrice avem valoarea size +1 inseamna ca avem pixel alb

			if(matrice[j][k]== size +1 )
			{
				imagine1[l]=255;

				imagine1[l+1]=255;

				imagine1[l+2]=255;
			}

			else
			{

			// altfel, punem culoarea care e in imagine la pozitia 
			//corespunzatoare valorii de pe pozitia j si k din matrice

				imagine1[l]=imagine[ matrice[j][k] ];

				imagine1[l+1]=imagine[ matrice[j][k]+1 ];

				imagine1[l+2]=imagine[ matrice[j][k]+2 ];
			}			
			l=l+3;
		}
	}
	
	//cream fisierul corespunzator task-ului si scriem imaginea

	s[ strlen(s) -5 ]= '\0';
	strcat(s,"3.bmp");

	fp=fopen(s, "wb");
	
	fwrite(&f,sizeof(fheader),1,fp);
	
	fwrite(&i,sizeof(iheader),1,fp);
	
	fwrite(imagine1,size,1,fp);

	fclose(fp);

	// Task-ul 1

	// modificam numele fisierului si apelam functia task1

	s[ strlen(s) -5 ]= '\0';

	strcat(s,"1.bmp");

	task1(f,i,imagine,size,pad,s,a,v,r);

	// eliberam memoria

	free(numere);

	free(vect);

	free(s);

	free(imagine);

	free(imagine1);

	for( j=0; j < 5 ; j++)
	{
		free(aux[j]);
	}
	free(aux);

	for( j=0; j < i.lungime; j++)
	{
		free( matrice[j] );
	}
	free(matrice);

	return 0;	
}
