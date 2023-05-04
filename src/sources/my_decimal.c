#include "../headers/my_decimal.h"

#include "../headers/extra.h"

/*
========================================================
                  CONVERTATION
========================================================
*/

/// @brief | Преобразование числа int в число my_decimal |
/// @param  src -  число int
/// @param  dst -  указатель на число my_decimal
/// @return
/// 0 - OK;
/// 1 - ошибка конвертации
int my_from_int_to_decimal(int src, my_decimal* dst) {
  int result = OK_CONVERTATION;
  if (dst == NULL) {
    result = ERROR_CONVERTATION;
  } else {
    initDecimal(dst);
    if (src < 0) {
      setSign(dst, NEGATIVE);
      src *= -1;
    }
    dst->bits[LOW] = src;
  }
  return result;
}

/// @brief | Преобразование числа my_decimal в число int |
/// @param  src -  число my_decimal
/// @param  dst -  указатель на число int
/// @return
/// 0 - OK;
/// 1 - ошибка конвертации
int my_from_decimal_to_int(my_decimal src, int* dst) {
  int output = OK_CONVERTATION;
  if (dst == NULL) {
    output = ERROR_CONVERTATION;
  } else {
    *dst = 0;
    my_truncate(src, &src);
    int non_zero_bit = getFirstNonZeroBitNumber(src);
    if (non_zero_bit >= MAX_NUM_OF_BITS_IN_A_WORD - 1) {
      output = ERROR_CONVERTATION;
    } else {
      *dst = src.bits[0];
      unsigned int sign = getSign(src);
      if (sign) {
        *dst *= -1;
      }
    }
  }
  return output;
}

/// @brief | Преобразование числа float в число my_decimal |
/// @param src -  число float
/// @param dst -  указатель на число my_decimal
/// @return
/// 0 - OK;
/// 1 - ошибка конвертации
int my_from_float_to_decimal(float src, my_decimal* dst) {
  int result = OK_CONVERTATION;
  if (dst == NULL) {
    result = ERROR_CONVERTATION;
  } else if (isinf(src) || isnan(src)) {
    initDecimal(dst);
    result = ERROR_CONVERTATION;
  } else {
    int exponent_float = getExponentFloat(src);
    if (exponent_float > MAX_EXPONENT_OF_FLOAT ||
        (exponent_float < MIN_EXPONENT_OF_FLOAT && src != 0.0)) {
      result = ERROR_CONVERTATION;
    } else if (src == 0.0) {
      initDecimal(dst);
    } else {
      initDecimal(dst);
      if (signbit(src)) {
        setSign(dst, NEGATIVE);
        src *= -1;
      }
      double dnum = src;
      int scale_decimal = 0;
      unsigned int amount_significant_digits = 0;
      if (exponent_float < 0) {
        scale_decimal = normalizeFloat(&dnum);
      }
      amount_significant_digits = countSignificantDigits(dnum);
      scalingFloat(&dnum, &scale_decimal, amount_significant_digits);
      result = writeFloatToDecimal(dnum, dst, scale_decimal);
    }
  }
  return result;
}

/// @brief | Преобразование числа my_decimal в число float |
/// @param  src -  число my_decimal
/// @param  dst -  указатель на число float
/// @return
/// 0 - OK;
/// 1 - ошибка конвертации
int my_from_decimal_to_float(my_decimal src, float* dst) {
  int output = OK_CONVERTATION;
  if (dst == NULL) {
    output = ERROR_CONVERTATION;
  } else {
    *dst = 0.0;
    for (int i = 0; i <= MAX_BIT_POSITION; i++) {
      if (getBit(src, i)) {
        *dst += pow(2, i);
      }
    }
    unsigned int scale = getScale(src);
    while (scale) {
      *dst /= 10.0;
      scale--;
    }
    if (getSign(src)) {
      *dst = *dst * (-1);
    }
  }
  return output;
}

/*
========================================================
                  ARITHMETIC
========================================================
*/

