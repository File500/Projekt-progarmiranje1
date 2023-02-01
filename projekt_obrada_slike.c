// C Program to read a PPM & PGM image
// and print its parameters and histograms.
// Used to normalise histograms and sharpen the image.
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <windows.h>

// Structure for storing the PPM
// image data 
typedef struct PPMImage {

	char ppmType[3];
	int data[1000][1000];
	int sharpRGB[1000][1000];

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
//function used to store histogram data in trxt files
void createHistogramFile(char tmp[][256], FILE *file, int flag){

	int yos = 2000;

	//creation of txt file and writing in it to save histogram
	if (file == NULL)
	{
		printf("File creation failed!");
		return;
	}

	if (flag == 1)
	{
		yos = 90000;
	}

	for (int i = 0; i < 181; ++i)
	{
		//checking if the sent file is for cumulative or normal histogram
		//becouse of printed numbers on the y axsis
		if (flag == 1)
		{
			if (i % 20 == 0)
			{
				if (yos == 0)
				{
					fprintf(file, "%d        |", yos);

				}else{
					fprintf(file, "%d|", yos);
				}

				yos -= 10000;
			
			}else{

				fprintf(file, "         |");
			}
		}else{

			if (i % 45 == 0)
			{
				if (yos >= 1000)
				{
					fprintf(file, "%d|", yos);

				}else if (yos == 500)
				{
					fprintf(file, "%d  |", yos);

				}else{

					fprintf(file, "%d      |", yos);
				}

				yos -= 500;
			
			}else{

				fprintf(file, "       |");
			}
		}
		
		for (int j = 0; j < 256; ++j)
		{
			fprintf(file, "%c ", tmp[i][j]);
		}

		fprintf(file, "\n");
	}

	for (int i = 0; i < 776; ++i)
	{
		fprintf(file, "-");
	}

	fprintf(file, "\n       |");
}

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
	
	if (count > 90000)
	{
		return number;
	}

	number = count / 500;
	return number;
}

void show_image_histogramsPPM(PPMImage* ppm){

	int i, j;
	int tmp_i=180, tmp_j=-1;
	char tmp[181][256];

	FILE* color_hist = fopen("RGB_histogram.txt", "w");

	//red histogram////////////////////////////////////////////////
	memset(tmp, '=', sizeof tmp);
	for (int i = 0; i < 256; ++i)
	{
		tmp[tmp_i][i] = '#';
	}

	fprintf(color_hist, "RED HISTOGRAM\n\n");
	
	printf("RED HISTOGRAM\nLegend:\n\'()\'->red value\n\'[]\'->number of scaned values in image\n\n");

	for (i = 0; i <= ppm->maxValue; ++i)
	{
		printf("(%d)", i);

		tmp_i = 179;
		tmp_j++;

		int barrier = decide_barrier(ppm->red_counter[i]);
		
		for (j = 0; j < barrier; ++j)
		{
			printf("=");
			tmp[tmp_i][tmp_j] = '#';
			tmp_i--;
		}

		printf("[%d]\n", ppm->red_counter[i]);		
		
	}

	createHistogramFile(tmp, color_hist, 0);

	//green histogram//////////////////////////////////////////////
	tmp_i=180; 
	tmp_j=-1;
	memset(tmp, '=', sizeof tmp);

	for (int i = 0; i < 256; ++i)
	{
		tmp[tmp_i][i] = '#';
	}

	fprintf(color_hist, "\n\nGREEN HISTOGRAM\n\n");
	
	printf("\nGREEN HISTOGRAM\nLegend:\n\'()\'->green value\n\'[]\'->number of scaned values in image\n\n");

	for (i = 0; i <= ppm->maxValue; ++i)
	{
		printf("(%d)", i);

		tmp_i = 179;
		tmp_j++;

		int barrier = decide_barrier(ppm->green_counter[i]);

		for (j = 0; j < barrier; ++j)
		{
			printf("#");
			tmp[tmp_i][tmp_j] = '#';
			tmp_i--;
		}

		printf("[%d]\n", ppm->green_counter[i]);
		
	}

	createHistogramFile(tmp, color_hist, 0);

	//blue histogram///////////////////////////////////////////////
	tmp_i=180; 
	tmp_j=-1;
	memset(tmp, '=', sizeof tmp);

	for (int i = 0; i < 256; ++i)
	{ 
		tmp[tmp_i][i] = '#';
	}

	fprintf(color_hist, "\n\nBLUE HISTOGRAM\n\n");
	
	printf("\nBLUE HISTOGRAM\nLegend:\n\'()\'->blue value\n\'[]\'->number of scaned values in image\n\n");

	for (i = 0; i <= ppm->maxValue; ++i)
	{	
		printf("(%d)", i);

		tmp_i = 179;
		tmp_j++;
		
		int barrier = decide_barrier(ppm->blue_counter[i]);

		for (j = 0; j < barrier; ++j)
		{
			printf("+");
			tmp[tmp_i][tmp_j] = '#';
			tmp_i--;
		}

		printf("[%d]\n", ppm->blue_counter[i]);	
		
	}

	createHistogramFile(tmp, color_hist, 0);

	fclose(color_hist);
}

