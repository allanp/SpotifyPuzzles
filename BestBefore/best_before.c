#if defined(_MSC_VER)
#define _CRT_SECURE_NO_WARNINGS
#endif

#define OS_INLINE #if defined(_MSC_VER)\
__inline \
#elif\
inline\
#endif\
\

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX(a, b) (((a)>(b)) ? (a) : (b))
#define MIN(a, b) (((a)<(b)) ? (a) : (b))
#define MIN_3(a, b, c) ( (((a) <= (b)) && ((a) <= (c))) ? (a) : ((((b) <= (a)) && ((b) <= (c))) ? (b) : (c)))

const int mmin_year = 100;
const int min_year = 2000; 
const int max_year = 2999;
const char sp = '/';
const char* is_illegal_message = "is illegal";
const int max_month = 12;
const int max_day = 31;
const int days[12] = {0, -3, 0, -1, 0, -1, 0, 0, -1, 0, -1, 0 }; // days ref to 31
// returns 1 if it is leap, otherwise, return 0

const int is_leap_year(int year){ return (year % 4 > 0) || ( year % 100 == 0 && year % 400 > 0) ? 0 : 1; }

OS_INLINE const int is_valid_date(int year, int month, int day){
	year += (year <= mmin_year) ? min_year : 0;
	return (year >= min_year && month > 0 && day > 0) && 
		   (year <= max_year && month <= max_month) && 
		   (day <= ( month == 2 ? (max_day + days[month - 1] + is_leap_year(year)) : max_day + days[month - 1]));
}

int strtoi(const char* s, int* i){
	const char** r;
	char* rr;
	rr = (char*)malloc(sizeof(char*) * (strlen(s) + 1));
	if( rr == NULL){
		perror("allocate rr failed.");
		exit(0);
	}
	sprintf(rr, s);
	r = (const char**)malloc(sizeof(const char*) * 3);
	if(r == NULL){
		perror("allocate r failed.");
		exit(0);
	}
	r[0] = strtok(rr, &sp);
	r[1] = strtok(NULL, &sp);
	r[2] = strtok(NULL, &sp);
	if(r[0] == NULL || r[1] == NULL || r[2] == NULL)
		return -1;
	i[0] = atoi(r[0]);
	i[1] = atoi(r[1]);
	i[2] = atoi(r[2]);
	return 0;
}

void sort_array(int* i){
	int x = MIN_3(i[0], i[1], i[2]);
	int y = 0, z = 0;
	if(x == i[0]){ y = MIN(i[1], i[2]); z = MAX(i[1], i[2]); }
	else if(x == i[1]){ y = MIN(i[0], i[2]); z = MAX(i[0], i[2]); }
	else if(x == i[2]){ y = MIN(i[0], i[1]); z = MAX(i[0], i[1]); }
	i[0] = x; i[1] = y; i[2] = z;
}

void reorder(int* r, int index){
	int x = r[0], y = r[1], z = r[2];
	switch(index){ // x,y,z; y,x,z; y,z,x; z,x,y; z,y,x
		case 0:
			r[0] = x; r[1] = z; r[2] = y;
			break;
		case 1:
			r[0] = z; r[1] = x; r[2] = y;
			break;
		case 2:
			r[0] = x; r[1] = z; r[2] = y;
			break;
		case 3:
			r[0] = y; r[1] = z; r[2] = x;
			break;
		case 4:
			r[0] = x; r[1] = z; r[2] = y;
			break;
		default:
			break;
	}
}

int best_before(const char* input, char* output){
	int is_valid = 0, index = 0;
	int r[3] = {0, 0, 0};
	if(!output)
		return -1;
	strtoi(input, r); // convert to digits
	if(r == NULL){
		fprintf(stderr, "convert failed.");
		return -1;
	}
	sort_array(r); // sort the digits
	while(!(is_valid = is_valid_date(r[0], r[1], r[2]))){
		reorder(r, index++); // shift over, from smallest possible to largest possible
		if(index == 5) break;
	}
	if(is_valid){
		sprintf(output, "%d-%02d-%02d", r[0] < mmin_year ? r[0] + min_year : r[0], r[1], r[2]);
		return 0;
	}
	sprintf(output, is_illegal_message);
	return -1;
}

void show_usage(){
	fprintf(stdout, "\n Usage: \n"\
					"   $ BestBefore 1/12/13 02/28/00\n\n");
}

int main(int argc, char** argv){
	int i = 0;
	if(argc == 1){
		show_usage();
		return -1;
	}
	for(i = 1; i<argc; i++){
		char output[11]; // 1234-56-78
		best_before(argv[i], output);
		fprintf(stdout, "%s", output);
	}
	return 0;
}
