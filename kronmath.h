#ifndef KRONMATH_H
#define KRONMATH_H

#include <stdint.h>

/* Math constants */
#define KRON_PI      3.14159265f
#define KRON_HALF_PI 1.57079633f
#define KRON_TWO_PI  6.28318531f
#define KRON_E       2.71828183f

/* =========================================================
 * Typed function declarations
 *
 * _F suffix  → operates on float  (IEC REAL)
 * _I suffix  → operates on int32_t (IEC DINT; smaller integer
 *               types are implicitly promoted by the compiler)
 * ========================================================= */

/* Basic math – float */
float   KRON_ADD_F  (float a, float b);
float   KRON_SUB_F  (float a, float b);
float   KRON_MUL_F  (float a, float b);
float   KRON_DIV_F  (float a, float b);   /* returns 0 on div-by-zero */
float   KRON_MOD_F  (float a, float b);   /* returns 0 on div-by-zero */
float   KRON_MOVE_F (float a);

/* Basic math – integer (int32_t / DINT) */
int32_t KRON_ADD_I  (int32_t a, int32_t b);
int32_t KRON_SUB_I  (int32_t a, int32_t b);
int32_t KRON_MUL_I  (int32_t a, int32_t b);
int32_t KRON_DIV_I  (int32_t a, int32_t b); /* returns 0 on div-by-zero */
int32_t KRON_MOD_I  (int32_t a, int32_t b); /* returns 0 on div-by-zero */
int32_t KRON_MOVE_I (int32_t a);

/* Absolute value */
float   KRON_ABS_F (float x);
int32_t KRON_ABS_I (int32_t x);

/* Floating-point special functions (always float, integer inputs are
 * implicitly promoted by the compiler – no explicit overload needed) */
float KRON_SQRT (float x);          /* returns 0 for negative input */
float KRON_EXPT (float base, float exponent); /* returns 0 for negative base;
                                                 KRON_EXPT(0,0) == 1 */

/* Trigonometric functions – angles in radians */
float KRON_SIN  (float x);
float KRON_COS  (float x);
float KRON_TAN  (float x);          /* large value near singularities */
float KRON_ASIN (float x);          /* input clamped to [-1, 1] */
float KRON_ACOS (float x);          /* input clamped to [-1, 1] */
float KRON_ATAN (float x);

/* =========================================================
 * Generic macros  (C11 _Generic)
 *
 * Dispatch table (on the type of the FIRST argument):
 *   float            → _F  (IEC REAL)
 *   int32_t          → _I  (IEC DINT  – on ARM, same type as int)
 *   uint32_t         → _I  (IEC UDINT – cast to signed)
 *   int16_t          → _I  (IEC INT)
 *   uint16_t         → _I  (IEC UINT)
 *   int8_t           → _I  (IEC SINT)
 *   uint8_t          → _I  (IEC USINT)
 *   default          → _F  (double, long, … get float conversion)
 *
 * Usage examples:
 *   KRON_ADD(3.14f,  1.0f )        → float   4.14f
 *   KRON_ADD((int32_t)3, (int32_t)4)  → int32_t 7
 *   KRON_ABS(-5)                   → int32_t 5   (on ARM where int==int32_t)
 *   KRON_ABS(-3.5f)                → float   3.5f
 * ========================================================= */

#define KRON_ADD(a, b) _Generic((a),    \
    float:    KRON_ADD_F,               \
    int32_t:  KRON_ADD_I,               \
    uint32_t: KRON_ADD_I,               \
    int16_t:  KRON_ADD_I,               \
    uint16_t: KRON_ADD_I,               \
    int8_t:   KRON_ADD_I,               \
    uint8_t:  KRON_ADD_I,               \
    default:  KRON_ADD_F                \
)((a), (b))

#define KRON_SUB(a, b) _Generic((a),    \
    float:    KRON_SUB_F,               \
    int32_t:  KRON_SUB_I,               \
    uint32_t: KRON_SUB_I,               \
    int16_t:  KRON_SUB_I,               \
    uint16_t: KRON_SUB_I,               \
    int8_t:   KRON_SUB_I,               \
    uint8_t:  KRON_SUB_I,               \
    default:  KRON_SUB_F                \
)((a), (b))

#define KRON_MUL(a, b) _Generic((a),    \
    float:    KRON_MUL_F,               \
    int32_t:  KRON_MUL_I,               \
    uint32_t: KRON_MUL_I,               \
    int16_t:  KRON_MUL_I,               \
    uint16_t: KRON_MUL_I,               \
    int8_t:   KRON_MUL_I,               \
    uint8_t:  KRON_MUL_I,               \
    default:  KRON_MUL_F                \
)((a), (b))

#define KRON_DIV(a, b) _Generic((a),    \
    float:    KRON_DIV_F,               \
    int32_t:  KRON_DIV_I,               \
    uint32_t: KRON_DIV_I,               \
    int16_t:  KRON_DIV_I,               \
    uint16_t: KRON_DIV_I,               \
    int8_t:   KRON_DIV_I,               \
    uint8_t:  KRON_DIV_I,               \
    default:  KRON_DIV_F                \
)((a), (b))

#define KRON_MOD(a, b) _Generic((a),    \
    float:    KRON_MOD_F,               \
    int32_t:  KRON_MOD_I,               \
    uint32_t: KRON_MOD_I,               \
    int16_t:  KRON_MOD_I,               \
    uint16_t: KRON_MOD_I,               \
    int8_t:   KRON_MOD_I,               \
    uint8_t:  KRON_MOD_I,               \
    default:  KRON_MOD_F                \
)((a), (b))

#define KRON_MOVE(a) _Generic((a),      \
    float:    KRON_MOVE_F,              \
    int32_t:  KRON_MOVE_I,              \
    uint32_t: KRON_MOVE_I,              \
    int16_t:  KRON_MOVE_I,              \
    uint16_t: KRON_MOVE_I,              \
    int8_t:   KRON_MOVE_I,              \
    uint8_t:  KRON_MOVE_I,              \
    default:  KRON_MOVE_F               \
)(a)

#define KRON_ABS(a) _Generic((a),       \
    float:    KRON_ABS_F,               \
    int32_t:  KRON_ABS_I,               \
    uint32_t: KRON_ABS_I,               \
    int16_t:  KRON_ABS_I,               \
    uint16_t: KRON_ABS_I,               \
    int8_t:   KRON_ABS_I,               \
    uint8_t:  KRON_ABS_I,               \
    default:  KRON_ABS_F                \
)(a)

#endif /* KRONMATH_H */
