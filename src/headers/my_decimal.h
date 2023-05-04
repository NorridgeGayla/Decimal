#ifndef DECIMAL_SRC_HEADERS_MY_DECIMAL_H_
#define DECIMAL_SRC_HEADERS_MY_DECIMAL_H_

#include <math.h>
#include <string.h>

#define DEFAULT_PRECISION 7
#define MAX_BIT_POSITION 95
#define MAX_BIT_POSITION_BIG_DECIMAL 223
#define MAX_NUM_OF_BITS_IN_A_WORD 32
#define MAX_EXPONENT_OF_FLOAT 95
#define MIN_EXPONENT_OF_FLOAT -95
#define MAX_NUM_OF_BYTE_DECIMAL 4
#define MAX_NUM_OF_BYTES_BIG_DECIMAL 8
#define SCALE_BYTE_BIG_DECIMAL 7
#define MAX_NUM_SCALE_DECIMAL 28

typedef enum CodeErrors { OK, INF, N_INF, DIV_BY_ZERO } CodeErrors;

typedef enum ConvertationErrors {
  OK_CONVERTATION,
  ERROR_CONVERTATION
} ConvertationErrors;

typedef enum WordsDecimal { LOW, MID, HIGH, SCALE } WordsDecimal;

typedef enum SignDecimal { POSITIVE, NEGATIVE } SignDecimal;

typedef enum BoolType { FALSE, TRUE } BoolType;

typedef struct my_decimal {
  unsigned int bits[4];
} my_decimal;

typedef struct my_big_decimal {
  unsigned int bits[8];
} my_big_decimal;

int my_add(my_decimal value_1, my_decimal value_2, my_decimal *result);
int my_sub(my_decimal value_1, my_decimal value_2, my_decimal *result);
int my_mul(my_decimal value_1, my_decimal value_2, my_decimal *result);
int my_div(my_decimal value_1, my_decimal value_2, my_decimal *result);
int my_mod(my_decimal value_1, my_decimal value_2, my_decimal *result);

int my_is_less(my_decimal value_1, my_decimal value_2);
int my_is_less_or_equal(my_decimal value_1, my_decimal value_2);
int my_is_greater(my_decimal value_1, my_decimal value_2);
int my_is_greater_or_equal(my_decimal value_1, my_decimal value_2);
int my_is_equal(my_decimal value_1, my_decimal value_2);
int my_is_not_equal(my_decimal value_1, my_decimal value_2);

int my_from_int_to_decimal(int src, my_decimal *dst);
int my_from_float_to_decimal(float src, my_decimal *dst);
int my_from_decimal_to_int(my_decimal src, int *dst);
int my_from_decimal_to_float(my_decimal src, float *dst);

int my_floor(my_decimal value, my_decimal *result);
int my_round(my_decimal value, my_decimal *result);
int my_truncate(my_decimal value, my_decimal *result);
int my_negate(my_decimal value, my_decimal *result);

#endif  // DECIMAL_SRC_HEADERS_MY_DECIMAL_H_
