#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ucitaj_sliku(int len, int hi, int slika[][len]){

	for (int i = 0; i < hi; ++i)
	{
		for (int j = 0; j < len; ++j)
		{
			scanf("%d", &slika[i][j]);
		}
	}
}

void ispisi_sliku(int len, int hi, int slika[][len]){

	for (int i = 0; i < hi; ++i)
	{
		for (int j = 0; j < len; ++j)
		{

			if (j > 0)
			{
				if (slika[i][j] - 10 >= 0)
				{
					printf(" %d", slika[i][j]);

				}else{

					printf("  %d", slika[i][j]);
				}
			}else{

				printf("%d", slika[i][j]);
			}
			
			
		}

		printf("\n");
	}
}

void promjeni_silku (int len, int hi, int slika[][len]){

	for (int i = 0; i < hi ; ++i)
	{
		for (int j = 0; j < len; ++j)
		{
			int tmp = 0;
			int broj_susjeda = 0;

			if (j - 1 >= 0)
			{
				broj_susjeda++;
				tmp += slika[i][j-1];
			}

			if (i - 1 >= 0)
			{
				broj_susjeda++;
				tmp += slika[i-1][j];
			}

			if (j + 1 < len)
			{
				broj_susjeda++;
				tmp += slika[i][j+1];
			}

			if (i + 1 < hi)
			{
				broj_susjeda++;
				tmp += slika[i+1][j];
			}

			slika[i][j] = tmp/broj_susjeda;
		}
	}
}


void alternativna_metoda(int len, int hi, int slika[][len]){

	int tmp_pic[hi][len];

		for (int i = 0; i < hi ; ++i){
		for (int j = 0; j < len; ++j)
		{
			int tmp = 0;
			int broj_susjeda = 0;

			if (j - 1 >= 0)
			{
				broj_susjeda++;
				tmp += slika[i][j-1];
			}

			if (i - 1 >= 0)
			{
				broj_susjeda++;
				tmp += slika[i-1][j];
			}

			if (j + 1 < len)
			{
				broj_susjeda++;
				tmp += slika[i][j+1];
			}

			if (i + 1 < hi)
			{
				broj_susjeda++;
				tmp += slika[i+1][j];
			}

			tmp_pic[i][j] = tmp/broj_susjeda;
		}
	}

	for (int i = 0; i < hi; ++i)
	{
		for (int j = 0; j < len; ++j)
		{
			slika[i][j] = tmp_pic[i][j];
		}
	}
}



int main()
{
	char tip_slike[2];
	int lenght, height, max_val;

	scanf("%s\n%d %d\n%d\n", &tip_slike, &lenght, &height, &max_val);
	printf("%s\n%d %d\n%d\n", tip_slike, lenght, height, max_val);

	int slika[height][lenght];

	ucitaj_sliku(lenght, height, slika);
	//promjeni_silku(lenght, height, slika);
	alternativna_metoda(lenght, height, slika);
	ispisi_sliku(lenght, height, slika);
	


	return 0;
}