void show_cumulative_image_histogramsPPM(PPMImage* ppm){

	int i, j;
	int cdfRED = 0, cdfGREEN = 0, cdfBLUE = 0;
	int tmp_i=180, tmp_j=-1;
	char tmp[181][256];

	FILE* color_cumuhist = fopen("RGB_histogram_cumulative.txt", "w");

	//red histogram////////////////////////////////////////////////
	memset(tmp, '=', sizeof tmp);
	for (int i = 0; i < 256; ++i)
	{
		tmp[tmp_i][i] = '#';
	}

	fprintf(color_cumuhist, "RED CUMULATIVE HISTOGRAM\n\n");
	
	printf("\nRED CUMULATIVE HISTOGRAM\n\n");

	for (i = 0; i <= ppm->maxValue; ++i)
	{
		cdfRED += ppm->red_counter[i];
		
		printf("(%d)", i);

		tmp_i = 179;
		tmp_j++;

		int barrierRED = decide_barrier_cumulative(cdfRED);

		for (j = 0; j < barrierRED; ++j)
		{
			printf(" ");
			tmp_i--;
		}

		tmp[tmp_i][tmp_j] = '#';

		printf("=\n");	
	}

	createHistogramFile(tmp, color_cumuhist, 1);

	//green histogram//////////////////////////////////////////////
	tmp_i=180; 
	tmp_j=-1;
	memset(tmp, '=', sizeof tmp);

	for (int i = 0; i < 256; ++i)
	{
		tmp[tmp_i][i] = '#';
	}

	fprintf(color_cumuhist, "\n\nGREEN CUMULATIVE HISTOGRAM\n\n");

	printf("\nGREEN CUMULATIVE HISTOGRAM\n\n");

	for (int i = 0; i <= ppm->maxValue; ++i)
	{
		cdfGREEN += ppm->green_counter[i];
		
		printf("(%d)", i);

		tmp_i = 179;
		tmp_j++;

		int barrierGREEN = decide_barrier_cumulative(cdfGREEN);

		for (j = 0; j < barrierGREEN; ++j)
		{
			printf(" ");
			tmp_i--;
		}

		tmp[tmp_i][tmp_j] = '#';

		printf("#\n");
	}

	createHistogramFile(tmp, color_cumuhist, 1);

	//blue histogram///////////////////////////////////////////////
	tmp_i=180; 
	tmp_j=-1;
	memset(tmp, '=', sizeof tmp);

	for (int i = 0; i < 256; ++i)
	{ 
		tmp[tmp_i][i] = '#';
	}

	fprintf(color_cumuhist, "\n\nBLUE CUMULATIVE HISTOGRAM\n\n");

	printf("\nBLUE CUMULATIVE HISTOGRAM\n\n");

	for (int i = 0; i <= ppm->maxValue; ++i)
	{
		cdfBLUE += ppm->blue_counter[i];
		
		printf("(%d)", i);

		tmp_i = 179;
		tmp_j++;
		
		int barrierBLUE = decide_barrier_cumulative(cdfBLUE);

		for (j = 0; j < barrierBLUE; ++j)
		{
			printf(" ");
			tmp_i--;
		}

		tmp[tmp_i][tmp_j] = '#';

		printf("+\n");	
	}

	createHistogramFile(tmp, color_cumuhist, 0);

	fclose(color_cumuhist);

}

void show_image_histogramPGM(PGMImage* pgm){

	printf("GRAY HISTOGRAM\nLegend:\n\'()\'->gray value\n\'[]\'->number of scaned values in image\n\n");

	int i, j, tmp_i=180, tmp_j=-1;
	char tmp[181][256];
	memset(tmp, '=', sizeof tmp);

	for (int i = 0; i < 256; ++i)
	{
		tmp[tmp_i][i] = '#';
	}

	for (i = 0; i <= pgm->maxValue; ++i)
	{
		printf("(%d)", i);

		tmp_i = 179;
		tmp_j++;

		int barrierGRAY = decide_barrier(pgm->gray_counter[i]);

		for (j = 0; j < barrierGRAY; ++j)
		{
			printf("=");
			tmp[tmp_i][tmp_j] = '#';
			tmp_i--;
		}

		printf("[%d]\n", pgm->gray_counter[i]);
	}

	FILE* gray_hist = fopen("Gray_histogram.txt", "w");
	createHistogramFile(tmp, gray_hist, 0);
	fclose(gray_hist);
}

