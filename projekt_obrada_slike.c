// C Program to read a PPM & PGM image
// and print its parameters and histograms.
// Used to normalise histograms and sharpen the image.
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Structure for storing the PPM
// image data 
typedef struct PPMImage {

	char ppmType[3];
	int data[1000][1000];
	int dataHSL[1000][1000];
	int sharpRGB[1000][1000];

	unsigned int width;
	unsigned int height;
	unsigned int maxValue;

	int data_height;
	int data_width;

	int red_counter[256];
	int min_red_occ;

	int blue_counter[256];
	int min_blue_occ;

	int green_counter[256];
	int min_green_occ;

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
	int min_occ;
} PGMImage;

// Function to ignore any comments
// in file
void ignoreComments(FILE* fp){
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

void count_color_values(PPMImage* ppm){
	int iterator = 0;
	//counting color values in triplets for each R , B and G values seperately
	for (int i = 0; i < ppm->data_height; ++i)
	{
		for (int j = 0; j < ppm->data_width; ++j)
		{

			if(iterator == 0){
				iterator++;
				ppm->red_counter[ppm->data[i][j]]++;

			}else if(iterator == 1){
				iterator++;
				ppm->green_counter[ppm->data[i][j]]++;

			}else{
				iterator = 0;
				ppm->blue_counter[ppm->data[i][j]]++;

			}
		}
	}
}

// Function to open the input a PPM
// file and process it
bool openPPM(PPMImage* ppm, const char* filename){
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

	//set the counters and data width and height in ppm picture
	memset(ppm->red_counter, 0, sizeof(ppm->red_counter));
	memset(ppm->blue_counter, 0, sizeof(ppm->blue_counter));
	memset(ppm->green_counter, 0, sizeof(ppm->green_counter));
	ppm->data_width = 3*ppm->width;
	ppm->data_height = ppm->height;

	ignoreComments(ppmfile);

	// Storing the pixel info in
	// the struct
		
	for (int i = 0; i < ppm->data_height; ++i) {

		for (int j = 0; j < ppm->data_width; ++j)
		{
			fscanf(ppmfile, "%d ", &(ppm->data[i][j]));

		}

	}

	count_color_values(ppm);

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
void printImageDetailsPPM(PPMImage* ppm){

	printf("PPM File type : %s\n", ppm->ppmType);

	// Print deatils of image

	printf("Width of img : %d px\n", ppm->width);
	printf("Height of img : %d px\n",ppm->height);
	printf("Max value : %d\n\n",ppm->maxValue);
}

//////////////////////////////////////////////////////////////////////GRAY/////////////////////////////////////
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

	count_gray_values(pgm);

	// Close the file
	fclose(pgmfile);


	return true;
}

void printImageDetailsPGM(PGMImage* pgm){

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


/////////////////////////////////////////////////////////////////Histograms///////////////////////////////////////
//Function used to optimise output based on values of counters
int decide_barrier(int count){
	int num = 180;

	if (count < 2000)
		{
			if (count > 1500)
			{
				num = 160;

			}else if (count > 1000)
			{
				num = 140;

			}else if(count > 500)
			{
				num = count/6;

			}else{
				num = count/7;
			}
		}

	return num;
}

int decide_barrier_cumulative(int count){

	int number = 180;
	
	if (count > 40000)
	{
		return number;
	}

	number = count / 223;
	return number;
}

void show_image_histogramsPPM(PPMImage* ppm){

	//red histogram
	int i, j;
	printf("RED HISTOGRAM\nLegend:\n\'()\'->red value\n\'[]\'->number of scaned values in image\n\n");

	for (i = 0; i <= ppm->maxValue; ++i)
	{
		printf("(%d)", i);

		int barrier = decide_barrier(ppm->red_counter[i]);
		

		for (j = 0; j < barrier; ++j)
		{
			printf("=");
		}

		printf("[%d]\n", ppm->red_counter[i]);		
		
	}

	//green histogram
	printf("\nGREEN HISTOGRAM\nLegend:\n\'()\'->green value\n\'[]\'->number of scaned values in image\n\n");

	for (i = 0; i <= ppm->maxValue; ++i)
	{
		printf("(%d)", i);

		int barrier = decide_barrier(ppm->green_counter[i]);

		for (j = 0; j < barrier; ++j)
		{
			printf("#");
		}

		printf("[%d]\n", ppm->green_counter[i]);
		
	}

	//blue histogram
	printf("\nBLUE HISTOGRAM\nLegend:\n\'()\'->blue value\n\'[]\'->number of scaned values in image\n\n");

	for (i = 0; i <= ppm->maxValue; ++i)
	{	
		printf("(%d)", i);
		
		int barrier = decide_barrier(ppm->blue_counter[i]);

		for (j = 0; j < barrier; ++j)
		{
			printf("+");
		}

		printf("[%d]\n", ppm->blue_counter[i]);	
		
	}


}

void show_cumulative_image_histogramPPM(PPMImage* ppm){

	int i, j;
	int cdfRED = 0, cdfGREEN = 0, cdfBLUE = 0;
	ppm->min_red_occ = ppm->min_green_occ = ppm->min_blue_occ = 0;
	//cummulative histogram
	printf("\nRED CUMULATIVE HISTOGRAM\n\n");

	for (i = 0; i <= ppm->maxValue; ++i)
	{
		cdfRED += ppm->red_counter[i];

		if (cdfRED > 0 && ppm->min_red_occ == 0)
		{
			ppm->min_red_occ = cdfRED;
		}

		//red
		printf("(%d)", i);

		int barrierRED = decide_barrier_cumulative(cdfRED);

		for (j = 0; j < barrierRED; ++j)
		{
			printf(" ");
		}

		printf("=\n");	
	}

	printf("\nGREEN CUMULATIVE HISTOGRAM\n\n");

	for (int i = 0; i <= ppm->maxValue; ++i)
	{
		cdfGREEN += ppm->green_counter[i];

		if (cdfGREEN > 0 && ppm->min_green_occ == 0)
		{
			ppm->min_green_occ = cdfGREEN;
		}

		//green
		printf("(%d)", i);

		int barrierGREEN = decide_barrier_cumulative(cdfGREEN);

		for (j = 0; j < barrierGREEN; ++j)
		{
			printf(" ");
		}

		printf("#\n");
	}

	printf("\nBLUE CUMULATIVE HISTOGRAM\n\n");

	for (int i = 0; i <= ppm->maxValue; ++i)
	{
		cdfBLUE += ppm->blue_counter[i];

		if (cdfBLUE > 0 && ppm->min_blue_occ == 0)
		{
			ppm->min_blue_occ = cdfBLUE;
		}

		//blue
		printf("(%d)", i);
		
		int barrierBLUE = decide_barrier_cumulative(cdfBLUE);

		for (j = 0; j < barrierBLUE; ++j)
		{
			printf(" ");
		}

		printf("+\n");	
	}

}

void show_image_histogramPGM(PGMImage* pgm){

	printf("GRAY HISTOGRAM\nLegend:\n\'()\'->gray value\n\'[]\'->number of scaned values in image\n\n");
	int i, j;

	for (i = 0; i <= pgm->maxValue; ++i)
	{
		printf("(%d)", i);

		int barrierGRAY = decide_barrier(pgm->gray_counter[i]);

		for (j = 0; j < barrierGRAY; ++j)
		{
			printf("=");
		}

		printf("[%d]\n", pgm->gray_counter[i]);
	}
}

void show_cumulative_image_histogramPGM(PGMImage* pgm){

	int i, j;
	int cdfGRAY = 0;
	pgm->min_occ = 0;

	//cummulative histogram
	printf("\nGRAY CUMULATIVE HISTOGRAM\n\n");

	for (i = 0; i <= pgm->maxValue; ++i)
	{
		cdfGRAY += pgm->gray_counter[i];

		if (cdfGRAY > 0 && pgm->min_occ == 0)
		{
			pgm->min_occ = cdfGRAY;
		}

		//red
		printf("(%d)", i);

		int barrierGRAY = decide_barrier_cumulative(cdfGRAY);

		for (j = 0; j < barrierGRAY; ++j)
		{
			printf(" ");
		}

		printf("=\n");	
	}

}
//////////////////////////////////////Image Sharpening////////////////////////////////////////////////////////////////////
int optimisePGM(PGMImage* pgm, int pos){

	double h_val = 0.0;
	int opti = 0;
	int cdf = 0;

	for (int i = 0; i <= pos; ++i)
	{
		cdf += pgm->gray_counter[i];
	}

	double a = (cdf - pgm->min_occ);
	double b = ((pgm->height * pgm->width) - pgm->min_occ);

	h_val =  (a / b)  * pgm->maxValue;

	h_val = round(h_val);

	opti = (int)h_val;

	return opti;
}

void sharpen_image_PGM(PGMImage* pgm){
	FILE* newimmage = fopen("SharpenedGRAYimage.pgm", "w"); 

	//temporary field for new image pix values
	int new_val_table[256];

	memset(new_val_table, 0, sizeof(new_val_table));

	if (newimmage == NULL)
	{
		printf("File creation failed!");
		return;
	}

	//normalization of values
	for (int i = 0; i <= pgm->maxValue; ++i)
	{
		int new_pix_val = optimisePGM(pgm, i);
		new_val_table[i] = new_pix_val;
	}

	//new image creation
	fprintf(newimmage, "%s\n%d %d\n%d\n", pgm->pgmType, pgm->width, pgm->height, pgm->maxValue);

	for (int i = 0; i < pgm->height; ++i)
	{
		for (int j = 0; j < pgm->width; ++j)
		{
			int inputVal = new_val_table[pgm->data[i][j]];

			fprintf(newimmage, "%d ", inputVal);
		}

		fprintf(newimmage, "\n");
	}

	fclose(newimmage);

}

//WIP

void RGB_to_HSL(PPMImage* ppm){}

int optimisePPM(PPMImage* ppm, int pos, int RGB){

	double h_val = 0.0;
	int opti = 0;
	int cdf = 0;
	double a, b;

	if (RGB == 0)
	{
		for (int i = 0; i <= pos; ++i)
		{
			cdf += ppm->red_counter[i];
		}

		a = (cdf - ppm->min_red_occ);
		b = ((ppm->height * ppm->width) - ppm->min_red_occ);

	}else if(RGB == 1){

		for (int i = 0; i <= pos; ++i)
		{
			cdf += ppm->blue_counter[i];
		}

		a = (cdf - ppm->min_green_occ);
		b = ((ppm->height * ppm->width) - ppm->min_green_occ);

	}else{

		for (int i = 0; i <= pos; ++i)
		{
			cdf += ppm->green_counter[i];
		}

		a = (cdf - ppm->min_blue_occ);
		b = ((ppm->height * ppm->width) - ppm->min_blue_occ);
	}

	h_val =  (a / b)  * ppm->maxValue;

	h_val = round(h_val);

	opti = (int)h_val;

	return opti;
}

void sharpen_image_PPM(PPMImage* ppm){

	FILE* newimmage = fopen("SharpenedCOLOREDimage.pgm", "w");

	int iterator = 0;
	int inputVal;

	//temporary fields for new image pix values
	int new_val_tableRED[256], new_val_tableGREEN[256], new_val_tableBLUE[256];

	if (newimmage == NULL)
	{
		printf("File creation failed!");
		return;
	}

	//normalization of values
	for (int i = 0; i <= ppm->maxValue; ++i)
	{
		int new_pix_val = optimisePPM(ppm, i, iterator);
		
		if (iterator == 0)
		{
			iterator++;
			new_val_tableRED[i] = new_pix_val;

		}else if (iterator == 1)
		{
			iterator++;
			new_val_tableGREEN[i] = new_pix_val; 

		}else{

			iterator = 0;
			new_val_tableBLUE[i] = new_pix_val;
		}
	}

	iterator = 0;

	//new image creation
	fprintf(newimmage, "%s\n%d %d\n%d\n", ppm->ppmType, ppm->width, ppm->height, ppm->maxValue);

	for (int i = 0; i < ppm->data_height; ++i)
	{
		for (int j = 0; j < ppm->data_width; ++j)
		{
			if (iterator == 0)
			{
				iterator++;
				inputVal = new_val_tableRED[ppm->data[i][j]];

			}else if (iterator == 1)
			{
				iterator++;
				inputVal = new_val_tableGREEN[ppm->data[i][j]];

			}else{
				iterator = 0;
				inputVal = new_val_tableBLUE[ppm->data[i][j]];
			}

			fprintf(newimmage, "%d ", inputVal);
		}

		fprintf(newimmage, "\n");
	}

	fclose(newimmage);

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Function used to determine wich file was used as an input to the program
void chose_file(PPMImage* ppm, PGMImage* pgm, char ipfile[1000]){
	// Retrieving the file extension

	char* ext = strrchr(ipfile, '.');

	if (!ext){

		printf("No extension found" "in file %s",ipfile);
		return;

	}else{
		printf("File format" " : %s\n", ext + 1);
	}


	//Check for file type
	if (strcmp(ext+1, "ppm") == 0)
	{
		if(openPPM(ppm, ipfile)){

			// PPM
			// Process the image and print details

			printImageDetailsPPM(ppm);
			//print_immagePPM(ppm); //can be used if needed to check if data was read properly
			show_image_histogramsPPM(ppm);
			show_cumulative_image_histogramPPM(ppm);
			sharpen_image_PPM(ppm);
		}

	}else if (strcmp(ext+1, "pgm") == 0){

		if(openPGM(pgm, ipfile)){

			// PGM
			// Proces the image and print deatails

			printImageDetailsPGM(pgm);
			//print_immagePGM(pgm); //can be used if needed to check if data was read properly
			show_image_histogramPGM(pgm);
			show_cumulative_image_histogramPGM(pgm);
			sharpen_image_PGM(pgm);
		}

	}else{
		printf("Wrong file entered!\nTry .ppm or .pgm files instead.");
	}

}

/////////////////////////////////////////////MAIN//////////////////////////////////////////////////////////////////
int main(int argc, char const* argv[])
{
	PPMImage* ppm = malloc(sizeof(PPMImage));
	PGMImage* pgm = malloc(sizeof(PGMImage));
	char ipfile[1000];

	scanf("%s", &ipfile);
	printf("ip file : %s\n", ipfile);

	chose_file(ppm, pgm, ipfile);

	return 0;
}
