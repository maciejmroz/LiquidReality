#ifndef _YACC_EXPR_PARSER_H
#define _YACC_EXPR_PARSER_H

#include <myacc.h>

extern YYCONST yymparse_t YYNEAR expr_parser;

#ifndef YYPARSENAME
#define YYPARSENAME expr_parser
#endif

#define EXPR_FPNUMBER 257
#define EXPR_MINUS 258
#define EXPR_PLUS 259
#define EXPR_MULTIPLY 260
#define EXPR_DIVIDE 261
#define EXPR_LBRACKET 262
#define EXPR_RBRACKET 263
#define EXPR_POWER 264
#define EXPR_TIME 265
#define EXPR_ENVELOPE 266
#define EXPR_VOLUME 267
#define EXPR_SIN 268
#define EXPR_COS 269
#define EXPR_TG 270
#define EXPR_CTG 271
#define EXPR_ASIN 272
#define EXPR_ACOS 273
#define EXPR_ATAN 274
#define EXPR_SQRT 275
#define EXPR_EXP 276
#define EXPR_LN 277
#define EXPR_FLOOR 278
#define EXPR_CEIL 279
#define EXPR_ABS 280
#define EXPR_FFT 281
#define EXPR_CHANNEL 282
#define EXPR_MATERIAL_CHANNEL 283
#define EXPR_MESH_NODE_CHANNEL 284
#define EXPR_TRANSFORM_NODE_CHANNEL 285
#define EXPR_INPUT_VARIABLE 286
#define EXPR_COND_START 287
#define EXPR_COND_END 288
#define EXPR_EQUAL 289
#define EXPR_GREATER 290
#define EXPR_GEQUAL 291
#define EXPR_LESS 292
#define EXPR_LEQUAL 293
#define EXPR_NOTEQUAL 294
#define EXPR_PARSE_ERROR 295
#endif
