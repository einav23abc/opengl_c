#include "utils.h"


uint32_t hash(uint32_t x) {
    x ^= x >> 16;
    x *= 0x7feb352dU;
    x ^= x >> 15;
    x *= 0x846ca68bU;
    x ^= x >> 16;
    return x;
}
uint32_t uintmin(uint32_t a, uint32_t b) {
    if (a < b){
        return a;
    }
    return b;
}
uint32_t uintmax(uint32_t a, uint32_t b) {
    if (a > b){
        return a;
    }
    return b;
}
int32_t intmin(int32_t a, int32_t b) {
    if (a < b){
        return a;
    }
    return b;
}
int32_t intmax(int32_t a, int32_t b) {
    if (a > b){
        return a;
    }
    return b;
}
int64_t int64min(int64_t a, int64_t b) {
    if (a < b){
        return a;
    }
    return b;
}
int64_t int64max(int64_t a, int64_t b) {
    if (a > b){
        return a;
    }
    return b;
}
uint64_t load_file_contents(char** load_to, const char* file_path) {
    FILE* file_pointer = fopen(file_path, "r");
    if (file_pointer == NULL){
        printf("failed to open:\"%s\".\n", file_path);
        
        return 0;
    };

    uint32_t file_size = 0;

    //Getting File Size
    fseek(file_pointer, 0, SEEK_END);
    file_size = ftell(file_pointer);
    rewind(file_pointer);

    //Reading From File
    *load_to = (char*)malloc(sizeof(char) * (file_size+1));
    if (*load_to == NULL){
        printf("malloc of size %u failed\n", file_size+1);
        return 1;
    };
    uint64_t read_count = fread(*load_to, sizeof(char), file_size, file_pointer);
    (*load_to)[read_count] = '\0';
    fclose(file_pointer);

    return read_count;
}
int64_t str_find_substr(char* str, char* substr) {
    uint64_t i = 0;
    uint64_t j = 0;
    char ch = str[0];
    while (ch != '\0') {
        j = 0;
        while (ch == substr[j]) {
            j += 1;
            ch = str[i+j];
            if (substr[j] == '\0'){
                return i;
            }
            if (ch == '\0'){
                return -1;
            }
        }
        
        i += 1;
        ch = str[i];
    }

    return -1;
}
// assumes str is numbers with spaces in between
// array needs to be freed at the end of use
float* str_to_float_array(char* str, uint64_t arr_size) {
    uint64_t i = 0;
    float* arr = malloc(sizeof(float)*arr_size);
    if (arr == NULL){
        return NULL;
    }
    for (uint64_t j = 0; j < arr_size; j++) {
        arr[j] = atof(&(str[i]));
        int64_t di = str_find_substr(&(str[i+1]), " ");
        if (di == -1){
            return arr;
        }
        i += di+1;
    }
    return arr;
}
// assumes str is numbers with spaces in between
// array needs to be freed at the end of use
int32_t* str_to_int_array(char* str, uint64_t arr_size) {
    uint64_t i = 0;
    int32_t* arr = calloc(1, sizeof(int32_t)*arr_size);
    if (arr == NULL){
        return NULL;
    }
    for (uint64_t j = 0; j < arr_size; j++) {
        arr[j] = atoi(&(str[i]));
        int64_t di = str_find_substr(&(str[i+1]), " ");
        if (di == -1){
            return arr;
        }
        i += di+1;
    }
    return arr;
}
// assumes str is numbers with spaces in between
// array needs to be bigger/equal to floats_amount
void str_to_existing_float_array(char* str, uint64_t floats_amount, float* arr) {
    uint64_t i = 0;
    for (uint64_t j = 0; j < floats_amount; j++) {
        arr[j] = atof(&(str[i]));
        int64_t di = str_find_substr(&(str[i+1]), " \0");
        if (di = -1){
            return;
        }
        i += di;
    }
    return;
}


double ease_out_back(double x) {
    const double c1 = 1.70158;
    const double c3 = c1 + 1;

    return (1 + c3*pow(x - 1, 3) + c1 * pow(x - 1, 2));
}
double ease_in_out_back(double x) {
    const double c1 = 1.70158;
    const double c2 = c1*1.525;

    if (x < 0.5) {
        return (pow(2*x, 2) * ((c2+1)*2*x - c2))/2;
    }
    return (pow(2*x - 2, 2) * ((c2+1)*(2*x - 2) + c2) + 2)/2;
}