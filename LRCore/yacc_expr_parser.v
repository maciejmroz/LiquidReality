    0  $accept : wyrazenie $end

    1  wyrazenie : wyrazenie EXPR_COND_START wyrazenie EXPR_COND_END wyrazenie
    2            | wyrazenie_zwykle

    3  wyrazenie_zwykle : wyrazenie_zwykle op_warunku warunek
    4                   | warunek

    5  op_warunku : EXPR_EQUAL
    6             | EXPR_GREATER
    7             | EXPR_GEQUAL
    8             | EXPR_LESS
    9             | EXPR_LEQUAL
   10             | EXPR_NOTEQUAL

   11  warunek : warunek EXPR_PLUS skladnik
   12          | warunek EXPR_MINUS skladnik
   13          | skladnik

   14  skladnik : skladnik EXPR_MULTIPLY czynnik
   15           | skladnik EXPR_DIVIDE czynnik
   16           | czynnik

   17  czynnik : czynnik EXPR_POWER wykladnik
   18          | wykladnik

   19  wykladnik : funkcja argument
   20            | argument

   21  funkcja : EXPR_SIN
   22          | EXPR_COS
   23          | EXPR_TG
   24          | EXPR_CTG
   25          | EXPR_ASIN
   26          | EXPR_ACOS
   27          | EXPR_ATAN
   28          | EXPR_SQRT
   29          | EXPR_EXP
   30          | EXPR_LN
   31          | EXPR_FLOOR
   32          | EXPR_CEIL
   33          | EXPR_ABS
   34          | EXPR_FFT
   35          | EXPR_CHANNEL
   36          | EXPR_MATERIAL_CHANNEL
   37          | EXPR_MESH_NODE_CHANNEL
   38          | EXPR_TRANSFORM_NODE_CHANNEL
   39          | EXPR_MINUS

   40  argument : EXPR_LBRACKET wyrazenie EXPR_RBRACKET
   41           | EXPR_FPNUMBER
   42           | EXPR_TIME
   43           | EXPR_ENVELOPE
   44           | EXPR_VOLUME
   45           | EXPR_INPUT_VARIABLE


state 0
	$accept : . wyrazenie $end

	EXPR_FPNUMBER  shift 1
	EXPR_MINUS  shift 2
	EXPR_LBRACKET  shift 3
	EXPR_TIME  shift 4
	EXPR_ENVELOPE  shift 5
	EXPR_VOLUME  shift 6
	EXPR_SIN  shift 7
	EXPR_COS  shift 8
	EXPR_TG  shift 9
	EXPR_CTG  shift 10
	EXPR_ASIN  shift 11
	EXPR_ACOS  shift 12
	EXPR_ATAN  shift 13
	EXPR_SQRT  shift 14
	EXPR_EXP  shift 15
	EXPR_LN  shift 16
	EXPR_FLOOR  shift 17
	EXPR_CEIL  shift 18
	EXPR_ABS  shift 19
	EXPR_FFT  shift 20
	EXPR_CHANNEL  shift 21
	EXPR_MATERIAL_CHANNEL  shift 22
	EXPR_MESH_NODE_CHANNEL  shift 23
	EXPR_TRANSFORM_NODE_CHANNEL  shift 24
	EXPR_INPUT_VARIABLE  shift 25

	wyrazenie goto 26
	wyrazenie_zwykle goto 27
	warunek goto 28
	skladnik goto 29
	czynnik goto 30
	wykladnik goto 31
	funkcja goto 32
	argument goto 33


state 1
	argument : EXPR_FPNUMBER .  (41)

	.  reduce 41


state 2
	funkcja : EXPR_MINUS .  (39)

	.  reduce 39


state 3
	argument : EXPR_LBRACKET . wyrazenie EXPR_RBRACKET

	EXPR_FPNUMBER  shift 1
	EXPR_MINUS  shift 2
	EXPR_LBRACKET  shift 3
	EXPR_TIME  shift 4
	EXPR_ENVELOPE  shift 5
	EXPR_VOLUME  shift 6
	EXPR_SIN  shift 7
	EXPR_COS  shift 8
	EXPR_TG  shift 9
	EXPR_CTG  shift 10
	EXPR_ASIN  shift 11
	EXPR_ACOS  shift 12
	EXPR_ATAN  shift 13
	EXPR_SQRT  shift 14
	EXPR_EXP  shift 15
	EXPR_LN  shift 16
	EXPR_FLOOR  shift 17
	EXPR_CEIL  shift 18
	EXPR_ABS  shift 19
	EXPR_FFT  shift 20
	EXPR_CHANNEL  shift 21
	EXPR_MATERIAL_CHANNEL  shift 22
	EXPR_MESH_NODE_CHANNEL  shift 23
	EXPR_TRANSFORM_NODE_CHANNEL  shift 24
	EXPR_INPUT_VARIABLE  shift 25

	wyrazenie goto 34
	wyrazenie_zwykle goto 27
	warunek goto 28
	skladnik goto 29
	czynnik goto 30
	wykladnik goto 31
	funkcja goto 32
	argument goto 33


