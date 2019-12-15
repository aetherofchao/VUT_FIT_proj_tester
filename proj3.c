/*
 *
 * 	projekt 3
 *	main.c
 *	
 *	Alukard 
 *	04.12.2019
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define pmesg(s, ...) fprintf(stderr, __FILE__":%u: " s "\n", __LINE__,__VA_ARGS__ )
#define INITSIZE 5
#define HELP "Usage: \n  ./proj3 [OPTION]... [FILE]...\n""\n""Optins:\n""\n""--help\n\t způsobí, že program vytiskne nápovědu používání programu a skončí.\n""--test\n\t pouze zkontroluje, že soubor daný druhým argumentem \n\t ""programu obsahuje řádnou definici mapy bludiště.\n\t ""V případě, že formát obrázku odpovídá definici (viz níže), vytiskne Valid. \n\t ""V opačném případě (např. ilegálních znaků,\n\t ""chybějících údajů nebo špatných hodnot) program tiskne Invalid.\n""--rpath R C\n\t hledá průchod bludištěm na vstupu na řádku R a sloupci C.\n\t"" Průchod hledá pomocí pravidla pravé ruky (pravá ruka vždy na zdi).\n""--lpath R C\n\t hledá průchod bludištěm na vstupu na řádku R a sloupci C,\n\t ""ale za pomoci pravidla levé ruky.\n""--shortest R C (prémiové řešení) \n\t hledá nejkratší cestu z bludiště při vstupu na řádku R a sloupci C.\n""--print\n\t vytiskne shity ascii reprezentaci bludiste\n"
#define USAGE ".proj3: bad usage\n""Usage: \n  ./proj3 [OPTION]... [FILE]...\n""Try './proj3 --help' for more information.\n"


typedef struct 
{
	int rows;
	int cols;
	unsigned char *cells;
} Map;

typedef struct 
{
	int row;
	int col;
	int dir;
} Pointer;


bool isborder(Map *map, int r, int c, int border);

int loadMap(Map *map, char *path);

int desttructMap(Map *map);

int getNum (char *str);

int testMap (Map *map);

int start_border(Map *map, int r, int c, int leftright);

int leftRightStep(Map *map, Pointer *point, bool leftright);

int tryToGo(Map *map, Pointer *from, int to);


// shortest fc
typedef struct 
{
	Map *map;
	unsigned int *distance;
	int *inR;
	int *inC;
	int pntIn;
	int *outR;
	int *outC;
	int pntOut;
	int bufferLen;
	unsigned int weight;
} Vector;

int shortest (Map *map, int c, int r);

int quickSolve (Vector *v);

Vector VectorInit(Map *map, int r, int c);

void VectorDstr(Vector *v);

int VectorAdd(Vector *v, int r, int c, bool mode);

bool VectorBasecase(Vector *v, bool b, int r, int c);

void MarkNode(Map *map, int r, int c);

int backTrace (Vector *v);


// for the purpus of degub only
void printMap(Map *map);

/*****************
 *	main
 */
int main(int argc, char **argv)
{	
	bool testFlag = 0;
	int pathFlag = -1;
	bool printFlag = 0;
	int R, C;

	//parse argv
	if (argc < 2) 
	{
		fprintf(stderr, USAGE);
		return -1;
	}
	for (int i = 1; i< argc; i++)
	{
		if (!strcmp("--help",argv[i])) 
		{
			printf(HELP);
			return 0;
		}
		else if (!strcmp("--print",argv[i])) 
			printFlag = 1;

		else if (!strcmp("--test",argv[i])) 
			testFlag = 1;
		
		else if (!strcmp("--lpath",argv[i]))
		{
			pathFlag = 1;
			R = getNum(argv[++i]);
			C = getNum(argv[++i]);
		}
		else if (!strcmp("--rpath",argv[i]))
		{
			pathFlag = 0;
			R = getNum(argv[++i]);
			C = getNum(argv[++i]);
		}
		else if (!strcmp("--shortest",argv[i])) 
		{
			pathFlag = 2;
			R = getNum(argv[++i]);
			C = getNum(argv[++i]);
		}
		else if (argv[i][0] != '-')
		{
			Map map;

			int load = loadMap(&map, argv[i]);
			if (!load) return 0;
			if (load < 0)
			{
				if (testFlag) printf("Invalid\n");
				desttructMap(&map);
				return -1;
			}

			int Valid = testMap(&map);
			if (!Valid) 
			{
				printf("Invalid\n");
				desttructMap(&map);
				return -1;
			}
			if (testFlag) printf("Valid\n");

			if (printFlag) printMap(&map);

			switch(pathFlag)
			{
				case 0:
				case 1:
					start_border(&map, R, C, pathFlag);
					break;
				case 2:
					shortest(&map,R, C);
					if (printFlag) printMap(&map);
				default:
					break;
			}


			desttructMap(&map);
		}
		else 
		{
			fprintf(stderr, USAGE);
			return -1;
		}
	}
}