/// @brief | Сложение двух чисел my_decimal | + |
/// @param value_1 -  число my_decimal
/// @param value_2 -  число my_decimal
/// @param result -  указатель на my_decimal
/// @return
/// 0 - OK;
/// 1 - число слишком большое (INF);
/// 2 - число слишком маленькое (N_INF);
/// 3 - NAN
int my_add(my_decimal value_1, my_decimal value_2, my_decimal* result) {
  int output = OK;
  if (result == NULL) {
    output = DIV_BY_ZERO;
  } else {
    initDecimal(result);
    if (checkDecimalIsZero(value_1) && checkDecimalIsZero(value_2)) {
      unsigned int scale_value_1 = getScale(value_1);
      unsigned int scale_value_2 = getScale(value_2);
      if (scale_value_1 >= scale_value_2) {
        *result = value_1;
      } else {
        *result = value_2;
      }
    } else {
      my_big_decimal value_1_big_decimal = {0};
      my_big_decimal value_2_big_decimal = {0};
      my_big_decimal result_big_decimal = {0};
      from_decimal_to_big_decimal(value_1, &value_1_big_decimal);
      from_decimal_to_big_decimal(value_2, &value_2_big_decimal);
      addValuesBigDecimal(value_1_big_decimal, value_2_big_decimal,
                          &result_big_decimal);
      output = from_big_decimal_to_decimal(result_big_decimal, result);
      if (output == OK) {
        if (checkDecimalIsZero(*result)) {
          unsigned int sign = getSign(value_1);
          if (sign) {
            setSign(result, sign);
          }
        }
      }
    }
  }
  return output;
}

/// @brief | Вычитание двух чисел my_decimal | - |
/// @param value_1 -  число my_decimal
/// @param value_2 -  число my_decimal
/// @param result -  указатель на my_decimal
/// @return
/// 0 - OK;
/// 1 - число слишком большое (INF);
/// 2 - число слишком маленькое (N_INF);
/// 3 - NAN
int my_sub(my_decimal value_1, my_decimal value_2, my_decimal* result) {
  int output = OK;
  if (result == NULL) {
    output = DIV_BY_ZERO;
  } else {
    initDecimal(result);
    if (checkDecimalIsZero(value_1) && checkDecimalIsZero(value_2)) {
      unsigned int scale_value_1 = getScale(value_1);
      unsigned int scale_value_2 = getScale(value_2);
      if (scale_value_1 >= scale_value_2) {
        *result = value_1;
      } else {
        *result = value_2;
        int sign_value_2 = getSign(value_2);
        setSign(result, !sign_value_2);
      }
    } else {
      my_big_decimal value_1_big_decimal = {0};
      my_big_decimal value_2_big_decimal = {0};
      my_big_decimal result_big_decimal = {0};
      from_decimal_to_big_decimal(value_1, &value_1_big_decimal);
      from_decimal_to_big_decimal(value_2, &value_2_big_decimal);
      subValuesBigDecimal(value_1_big_decimal, value_2_big_decimal,
                          &result_big_decimal);
      output = from_big_decimal_to_decimal(result_big_decimal, result);
      if (output == OK) {
        if (checkDecimalIsZero(*result)) {
          unsigned int sign = getSign(value_1);
          if (sign) {
            setSign(result, sign);
          }
        }
      }
    }
  }
  return output;
}

/// @brief | Умножение двух чисел my_decimal | * |
/// @param value_1 -  число my_decimal
/// @param value_2 -  число my_decimal
/// @param result -  указатель на my_decimal
/// @return
/// 0 - OK;
/// 1 - число слишком большое (INF);
/// 2 - число слишком маленькое (N_INF);
/// 3 - NAN
int my_mul(my_decimal value_1, my_decimal value_2, my_decimal* result) {
  int output = OK;
  if (result == NULL) {
    output = DIV_BY_ZERO;
  } else {
    initDecimal(result);
    if (checkDecimalIsZero(value_1) || checkDecimalIsZero(value_2)) {
      unsigned int scale_value_1 = getScale(value_1);
      unsigned int scale_value_2 = getScale(value_2);
      unsigned int scale_result = scale_value_1 + scale_value_2;
      if (scale_result > MAX_NUM_SCALE_DECIMAL) {
        scale_result = MAX_NUM_SCALE_DECIMAL;
      }
      setScale(result, scale_result);
      unsigned int sign_value_1 = getSign(value_1);
      unsigned int sign_value_2 = getSign(value_2);
      setSign(result, (sign_value_1 + sign_value_2) % 2);
    } else {
      my_big_decimal big_decimal_value_1 = {0};
      my_big_decimal big_decimal_value_2 = {0};
      my_big_decimal big_decimal_result = {0};
      from_decimal_to_big_decimal(value_1, &big_decimal_value_1);
      from_decimal_to_big_decimal(value_2, &big_decimal_value_2);
      mulBigDecimal(big_decimal_value_1, big_decimal_value_2,
                    &big_decimal_result);
      output = from_big_decimal_to_decimal(big_decimal_result, result);
    }
  }
  return output;
}

