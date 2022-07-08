#include "contours.h"
#include "queue.h"

// #define _CRT_SECURE_NO_WARNINGS
#define HOLE_BORDER 1
#define OUTER_BORDER 2

int** create2dArray(int r, int c) {
	int **arr;
	if ((arr = (int **)malloc(sizeof(int*)*r)) == NULL)
		perror("malloc failed");

	for (int i = 0; i < r; i++) {
		if ((arr[i] = (int*)malloc(sizeof(int)*c)) == NULL)
			perror("malloc failed");
	}

	return arr;
}

void free2dArray(int **arr, int r) {
	for (int i = 0; i < r; i++) {
		free(arr[i]);
	}
	free(arr);
}


bool samePoint(Point a, Point b) {
	return a.row == b.row && a.col == b.col;
}

void setPoint(Point *p, int r, int c) {
	assert(p);
	p->col = c;
	p->row = r;
}
//====================================================================//

//======================Node==========================================//

void setNode(Node *n, int p, int fc, int ns) {
	assert(n);
	n->parent = p;
	n->first_child = fc;
	n->next_sibling = ns;
}

void resetNode(Node *n) {
	assert(n);
	n->parent = -1;
	n->first_child = -1;
	n->next_sibling = -1;
}
//====================================================================//

//======================NodeVector====================================//

void initNodeVector(nodeVector *nv) {
	nv->current_max = 10;
	nv->current_index = 0;
	nv->vector = (Node *) malloc(sizeof(Node) * (nv->current_max));
}

void resizeNodeVector(nodeVector *nv) {
	Node *tmp;
	if ((tmp = (Node*) realloc(nv->vector, sizeof(Node) * (nv->current_max * 2))) == NULL) {
		free(nv->vector);
		perror("malloc failed");
	}
	nv->current_max *= 2;
	nv->vector = tmp;
}

void addNodeVector(nodeVector *nv, Node node) {
	if (nv->current_index + 1 >= nv->current_max)
		resizeNodeVector(nv);
	nv->vector[nv->current_index] = node;
	nv->current_index += 1;
}

Node* trimNodeVector(nodeVector *nv, int *vector_size) {
	Node *tmp;
	if ((tmp = (Node*) realloc(nv->vector, sizeof(Node) * (nv->current_index))) == NULL) {
		free(nv->vector);
		perror("malloc failed");
	}
	(*vector_size) = nv->current_index;
	return tmp;
}
//=====================================================================//

//=====================IntVector=======================================//

void initIntVector(intVector *iv) {
	iv->current_index = 0;
	iv->current_max = 10;
	iv->vector = (int*)malloc(sizeof(int) * (iv->current_max));
}

void resizeIntVector(intVector *iv) {
	int *tmp;
	if ((tmp = (int*)realloc(iv->vector, sizeof(int) * (iv->current_max * 2))) == NULL) {
		free(iv->vector);
		perror("malloc failed");
	}
	iv->current_max *= 2;
	iv->vector = tmp;
}

void addIntVector(intVector *iv, int value) {

	if (iv->current_index + 1 >= iv->current_max)
		resizeIntVector(iv);
	iv->vector[iv->current_index] = value;
	iv->current_index += 1;
}

int* trimIntVector(intVector *iv, int *vector_size) {
	int *tmp;
	if ((tmp = realloc(iv->vector, sizeof(int) * (iv->current_index))) == NULL) {
		free(iv->vector);
		perror("malloc failed");
	}
	(*vector_size) = iv->current_index;
	return tmp;
}
//=====================================================================//

//=====================Pixel2dArray=======================================//

void setPixel(Pixel *p, unsigned char r, unsigned char g, unsigned char b) {
	p->red = r;
	p->green = g;
	p->blue = b;
}
//=====================================================================//

//=====================PointVector=====================================//

void initPoint2dVector(point2dVector *p2v) {
	p2v->current_max = 10;
	p2v->current_index = 0;
	p2v->vector = (Point **) malloc(sizeof(Point*) * (p2v->current_max));
}