state 4
	argument : EXPR_TIME .  (42)

	.  reduce 42


state 5
	argument : EXPR_ENVELOPE .  (43)

	.  reduce 43


state 6
	argument : EXPR_VOLUME .  (44)

	.  reduce 44


state 7
	funkcja : EXPR_SIN .  (21)

	.  reduce 21


state 8
	funkcja : EXPR_COS .  (22)

	.  reduce 22


state 9
	funkcja : EXPR_TG .  (23)

	.  reduce 23


state 10
	funkcja : EXPR_CTG .  (24)

	.  reduce 24


state 11
	funkcja : EXPR_ASIN .  (25)

	.  reduce 25


state 12
	funkcja : EXPR_ACOS .  (26)

	.  reduce 26


state 13
	funkcja : EXPR_ATAN .  (27)

	.  reduce 27


state 14
	funkcja : EXPR_SQRT .  (28)

	.  reduce 28


state 15
	funkcja : EXPR_EXP .  (29)

	.  reduce 29


state 16
	funkcja : EXPR_LN .  (30)

	.  reduce 30


state 17
	funkcja : EXPR_FLOOR .  (31)

	.  reduce 31


state 18
	funkcja : EXPR_CEIL .  (32)

	.  reduce 32


state 19
	funkcja : EXPR_ABS .  (33)

	.  reduce 33


state 20
	funkcja : EXPR_FFT .  (34)

	.  reduce 34


state 21
	funkcja : EXPR_CHANNEL .  (35)

	.  reduce 35


state 22
	funkcja : EXPR_MATERIAL_CHANNEL .  (36)

	.  reduce 36


state 23
	funkcja : EXPR_MESH_NODE_CHANNEL .  (37)

	.  reduce 37


state 24
	funkcja : EXPR_TRANSFORM_NODE_CHANNEL .  (38)

	.  reduce 38


state 25
	argument : EXPR_INPUT_VARIABLE .  (45)

	.  reduce 45


state 26
	$accept : wyrazenie . $end  (0)
	wyrazenie : wyrazenie . EXPR_COND_START wyrazenie EXPR_COND_END wyrazenie

	$end  accept
	EXPR_COND_START  shift 35


state 27
	wyrazenie : wyrazenie_zwykle .  (2)
	wyrazenie_zwykle : wyrazenie_zwykle . op_warunku warunek

	EXPR_EQUAL  shift 36
	EXPR_GREATER  shift 37
	EXPR_GEQUAL  shift 38
	EXPR_LESS  shift 39
	EXPR_LEQUAL  shift 40
	EXPR_NOTEQUAL  shift 41
	.  reduce 2

	op_warunku goto 42


state 28
	wyrazenie_zwykle : warunek .  (4)
	warunek : warunek . EXPR_PLUS skladnik
	warunek : warunek . EXPR_MINUS skladnik

	EXPR_MINUS  shift 43
	EXPR_PLUS  shift 44
	.  reduce 4


state 29
	warunek : skladnik .  (13)
	skladnik : skladnik . EXPR_MULTIPLY czynnik
	skladnik : skladnik . EXPR_DIVIDE czynnik

	EXPR_MULTIPLY  shift 45
	EXPR_DIVIDE  shift 46
	.  reduce 13


state 30
	skladnik : czynnik .  (16)
	czynnik : czynnik . EXPR_POWER wykladnik

	EXPR_POWER  shift 47
	.  reduce 16


state 31
	czynnik : wykladnik .  (18)

	.  reduce 18


state 32
	wykladnik : funkcja . argument

	EXPR_FPNUMBER  shift 1
	EXPR_LBRACKET  shift 3
	EXPR_TIME  shift 4
	EXPR_ENVELOPE  shift 5
	EXPR_VOLUME  shift 6
	EXPR_INPUT_VARIABLE  shift 25

	argument goto 48


state 33
	wykladnik : argument .  (20)

	.  reduce 20


state 34
	wyrazenie : wyrazenie . EXPR_COND_START wyrazenie EXPR_COND_END wyrazenie
	argument : EXPR_LBRACKET wyrazenie . EXPR_RBRACKET

	EXPR_RBRACKET  shift 49
	EXPR_COND_START  shift 35


