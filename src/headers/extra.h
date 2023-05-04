#ifndef DECIMAL_SRC_HEADERS_EXTRA_H_
#define DECIMAL_SRC_HEADERS_EXTRA_H_

#include "my_decimal.h"

void setSign(my_decimal *decimal, unsigned int sign);
void setSignBigDecimal(my_big_decimal *decimal, unsigned int sign);
int getExponentFloat(float fnum);
int initDecimal(my_decimal *decimal);
int initBigDecimal(my_big_decimal *decimal);
int from_decimal_to_big_decimal(my_decimal src, my_big_decimal *dst);
int from_big_decimal_to_decimal(my_big_decimal src, my_decimal *dst);
int normalizeFloat(double *fnum);
unsigned int countSignificantDigits(float fnum);
void scalingFloat(double *fnum, int *scale,
                  unsigned int num_significant_digits);
int checkFirstDigitInFract(float fract);
void setScale(my_decimal *decimal, unsigned int scale_decimal);
void setScaleBigDecimal(my_big_decimal *decimal, unsigned int scale_decimal);
void setBit(my_decimal *dst, unsigned int bit_value, unsigned int bit_position);
void setBitBigDecimal(my_big_decimal *decimal, unsigned int bit_value,
                      unsigned int bit_position);
int writeFloatToDecimal(double src, my_decimal *decimal, int scale_decimal);
void mulDecimalBy10(my_decimal *decimal);
void mulBigDecimal(my_big_decimal value_1, my_big_decimal value_2,
                   my_big_decimal *result);
void mulBigDecimalBy10(my_big_decimal *decimal);
void divBigDecimalBy10(my_big_decimal *decimal, my_big_decimal *remainder);
my_big_decimal leftShiftBigDecimal(my_big_decimal decimal, int shift);
my_big_decimal rightShiftBigDecimal(my_big_decimal decimal, int shift);
int getFirstNonZeroBitNumber(my_decimal decimal);
int getFirstNonZeroBitNumberBigDecimal(my_big_decimal decimal);
int getSign(my_decimal decimal);
int getSignBigDecimal(my_big_decimal decimal);
unsigned int getScale(my_decimal decimal);
unsigned int getScaleBigDecimal(my_big_decimal decimal);
unsigned int checkDecimalIsZero(my_decimal decimal);
unsigned int getBit(my_decimal decimal, unsigned int bit_position);
unsigned int getBitBigDecimal(my_big_decimal decimal,
                              unsigned int bit_position);
void getTwosComplementBigDecimal(my_big_decimal src, my_big_decimal *dst);
void inverseBigDecimal(my_big_decimal *src);
void addValuesBigDecimal(my_big_decimal value_1, my_big_decimal value_2,
                         my_big_decimal *result);
void subValuesBigDecimal(my_big_decimal value_1, my_big_decimal value_2,
                         my_big_decimal *result);
void copyBigDecimalToDecimal(my_big_decimal src, my_decimal *dst);
int divBigDecimal(my_big_decimal value_1, my_big_decimal value_2,
                  my_big_decimal *result);
void divWithRemainder(my_big_decimal value_1, my_big_decimal value_2,
                      my_big_decimal *unit, my_big_decimal *remainder);
void bankRounding(my_big_decimal *decimal, my_big_decimal remainder);
void correctionDecimal(my_decimal *decimal);

// void printDecimal(my_decimal decimal);
// void printBigDecimal(my_big_decimal decimal);
// void printFloatInBites(float fnum);
// void printDecimalInBits(my_decimal decimal);
// void printBigDecimalInBits(my_big_decimal decimal);

#endif  // DECIMAL_SRC_HEADERS_EXTRA_H_
