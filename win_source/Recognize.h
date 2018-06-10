#ifndef _RECOGNIZE_H
#define _RECOGNIZE_H

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commctrl.h>
#include <imm.h>
#include "resource.h"

#define RAW_COORDINATE_ARRAY_FILE		   ("tmp.dat")
#define FILTERED_COORDINATE_ARRAY_FILE     ("filtered_coordinate.dat")      
#define NORMALIZED_COORDINATE_ARRAY_FILE   ("normalized_coordinate.dat")    
#define MULTIPLEXED_COORDINATE_ARRAY_FILE  ("multiplexed_coordinate.dat")   
#define REGULATED_COORDINATE_ARRAY_FILE    ("regulated_coordinate.dat")     
#define TOTAL_COORDINATE_ARRAY_FILE		   ("total_coordinate.dat")			
#define MAX_DATA_SIZE	1000
#define LENGTH			200

#define NUMBER	0
#define ENGLISH 1
#define HANGUL	2
//////////////////////////////////////////////////////////////////////////
// Variable
//////////////////////////////////////////////////////////////////////////


struct coordinate
{
	short x;
	short y;
	short z;
	short reserved;
}; 

typedef struct processing_data
{
	double x;
	double y;
	double z;
} processing_data;


//////////////////////////////////////////////////////////////////////////
// Operation
//////////////////////////////////////////////////////////////////////////

int precedure_filtering(int, coordinate *, processing_data *);	


int normalization(int, processing_data *);		

int regulation(int, processing_data *, processing_data *);

void recognize(int, coordinate *, int, BOOL);

void init_context(int mode);

void number_print(int number);

void english_print(int number);

void consonant_print(int number);

void vowel_composition(int number);

void resultprint(int number, int mode, BOOL spot);

#endif