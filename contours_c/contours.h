#ifndef _CONTOURS_H
#define _CONTOURS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

int** create2dArray(int r, int c);

void free2dArray(int **arr, int r);

typedef struct Border {
	int seq_num;
	int border_type;
} Border;

//======================Point=========================================//
typedef struct Point {
	int row;
	int col;
} Point;

bool samePoint(Point a, Point b);

void setPoint(Point *p, int r, int c);

//====================================================================//

//======================Node==========================================//
typedef struct Node {
	Border border;
	int parent;
	int first_child;
	int next_sibling;
} Node;

void setNode(Node *n, int p, int fc, int ns);

void resetNode(Node *n);
//====================================================================//

//======================NodeVector====================================//
typedef struct nodeVector {
	struct Node *vector;
	int current_max;
	int current_index;
} nodeVector;

void initNodeVector(nodeVector *nv);

void resizeNodeVector(nodeVector *nv);

void addNodeVector(nodeVector *nv, Node node);

Node* trimNodeVector(nodeVector *nv, int *vector_size);

//=====================================================================//

//=====================IntVector=======================================//
typedef struct intVector {
	int *vector;
	int current_max;
	int current_index;
} intVector;

void initIntVector(intVector *iv);

void resizeIntVector(intVector *iv);

void addIntVector(intVector *iv, int value);

int* trimIntVector(intVector *iv, int *vector_size); 

//=====================================================================//

//=====================Pixel2dArray=======================================//

typedef struct Pixel {
	unsigned char red;
	unsigned char blue;
	unsigned char green;
} Pixel;

void setPixel(Pixel *p, unsigned char r, unsigned char g, unsigned char b);

//=====================================================================//

//=====================PointVector=====================================//
typedef struct pointVector {
	struct Point *vector;
	int current_max;
	int current_index;
} pointVector;

typedef struct point2dVector {
	Point **vector;
	int current_max;
	int current_index;
} point2dVector;

void initPoint2dVector(point2dVector *p2v);

void initPointVector(pointVector *pv);

void resizePoint2dVector(point2dVector *p2v);

void resizePointVector(pointVector *pv);

void addPoint2dVector(point2dVector *p2v, Point *point_vector);

void addPointVector(pointVector *pv, Point point);

Point** trimPoint2dVector(point2dVector *p2v, int *vector_size);

Point* trimPointVector(pointVector *pv, int *vector_size);

//======================================================================//

//===========================Algorithm==================================//

//step around a pixel CCW
void stepCCW(Point *current, Point pivot);

//step around a pixel CW
void stepCW(Point *current, Point pivot);

//checks if a given pixel is out of bounds of the image
bool pixelOutOfBounds(Point p, int numrows, int numcols);

//marks a pixel as examined after passing through
void markExamined(Point mark, Point center, bool checked[4]);


//checks if given pixel has already been examined
bool isExamined(bool checked[4]);

void followBorder(int *image, int numrows, int numcols, int row, int col, Point p2, 
    Border NBD, point2dVector *contour_vector, intVector *contour_counter);
//============================================================================//

//======================InputOutputUI=========================================//
int** readFile(const char * s, int *numrows, int *numcols);

//prints the hierarchy list
void printHierarchy(Node *hierarchy, int hierarchy_size);

void drawContour(Point **contours, int *contour_index, Pixel **color, int seq_num, Pixel pix);

Pixel chooseColor(int n);

//creates a 2D array of struct Pixel, which is the 3 channel image needed to convert the 2D vector contours to a drawn bmp file
//uses DFS to step through the hierarchy tree, can be set to draw only the top 2 levels of contours, for example.
Pixel** createChannels(int h, int w, Node *hierarchy, Point **contours, int *contour_index, int contour_size);

//save image to bmp
void saveImageFile(const char * file_name, int h, int w, Node *hierarchy, 
    Point **contours, int *contour_index, int contour_size);

void findContours(int* im, 
	int numrows, 
	int numcols, 
	int *hierarchy_size,
	int *contour_size,
	int *contour_index_size,
	Point ***contours,
	int **contours_index,
	Node **hierarchy);

//==============================================================================//

#endif