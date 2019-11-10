#include "count_vector.h"

count_vector* count_vector_create(int initial_value, int max_value){
	count_vector* cv = NULL;
	int i;

	if(max_value < initial_value){
		return NULL;
	}

	cv = (count_vector*)malloc(sizeof(count_vector));
	cv->vet = (int*)malloc((max_value - initial_value + 1)*sizeof(count_vector));

	if(cv->vet == NULL){
		free(cv);
		return NULL;
	}

	cv->vet_size = max_value - initial_value + 1;
	cv->shift = initial_value;

	for(i = 0; i < cv->vet_size; i++){
		cv->vet[i] = 0;
	}

	#if COUNT_VECTOR_OPT_STORE_ELEM == 1
	cv->elements = 0;
	#endif
	#if COUNT_VECTOR_OPT_STORE_MIN == 1
	cv->min = max_value;
	#endif
	#if COUNT_VECTOR_OPT_STORE_MAX == 1
	cv->max = initial_value;
	#endif
	return cv;
}

void count_vector_exit_invalid(count_vector* cv){
	if(cv != NULL){
		if(cv->vet != NULL){
			return;
		}
	}
	exit(0);
}

void count_vector_free(count_vector* cv){
	free(cv->vet);
	free(cv);
}

//count_vector* count_vector_resize(int initial_value, int max_value);

void count_vector_insert(count_vector* cv, int value){
	if(value-cv->shift < 0 || value-cv->shift >= cv->vet_size){
		return;
	}
	cv->vet[value-cv->shift]++;

	#if COUNT_VECTOR_OPT_STORE_ELEM == 1
	cv->elements++;
	#endif

	#if COUNT_VECTOR_OPT_STORE_MIN == 1
	if(value < cv->min){
		cv->min = value;
	}
	#endif

	#if COUNT_VECTOR_OPT_STORE_MAX == 1
	if(value > cv->max){
		cv->max = value;
	}
	#endif
}

//int count_vector_remove(count_vector* cv, int value);

//int count_vector_element_at(count_vector* cv, int pos);

int count_vector_elements(count_vector* cv){
	#if COUNT_VECTOR_OPT_STORE_ELEM == 1
	return cv->elements;
	#else
	int i, elements;
	for(i = 0, elements = 0; i < cv->vet_size ; i++){
		elements += cv->vet[i];
	}
	return elements;
	#endif
}

//int count_vector_is_empty(count_vector* cv);

int count_vector_min(count_vector* cv){
	#if COUNT_VECTOR_OPT_STORE_MIN == 1
	return cv->min;
	#else
	int i;
	for(i = 0; i < cv->vet_size ; i++){
		if(cv->vet[i] > 0){
			return i+cv->shift;
		}
	}
	return cv->vet_size+cv->shift-1;
	#endif
}

int count_vector_max(count_vector* cv){
	#if COUNT_VECTOR_OPT_STORE_MAX == 1
	return cv->max;
	#else
	int i;
	for(i = cv->vet_size-1; i >= 0 ; i--){
		if(cv->vet[i] > 0){
			return i+cv->shift;
		}
	}
	return cv->shift;
	#endif
}

double count_vector_median(count_vector* cv){
	int elem = count_vector_elements(cv);
	int i, count = 0;
	double result = 0;

	if(elem%2 == 1){	// Numero impar de elementos
		// Example:
		// elem = 5
		// vet = [0,1,2,3,4]
		//            ^
		//            |
		//          elem/2
		//
		// median -> count >= elem/2 + 1 = 3
		for(i = 0, count = 0; count < elem/2+1; i++){
			count += cv->vet[i];
		}
		return (double)((--i)+cv->shift);
	}
	else{				// Numero par de elementos
		// Example:
		// elem = 6
		// vet = [0,1,2,3,4,5]
		//            ^ ^
		//            | |
		//     elem/2-1,elem/2
		//
		// low  -> count >= elem/2 = 3
		// high -> count >= elem/2+1 = 4
		for(i = 0, count = 0; count < elem/2; i++){		// Gets lower element
			count += cv->vet[i];
		}
		if(count >= elem/2+1){							// Verifies if lower an greater elements are the same number
			return (double)((--i)+cv->shift);
		}
		result += (double)(i-1);						// If lower and higher elemnts are different numbers
		for(i = 0, count = 0; count < elem/2+1; i++){	// Gets higher element
			count += cv->vet[i];
		}
		return (result + (double)(--i))/2.0 + cv->shift;
	}
}

double count_vector_mean(count_vector* cv){
	double elem = count_vector_elements(cv);
	int i, mean;

	for(i = 0, mean = 0; i < cv->vet_size ; i++){
		mean += cv->vet[i];
	}

	return (double)(mean/elem) + cv->shift;
}

void count_vector_mean_dp(count_vector* cv, double* mean, double* dp){
	/*
	 *	Important proprieties:
	 *	E(X + a) = E(x) + a
	 *	DP(X + a) = DP(x)
	 */
	int i;
	double elem = (double)count_vector_elements(cv);

	for(i = 0, *mean = 0; i < cv->vet_size ; i++){
		*mean += cv->vet[i]*i;
	}

	*mean = (*mean/elem);

	for(i = 0, *dp = 0; i < cv->vet_size ; i++){
		*dp += (cv->vet[i])*((double)(i)-*mean)*((double)(i)-*mean);
	}

	*mean += (double)(cv->shift);
	*dp = sqrt((double)(*dp/(elem-1)));
}

void count_vector_merge(count_vector* cv_dest, count_vector* cv_src){
	/*
	 *	This version is currently unsafe for vectors tha have different number intervals
	 */
	int i;

	for(i = 0; i < cv_dest->vet_size ; i++){
		cv_dest->vet[i] += cv_src->vet[i];
	}

	#if COUNT_VECTOR_OPT_STORE_ELEM == 1
	cv_dest->elements += cv_src->elements;
	#endif
}

#if COUNT_VECTOR_OPT_CONSOLE == 1
void printf_count_vector_all(count_vector* cv){
	int i;

	printf("CV = %p: vect = %p, size = %d, shift = %d | ", cv, cv->vet, cv->vet_size, cv->shift);
	printf("elements = %d, min = %d, max = %d\n", count_vector_elements(cv), count_vector_min(cv), count_vector_max(cv));
	for(i = 0; i < cv->vet_size; i++){
		printf("logic_vector[%d] = real_vector[%d] = %d\n", i+cv->shift, i, cv->vet[i]);
	}
}

void printf_count_vector(count_vector* cv){
	int i;

	printf("CV = %p: vect = %p, size = %d, shift = %d | ", cv, cv->vet, cv->vet_size, cv->shift);
	printf("elements = %d, min = %d, max = %d\n", count_vector_elements(cv), count_vector_min(cv), count_vector_max(cv));
	for(i = 0; i < cv->vet_size; i++){
		if(cv->vet[i] != 0){
			printf("logic_vector[%d] = real_vector[%d] = %d\n", i+cv->shift, i, cv->vet[i]);
		}
	}
}
#endif