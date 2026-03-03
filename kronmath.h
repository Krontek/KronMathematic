#ifndef KRONMATH_H
#define KRONMATH_H

#include <stdint.h>

/* Math constants */
#define KRON_PI      3.14159265f
#define KRON_HALF_PI 1.57079633f
#define KRON_TWO_PI  6.28318531f
#define KRON_E       2.71828183f

/* =========================================================
 * Basic Math - Float (REAL)
 *
 * KRON_DIV returns 0 on division by zero.
 * KRON_MOD returns 0 when divisor is zero.
 * ========================================================= */
float   KRON_ADD  (float a, float b);
float   KRON_SUB  (float a, float b);
float   KRON_MUL  (float a, float b);
float   KRON_DIV  (float a, float b);
float   KRON_MOD  (float a, float b);
float   KRON_MOVE (float a);

/* =========================================================
 * Basic Math - Integer (DINT / int32_t)
 *
 * KRON_DIV_I and KRON_MOD_I return 0 on division by zero.
 * ========================================================= */
int32_t KRON_ADD_I  (int32_t a, int32_t b);
int32_t KRON_SUB_I  (int32_t a, int32_t b);
int32_t KRON_MUL_I  (int32_t a, int32_t b);
int32_t KRON_DIV_I  (int32_t a, int32_t b);
int32_t KRON_MOD_I  (int32_t a, int32_t b);
int32_t KRON_MOVE_I (int32_t a);

/* =========================================================
 * Floating Point Functions
 *
 * KRON_ABS  : absolute value
 * KRON_SQRT : square root; returns 0 for negative input
 * KRON_EXPT : base^exponent; returns 0 for negative base
 *             KRON_EXPT(0,0) = 1 by convention
 * ========================================================= */
float KRON_ABS  (float x);
float KRON_SQRT (float x);
float KRON_EXPT (float base, float exponent);

/* =========================================================
 * Trigonometric Functions
 *
 * All angles are in radians.
 * KRON_ASIN / KRON_ACOS clamp input to [-1, 1].
 * KRON_TAN  returns a large value near singularities.
 * ========================================================= */
float KRON_SIN  (float x);
float KRON_COS  (float x);
float KRON_TAN  (float x);
float KRON_ASIN (float x);
float KRON_ACOS (float x);
float KRON_ATAN (float x);

#endif /* KRONMATH_H */