state 35
	wyrazenie : wyrazenie EXPR_COND_START . wyrazenie EXPR_COND_END wyrazenie

	EXPR_FPNUMBER  shift 1
	EXPR_MINUS  shift 2
	EXPR_LBRACKET  shift 3
	EXPR_TIME  shift 4
	EXPR_ENVELOPE  shift 5
	EXPR_VOLUME  shift 6
	EXPR_SIN  shift 7
	EXPR_COS  shift 8
	EXPR_TG  shift 9
	EXPR_CTG  shift 10
	EXPR_ASIN  shift 11
	EXPR_ACOS  shift 12
	EXPR_ATAN  shift 13
	EXPR_SQRT  shift 14
	EXPR_EXP  shift 15
	EXPR_LN  shift 16
	EXPR_FLOOR  shift 17
	EXPR_CEIL  shift 18
	EXPR_ABS  shift 19
	EXPR_FFT  shift 20
	EXPR_CHANNEL  shift 21
	EXPR_MATERIAL_CHANNEL  shift 22
	EXPR_MESH_NODE_CHANNEL  shift 23
	EXPR_TRANSFORM_NODE_CHANNEL  shift 24
	EXPR_INPUT_VARIABLE  shift 25

	wyrazenie goto 50
	wyrazenie_zwykle goto 27
	warunek goto 28
	skladnik goto 29
	czynnik goto 30
	wykladnik goto 31
	funkcja goto 32
	argument goto 33


state 36
	op_warunku : EXPR_EQUAL .  (5)

	.  reduce 5


state 37
	op_warunku : EXPR_GREATER .  (6)

	.  reduce 6


state 38
	op_warunku : EXPR_GEQUAL .  (7)

	.  reduce 7


state 39
	op_warunku : EXPR_LESS .  (8)

	.  reduce 8


state 40
	op_warunku : EXPR_LEQUAL .  (9)

	.  reduce 9


state 41
	op_warunku : EXPR_NOTEQUAL .  (10)

	.  reduce 10


state 42
	wyrazenie_zwykle : wyrazenie_zwykle op_warunku . warunek

	EXPR_FPNUMBER  shift 1
	EXPR_MINUS  shift 2
	EXPR_LBRACKET  shift 3
	EXPR_TIME  shift 4
	EXPR_ENVELOPE  shift 5
	EXPR_VOLUME  shift 6
	EXPR_SIN  shift 7
	EXPR_COS  shift 8
	EXPR_TG  shift 9
	EXPR_CTG  shift 10
	EXPR_ASIN  shift 11
	EXPR_ACOS  shift 12
	EXPR_ATAN  shift 13
	EXPR_SQRT  shift 14
	EXPR_EXP  shift 15
	EXPR_LN  shift 16
	EXPR_FLOOR  shift 17
	EXPR_CEIL  shift 18
	EXPR_ABS  shift 19
	EXPR_FFT  shift 20
	EXPR_CHANNEL  shift 21
	EXPR_MATERIAL_CHANNEL  shift 22
	EXPR_MESH_NODE_CHANNEL  shift 23
	EXPR_TRANSFORM_NODE_CHANNEL  shift 24
	EXPR_INPUT_VARIABLE  shift 25

	warunek goto 51
	skladnik goto 29
	czynnik goto 30
	wykladnik goto 31
	funkcja goto 32
	argument goto 33


state 43
	warunek : warunek EXPR_MINUS . skladnik

	EXPR_FPNUMBER  shift 1
	EXPR_MINUS  shift 2
	EXPR_LBRACKET  shift 3
	EXPR_TIME  shift 4
	EXPR_ENVELOPE  shift 5
	EXPR_VOLUME  shift 6
	EXPR_SIN  shift 7
	EXPR_COS  shift 8
	EXPR_TG  shift 9
	EXPR_CTG  shift 10
	EXPR_ASIN  shift 11
	EXPR_ACOS  shift 12
	EXPR_ATAN  shift 13
	EXPR_SQRT  shift 14
	EXPR_EXP  shift 15
	EXPR_LN  shift 16
	EXPR_FLOOR  shift 17
	EXPR_CEIL  shift 18
	EXPR_ABS  shift 19
	EXPR_FFT  shift 20
	EXPR_CHANNEL  shift 21
	EXPR_MATERIAL_CHANNEL  shift 22
	EXPR_MESH_NODE_CHANNEL  shift 23
	EXPR_TRANSFORM_NODE_CHANNEL  shift 24
	EXPR_INPUT_VARIABLE  shift 25

	skladnik goto 52
	czynnik goto 30
	wykladnik goto 31
	funkcja goto 32
	argument goto 33