void initPointVector(pointVector *pv) {
	pv->current_max = 10;
	pv->current_index = 0;
	pv->vector = (Point *) malloc(sizeof(Point) * (pv->current_max));
}

void resizePoint2dVector(point2dVector *p2v) {
	Point **tmp;
	if ((tmp = (Point**) realloc(p2v->vector, sizeof(Point*) * (p2v->current_max * 2))) == NULL) {
		free(p2v->vector);
		perror("malloc failed");
	}
	p2v->current_max *= 2;
	p2v->vector = tmp;
}

void resizePointVector(pointVector *pv) {
	Point *tmp;
	if ((tmp = (Point*) realloc(pv->vector, sizeof(Point) * (pv->current_max * 2))) == NULL) {
		free(pv->vector);
		perror("malloc failed");
	}
	pv->current_max *= 2;
	pv->vector = tmp;
}

void addPoint2dVector(point2dVector *p2v, Point *point_vector) {

	if (p2v->current_index + 1 >= p2v->current_max)
		resizePoint2dVector(p2v);
	p2v->vector[p2v->current_index] = point_vector;
	p2v->current_index += 1;
}

void addPointVector(pointVector *pv, Point point) {

	if (pv->current_index + 1 >= pv->current_max)
		resizePointVector(pv);
	pv->vector[pv->current_index] = point;
	pv->current_index += 1;
}

Point** trimPoint2dVector(point2dVector *p2v, int *vector_size) {
	Point **tmp;
	if ((tmp = (Point**) realloc(p2v->vector, sizeof(Point*) * (p2v->current_index))) == NULL) {
		free(p2v->vector);
		perror("malloc failed");
	}
	(*vector_size) = p2v->current_index;
	return tmp;
}

Point* trimPointVector(pointVector *pv, int *vector_size) {
	Point *tmp;
	if ((tmp = (Point*) realloc(pv->vector, sizeof(Point) * (pv->current_index))) == NULL) {
		free(pv->vector);
		perror("malloc failed");
	}
	(*vector_size) = pv->current_index;
	return tmp;
}
//======================================================================//

//===========================Algorithm==================================//

//step around a pixel CCW
void stepCCW(Point *current, Point pivot) {
	if (current->col > pivot.col)
		setPoint(current, pivot.row - 1, pivot.col);
	else if (current->col < pivot.col)
		setPoint(current, pivot.row + 1, pivot.col);
	else if (current->row > pivot.row)
		setPoint(current, pivot.row, pivot.col + 1);
	else if (current->row < pivot.row)
		setPoint(current, pivot.row, pivot.col - 1);
}

//step around a pixel CW
void stepCW(Point *current, Point pivot) {
	if (current->col > pivot.col)
		setPoint(current, pivot.row + 1, pivot.col);
	else if (current->col < pivot.col)
		setPoint(current, pivot.row - 1, pivot.col);
	else if (current->row > pivot.row)
		setPoint(current, pivot.row, pivot.col - 1);
	else if (current->row < pivot.row)
		setPoint(current, pivot.row, pivot.col + 1);
}

//checks if a given pixel is out of bounds of the image
bool pixelOutOfBounds(Point p, int numrows, int numcols) {
	return (p.col >= numcols || p.row >= numrows || p.col < 0 || p.row < 0);
}

//marks a pixel as examined after passing through
void markExamined(Point mark, Point center, bool checked[4]) {
	//p3.row, p3.col + 1
	int loc = -1;
	//    3
	//  2 x 0
	//    1
	if (mark.col > center.col)
		loc = 0;
	else if (mark.col < center.col)
		loc = 2;
	else if (mark.row > center.row)
		loc = 1;
	else if (mark.row < center.row)
		loc = 3;

	if (loc == -1)
		perror("Error: markExamined Failed");

	checked[loc] = true;
	return;
}

//checks if given pixel has already been examined
bool isExamined(bool checked[4]) {
	//p3.row, p3.col + 1
	return checked[0];
}