/*****************
 *	exits program on err
 *	called for parsing numbers
 *	@param str number
 *	@retrn number
 */
int getNum(char *str)
{
	char *next;
	int val = strtol (str, &next, 10);

	// Check for empty string and characters left after conversion.
	if (next == str || (*next != '\0'))
		return -1;
	if (val < 0)
		return -1;
	return val;
	}

/*****************
 * 	loads map 
 *	@param map load into
 * 	@param path from what file
 *	@retrn susces
 */
int loadMap(Map *map, char *path)
{
	FILE *file = fopen(path, "r");
	if (!file) 
	{
		fprintf(stderr, "cannot access %s: No such file or directory\n", path);
		return 0;
	}
	
	// init map
	char srow[1024]; char scol[1024];
	fscanf(file,"%s %s ",srow, scol);

	map->rows = getNum(srow);
	if (map->rows < 0) return -1;
	map->cols = getNum(scol);
	if (map->cols < 0) return -1;
	
	map->cells = malloc(sizeof(unsigned char) * map->rows * map->cols);
	if(!map->cells) 
	{
		fprintf(stderr, "could not load malloc\n");
		return -2;
	}

	// read the file
	for(int row = 0; row < map->rows; row++)
	{
		for(int col = 0; col < map->cols; col++)
		{
			map->cells[map->cols * row + col] =(unsigned char)fgetc(file) % 8 ;
			if(fgetc(file) == EOF) 
			{
				fclose(file);
				return 1;
			};
		}
	}
	fclose(file);
	return 1;
}

int desttructMap(Map *map)
{
	free(map->cells);
	map = NULL;

	return 1;
}

/*****************
 * @param border 0 left; 1 down; 2 right; 3 up		  
 * @return	is border / is not border
 */
bool isborder(Map *map, int r, int c,  int border)
{
	// 3 + 1 = 1
	switch (border%4)
	{
		case 0:
			return (map->cells[map->cols * r + c] >> 0) & 1;
		case 1:
			if (r%2 == c%2) return true;
			else return (map->cells[map->cols * r + c] >> 2) & 1;
		case 2:
			return (map->cells[map->cols * r + c] >> 1) & 1;
		case 3:
			if (r%2 != c%2) return true;
			else return (map->cells[map->cols * r + c] >> 2) & 1;
	}
	return false; // avoid warnig 
}

/*****************
 * 	tests map
 *	@param map
 *	@retrn susces 
 */
int testMap (Map *map)
{
	for(int row = 0; row < map->rows -1; row++)
	{
		for(int col = 0; col < map->cols -1; col++)
		{
			if (isborder(map,row,col,1) != isborder(map,row +1,col,3)||
				isborder(map,row,col,2) != isborder(map,row,col +1,0))
				return 0;
		}
	}
	return 1;
}

/*****************
 * 	starts left / right algorithm 
 * 	@param leftright: true -> left / false -> right
 * 	@return fail / path found
 */
int start_border(Map *map, int r, int c, int leftright)
{
	// the value of cell is 7
	if (isborder(map,--r,--c,0) && 
		isborder(map,r,c,1) && 
		isborder(map,r,c,2) && 
		isborder(map,r,c,3) )
		return 0;

	Pointer point = {r,c,0};

	if (leftright) // left
	{
		if (!c) point.dir = (r%2) ? 0 : 3; 			// case 1,2
		else if (c == map->cols-1) point.dir = 1;	// case 5,6
		else if (!r) point.dir = 3; 				// case 3
		else if (r == map->rows-1) point.dir = 1 ;	// case 4

		return leftRightStep(map, &point, true);
	}
	else // right
	{
		if (!c) point.dir = (r%2) ? 1 : 0; 			// case 1,2
		else if (c == map->cols-1) point.dir = 2;	// case 5,6
		else if (!r) point.dir = 3; 				// case 3
		else if (r == map->rows-1) point.dir = 1 ;	// case 4
		
		if (leftRightStep(map, &point, false))
		{
			printf("\n");
			return 1;
		}
		return 0;
	}
}