/// @brief | Деление двух чисел my_decimal | / |
/// @param value_1 -  число my_decimal
/// @param value_2 -  число my_decimal
/// @param result -  указатель на my_decimal
/// @return
/// 0 - OK;
/// 1 - число слишком большое (INF);
/// 2 - число слишком маленькое (N_INF);
/// 3 - NAN
int my_div(my_decimal value_1, my_decimal value_2, my_decimal* result) {
  int output = OK;
  if (result == NULL) {
    output = DIV_BY_ZERO;
  } else {
    initDecimal(result);
    if (checkDecimalIsZero(value_2)) {
      output = DIV_BY_ZERO;
    } else if (checkDecimalIsZero(value_1)) {
      unsigned int sign_value_1 = getSign(value_1);
      unsigned int sign_value_2 = getSign(value_2);
      setSign(result, (sign_value_1 + sign_value_2) % 2);
      int scale_value_1 = getScale(value_1);
      int scale_value_2 = getScale(value_2);
      int scale_result = scale_value_1 - scale_value_2;
      if (scale_result < 0) {
        scale_result = 0;
      }
      setScale(result, scale_result);
    } else {
      my_big_decimal big_decimal_value_1 = {0};
      my_big_decimal big_decimal_value_2 = {0};
      my_big_decimal big_decimal_result = {0};
      from_decimal_to_big_decimal(value_1, &big_decimal_value_1);
      from_decimal_to_big_decimal(value_2, &big_decimal_value_2);
      divBigDecimal(big_decimal_value_1, big_decimal_value_2,
                    &big_decimal_result);
      output = from_big_decimal_to_decimal(big_decimal_result, result);
      if (output == OK) {
        if (checkDecimalIsZero(*result)) {
          initDecimal(result);
        }
      }
    }
  }
  return output;
}

/// @brief | Остаток от дления двух чисел my_decimal | Mod |
/// @param value_1 -  число my_decimal
/// @param value_2 -  число my_decimal
/// @param result -  указатель на my_decimal
/// @return
/// 0 - OK;
/// 1 - число слишком большое (INF);
/// 2 - число слишком маленькое (N_INF);
/// 3 - NAN
int my_mod(my_decimal value_1, my_decimal value_2, my_decimal* result) {
  int output = OK;
  if (result == NULL) {
    output = DIV_BY_ZERO;
  } else {
    if (checkDecimalIsZero(value_2)) {
      output = DIV_BY_ZERO;
    } else {
      unsigned int sign_value_1 = getSign(value_1);
      unsigned int sign_value_2 = getSign(value_2);
      if (sign_value_1) {
        setSign(&value_1, POSITIVE);
      }
      if (sign_value_2) {
        setSign(&value_2, POSITIVE);
      }
      my_big_decimal value_1_big_decimal = {0};
      my_big_decimal value_2_big_decimal = {0};
      my_big_decimal remainder = {0};
      from_decimal_to_big_decimal(value_1, &value_1_big_decimal);
      from_decimal_to_big_decimal(value_2, &value_2_big_decimal);
      divWithRemainder(value_1_big_decimal, value_2_big_decimal,
                       &value_1_big_decimal, &remainder);
      output = from_big_decimal_to_decimal(remainder, result);
      if (output == OK) {
        if (!(checkDecimalIsZero(*result))) {
          correctionDecimal(result);
        }
        if (sign_value_1) {
          setSign(result, NEGATIVE);
        }
      }
    }
  }
  return output;
}

