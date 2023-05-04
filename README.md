# Decimal

Реализация собственной библиотеки my_decimal.h.

## Содержание
- [Decimal](#decimal)
  - [Содержание](#содержание)
  - [Справка](#справка)
  - [Общая информация ](#общая-информация-)
  - [1. Реализация функций ](#1-реализация-функций-)
    - [1.1 Арифметические функции ](#11-арифметические-функции-)
    - [1.2 Операторы сравнения ](#12-операторы-сравнения-)
    - [1.3 Преобразователи ](#13-преобразователи-)
    - [1.4 Другие функции ](#14-другие-функции-)

## Справка

> Тип `Decimal` представляет десятичные числа в диапазоне от положительных 79,228,162,514,264,337,593,543,950,335 до отрицательных 79,228,162,514,264,337,593,543,950,335. Значение `Decimal` по умолчанию равно 0. `Decimal` подходит для финансовых расчетов, которые требуют большого количества значимых целых и дробных цифр и отсутствия ошибок округления. Этот тип не устраняет необходимость округления. Скорее, сводит к минимуму количество ошибок из-за округления.<br/>
> Когда результат деления и умножения передается методу округления, результат не страдает от потери точности.<br/>
> `Decimal` число - это значение с плавающей точкой, состоящее из знака, числового значения, где каждая цифра находится в диапазоне от 0 до 9, и коэффициента масштабирования, который указывает положение десятичной точки, разделяющей целые и дробные части числового значения.<br/>
> Двоичное представление `Decimal` состоит из 1-разрядного знака, 96-разрядного целого числа и коэффициента масштабирования, используемого для деления 96-разрядного целого числа и указания того, какая его часть является десятичной дробью. Коэффициент масштабирования неявно равен числу 10, возведенному в степень в диапазоне от 0 до 28. Следовательно, двоичное представление `Decimal` имеет вид ((от -2^96 до 2^96) / 10^(от 0 до 28)), где -(2^96-1) равно минимальному значению, а 2^96-1 равно максимальному значению.<br/>
> Коэффициент масштабирования также может сохранять любые конечные нули в `Decimal`. Эти конечные нули не влияют на значение в арифметических операциях или операциях сравнения.<br/>

## Общая информация <br/>

- Библиотека разработана на языке Си стандарта C11 с использованием компилятора gcc <br/>
- Решение оформлено как статическая библиотека (с заголовочным файлом my_decimal.h) <br/>
- Расположение файлов <br/>
    * исходные файлы в src/sources <br/>
    * заголовочные файлы в src/headers <br/>
    * сборочный файл в src <br/>
    * собранная статическая библиотека располагается в директории build/my_decimal.a <br/>
- Обеспечено полное покрытие unit-тестами функций библиотеки c помощью библиотеки Check <br/>
- Unit-тесты проверяют результаты работы реализованной библиотеки путём сравнения ее с реализацией библиотеки в С# <br/>
- Unit-тесты покрывают не менее 80% каждой функции <br/>
- Предусмотрен Makefile для сборки библиотеки и тестов (с целями all, clean, test, my_decimal.a, gcov_report) <br/> 
- В цели gcov_report формируется отчёт gcov в виде html-страницы <br/>
- Определяемый тип должен поддерживает числа от -79,228,162,514,264,337,593,543,950,335 до +79,228,162,514,264,337,593,543,950,335 <br/> 

## 1. Реализация функций <br/> 

### 1.1 Арифметические функции <br/> 

    | Название оператора | Оператор | Функция                                                                |
    | ------------------ | -------- | ---------------------------------------------------------------------- |
    | Сложение           | +        | int my_add(my_decimal value_1, my_decimal value_2, my_decimal *result) |
    | Вычитание          | -        | int my_sub(my_decimal value_1, my_decimal value_2, my_decimal *result) |
    | Умножение          | *        | int my_mul(my_decimal value_1, my_decimal value_2, my_decimal *result) |
    | Деление            | /        | int my_div(my_decimal value_1, my_decimal value_2, my_decimal *result) |
    | Остаток от деления | Mod      | int my_mod(my_decimal value_1, my_decimal value_2, my_decimal *result) |

* Функции возвращают код ошибки: <br/> 
- 0 - OK  <br/> 
- 1 - число слишком велико или равно бесконечности<br/> 
- 2 - число слишком мало или равно отрицательной бесконечности<br/> 
- 3 - деление на 0<br/> 

*Уточнение про числа, не вмещающиеся в мантиссу:*<br/> 
- *При получении чисел, не вмещающихся в мантиссу при арифметических операциях, используется банковское округление (например, 79,228,162,514,264,337,593,543,950,335 - 0.6 = 79,228,162,514,264,337,593,543,950,334)*<br/> 


### 1.2 Операторы сравнения <br/> 

| Название оператора | Оператор | Функция                                            |
| ------------------ | -------- | -------------------------------------------------- |
| Меньше             | <        | int my_is_less(my_decimal, my_decimal)             |
| Меньше или равно   | <=       | int my_is_less_or_equal(my_decimal, my_decimal)    |
| Больше             | \>       | int my_is_greater(my_decimal, my_decimal)          |
| Больше или равно   | \>=      | int my_is_greater_or_equal(my_decimal, my_decimal) |
| Равно              | ==       | int my_is_equal(my_decimal, my_decimal)            |
| Не равно           | !=       | int my_is_not_equal(my_decimal, my_decimal)        |

Возвращаемое значение:
- 0 - FALSE
- 1 - TRUE

### 1.3 Преобразователи <br/> 

| Преобразователь | Функция                                                  |
| --------------- | -------------------------------------------------------- |
| Из int          | int my_from_int_to_decimal(int src, my_decimal *dst)     |
| Из float        | int my_from_float_to_decimal(float src, my_decimal *dst) |
| В int           | int my_from_decimal_to_int(my_decimal src, int *dst)     |
| В float         | int my_from_decimal_to_float(my_decimal src, float *dst) |

Возвращаемое значение - код ошибки:<br/> 
 - 0 - OK<br/> 
 - 1 - ошибка конвертации<br/> 

*Уточнение про преобразование числа типа float:*<br/> 
  - *Если числа слишком малы (0 < |x| < 1e-28), возвращается ошибка и значение, равное 0*<br/> 
  - *Если числа слишком велики (|x| > 79,228,162,514,264,337,593,543,950,335) или равны бесконечности, возвращается ошибка*<br/> 
  - *При обработке числа с типом float преобразовываются все содержащиеся в нём значимые десятичные цифры. Если таких цифр больше 7, то значение числа округляется к ближайшему, у которого не больше 7 значимых цифр.*<br/> 

*Уточнение про преобразование из числа типа decimal в тип int:*<br/> 
  - *Если в числе типа decimal есть дробная часть, то она отрасывается(например, 0.9 преобразуется 0)*<br/> 

### 1.4 Другие функции <br/> 

| Описание                                                                                                   | Функция                                               |
| ---------------------------------------------------------------------------------------------------------- | ----------------------------------------------------- |
| Округляет указанное Decimal число до ближайшего целого числа в сторону отрицательной бесконечности.        | int my_floor(my_decimal value, my_decimal *result)    |
| Округляет Decimal до ближайшего целого числа.                                                              | int my_round(my_decimal value, my_decimal *result)    |
| Возвращает целые цифры указанного Decimal числа; любые дробные цифры отбрасываются, включая конечные нули. | int my_truncate(my_decimal value, my_decimal *result) |
| Возвращает результат умножения указанного Decimal на -1.                                                   | int my_negate(my_decimal value, my_decimal *result)   |

Возвращаемое значение - код ошибки:
 - 0 - OK
 - 1 - ошибка вычисления