/*****************
 * 	tryes to enter node
 * 	@param map map
 *	@param from point of entry
 *	@param to direction of next entry
 *	@retrn susces
 */
int tryToGo(Map *map, Pointer *from, int to)
{
	if (isborder(map, from->row, from->col, to)) return 0;
	
	switch(to%4)
	{
		case 0:
			from->col -= 1; 
			break;
		case 1:
			from->row += 1; 
			break;
		case 2:
			from->col += 1; 
			break;
		case 3:
			from->row -= 1; 
			break;
	}
	from->dir = (to+2)%4;
	return 1;
}

/*****************
 * 	next step 
 *	@param map map
 *	@param point point of entry to the node
 *	@param leftright left / right
 * 	@retrn susces
 */
int leftRightStep(Map *map, Pointer *point, bool leftright)
{
	// basecase: hiting a border
	if (point->row == -1 || point->col == -1 || 
		point->row == map->rows || point->col == map->cols) 
		return 1;

	// print the path 
	printf("%i,%i\n",point->row+1, point->col+1);

	for (int i = 1; i < 5; i++) 
		if(tryToGo(map, point, point->dir + ((leftright) ? (4-i) : (i)) )) 
		// recursion
		return leftRightStep(map, point, leftright);
	
	return -1; 
}

/*****************
 * 	prints the map
 *	@param map
 */
void printMap(Map *map) 
{
	printf("--Map--------------------------\n|\t");
	for(int col = 0; col < map->cols; col++)
	{
		if(isborder(map,0,col,3)) printf("_");else printf(" ");
		if(isborder(map,0,col,3)) printf("_");else printf(" ");
	}	
	printf("\n|\t");
	for(int row = 0; row < map->rows; row++)
	{
		if(isborder(map,row,0,0)) printf("|");else printf(" ");
		for(int col = 0; col < map->cols; col++)
		{
			if((map->cells[map->cols * row + col] > 7)) 
			{
				int tmpi =  map->cells[map->cols * row + col];
				if (tmpi < 10) printf(" %i",tmpi);
				else printf("%i",tmpi);
			}
			else if(isborder(map,row,col,1)) printf("__");else printf("  ");
			
			if(isborder(map,row,col,2)) printf("|");else if(isborder(map,row,col,1)) printf("_"); 
			else printf(" ");
		}	
		printf("\n|\t");
	}
	printf("\n-------------------------------\n");
}

/*****************
 *	shortest
 */
int shortest (Map *map, int r, int c)
{
	c--;
	r--;

	Vector v = VectorInit(map,r,c);

	

	// incapsulate maze
	if (!c && !isborder(map,r,c,0)) 
		map ->cells[map ->cols * r + c] += 1;
	if (r == map->rows -1 && !isborder(map,r,c,1)) 
		map ->cells[map ->cols * r + c] += 1<<2;
	if (c == map->cols -1 && !isborder(map,r,c,2))
	 	map ->cells[map ->cols * r + c] += 1<<1;
	if (!r && !isborder(map,r,c,3))
		map ->cells[map ->cols * r + c] += 1<<2;


	// mark entering point
	map ->cells[map ->cols * r + c] += 8;
	v.distance[map ->cols * r + c] = 0;

	if(!quickSolve(&v))
	{
		fprintf(stderr, "error has acoured \n");
	}

	v.pntOut = 1;	

	backTrace(&v);

	for (int i = v.pntOut-1; i >=0; i--)
	{
		printf("%i,%i\n",v.outR[i] +1, v.outC[i]+1);
	}	
	printf("\n");

	VectorDstr(&v);

	return 1;
}

/*****************
 * 	backTrace -- helper 
 */
bool goBack(Vector *v, int r, int c, unsigned int i)
{
	if (v->distance[v->map ->cols * r + c] == i-1)
	{
		VectorAdd(v, r, c, true);
		return 1;
	}
	return 0;
}