/*
========================================================
                    COMPARE
========================================================
*/

/// @brief | Сравнение двух чисел my_decimal | == |
/// @param value_1 -  число my_decimal
/// @param value_2 -  число my_decimal
/// @return
/// 0 - FALSE;
/// 1 - TRUE
int my_is_equal(my_decimal value_1, my_decimal value_2) {
  int output = FALSE;
  my_decimal difference = {0};
  int result = my_sub(value_1, value_2, &difference);
  if (result == OK && checkDecimalIsZero(difference)) {
    output = TRUE;
  }
  return output;
}

/// @brief | Сравнение двух чисел my_decimal | < |
/// @param value_1 -  число my_decimal
/// @param value_2 -  число my_decimal
/// @return
/// 0 - FALSE
/// 1 - TRUE
int my_is_less(my_decimal value_1, my_decimal value_2) {
  int output = FALSE;
  if (my_is_equal(value_1, value_2) == FALSE) {
    unsigned int sign_value_1 = getSign(value_1);
    unsigned int sign_value_2 = getSign(value_2);
    if (sign_value_1 != sign_value_2 && sign_value_1) {
      output = TRUE;
    } else if (sign_value_1 == sign_value_2) {
      my_decimal difference = {0};
      setSign(&value_1, POSITIVE);
      setSign(&value_2, POSITIVE);
      int result = my_sub(value_1, value_2, &difference);
      if (result == OK) {
        unsigned int sign_difference = getSign(difference);
        if ((sign_difference && !sign_value_1) ||
            (!sign_difference && sign_value_1)) {
          output = TRUE;
        }
      }
    }
  }
  return output;
}

/// @brief | Сравнение двух чисел my_decimal | > |
/// @param value_1 -  число my_decimal
/// @param value_2 -  число my_decimal
/// @return
/// 0 - FALSE;
/// 1 - TRUE
int my_is_greater(my_decimal value_1, my_decimal value_2) {
  return !(my_is_less(value_1, value_2)) && !(my_is_equal(value_1, value_2));
}

/// @brief | Сравнение двух чисел my_decimal | <= |
/// @param value_1 -  число my_decimal
/// @param value_2 -  число my_decimal
/// @return
/// 0 - FALSE;
/// 1 - TRUE
int my_is_less_or_equal(my_decimal value_1, my_decimal value_2) {
  return my_is_less(value_1, value_2) || my_is_equal(value_1, value_2);
}

/// @brief | Сравнение двух чисел my_decimal | >= |
/// @param value_1 -  число my_decimal
/// @param value_2 -  число my_decimal
/// @return
/// 0 - FALSE;
/// 1 - TRUE
int my_is_greater_or_equal(my_decimal value_1, my_decimal value_2) {
  return !(my_is_less(value_1, value_2));
}

/// @brief | Сравнение двух чисел my_decimal | != |
/// @param value_1 -  число my_decimal
/// @param value_2 -  число my_decimal
/// @return
/// 0 - FALSE;
/// 1 - TRUE
int my_is_not_equal(my_decimal value_1, my_decimal value_2) {
  return !(my_is_equal(value_1, value_2));
}

/*
========================================================
                    ANOTHER FUNCTIONS
========================================================
*/

/// @brief | Округляет вниз до целого число value |
/// @param value -  число my_decimal
/// @param result -  указатель на число my_decimal
/// @return
/// 0 - OK;
/// 1 -ошибка вычисления
int my_floor(my_decimal value, my_decimal* result) {
  int output = OK_CONVERTATION;
  if (result == NULL) {
    output = ERROR_CONVERTATION;
  } else {
    initDecimal(result);
    unsigned int scale = getScale(value);
    unsigned int sign = getSign(value);
    if (!scale) {
      *result = value;
    } else if (checkDecimalIsZero(value)) {
      setSign(result, sign);
    } else {
      my_truncate(value, &value);
      scale = 0;
      *result = value;
      if (sign) {
        my_decimal unit = {{1, 0, 0, 0}};
        setScale(&unit, scale);
        output = my_sub(value, unit, result);
      }
    }
  }
  return output;
}

