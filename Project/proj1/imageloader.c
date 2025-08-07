/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"


//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) 
{
	//YOUR CODE HERE
	FILE *fp = fopen(filename, "r");
	if (fp == NULL) return NULL;
	uint32_t rows, cols;
	char type[10];
	if (fscanf(fp, "%s", type) != 1 || type[0] != 'P' || type[1] != '3') {fclose(fp); return NULL;}
	if (fscanf(fp, "%u %u", &cols, &rows) != 2) {fclose(fp);return NULL;}

	int max_val;
    if (fscanf(fp, "%d", &max_val) != 1 || max_val != 255) {
		fclose(fp);
        return NULL;
    }

	Image *ii = malloc(sizeof(Image));
	if (!ii) {fclose(fp);return NULL;}

	ii->cols = cols; ii->rows = rows;
	ii->image = malloc(rows * sizeof(Color *));
	if (!(ii->image)) {free(ii); fclose(fp); return NULL;}
	for (uint32_t i = 0; i < rows; i++){
		ii->image[i] = malloc(cols * sizeof(Color));
		if (!(ii->image[i])){
			for (uint32_t j = 0; j < i; j++) free(ii->image[j]);
			free(ii->image);
			free(ii);
			fclose(fp);
			return NULL;
		}
	}
	for (uint32_t i = 0; i < rows; i++){
		for (uint32_t j = 0; j < cols; j++){
			uint8_t r, g, b;
			if (fscanf(fp, "%hhu %hhu %hhu", &r, &g, &b) != 3) {
				for (uint32_t k = 0; k < rows; k++){
					free(ii->image[k]);
				}
				free(ii->image);
				free(ii);
				fclose(fp);
				return NULL;
			}
			ii->image[i][j].R = r;
			ii->image[i][j].G = g;
			ii->image[i][j].B = b;
		}
	}
	fclose(fp);
	return ii;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	//YOUR CODE HERE
	if (!image) return;
	fprintf(stdout, "P3\n%u %u\n255\n", image->cols, image->rows);
	for (uint32_t i = 0; i < image->rows; i++){
		for(uint32_t j = 0; j < image->cols; j++){
			Color *cc = &(image->image[i][j]);
			if (j == 0) {fprintf(stdout, "%3d %3d %3d", cc->R, cc->G, cc->B);}
			else {fprintf(stdout, "   %3d %3d %3d", cc->R, cc->G, cc->B);}
			
		}
		fprintf(stdout, "\n");
	}
}

//Frees an image
void freeImage(Image *image)
{
	//YOUR CODE HERE
	if(!image) return;
	for (uint32_t i = 0 ; i < image->rows; i++) free(image->image[i]);
	free (image->image);
	free (image);

}
	
