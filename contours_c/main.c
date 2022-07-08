#include "contours.h"
#include <stdio.h>

int main(void) {
	int **image;
	int numrows = 0;
	int numcols = 0;

	int hierarchy_size;
	int contour_size;
	int contour_index_size;
	Point **contours;
	int *contours_index;
	Node* hierarchy;
	

	image = readFile("test1.pgm", &numrows, &numcols);

	int* im = malloc(numcols*numrows*sizeof(int));

	for (int i=0; i<numrows; i++) {
		for (int j=0; j<numcols; j++) {
			im[i*numrows+j] = image[i][j];
		}
	}

	int* res = findSCC(im, numrows, numcols);

	for (int i=0; i<numrows; i++) {
		for (int j=0; j<numcols; j++) {
			printf("%3d ", res[i*numrows+j]);
		}
		printf("\n");
	}

	// findContours(im, numrows, numcols, &hierarchy_size, &contour_size, 
	// 	&contour_index_size, &contours, &contours_index, &hierarchy);

	// assert(contours_index != NULL);

    // getExternalContours(&hierarchy_size, &contour_size, 
	// 	&contour_index_size, &contours, &contours_index, &hierarchy);

    // saveImageFile("test1.bmp", numrows, numcols, hierarchy, contours, contours_index, contour_size);	

	// // printHierarchy(hierarchy, hierarchy_size);

    // int r, c;
	// int x = 0, y = 0;
	// for (int i = 1; i < contour_size; i++) {
	// 	printf("contour %d: ", i);
	// 	x = 0;
	// 	y = 0;
	// 	for (int j = 0; j < contours_index[i]; j++) {
	// 		r = contours[i][j].row;
	// 		c = contours[i][j].col;
	// 		x += r;
	// 		y += c;
	// 		// printf("(%d,%d) ", r, c);
	// 	}
	// 	x /= contours_index[i];
	// 	y /= contours_index[i];
    //     Point c;
    //     c.row = x;
    //     c.col = y;
	// 	printf("(%d,%d) %d", c.row, c.col, contours_index[i]);
    //     // printf(" area: %d", contourArea(contours[i], c, contours_index[i]));
	// 	printf("\n");
	// }

	free(im);

	free2dArray(image, numrows);

	return 0;
}