/// @brief | Округляет вниз до ближайшего целого число value |
/// @param value -  число my_decimal
/// @param result -  указатель на число my_decimal
/// @return
/// 0 - OK;
/// 1 -ошибка вычисления
int my_round(my_decimal value, my_decimal* result) {
  int output = OK_CONVERTATION;
  if (result == NULL) {
    output = ERROR_CONVERTATION;
  } else {
    initDecimal(result);
    unsigned int scale = getScale(value);
    unsigned int sign = getSign(value);
    if (!scale) {
      *result = value;
    } else if (checkDecimalIsZero(value)) {
      setSign(result, sign);
    } else {
      setSign(&value, POSITIVE);
      my_big_decimal value_big_decimal = {0};
      my_big_decimal remainder = {0};
      from_decimal_to_big_decimal(value, &value_big_decimal);
      while (scale) {
        divBigDecimalBy10(&value_big_decimal, &remainder);
        scale--;
      }
      from_big_decimal_to_decimal(value_big_decimal, &value);
      *result = value;
      my_big_decimal half_divisor = {{5, 0, 0, 0, 0, 0, 0, 0x0001000}};
      unsigned int scale_divisor = 1;
      unsigned int scale_remainder = getScaleBigDecimal(remainder);
      while (scale_divisor < scale_remainder) {
        mulBigDecimalBy10(&half_divisor);
        scale_divisor++;
      }
      my_decimal unit = {{1, 0, 0, 0}};
      int bit_position_remainder =
          getFirstNonZeroBitNumberBigDecimal(remainder);
      int bit_position_divisor =
          getFirstNonZeroBitNumberBigDecimal(half_divisor);
      if (bit_position_remainder > bit_position_divisor) {
        output = my_add(value, unit, result);
      } else if (bit_position_remainder == bit_position_divisor) {
        unsigned int bit_value_remainder = 0;
        unsigned int bit_value_divisor = 0;
        int num_bit = 0;
        for (num_bit = bit_position_remainder;
             bit_value_remainder == bit_value_divisor && num_bit >= 0;
             num_bit--) {
          bit_value_remainder = getBitBigDecimal(remainder, num_bit);
          bit_value_divisor = getBitBigDecimal(half_divisor, num_bit);
        }
        if (num_bit == -1 || bit_value_remainder) {
          output = my_add(value, unit, result);
        }
      }
      if (output == OK) {
        setSign(result, sign);
      }
    }
  }
  return output;
}

/// @brief | Отбрасывает дробную часть и незначащие нули числа value |
/// @param value -  число my_decimal
/// @param result -  указатель на число my_decimal
/// @return
/// 0 - OK;
/// 1 -ошибка вычисления
int my_truncate(my_decimal value, my_decimal* result) {
  int output = OK_CONVERTATION;
  if (result == NULL) {
    output = ERROR_CONVERTATION;
  } else {
    initDecimal(result);
    int non_zero_bit = getFirstNonZeroBitNumber(value);
    if (non_zero_bit == -1) {
      setScale(&value, 0);
      *result = value;
    } else {
      int scale = getScale(value);
      if (scale) {
        my_big_decimal tmp = {0};
        my_big_decimal remainder = {0};
        from_decimal_to_big_decimal(value, &tmp);
        while (scale) {
          divBigDecimalBy10(&tmp, &remainder);
          scale--;
        }
        from_big_decimal_to_decimal(tmp, result);
      } else {
        *result = value;
      }
    }
  }
  return output;
}

/// @brief | Изменяет знак числа value на противоположный |
/// @param value -  число my_decimal
/// @param result -  указатель на число my_decimal
/// @return
/// 0 - OK;
/// 1 -ошибка вычисления
int my_negate(my_decimal value, my_decimal* result) {
  int output = OK;
  if (result == NULL) {
    output = ERROR_CONVERTATION;
  } else {
    *result = value;
    if (getSign(value)) {
      setSign(result, POSITIVE);
    } else {
      setSign(result, NEGATIVE);
    }
  }
  return output;
}