state 44
	warunek : warunek EXPR_PLUS . skladnik

	EXPR_FPNUMBER  shift 1
	EXPR_MINUS  shift 2
	EXPR_LBRACKET  shift 3
	EXPR_TIME  shift 4
	EXPR_ENVELOPE  shift 5
	EXPR_VOLUME  shift 6
	EXPR_SIN  shift 7
	EXPR_COS  shift 8
	EXPR_TG  shift 9
	EXPR_CTG  shift 10
	EXPR_ASIN  shift 11
	EXPR_ACOS  shift 12
	EXPR_ATAN  shift 13
	EXPR_SQRT  shift 14
	EXPR_EXP  shift 15
	EXPR_LN  shift 16
	EXPR_FLOOR  shift 17
	EXPR_CEIL  shift 18
	EXPR_ABS  shift 19
	EXPR_FFT  shift 20
	EXPR_CHANNEL  shift 21
	EXPR_MATERIAL_CHANNEL  shift 22
	EXPR_MESH_NODE_CHANNEL  shift 23
	EXPR_TRANSFORM_NODE_CHANNEL  shift 24
	EXPR_INPUT_VARIABLE  shift 25

	skladnik goto 53
	czynnik goto 30
	wykladnik goto 31
	funkcja goto 32
	argument goto 33


state 45
	skladnik : skladnik EXPR_MULTIPLY . czynnik

	EXPR_FPNUMBER  shift 1
	EXPR_MINUS  shift 2
	EXPR_LBRACKET  shift 3
	EXPR_TIME  shift 4
	EXPR_ENVELOPE  shift 5
	EXPR_VOLUME  shift 6
	EXPR_SIN  shift 7
	EXPR_COS  shift 8
	EXPR_TG  shift 9
	EXPR_CTG  shift 10
	EXPR_ASIN  shift 11
	EXPR_ACOS  shift 12
	EXPR_ATAN  shift 13
	EXPR_SQRT  shift 14
	EXPR_EXP  shift 15
	EXPR_LN  shift 16
	EXPR_FLOOR  shift 17
	EXPR_CEIL  shift 18
	EXPR_ABS  shift 19
	EXPR_FFT  shift 20
	EXPR_CHANNEL  shift 21
	EXPR_MATERIAL_CHANNEL  shift 22
	EXPR_MESH_NODE_CHANNEL  shift 23
	EXPR_TRANSFORM_NODE_CHANNEL  shift 24
	EXPR_INPUT_VARIABLE  shift 25

	czynnik goto 54
	wykladnik goto 31
	funkcja goto 32
	argument goto 33


state 46
	skladnik : skladnik EXPR_DIVIDE . czynnik

	EXPR_FPNUMBER  shift 1
	EXPR_MINUS  shift 2
	EXPR_LBRACKET  shift 3
	EXPR_TIME  shift 4
	EXPR_ENVELOPE  shift 5
	EXPR_VOLUME  shift 6
	EXPR_SIN  shift 7
	EXPR_COS  shift 8
	EXPR_TG  shift 9
	EXPR_CTG  shift 10
	EXPR_ASIN  shift 11
	EXPR_ACOS  shift 12
	EXPR_ATAN  shift 13
	EXPR_SQRT  shift 14
	EXPR_EXP  shift 15
	EXPR_LN  shift 16
	EXPR_FLOOR  shift 17
	EXPR_CEIL  shift 18
	EXPR_ABS  shift 19
	EXPR_FFT  shift 20
	EXPR_CHANNEL  shift 21
	EXPR_MATERIAL_CHANNEL  shift 22
	EXPR_MESH_NODE_CHANNEL  shift 23
	EXPR_TRANSFORM_NODE_CHANNEL  shift 24
	EXPR_INPUT_VARIABLE  shift 25

	czynnik goto 55
	wykladnik goto 31
	funkcja goto 32
	argument goto 33


state 47
	czynnik : czynnik EXPR_POWER . wykladnik

	EXPR_FPNUMBER  shift 1
	EXPR_MINUS  shift 2
	EXPR_LBRACKET  shift 3
	EXPR_TIME  shift 4
	EXPR_ENVELOPE  shift 5
	EXPR_VOLUME  shift 6
	EXPR_SIN  shift 7
	EXPR_COS  shift 8
	EXPR_TG  shift 9
	EXPR_CTG  shift 10
	EXPR_ASIN  shift 11
	EXPR_ACOS  shift 12
	EXPR_ATAN  shift 13
	EXPR_SQRT  shift 14
	EXPR_EXP  shift 15
	EXPR_LN  shift 16
	EXPR_FLOOR  shift 17
	EXPR_CEIL  shift 18
	EXPR_ABS  shift 19
	EXPR_FFT  shift 20
	EXPR_CHANNEL  shift 21
	EXPR_MATERIAL_CHANNEL  shift 22
	EXPR_MESH_NODE_CHANNEL  shift 23
	EXPR_TRANSFORM_NODE_CHANNEL  shift 24
	EXPR_INPUT_VARIABLE  shift 25

	wykladnik goto 56
	funkcja goto 32
	argument goto 33