void followBorder(int *image, int numrows, int numcols, int row, int col, Point p2, Border NBD, point2dVector *contour_vector, intVector *contour_counter) {
	Point current;
	setPoint(&current, p2.row, p2.col);
	Point start;
	setPoint(&start, row, col);

	//(3.1)
	//Starting from (i2, j2), look around clockwise the pixels in the neighborhood of (i, j) and find a nonzero pixel.
	//Let (i1, j1) be the first found nonzero pixel. If no nonzero pixel is found, assign -NBD to fij and go to (4).
	do {
		stepCW(&current, start);
		if (samePoint(current, p2)) {
			image[start.row*numrows+start.col] = -NBD.seq_num;
			Point *temp = (Point*)malloc(sizeof(Point));
			temp[0] = start;
			addPoint2dVector(contour_vector, temp);
			addIntVector(contour_counter, 1);
			return;
		}
	} while (pixelOutOfBounds(current, numrows, numcols) || image[current.row*numrows+current.col] == 0);
	
	pointVector point_storage;
	initPointVector(&point_storage);

	Point p1 = current;

	//(3.2)
	//(i2, j2) <- (i1, j1) and (i3, j3) <- (i, j).

	Point p3 = start;
	Point p4;
	p2 = p1;
	bool checked[4];
	while (true) {
		//(3.3)
		//Starting from the next element of the pixel(i2, j2) in the counterclockwise order, examine counterclockwise the pixels in the
		//neighborhood of the current pixel(i3, j3) to find a nonzero pixel and let the first one be(i4, j4).
		current = p2;

		for (int i = 0; i < 4; i++)
			checked[i] = false;

		do {
			markExamined(current, p3, checked);
			stepCCW(&current, p3);
		} while (pixelOutOfBounds(current, numrows, numcols) || image[current.row*numrows+current.col] == 0);
		p4 = current;

		//Change the value fi3, j3 of the pixel(i3, j3) as follows :
		//	If the pixel(i3, j3 + 1) is a 0 - pixel examined in the substep(3.3) then fi3, j3 <- - NBD.
		//	If the pixel(i3, j3 + 1) is not a 0 - pixel examined in the substep(3.3) and fi3, j3 = 1, then fi3, j3 ←NBD.
		//	Otherwise, do not change fi3, j3.

		if ((p3.col + 1 >= numcols || image[p3.row*numrows+p3.col + 1] == 0) && isExamined(checked)) {
			image[p3.row*numrows+p3.col] = -NBD.seq_num;
		}
		else if (p3.col + 1 < numcols && image[p3.row*numrows+p3.col] == 1) {
			image[p3.row*numrows+p3.col] = NBD.seq_num;
		}

		addPointVector(&point_storage, p3);
		//printImage(image, image.size(), image[0].size());
		//(3.5)
		//If(i4, j4) = (i, j) and (i3, j3) = (i1, j1) (coming back to the starting point), then go to(4);
		//otherwise, (i2, j2) <- (i3, j3), (i3, j3) <- (i4, j4), and go back to(3.3).
		if (samePoint(start, p4) && samePoint(p1, p3)) {
			int vector_size;
			Point *temp = trimPointVector(&point_storage, &vector_size);
			addPoint2dVector(contour_vector, temp);
			addIntVector(contour_counter, vector_size);
			return;
		}

		p2 = p3;
		p3 = p4;
	}
}
//============================================================================//

//======================InputOutputUI=========================================//
int** readFile(const char * s, int *numrows, int *numcols) {
	FILE * pFile;
	char header[100];

	pFile = fopen(s, "r");

	if (pFile == NULL) {
		perror("Error");
		exit(EXIT_FAILURE);
	}

	int c = 0;
	int r = 0;
	int temp = 0;
	int **image;
	if (pFile != NULL) {
		fscanf(pFile, "%s", header);
		fscanf(pFile, "%d %d", &c, &r);
		image = create2dArray(r, c);
		fscanf(pFile, "%s", header);
		for (int i = 0; i < r; i++) {
			for (int j = 0; j < c; j++) {
				fscanf(pFile, "%d", &temp);
				if (temp != 0)
					temp = 1;
				image[i][j] = temp;
			}
		}
	}

	(*numrows) = r;
	(*numcols) = c;

	return  image;
}

