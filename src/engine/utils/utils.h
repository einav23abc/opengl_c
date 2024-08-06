#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

uint32_t hash(uint32_t x);
uint32_t uintmin(uint32_t a, uint32_t b);
uint32_t uintmax(uint32_t a, uint32_t b);
int32_t intmin(int32_t a, int32_t b);
int32_t intmax(int32_t a, int32_t b);
int64_t int64min(int64_t a, int64_t b);
int64_t int64max(int64_t a, int64_t b);
uint64_t load_file_contents(char** load_to, const char* file_path);
int64_t str_find_substr(char* str, char* substr);
// assumes str is numbers with spaces in between
// array needs to be freed at the end of use
float* str_to_float_array(char* str, uint64_t arr_size);
// assumes str is numbers with spaces in between
// array needs to be freed at the end of use
int32_t* str_to_int_array(char* str, uint64_t arr_size);
// assumes str is numbers with spaces in between
// array needs to be bigger/equal to floats_amount
void str_to_existing_float_array(char* str, uint64_t floats_amount, float* arr);

#endif