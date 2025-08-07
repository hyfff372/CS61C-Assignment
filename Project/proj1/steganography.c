/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				YOUR NAME HERE
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This should not affect Image, and should allocate space for a new Color.
Color *evaluateOnePixel(Image *image, int row, int col)
{
	//YOUR CODE HERE
	if (!image) return NULL;
	if (!image->image) return NULL;
	if (!image->image[row]) return NULL;

	uint8_t c = (image->image[row][col].B)&1;
	Color *new_color = malloc(sizeof(Color));
	if (c){
		new_color->R = 255;
        new_color->G = 255;
		new_color->B = 255;
	} else{
		new_color->R = 0;
		new_color->G = 0;
		new_color->B = 0;
	}
	return new_color;
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
{
	//YOUR CODE HERE
	if (!image) return NULL;

	Image *d = malloc(sizeof(Image));
	if (!d) return NULL;
	d->rows = image->rows;
	d->cols = image->cols;

	d->image = malloc((d->rows) * sizeof(Color*));
	if (!d->image){
		free(d);
		return NULL;
	}
	for (uint32_t i = 0; i < d->rows; i++){
		d->image[i] = malloc(d->cols * sizeof(Color));
		if (!d->image[i]){
			for (uint32_t j = 0; j < i; j++){
				free(d->image[j]);
			}
			free(d->image);
			free(d);
			return NULL;
		}
		for (uint32_t j = 0; j < d->cols; j++){
			d->image[i][j] = *(evaluateOnePixel(image, i, j));
		}
	}
	return d;



}

/*
Loads a file of ppm P3 format from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a file of ppm P3 format (not necessarily with .ppm file extension).
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
int main(int argc, char **argv)
{
	//YOUR CODE HERE
	if (argc != 2){
		fprintf(stderr, "Error: Failed to read input image.\n");
        exit(-1);
	}

	Image *input_img = readData(argv[1]);
	if (!input_img){
		//fprintf(stderr, "Error: Failed to read input image.\n");
        exit(-1);
	} 
	
	Image *d = steganography(input_img);
	if (!d){
		fprintf(stderr, "Error: Failed to decode image.\n");
        freeImage(input_img);
        exit(-1);
	}

	writeData(d);
	freeImage(input_img);
    freeImage(d);
	return 0;
}