void show_cumulative_image_histogramPGM(PGMImage* pgm){

	int cdfGRAY = 0;
	int i, j, tmp_i=180, tmp_j=-1;
	char tmp[181][256];
	memset(tmp, '=', sizeof tmp);

	pgm->min_occ = 0;

	for (int i = 0; i < 256; ++i)
	{
		tmp[tmp_i][i] = '#';
	}

	//cummulative histogram
	printf("\nGRAY CUMULATIVE HISTOGRAM\n\n");

	for (i = 0; i <= pgm->maxValue; ++i)
	{
		cdfGRAY += pgm->gray_counter[i];

		tmp_i = 179;
		tmp_j++;

		if (cdfGRAY > 0 && pgm->min_occ == 0)
		{
			pgm->min_occ = cdfGRAY;
		}

		printf("(%d)", i);

		int barrierGRAY = decide_barrier_cumulative(cdfGRAY);

		for (j = 0; j < barrierGRAY; ++j)
		{
			printf(" ");
			tmp_i--;
		}

		tmp[tmp_i][tmp_j] = '#';
		printf("=\n");	
	}

	FILE* gray_cumuhist = fopen("Gray_histogram_cumulative.txt", "w");
	createHistogramFile(tmp, gray_cumuhist, 1);
	fclose(gray_cumuhist);
}
////////////////////////////////////////////////Image Sharpening////////////////////////////////////////////////
int optimisePGM(PGMImage* pgm, int pos){

	//function used to normalize histogram and use it to 
	//sharpen the image 
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

void optimisePPM(PPMImage* ppm){

	//sharpening pixels using their neghbours
	//method used for gray image doesn't apply to RGB picture
	//so we can use alternative method 

	for (int i = 0; i < ppm->data_height; ++i)
	{
		for (int j = 0; j < ppm->data_width; ++j)
		{
			
			if ((i == 0 || i == ppm->data_height - 1) || (j < 3 || j >= ppm->data_width - 3))
			{
				ppm->sharpRGB[i][j] = ppm->data[i][j];

			}else{
				
				int current = ppm->data[i][j];
				int up = ppm->data[i-1][j];
				int down = ppm->data[i+1][j];
				int left = ppm->data[i][j-3];
				int right = ppm->data[i][j+3]; 
				
				int result = 5*current - left - right - up - down;

				if (result < 0)
				{
					result *= -1;
				}

				ppm->sharpRGB[i][j] = result;
			}
		}
	}
}

void sharpen_image_PPM(PPMImage* ppm){

	FILE* newimmage = fopen("SharpenedCOLOREDimage.ppm", "w");

	if (newimmage == NULL)
	{
		printf("File creation failed!");
		return;
	}

	optimisePPM(ppm);

	//new image creation
	fprintf(newimmage, "%s\n%d %d\n%d\n", ppm->ppmType, ppm->width, ppm->height, ppm->maxValue);

	for (int i = 0; i < ppm->data_height; ++i)
	{
		for (int j = 0; j < ppm->data_width; ++j)
		{
			int inputVal = ppm->sharpRGB[i][j];
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
			// uncomment next line to check if data was read properly
			//print_immagePPM(ppm); 
			printImageDetailsPPM(ppm);
			show_image_histogramsPPM(ppm);
			show_cumulative_image_histogramsPPM(ppm);
			sharpen_image_PPM(ppm);
		}

	}else if (strcmp(ext+1, "pgm") == 0){

		if(openPGM(pgm, ipfile)){

			// PGM
			// Proces the image and print deatails
			// uncomment next line to check if data was read properly
			//print_immagePGM(pgm); 
			printImageDetailsPGM(pgm);
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
	MessageBox(0,"To run this program successfully there are couple of rules and notes:\n\n1. You need to enter full name of picture file in the standard input (including .pgm or .ppm extention)\n 2. Picture size can be 1000x1000 maximum\n 3. Entered ppm or pgm file needs to be in the working directory where this program is located \n 4. If there is a problem with input or file the message will be presented in standard output\n 5. Histograms will be shown in standard output, new txt files will be also created to store histograms and a new sharpened picture with txt files will appear in the working directory of the program if compiled successfully\n\n To close this message and run code click OK button", "Inforamtion message", MB_OK );

	PPMImage* ppm = malloc(sizeof(PPMImage));
	PGMImage* pgm = malloc(sizeof(PGMImage));
	char ipfile[1000];

	scanf("%s", &ipfile);
	printf("ip file : %s\n", ipfile);

	chose_file(ppm, pgm, ipfile);

	return 0;
}