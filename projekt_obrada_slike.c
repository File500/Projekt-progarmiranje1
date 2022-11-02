// C Program to read a PGMB image
// and print its parameters
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure for storing the
// image data
typedef struct PGMImage {
	char pgmType[3];
	int data[1000][1000];
	unsigned int width;
	unsigned int height;
	unsigned int maxValue;
	int data_height;
	int color_counter[256];
} PGMImage;

// Function to ignore any comments
// in file
void ignoreComments(FILE* fp)
{
	int ch;
	char line[100];

	// Ignore any blank lines
	while ((ch = fgetc(fp)) != EOF&& isspace(ch));

	// Recursively ignore comments
	// in a PGM image commented lines
	// start with a '#'
	if (ch == '#') {
		fgets(line, sizeof(line), fp);
		ignoreComments(fp);
	}
	else
		fseek(fp, -1, SEEK_CUR);
}

// Function to open the input a PGM
// file and process it
bool openPGM(PGMImage* pgm, const char* filename)
{
	// Open the image file in the
	// 'read binary' mode
	FILE* pgmfile = fopen(filename, "rb");

	// If file does not exist,
	// then return
	if (pgmfile == NULL) {
		printf("File does not exist\n");
		return false;
	}

	ignoreComments(pgmfile);
	fscanf(pgmfile, "%s", pgm->pgmType);

	ignoreComments(pgmfile);

	// Read the image dimensions
	fscanf(pgmfile, "%d %d", &(pgm->width), &(pgm->height));

	ignoreComments(pgmfile);

	// Read maximum gray value
	fscanf(pgmfile, "%d", &(pgm->maxValue));
	memset(pgm->color_counter, 0, sizeof(pgm->color_counter));
	ignoreComments(pgmfile);

	// Storing the pixel info in
	// the struct

	if (pgm->pgmType[1] == '3')
	{
		pgm->data_height = pgm->width * pgm->height;
	}else{
		pgm->data_height = pgm->height;
	}

	for (int i = 0; i < pgm->data_height; ++i) {

		for (int j = 0; j < pgm->width; ++j)
		{
			fscanf(pgmfile, "%d ", &(pgm->data[i][j]));

		}

	}

	// Close the file
	fclose(pgmfile);

	return true;
}

//print out immage data 
void print_immage(PGMImage* pgm){

	for (int i = 0; i < pgm->data_height; ++i)
	{
		for (int j = 0; j < pgm->width; ++j)
		{

			if (pgm->data[i][j] >= 100)
			{						
				printf(" %d", pgm->data[i][j]);

			}else if(pgm->data[i][j] >= 10){

				printf("  %d", pgm->data[i][j]);

			}else{

				printf("   %d", pgm->data[i][j]);
			}		
		
		}

		printf("\n");
	}
}

// Function to print the file details
void printImageDetails(PGMImage* pgm, const char* filename)
{
	FILE* pgmfile = fopen(filename, "rb");

	// Retrieving the file extension
	char* ext = strrchr(filename, '.');

	if (!ext)
		printf("No extension found" "in file %s",filename);
	else
		printf("File format" " : %s\n", ext + 1);

	printf("PGM File type : %s\n", pgm->pgmType);

	// Print type of PGM file, in ascii
	// and binary format

	printf("Width of img : %d px\n", pgm->width);
	printf("Height of img : %d px\n",pgm->height);
	printf("Max value : %d\n",pgm->maxValue);

	// close file
	fclose(pgmfile);
}

void count_color_values(PGMImage* pgm){

	//counting color values in triplets
	for (int i = 0; i < pgm->data_height; ++i)
	{
		for (int j = 0; j < pgm->width; ++j)
		{
			pgm->color_counter[pgm->data[i][j]]++;
		}
	}

	//print brojaca za test ispravnosti
	/*for (int i = 0; i <= pgm->maxValue; ++i)
	{
		printf("%d ", pgm->color_counter[i]);
	}*/

}




int main(int argc, char const* argv[])
{
	PGMImage* pgm = malloc(sizeof(PGMImage));
	char ipfile[1000];

	scanf("%s", &ipfile);
	printf("ip file : %s\n", ipfile);

	// Process the image and print
	// its details

	if(openPGM(pgm, ipfile)){

		printImageDetails(pgm, ipfile);
		print_immage(pgm);
		count_color_values(pgm);

	}
	

	return 0;
}
