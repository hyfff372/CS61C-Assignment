/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	//YOUR CODE HERE
	Color current = image->image[row][col];
	int current_alive = (current.R == 255 && current.G == 255 && current.B == 255);
	int neighbors = 0;
	for (int dr = -1; dr <= 1; dr++)
		for (int dc = -1; dc <= 1; dc++){
			if (dr == 0 && dc == 0) continue;
			int r = (row + dr + image->rows) % image->rows;
			int c = (col + dc + image->cols) % image->cols;
			Color neighbor = image->image[r][c];
			if (neighbor.R == 255 && neighbor.G == 255 && neighbor.B == 255) neighbors++;
		}

	uint32_t alive_mask = (rule >> 9) & 0x1FF;
	uint32_t dead_mask = rule & 0x1FF;
	
	int next_alive;
	if (current_alive){
		int bit_pos = 8 - neighbors;
		next_alive = (alive_mask >> bit_pos) & 1;
	}else{
		int bit_pos = 8 - neighbors;
		next_alive = (dead_mask >> bit_pos) & 1;
	}
	Color *n = malloc(sizeof(Color));
	if (!n) return NULL;
	n->R = next_alive ? 255 : 0;
	n->G = next_alive ? 255 : 0;
	n->B = next_alive ? 255 : 0;
	return n;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
	//YOUR CODE HERE
	 if (!image) return NULL;

    // 初始化新图像结构体
    Image *new_image = malloc(sizeof(Image));
    if (!new_image) return NULL;
    new_image->rows = image->rows;
    new_image->cols = image->cols;

    // 分配行指针数组
    new_image->image = malloc(new_image->rows * sizeof(Color *));
    if (!new_image->image) {
        free(new_image);
        return NULL;
    }

	// 逐行处理像素
    for (uint32_t i = 0; i < new_image->rows; i++) {
        // 分配当前行的像素内存
        new_image->image[i] = malloc(new_image->cols * sizeof(Color));
        if (!new_image->image[i]) { // 内存分配失败时回滚
            for (uint32_t j = 0; j < i; j++) {
                free(new_image->image[j]);
            }
            free(new_image->image);
            free(new_image);
            return NULL;
        }

		// 逐像素计算下一代状态
        for (uint32_t j = 0; j < new_image->cols; j++) {
            new_image->image[i][j] = *evaluateOneCell(image, i, j, rule);
        }
    }
    return new_image;

}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
	//YOUR CODE HERE
	// 检查命令行参数数量
    if (argc != 3) {
        fprintf(stderr, "usage: ./gameOfLife filename rule\n");
        fprintf(stderr, "filename is an ASCII PPM file (type P3) with maximum value 255.\n");
        fprintf(stderr, "rule is a hex number beginning with 0x; Life is 0x1808.\n");
        exit(-1);
    }

	// 解析输入文件名和规则
    char *filename = argv[1];
    char *rule_str = argv[2];

    // 将十六进制规则字符串转换为数值（支持0x前缀）
    uint32_t rule = strtoul(rule_str, NULL, 0);
    if (rule > 0x3FFFF) { // 规则范围检查（0x00000到0x3FFFF）
        fprintf(stderr, "Error: Rule must be between 0x00000 and 0x3FFFF\n");
        exit(-1);
    }

	 // 加载输入图像
    Image *input_image = readData(filename);
    if (!input_image) {
        fprintf(stderr, "Error: Failed to read input image '%s'\n", filename);
        exit(-1);
    }

    // 计算下一代图像
    Image *output_image = life(input_image, rule);
    if (!output_image) {
        fprintf(stderr, "Error: Failed to compute next generation\n");
        freeImage(input_image);
        exit(-1);
    }

    // 输出结果图像（自动写入标准输出，重定向到文件）
    writeData(output_image);

    // 释放所有动态分配的内存
    freeImage(input_image);
    freeImage(output_image);

    return 0;
}
