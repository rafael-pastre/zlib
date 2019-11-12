/*! 
 *	\file count_vector.h
 *	\brief Library for manipulating an ordered vector of integers with fixed interval
 *
 *	This library implements a count vector which is a vector based on the count sort algorithm.
 *
 *	The structure defined in this library consists in a vector whose positions stores the number of
 *	repetitions of the number in the vector. For instance, if a vector v have 47 repetitions of the
 *	number 3 then we have v[3] = 47.
 *
 *	Made by: Rafael Pastre
 *
 *	Created in: 08 nov 2019
 */

#ifndef LIB_COUNT_VECTOR_H
#define LIB_COUNT_VECTOR_H
/* Library options (future) */
// Enable/disable console
#define COUNT_VECTOR_OPT_CONSOLE	1		// '0' = disable console functions, '1' = enable console functions
#define COUNT_VECTOR_OPT_STORE_ELEM	1		// '0' = don't stores number of elements, '1' = stores number of elements
#define COUNT_VECTOR_OPT_STORE_MIN	0		// '0' = don't stores min element, '1' = stores min element
#define COUNT_VECTOR_OPT_STORE_MAX	0		// '0' = don't stores max element, '1' = stores max element

/* Dependencies */
#if COUNT_VECTOR_OPT_CONSOLE == 1
#include <stdio.h>
#endif
#include <stdlib.h>
#include <math.h>

/* Data structure */
typedef struct _count_vector{
	int* vet;
	int vet_size;
	int shift;
	#if COUNT_VECTOR_OPT_STORE_ELEM == 1
	int elements;
	#endif
	#if COUNT_VECTOR_OPT_STORE_MIN == 1
	int min;
	#endif
	#if COUNT_VECTOR_OPT_STORE_MAX == 1
	int max;
	#endif
}count_vector;

/* Macros (avaliability depends on configurarion) */
#if COUNT_VECTOR_OPT_STORE_ELEM == 1
#define COUNT_VECT_ELEMENTS(cv) (cv->elements)

#define COUNT_VECT_IS_EMPTY(cv) ((cv->elements == 0)?(1):(0))
#endif

#if COUNT_VECTOR_OPT_STORE_MIN == 1
#define COUNT_VECT_MIN(cv) (cv->min)
#endif

#if COUNT_VECTOR_OPT_STORE_MAX == 1
#define COUNT_VECT_MAX(cv) (cv->max)
#endif

/* Functions */
count_vector* count_vector_create(int initial_value, int max_value);

void count_vector_exit_invalid(count_vector* cv);

void count_vector_free(count_vector* cv);

//count_vector* count_vector_resize(int initial_value, int max_value);

void count_vector_insert(count_vector* cv, int value);

//int count_vector_remove(count_vector* cv, int value);

//int count_vector_element_at(count_vector* cv, int pos);

int count_vector_elements(count_vector* cv);

//int count_vector_is_empty(count_vector* cv);

int count_vector_min(count_vector* cv);

int count_vector_max(count_vector* cv);

double count_vector_median(count_vector* cv);

double count_vector_mean(count_vector* cv);

void count_vector_mean_dp(count_vector* cv, double* mean, double* dp);

void count_vector_merge(count_vector* cv_dest, count_vector* cv_src);

/* Console printing functions (avaliability depends on configurarion) */
#if COUNT_VECTOR_OPT_CONSOLE == 1
void printf_count_vector_all(count_vector* cv);

void printf_count_vector(count_vector* cv);
#endif

#endif//LIB_COUNT_VECTOR_H