//prints the hierarchy list
void printHierarchy(Node *hierarchy, int hierarchy_size) {
	for (int i = 0; i < hierarchy_size; i++) {
		printf("%2d:: parent: %3d first child: %3d next sibling: %3d\n", i + 1, hierarchy[i].parent, hierarchy[i].first_child, hierarchy[i].next_sibling);
//		cout << setw(2) << i + 1 << ":: parent: " << setw(3) << hierarchy[i].parent << " first child: " << setw(3) << hierarchy[i].first_child << " next sibling: " << setw(3) << hierarchy[i].next_sibling << endl;
	}
}

void drawContour(Point **contours, int *contour_index, Pixel **color, int seq_num, Pixel pix) {
	int r, c;
	for (int i = 0; i < contour_index[seq_num]; i++) {
		r = contours[seq_num][i].row;
		c = contours[seq_num][i].col;
		color[r][c] = pix;
	}
}

Pixel chooseColor(int n) {

	Pixel tmp;
	switch (n % 6) {
	case 0:
		setPixel(&tmp, 255, 0, 0);
		return tmp;
	case 1:
		setPixel(&tmp, 255, 127, 0);
		return tmp;
	case 2:
		setPixel(&tmp, 255, 255, 0);
		return tmp;
	case 3:
		setPixel(&tmp, 0, 255, 0);
		return tmp;
	case 4:
		setPixel(&tmp, 0, 0, 255);
		return tmp;
	default:
		setPixel(&tmp, 139, 0, 255);
		return tmp;
	}
}

//creates a 2D array of Pixel, which is the 3 channel image needed to convert the 2D vector contours to a drawn bmp file
//uses DFS to step through the hierarchy tree, can be set to draw only the top 2 levels of contours, for example.
Pixel** createChannels(int h, int w, Node *hierarchy, Point **contours, int *contour_index, int contour_size) {

	Pixel **color;

	if ((color = (Pixel**)malloc(sizeof(Pixel*)*h)) == NULL)
		perror("malloc failed");

	for (int i = 0; i < h; i++) {
		if ((color[i] = (Pixel*)malloc(sizeof(Pixel)*w)) == NULL)
			perror("malloc failed");
		memset(color[i], 0, sizeof(Pixel)*w);
	}
	
	for (int i = 1; i < contour_size; i++) {
		drawContour(contours, contour_index, color, i, chooseColor(i));
	}

	return color;
}

