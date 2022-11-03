// C Program to read a PPM image
// and print its parameters
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure for storing the PPM
// image data 
typedef struct PPMImage {

	char ppmType[3];
	int data[1000][1000];

	unsigned int width;
	unsigned int height;
	unsigned int maxValue;

	int data_height;
	int data_width;

	int red_counter[256];
	int blue_counter[256];
	int green_counter[256];

} PPMImage;

// Structure for storing the PGM
// image data 
typedef struct PGMImage{

	char pgmType[3];
	int data[1000][1000];

	unsigned int width;
	unsigned int height;
	unsigned int maxValue;

	int gray_counter[256];
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
	// in a PPM image commented lines
	// start with a '#'
	if (ch == '#') {
		fgets(line, sizeof(line), fp);
		ignoreComments(fp);
	}
	else
		fseek(fp, -1, SEEK_CUR);
}

///////////////////////////////////////////////////////////RGB/////////////////////////////////////////////////

// Function to open the input a PPM
// file and process it
bool openPPM(PPMImage* ppm, const char* filename)
{
	// Open the image file in the
	// 'read binary' mode
	FILE* ppmfile = fopen(filename, "rb");

	// If file does not exist,
	// then return
	if (ppmfile == NULL) {
		printf("File does not exist\n");
		return false;
	}

	ignoreComments(ppmfile);
	fscanf(ppmfile, "%s", ppm->ppmType);

	ignoreComments(ppmfile);

	// Read the image dimensions
	fscanf(ppmfile, "%d %d", &(ppm->width), &(ppm->height));

	ignoreComments(ppmfile);

	// Read maximum value
	fscanf(ppmfile, "%d", &(ppm->maxValue));

	//set the counters and fixated data width in ppm picture
	memset(ppm->red_counter, 0, sizeof(ppm->red_counter));
	memset(ppm->blue_counter, 0, sizeof(ppm->blue_counter));
	memset(ppm->green_counter, 0, sizeof(ppm->green_counter));
	ppm->data_width = 3;

	ignoreComments(ppmfile);

	// Storing the pixel info in
	// the struct

	if (ppm->ppmType[1] == '3')
	{
		ppm->data_height = ppm->width * ppm->height;
	}else{
		ppm->data_height = ppm->height;
	}

	for (int i = 0; i < ppm->data_height; ++i) {

		for (int j = 0; j < ppm->data_width; ++j)
		{
			fscanf(ppmfile, "%d ", &(ppm->data[i][j]));

		}

	}

	// Close the file
	fclose(ppmfile);

	return true;
}

//print out immage data 
void print_immagePPM(PPMImage* ppm){

	for (int i = 0; i < ppm->data_height; ++i)
	{
		for (int j = 0; j < ppm->data_width; ++j)
		{

			if (ppm->data[i][j] >= 100)
			{						
				printf(" %d", ppm->data[i][j]);

			}else if(ppm->data[i][j] >= 10){

				printf("  %d", ppm->data[i][j]);

			}else{

				printf("   %d", ppm->data[i][j]);
			}		
		
		}

		printf("\n");
	}
}

// Function to print the file details
void printImageDetailsPPM(PPMImage* ppm, const char* filename)
{

	printf("PPM File type : %s\n", ppm->ppmType);

	// Print deatils of image

	printf("Width of img : %d px\n", ppm->width);
	printf("Height of img : %d px\n",ppm->height);
	printf("Max value : %d\n\n",ppm->maxValue);

}

void count_color_values(PPMImage* ppm){

	//counting color values in triplets for each R , B and G values seperately
	for (int i = 0; i < ppm->data_height; ++i)
	{
		for (int j = 0; j < ppm->data_width; ++j)
		{
			if(j == 0){

				ppm->red_counter[ppm->data[i][j]]++;

			}else if(j == 1){

				ppm->blue_counter[ppm->data[i][j]]++;

			}else{

				ppm->green_counter[ppm->data[i][j]]++;

			}
		}
	}

}
//////////////////////////////////////////////////////////////////////GRAY/////////////////////////////////////
bool openPGM(PGMImage* pgm, const char* filename){

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

	// Read maximum value
	fscanf(pgmfile, "%d", &(pgm->maxValue));

	//set the counter in pgm picture
	memset(pgm->gray_counter, 0, sizeof(pgm->gray_counter));

	ignoreComments(pgmfile);

	// Storing the pixel info in
	// the struct

	for (int i = 0; i < pgm->height; ++i) {

		for (int j = 0; j < pgm->width; ++j)
		{
			fscanf(pgmfile, "%d ", &(pgm->data[i][j]));

		}

	}

	// Close the file
	fclose(pgmfile);


	return true;
}

void printImageDetailsPGM(PGMImage* pgm, const char* filename){

	printf("PPM File type : %s\n", pgm->pgmType);

	// Print deatils of image

	printf("Width of img : %d px\n", pgm->width);
	printf("Height of img : %d px\n",pgm->height);
	printf("Max value : %d\n\n",pgm->maxValue);

}

void print_immagePGM(PGMImage* pgm){

	for (int i = 0; i < pgm->height; ++i)
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

	printf("\n");

}

void count_gray_values(PGMImage* pgm){

	//counting gray values
	for (int i = 0; i < pgm->height; ++i)
	{
		for (int j = 0; j < pgm->width; ++j)
		{
			pgm->gray_counter[pgm->data[i][j]]++;
		}
	}

}
/////////////////////////////////////////////////////////////////Histograms///////////////////////////////////////
//WIP
void show_image_histogramsPPM(){}

void show_image_histogramsPGM(PGMImage* pgm){

	printf("GRAY HISTOGRAM\nLegend:\n\'()\'->gray value\n\'[]\'->number of scaned values in image\n\n");
	int i, j;

	for (i = 0; i <= pgm->maxValue; ++i)
	{
		printf("(%d)", i);

		for (j = 0; j < pgm->gray_counter[i]; ++j)
		{
			printf("+");
		}

		printf("[%d]\n", j);
	}
}

void sharpen_image_PPM(){}
void sharpen_image_PGM(){}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int main(int argc, char const* argv[])
{
	PPMImage* ppm = malloc(sizeof(PPMImage));
	PGMImage* pgm = malloc(sizeof(PGMImage));
	char ipfile[1000];

	scanf("%s", &ipfile);
	printf("ip file : %s\n", ipfile);

	// Retrieving the file extension

	char* ext = strrchr(ipfile, '.');

	if (!ext)
		printf("No extension found" "in file %s",ipfile);
	else
		printf("File format" " : %s\n", ext + 1);


	//Check for file type
	if (strcmp(ext+1, "ppm") == 0)
	{
		if(openPPM(ppm, ipfile)){

			// PPM
			// Process the image and print details
			printImageDetailsPPM(ppm, ipfile);
			print_immagePPM(ppm);
			count_color_values(ppm);

		}

	}else if (strcmp(ext+1, "pgm") == 0){

		if(openPGM(pgm, ipfile)){

			// PGM
			// Proces the image and print deatails
			printImageDetailsPGM(pgm, ipfile);
			print_immagePGM(pgm);
			count_gray_values(pgm);
			show_image_histogramsPGM(pgm);
		}

	}else{
		printf("Wrong file entered!\nTry .ppm or .pgm files instead.");
	}
	
	

	return 0;
}
