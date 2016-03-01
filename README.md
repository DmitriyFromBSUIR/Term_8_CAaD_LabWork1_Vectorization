# Term_8_CAaD_LabWork1_Vectorization

Разработка алгоритмов с использованием автоматической и ручной векторизации

Краткие теоретические сведения:

    Введение

    1. Развертка циклов

    2. Автоматическая векторизация

    3. Ограничения автоматической векторизации

    4. Векторные инструкции

    5. Выравнивание данных

    6. Способы измерения времени

Задание к лабораторной работе

Требования к защите

     Введение

   Векторизация – это когда вместо использования обычных скалярных инструкций программа пишется с использованием векторных инструкций. Векторными называются те инструкции, операндами которых являются вектора. По другому векторные инструкции называются SIMD (Single Instruction Multiply Data). Операция, которая описывается инструкцией, исполняется как минимум над элементами из двух векторов одновременно. В результате вычисления также получается вектор. Процесс векторизации может выполняться компилятором или программистом.

   Данный подход является распространенным способом повышения производительности. Для этого необходимо, чтобы процессор поддерживал требуемый набор векторных инструкций аппаратно. Использование векторных инструкций основано на наличие в программе параллелизма на уровне данных. Параллелизм на уровне данных - это когда входные элементы данных могут обрабатываться параллельно. Для ускорения исполнения SIMD инструкций процессоры оснащаются несколькими АЛУ и расширенными векторными регистрами.

   Векторизация как подход к оптимизирующей компиляции основывается на развертке циклов.


     1. Развертка циклов

   Подход к повышению производительности, основанный на том, что тело цикла удваивается, а количество итераций уменьшается. Развертка циклов приводит к тому, что уменьшается количество исполняемых инструкций связанных с вычислением и проверкой условия. Рассмотрим пример развертки цикла:

  loop:

        ld r4, [r1]

        ld r5, [r2]

        add r4, r4, r5

        st [r3], r4

        add r1, r1, 4

        add r2, r2, 4

        add r3, r3, 4

        bne r1, r10, loop
	

  loop:

        ld r4, [r1]

        ld r5, [r2]

        add r4, r4, r5

        st [r3], r4

        ld r4, [r1 + 4]

        ld r5, [r2 + 4]

        add r4, r4, r5

        st [r3 + 4], r4

        add r1, r1, 8

        add r2, r2, 8

        add r3, r3, 8

        bne r1, r10, loop

   В первом случае на 4 вычислительные инструкции (выделены зеленым цветом) приходиться 4 сервисные инструкции (синий цвет). Во втором случае на 8 инструкций приходиться 4 сервисные инструкции. При дальнейшей развертке среднее количество инструкций на итерацию снижается до 4.

   Необходимые условия для развертки цикла:

    цикл должен иметь тип for, так как необходимо знать функцию вычисления индекса;
    количество итераций цикла не должно зависеть от данных;
    при не известном во время компиляции количестве итераций цикла, нужно генерировать код, который будет выполнять это во время исполнения. В этом случае цикл будет состоять из развернутой части и обычной части. Например степень развертки 8. Количество итераций цикла 22, тогда первые 16 итераций исполняются двумя итерациями развернутого цикла, а 6 итераций исполняются на обычном цикле.


     2. Автоматическая векторизация

   Векторизация схоже с процессом развертки цикла. Она выполняется только для операций которые находятся внутри цикла и только в том случае, если итерации цикла являются независимыми. Скалярные элементы объединяются в вектора, а скалярные операции заменяются векторными. Векторизация является разверткой цикла, где степень развертки равна длине вектора. При неизвестном количестве итераций на этапе компиляции, векторизованный цикл дополняется обычным циклом для вычисления количества итераций меньших, чем размер вектора.

   Необходимые условие автоматической векторизации компилятором:

    операция находится в цикле;
    итерации цикла независимы;
    число итераций в цикле не должно зависеть от данных. Это значит, что не должно быть break и continue;
    не должны присутствовать условные операторы;
    типы данных и операции, которые должны быть векторизованы, должны аппаратно поддерживаться процессором, т.е. для них должны существовать SIMD инструкции;
    внутри цикла не должно быть вызова функций;
    шаг между элементами, которые обрабатываются циклом, должен быть равен 1.

   Рассмотрим более подробно правило с шагом, равным 1. Оно гласит, что данные, которые объединяются в вектора, должны находиться по соседним адресам в памяти. Это нужно потому, что чтение данных из памяти выполняется вектором. Сбор вектора из отдельных элементов значительно более затратно.


   3. Ограничения автоматической векторизации

   Использование компилятора для векторизации не всегда является хорошим решением. Компилятор может выделить только очень простые шаблоны: операция вектора на число, операция вектора на вектор. Любые более сложные шаблоны которые содержат нерегулярные обращения к памяти, или условные инструкции могут быть векторизованы не самым оптимальным способом. Также векторные инструкции современных процессоров выполняют действия, которые очень сложно описать на языке высокого уровня хорошо распознаваемым паттером.


   4. Векторные инструкции

   Все векторные инструкции можно условно разделить на целочисленные и вещественные, по типу данных с которыми они оперируют. Векторные инструкции не добавлялись в архитектуру одним набором. Они добавляются постепенно различными расширениями: MMX, SSE и т.д.

   Существует три основных типа регистров, с которыми работают векторные инструкции - MMX, XMM, YMM. Исторически первыми у процессоров появились MMX регистры, их разрядность 64 бита. На текущий момент можно сказать, что эти регистры устарели, так как вместо них во всех инструкциях можно использовать XMM регистры, которые имеют разрядность 128 бит, и YMM регистры с разрядностью 256 либо 512 бит в зависимости от архитектуры процессора.

   Целочисленные векторные инструкции входят в наборы MMX, SSE2, SSE3, SSSE3, SSE4.1. В качестве аргументов инструкции могут использовать как 64-ех битные регистры MMX, так и 128-ми битные регистры XMM. Поддерживается два типа арифметики: с насыщением, без насыщения (циклическая). Целочисленный векторный набора характеризуется отсутствие ортогональности к типам данных.

   Вещественные векторные инструкции входят в наборы SSE, SSE2, SSE3, SSE4.1 В качестве аргументов инструкции используют только 128-ми битные регистры XMM. Вещественный векторный набор характеризуется большей степенью ортогональности к типам данных, чем целочисленный набор. Для написания лабораторной работ достаточно использовать инструкции из наборах до SSE2 включительно. Если архитектура процессора позволяет, можно использовать и более позднее наборы.


   5. Выравнивание данных

   Это еще один метод оптимизации, который можно использовать при разработке программ с использованием векторных инструкций. Дело в том, что вектора из памяти можно читать двумя инструкциями: movaps и movups. Первая инструкция может читать только те данные, адреса которых выровнены на границу 16 байт. Вторая инструкция может читать данные, которые находятся по невыровненным адресам. Первая инструкция работает значительно быстрее, чем вторая.

   Для выравнивания памяти можно использовать два подхода. Если память выделяется глобально или на стеке, тогда можно воспользоваться директивой компилятора declspec, которая в коде выглядит так: __declspec(align(n)), где n - это размер границы, на которую нужно выровнять стартовый указатель. Для нашего случая n равно 16. Второй подход используется в случае, когда память выделяется в куче. Тогда вместо обычных функций типа malloc используется функция align_malloc. Последним аргументом этой функции предается размер границы, на которую нужно выровнять. Любой из подходов выравнивает начальный адрес блока данных. Если в алгоритме используется двойной массив, нужно следить за тем, чтобы размер строки был кратен 16 байтам. Это нужно для того, чтобы начала всех строк двумерной матрицы были выровнены на 16 байтную границу.

   Еще одним способом ускорения работы программы является использование инструкций прямой записи в памяти минуя кэш. MOVNTPS - является одной из таких инструкций. Это помогает, если данные нужно только сохранить, но не нужно затем читать. В случае обычной записи данные сначала загружаются кэш, потом модифицируются в кэше, а только потом записываются в основную память.


   6. Способы измерения времени

   В текущей и последующих лабораторных работах нужно будет измерять время выполнения программы. Для CPU для этого можно использовать один из двух основных подходов. Первый основан на использовании функций WinAPI, второй - использование инструкции RDTSC.

   В операционной системе Windows существует функция QueryPerformanceCounter, которая возвращает текущее значение 64-ех битного счетчика. Частоту переключение этого счетчика можно определить функцией QueryPerformanceFrequency.

   У процессоров с архитектурой х86 есть специальная инструкций RDTSC. Эта инструкция возвращает текущее значение счетчика тактов в пару регистров edx:eax. Этот счетчик тактов инициализируется при сбросе процессора, а потом наращивается каждый такт. Актуальное значение тактов можно получить только в том случае, когда процессор не находиться в энергосберегающем режиме. Это особенно актуально для ноутбуков. Использование инструкции RDTSC позволяет наиболее точно выполнять замеры времени. Поэтому ее необходимо использовать там, где измеряемые интервалы малы.

Задание к лабораторной работе:

   Разработать программу перемножения двух матриц. Предусмотреть следующие возможности:

    классическое перемножение без оптимизации (эталонная версия);
    перемножение с применением автоматической векторизации средствами компилятора;
    выполнить ручную векторизацию кода.

Требования к защите:

    результат перемножения, полученный от векторизованного кода, сравнивается с эталоном;
    измеряется время выполнения каждого из реализованных алгоритмов.