state 48
	wykladnik : funkcja argument .  (19)

	.  reduce 19


state 49
	argument : EXPR_LBRACKET wyrazenie EXPR_RBRACKET .  (40)

	.  reduce 40


state 50
	wyrazenie : wyrazenie . EXPR_COND_START wyrazenie EXPR_COND_END wyrazenie
	wyrazenie : wyrazenie EXPR_COND_START wyrazenie . EXPR_COND_END wyrazenie

	EXPR_COND_START  shift 35
	EXPR_COND_END  shift 57


state 51
	wyrazenie_zwykle : wyrazenie_zwykle op_warunku warunek .  (3)
	warunek : warunek . EXPR_PLUS skladnik
	warunek : warunek . EXPR_MINUS skladnik

	EXPR_MINUS  shift 43
	EXPR_PLUS  shift 44
	.  reduce 3


state 52
	warunek : warunek EXPR_MINUS skladnik .  (12)
	skladnik : skladnik . EXPR_MULTIPLY czynnik
	skladnik : skladnik . EXPR_DIVIDE czynnik

	EXPR_MULTIPLY  shift 45
	EXPR_DIVIDE  shift 46
	.  reduce 12


state 53
	warunek : warunek EXPR_PLUS skladnik .  (11)
	skladnik : skladnik . EXPR_MULTIPLY czynnik
	skladnik : skladnik . EXPR_DIVIDE czynnik

	EXPR_MULTIPLY  shift 45
	EXPR_DIVIDE  shift 46
	.  reduce 11


state 54
	skladnik : skladnik EXPR_MULTIPLY czynnik .  (14)
	czynnik : czynnik . EXPR_POWER wykladnik

	EXPR_POWER  shift 47
	.  reduce 14


state 55
	skladnik : skladnik EXPR_DIVIDE czynnik .  (15)
	czynnik : czynnik . EXPR_POWER wykladnik

	EXPR_POWER  shift 47
	.  reduce 15


state 56
	czynnik : czynnik EXPR_POWER wykladnik .  (17)

	.  reduce 17


state 57
	wyrazenie : wyrazenie EXPR_COND_START wyrazenie EXPR_COND_END . wyrazenie

	EXPR_FPNUMBER  shift 1
	EXPR_MINUS  shift 2
	EXPR_LBRACKET  shift 3
	EXPR_TIME  shift 4
	EXPR_ENVELOPE  shift 5
	EXPR_VOLUME  shift 6
	EXPR_SIN  shift 7
	EXPR_COS  shift 8
	EXPR_TG  shift 9
	EXPR_CTG  shift 10
	EXPR_ASIN  shift 11
	EXPR_ACOS  shift 12
	EXPR_ATAN  shift 13
	EXPR_SQRT  shift 14
	EXPR_EXP  shift 15
	EXPR_LN  shift 16
	EXPR_FLOOR  shift 17
	EXPR_CEIL  shift 18
	EXPR_ABS  shift 19
	EXPR_FFT  shift 20
	EXPR_CHANNEL  shift 21
	EXPR_MATERIAL_CHANNEL  shift 22
	EXPR_MESH_NODE_CHANNEL  shift 23
	EXPR_TRANSFORM_NODE_CHANNEL  shift 24
	EXPR_INPUT_VARIABLE  shift 25

	wyrazenie goto 58
	wyrazenie_zwykle goto 27
	warunek goto 28
	skladnik goto 29
	czynnik goto 30
	wykladnik goto 31
	funkcja goto 32
	argument goto 33


58: shift-reduce conflict (shift 35, reduce 1) on EXPR_COND_START
state 58
	wyrazenie : wyrazenie EXPR_COND_START wyrazenie EXPR_COND_END wyrazenie .  (1)
	wyrazenie : wyrazenie . EXPR_COND_START wyrazenie EXPR_COND_END wyrazenie

	EXPR_COND_START  shift 35
	.  reduce 1


State 58 contains 1 shift-reduce conflict


40 tokens, 10 nonterminals
46 grammar rules, 59 states
