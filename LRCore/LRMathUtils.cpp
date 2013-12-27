/*****************************************************************
 Misc math utilities
 Crippled version of old code :)
 All matrix/vector/quat math uses D3DX now
 LiquidReality engine (c) 2001-2002 Maciej Mróz
 *****************************************************************/

#include "StdAfx.h"

/*** FAST INVERSE SQUARE ROOT ***/

/* From "Graphics Gems V", Alan Paeth (Editor)
 * ISBN 0125434553/9649-1547332-306386
 * Published by Ap Profession, 1995
 */

/* Compute the Inverse Square Root
 * of an IEEE Single Precision Floating-Point number.
 *
 * Written by Ken Turkowski.
 */

/* Specified parameters */
#define LOOKUP_BITS    6   /* Number of mantissa bits for lookup */
#define EXP_POS       23   /* Position of the exponent */
#define EXP_BIAS     127   /* Bias of exponent */
/* The mantissa is assumed to be just down from the exponent */

/* Derived parameters */
#define LOOKUP_POS   (EXP_POS-LOOKUP_BITS)  /* Position of mantissa lookup */
#define SEED_POS     (EXP_POS-8)            /* Position of mantissa seed */
#define TABLE_SIZE   (2 << LOOKUP_BITS)     /* Number of entries in table */
#define LOOKUP_MASK  (TABLE_SIZE - 1)           /* Mask for table input */
#define GET_EXP(a)   (((a) >> EXP_POS) & 0xFF)  /* Extract exponent */
#define SET_EXP(a)   ((a) << EXP_POS)           /* Set exponent */
#define GET_EMANT(a) (((a) >> LOOKUP_POS) & LOOKUP_MASK)  /* Extended mantissa
                                                           * MSB's */
#define SET_MANTSEED(a) (((unsigned long)(a)) << SEED_POS)  /* Set mantissa
                                                             * 8 MSB's */

static unsigned char iSqrt[TABLE_SIZE];

union _flint {
  unsigned long    i;
  float            f;
} _fi, _fo;

void
makeInverseSqrtLookupTable(void)
{
  register long f;
  register unsigned char *h;
  union _flint fi, fo;

  h = iSqrt;
  for (f = 0, h = iSqrt; f < TABLE_SIZE; f++) {
    fi.i = ((EXP_BIAS-1) << EXP_POS) | (f << LOOKUP_POS);
    fo.f = (float) (1.0 / sqrt(fi.f));
    *h++ = (unsigned char)
           (((fo.i + (1<<(SEED_POS-2))) >> SEED_POS) & 0xFF); /* rounding */
  }
  iSqrt[TABLE_SIZE / 2] = 0xFF;    /* Special case for 1.0 */
}

/* Non-WinTel platforms don't need fastcall. */
#ifndef FASTCALL
#define FASTCALL
#endif

/* The following returns the inverse square root. */
static float FASTCALL
invSqrt(float x)
{
  unsigned long a = ((union _flint*)(&x))->i;
  float arg = x;
  union _flint seed;
  float r;

  seed.i = SET_EXP(((3*EXP_BIAS-1) - GET_EXP(a)) >> 1)
         | SET_MANTSEED(iSqrt[GET_EMANT(a)]);

  /* Seed: accurate to LOOKUP_BITS */
  r = seed.f;

  /* First iteration: accurate to 2*LOOKUP_BITS */
  r = (float) ((3.0 - r * r * arg) * r * 0.5);

#if 0  /* Wow!  We don't need this much precision! */
  /* Second iteration: accurate to 4*LOOKUP_BITS */
  r = (float) ((3.0 - r * r * arg) * r * 0.5);
#endif

  return r;
}


/*---------------------------------------------------------------------------
	Floating Point math
	Author: Ignacio Castaño Aguado (part of his Titan engine code)
---------------------------------------------------------------------------*/

__declspec(naked) float __fastcall FastAbs(float a) {
	__asm {
		fld		DWORD PTR [esp+4] 
		fabs
		ret 4
	}
}

__declspec(naked) float __fastcall FastSin(float a) {
	__asm {
		fld		DWORD PTR [esp+4] 
		fsin
		ret 4
	}
}

__declspec(naked) float __fastcall FastCos(float a) {
	__asm {
		fld		DWORD PTR [esp+4] 
		fcos
		ret 4
	}
}