/*****************
 *	shortest -- helper
 */
int backTrace (Vector *v)
{
	int r = v->outR[v->pntOut-1];
	int c = v->outC[v->pntOut-1];

	for (unsigned int i = v->distance[v->map ->cols * r + c]; i > 0; i--)
	{
		r = v->outR[v->pntOut-1];
		c = v->outC[v->pntOut-1];

		if(!isborder(v->map, r, c, 0) && goBack(v, r, c-1, i)) continue;
		if(!isborder(v->map, r, c, 1) && goBack(v, r+1, c, i)) continue;
		if(!isborder(v->map, r, c, 2) && goBack(v, r, c+1, i)) continue;
		if(!isborder(v->map, r, c, 3) && goBack(v, r-1, c, i)) continue;
	}
	return 0;
}

Vector VectorInit(Map *map, int r, int c)
{
	Vector v;

	v.map = map;
	v.distance = malloc(sizeof(unsigned int) * map->rows * map->cols);

	v.bufferLen = INITSIZE ;//map->rows + map-> cols;
	v.inR = (int *)malloc(sizeof(int) * v.bufferLen);
	v.inC = (int *)malloc(sizeof(int) * v.bufferLen);
	v.pntIn = 1;

	v.outC = (int *)malloc(sizeof(int) * v.bufferLen);
	v.outR = (int *)malloc(sizeof(int) * v.bufferLen);
	v.weight = 1;

	v.inR[0] = r;
	v.inC[0] = c;


	return v;
}

void VectorDstr(Vector *v)
{
	free(v->distance);
	free(v->inR);
	free(v->inC);
	free(v->outC);
	free(v->outR);
	v=NULL;
}

int VectorAdd(Vector *v, int r, int c, bool mode)
{
	// not matked cell
	if (mode || v->map ->cells[v->map ->cols * r + c]<= 7)
	{
		// mark cell
		MarkNode(v->map,r,c);
		v->distance[v->map ->cols * r + c] = v->weight;
	
		// add cell to buffer
		if (v->bufferLen <= v->pntOut+1)
		{	
			// be awed for I have created a LIST in the bounds of c
			v->bufferLen *= 2;
			
			v->outC = (int *)realloc(v->outC, sizeof(int) * v->bufferLen);
			v->outR = (int *)realloc(v->outR, sizeof(int) * v->bufferLen);
		}
		v->outR[v->pntOut] = r; 
		v->outC[v->pntOut++] = c;
		return 1;
	}
	return 0;
}

bool VectorBasecase(Vector *v, bool b, int r, int c)
{
	if (b)
	{
		v->outR[0] = r;
		v->outC[0] = c;
		return true;
	}
	return false;
}

void MarkNode(Map *map, int r, int c)
{
	map ->cells[map ->cols * r + c] += 8;
}

/*****************
 *	shortest -- helper
 */
int quickSolve (Vector *v)
{
	if (!v->pntIn) return 0; 
	v->pntOut=0;

	for (int i =0, r = v->inR[0], c = v->inC[0]; i < v->pntIn; r = v->inR[++i], c = v->inC[i])
	{
		if (!isborder(v->map,r,c,0))  
		{
			if (VectorBasecase(v, !c, r, c)) return 1;
			VectorAdd(v, r, c -1, false);
		}
		if (!isborder(v->map,r,c,1)) 
		{
			if (VectorBasecase(v, r == v->map->rows-1, r, c)) return 1;
			VectorAdd(v, r+1, c, false);
		}
		if (!isborder(v->map,r,c,2))  
		{
			if (VectorBasecase(v, c == v->map->cols-1, r, c)) return 1;
			VectorAdd(v, r, c+1, false);
		}
		if (!isborder(v->map,r,c,3))  
		{
			if (VectorBasecase(v, !r, r, c)) return 1;
			VectorAdd(v, r-1, c, false);
		} 
	
		// mark this node 
		MarkNode(v->map,r,c);
	}

	// switch out to in and use in as a new bufer
	int *tmp = v->inR;
	v->inR = v->outR;
	v->outR = tmp;

	tmp = v->inC;
	v->inC = v->outC;
	v->outC = tmp;

	v->pntIn = v->pntOut;
	v->weight += 1; 

	// recursion 
	return quickSolve(v);
}