//save image to bmp
void saveImageFile(const char * file_name, int h, int w, Node *hierarchy, Point **contours, int *contour_index, int contour_size) {
	FILE *f;

	Pixel **color = createChannels(h, w, hierarchy, contours, contour_index, contour_size);

	unsigned char *img = NULL;
	int filesize = 54 + 3 * w*h;  //w is your image width, h is image height, both int

	img = (unsigned char *)malloc(3 * w*h);
	memset(img, 0, 3 * w*h);
	int x, y;
	unsigned char r, g, b;
	for (int i = 0; i<h; i++) {
		for (int j = 0; j<w; j++) {
			y = (h - 1) - i; x = j;
			r = color[i][j].red;
			g = color[i][j].green;
			b = color[i][j].blue;
			/*	        if (r > 255) r=255;
			if (g > 255) g=255;
			if (b > 255) b=255;*/
			img[(x + y * w) * 3 + 2] = (unsigned char)(r);
			img[(x + y * w) * 3 + 1] = (unsigned char)(g);
			img[(x + y * w) * 3 + 0] = (unsigned char)(b);
		}
	}

	unsigned char bmpfileheader[14] = { 'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0 };
	unsigned char bmpinfoheader[40] = { 40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0 };
	unsigned char bmppad[3] = { 0,0,0 };

	bmpfileheader[2] = (unsigned char)(filesize);
	bmpfileheader[3] = (unsigned char)(filesize >> 8);
	bmpfileheader[4] = (unsigned char)(filesize >> 16);
	bmpfileheader[5] = (unsigned char)(filesize >> 24);

	bmpinfoheader[4] = (unsigned char)(w);
	bmpinfoheader[5] = (unsigned char)(w >> 8);
	bmpinfoheader[6] = (unsigned char)(w >> 16);
	bmpinfoheader[7] = (unsigned char)(w >> 24);
	bmpinfoheader[8] = (unsigned char)(h);
	bmpinfoheader[9] = (unsigned char)(h >> 8);
	bmpinfoheader[10] = (unsigned char)(h >> 16);
	bmpinfoheader[11] = (unsigned char)(h >> 24);

	f = fopen(file_name, "wb");
	fwrite(bmpfileheader, 1, 14, f);
	fwrite(bmpinfoheader, 1, 40, f);
	for (int i = 0; i<h; i++) {
		fwrite(img + (w*(i) * 3), 3, w, f);
		fwrite(bmppad, 1, (4 - (w * 3) % 4) % 4, f);
	}

	free(img);
	for (int i = 0; i < h; i++) {
		free(color[i]);
	}
	free(color);
	fclose(f);
}

void findContours(int* im, 
	int numrows, 
	int numcols, 
	int *hierarchy_size,
	int *contour_size,
	int *contour_index_size,
	Point ***contours,
	int **contours_index,
	Node **hierarchy) {

	Border NBD;
	Border LNBD;

	LNBD.border_type = HOLE_BORDER;
	NBD.border_type = HOLE_BORDER;
	NBD.seq_num = 1;

	nodeVector hierarchy_vector;
	initNodeVector(&hierarchy_vector);
	Node temp_node;
	resetNode(&temp_node);
	temp_node.border = NBD;
	addNodeVector(&hierarchy_vector, temp_node);

	//add in padding for both contour and hierarchy have the same offset.
	point2dVector contour_vector;
	initPoint2dVector(&contour_vector);
	Point border;
	setPoint(&border, -1, -1);
	Point *padding;
	padding = (Point*)malloc(sizeof(Point));
	padding[0] = border;
	addPoint2dVector(&contour_vector, padding);

	intVector contour_counter;
	initIntVector(&contour_counter);
	addIntVector(&contour_counter, 1);

	Point p2;
	bool border_start_found;

	for (int r = 0; r < numrows; r++) {
		LNBD.seq_num = 1;
		LNBD.border_type = HOLE_BORDER;
		for (int c = 0; c < numcols; c++) {
			border_start_found = false;
			//Phase 1: Find border
			//If fij = 1 and fi, j-1 = 0, then decide that the pixel (i, j) is the border following starting point
			//of an outer border, increment NBD, and (i2, j2) <- (i, j - 1).
			if ((im[r*numrows+c] == 1 && c - 1 < 0) || (im[r*numrows+c] == 1 && im[r*numrows+c - 1] == 0)) {
				NBD.border_type = OUTER_BORDER;
				NBD.seq_num += 1;
				setPoint(&p2, r, c - 1);
				border_start_found = true;
			}

			//Else if fij >= 1 and fi,j+1 = 0, then decide that the pixel (i, j) is the border following
			//starting point of a hole border, increment NBD, (i2, j2) ←(i, j + 1), and LNBD ← fij in case fij > 1.
			else if (c + 1 < numcols && (im[r*numrows+c] >= 1 && im[r*numrows + c + 1] == 0)) {
				NBD.border_type = HOLE_BORDER;
				NBD.seq_num += 1;
				if (im[r*numrows+c] > 1) {
					LNBD.seq_num = im[r*numrows+c];
					LNBD.border_type = hierarchy_vector.vector[LNBD.seq_num - 1].border.border_type;
				}
				setPoint(&p2, r, c + 1);
				border_start_found = true;
			}

			if (border_start_found) {
				//Phase 2: Store Parent

				//current = new TreeNode(NBD);
				resetNode(&temp_node);
				if (NBD.border_type == LNBD.border_type) {
					temp_node.parent = hierarchy_vector.vector[LNBD.seq_num - 1].parent;
					temp_node.next_sibling = hierarchy_vector.vector[temp_node.parent - 1].first_child;
					hierarchy_vector.vector[temp_node.parent - 1].first_child = NBD.seq_num;
					temp_node.border = NBD;
					addNodeVector(&hierarchy_vector, temp_node);

					//cout << "indirect: " << NBD.seq_num << "  parent: " << LNBD.seq_num <<endl;
				}
				else {
					if (hierarchy_vector.vector[LNBD.seq_num - 1].first_child != -1) {
						temp_node.next_sibling = hierarchy_vector.vector[LNBD.seq_num - 1].first_child;
					}

					temp_node.parent = LNBD.seq_num;
					hierarchy_vector.vector[LNBD.seq_num - 1].first_child = NBD.seq_num;
					temp_node.border = NBD;
					addNodeVector(&hierarchy_vector, temp_node);
					//cout << "direct: " << NBD.seq_num << "  parent: " << LNBD.seq_num << endl;
				}

				//Phase 3: Follow border
				followBorder(im, numrows, numcols, r, c, p2, NBD, &contour_vector, &contour_counter);
			}

			//Phase 4: Continue to next border
			//If fij != 1, then LNBD <- abs( fij ) and resume the raster scan from the pixel(i, j + 1).
			//The algorithm terminates when the scan reaches the lower right corner of the picture.
			if (abs(im[r*numrows+c]) > 1) {
				LNBD.seq_num = abs(im[r*numrows+c]);
				LNBD.border_type = hierarchy_vector.vector[LNBD.seq_num - 1].border.border_type;
			}
		}
	}


	*contours = trimPoint2dVector(&contour_vector, contour_size);
	*contours_index = trimIntVector(&contour_counter, contour_index_size);
	*hierarchy = trimNodeVector(&hierarchy_vector, hierarchy_size);

	assert(contours_index != NULL);

	if (*hierarchy_size != *contour_index_size || *hierarchy_size != *contour_size)
		printf("Storage offset error");
}

void getExternalContours(int *hierarchy_size,
	int *contour_size,
	int *contour_index_size,
	Point ***contours,
	int **contours_index,
	Node **hierarchy) {

	printHierarchy(*hierarchy, *hierarchy_size);

	// sizes
	int new_contour_size = 0;
	int new_contour_index_size = 0;
	int new_hierarchy_size = 0;
	int *new_contours_index = malloc(sizeof(int));
	*new_contours_index = (*contours_index)[0];

	int* indices = NULL;

	// get head of new lists
	Node* hierarchyHead = *hierarchy;
	Point** contoursHead = *contours;

	// get new size
	for (int i = 0; i<*hierarchy_size; i++) {
		// if highest on hierarchy link to head
		if ((*hierarchy)[i].parent == 1 || (*hierarchy)[i].parent == -1) {
				// increment sizes
				new_contour_index_size++;
				new_contour_size++;
				new_hierarchy_size++;
			}
	}

	// allocate new containers
	new_contours_index = malloc(new_contour_size*sizeof(int));
	hierarchyHead = malloc(new_hierarchy_size*sizeof(Node));
	contoursHead = malloc(new_contour_size*sizeof(Point*));
	indices = malloc(new_contour_size*sizeof(int));

	// copy over relevant contours
	for (int i=0, j=0; i<*contour_size; i++) {
		if ((*hierarchy)[i].parent == 1 || (*hierarchy)[i].parent == -1) {
			new_contours_index[j] = (*contours_index)[i];
			indices[j++] = i;
		}
	}

	// copy over points and hierarchies
	for (int i=0; i<new_contour_size; i++) {
		hierarchyHead[i] = (*hierarchy)[indices[i]];
		contoursHead[i] = malloc(new_contours_index[i]*sizeof(Point));
		memcpy(contoursHead[i], (*contours)[indices[i]], new_contours_index[i]*sizeof(Point));
	}

	// set new sizes
	*contour_size = new_contour_size;
	*hierarchy_size = new_hierarchy_size;
	*contour_index_size = new_contour_index_size;

	// check for same size
	if (*hierarchy_size != *contour_index_size || *hierarchy_size != *contour_size)
		printf("Storage offset error");

	// set new points and hierarchies
	*contours = contoursHead;
	*hierarchy = hierarchyHead;
}

int contourArea(const Point* contour, 
	const Point center, int length) {

	// TODO: use BFS to get number of enclosed points

	return 0;
}

//==============================================================================//