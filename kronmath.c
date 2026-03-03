#include "kronmath.h"

/* =========================================================
 * Internal helpers  (no external dependencies)
 * ========================================================= */

/* Truncate toward zero (C cast behaviour) */
static float kron_truncf(float x) {
    return (float)((int32_t)x);
}

/* Floor – largest integer not greater than x */
static float kron_floorf(float x) {
    float t = kron_truncf(x);
    return (t > x) ? t - 1.0f : t;
}

/* Absolute value */
static float kron_fabsf(float x) {
    return (x < 0.0f) ? -x : x;
}

/* Floating-point modulo (same sign as dividend, like C fmod) */
static float kron_fmodf(float x, float y) {
    if (y == 0.0f) return 0.0f;
    return x - kron_truncf(x / y) * y;
}

/* ─────────────────────────────────────────────────────────
 * Internal: natural logarithm
 *   Uses IEEE 754 bit extraction to get exponent + mantissa,
 *   then evaluates  log(m) = 2·arctanh((m-1)/(m+1))
 *   via a degree-9 Maclaurin series (|t| < 0.172 on [1/√2,√2]).
 * ───────────────────────────────────────────────────────── */
static float kron_logf(float x) {
    if (x <= 0.0f) return -3.4028235e38f;

    /* Extract IEEE 754 fields via a union (well-defined in C99) */
    union { float f; uint32_t u; } v;
    v.f = x;

    int32_t e = (int32_t)((v.u >> 23u) & 0xFFu) - 127;
    v.u = (v.u & 0x007FFFFFu) | 0x3F800000u; /* mantissa m in [1, 2) */
    float m = v.f;

    /* Reduce m to [1/√2, √2) for faster convergence */
    if (m > 1.41421356f) { m *= 0.5f; e++; }

    /* log(m) = 2·arctanh(t), t = (m-1)/(m+1) ∈ [-0.172, 0.172] */
    float t  = (m - 1.0f) / (m + 1.0f);
    float t2 = t * t;
    /* Horner: 2t·(1 + t²/3 + t⁴/5 + t⁶/7 + t⁸/9) */
    float log_m = t * (2.0f + t2 * (0.66666667f + t2 * (0.4f +
                  t2 * (0.28571429f + t2 * 0.22222222f))));

    return (float)e * 0.69314718f + log_m;
}

/* ─────────────────────────────────────────────────────────
 * Internal: exponential
 *   Decomposes x = n·ln2 + r, |r| ≤ ln2/2, then
 *   exp(r) via degree-7 Horner, result scaled by 2ⁿ.
 * ───────────────────────────────────────────────────────── */
static float kron_expf(float x) {
    if (x >  88.722f) return  3.4028235e38f;
    if (x < -87.337f) return  0.0f;

    /* n = floor(x / ln2) */
    int32_t n = (int32_t)kron_floorf(x * 1.44269504f);

    /* r = x - n·ln2  (two-step to reduce rounding) */
    float r = x - (float)n * 0.69314718f;

    /* exp(r) Horner degree-7: 1 + r + r²/2 + … + r⁷/5040 */
    float p = 1.0f + r * (1.0f + r * (0.5f + r * (0.16666667f +
              r * (0.04166667f + r * (0.00833333f + r * (0.00138889f +
              r * 0.00019841f))))));

    /* Multiply by 2ⁿ via exponent field manipulation */
    union { float f; uint32_t u; } scale;
    scale.u = (uint32_t)(n + 127) << 23u;
    return p * scale.f;
}

/* ─────────────────────────────────────────────────────────
 * Internal: sin on reduced domain [-π/2, π/2]
 *   Degree-9 Horner (Taylor coefficients exact for sine)
 * ───────────────────────────────────────────────────────── */
static float sin_core(float x) {
    float x2 = x * x;
    return x * (1.0f + x2 * (-0.16666667f + x2 * (0.00833333f +
           x2 * (-0.00019841f + x2 * 2.75573e-6f))));
}

/* ─────────────────────────────────────────────────────────
 * Internal: cos on reduced domain [-π/2, π/2]
 *   Degree-8 Horner (Taylor coefficients exact for cosine)
 * ───────────────────────────────────────────────────────── */
static float cos_core(float x) {
    float x2 = x * x;
    return 1.0f + x2 * (-0.5f + x2 * (0.04166667f +
           x2 * (-0.00138889f + x2 * 2.48016e-5f)));
}

/* ─────────────────────────────────────────────────────────
 * Internal: atan on [0, tan(π/8)] ≈ [0, 0.4142]
 *   Degree-15 Horner (Taylor series, 8 terms)
 *   Gives float precision on the reduced range.
 * ───────────────────────────────────────────────────────── */
static float atan_core(float x) {
    float x2 = x * x;
    return x * (1.0f + x2 * (-0.33333333f + x2 * (0.2f +
           x2 * (-0.14285714f + x2 * (0.11111111f +
           x2 * (-0.09090909f + x2 * (0.07692308f +
           x2 *  -0.06666667f)))))));
}

/* =========================================================
 * Basic Math – Float
 * ========================================================= */

float KRON_ADD(float a, float b)  { return a + b; }
float KRON_SUB(float a, float b)  { return a - b; }
float KRON_MUL(float a, float b)  { return a * b; }

float KRON_DIV(float a, float b) {
    if (b == 0.0f) return 0.0f;
    return a / b;
}

float KRON_MOD(float a, float b)  { return kron_fmodf(a, b); }
float KRON_MOVE(float a)          { return a; }

/* =========================================================
 * Basic Math – Integer (DINT)
 * ========================================================= */

int32_t KRON_ADD_I(int32_t a, int32_t b)  { return a + b; }
int32_t KRON_SUB_I(int32_t a, int32_t b)  { return a - b; }
int32_t KRON_MUL_I(int32_t a, int32_t b)  { return a * b; }

int32_t KRON_DIV_I(int32_t a, int32_t b) {
    if (b == 0) return 0;
    return a / b;
}

int32_t KRON_MOD_I(int32_t a, int32_t b) {
    if (b == 0) return 0;
    return a % b;
}

int32_t KRON_MOVE_I(int32_t a) { return a; }

/* =========================================================
 * Floating Point Functions
 * ========================================================= */

float KRON_ABS(float x) {
    return kron_fabsf(x);
}

/* Newton-Raphson SQRT.
 * Initial guess via bit-manipulation on IEEE 754 representation;
 * 4 iterations give full float precision. */
float KRON_SQRT(float x) {
    if (x <= 0.0f) return 0.0f;

    union { float f; uint32_t u; } v;
    v.f = x;
    v.u = 0x1fbd1df5u + (v.u >> 1u); /* initial estimate */
    float g = v.f;

    g = 0.5f * (g + x / g);
    g = 0.5f * (g + x / g);
    g = 0.5f * (g + x / g);
    g = 0.5f * (g + x / g);
    return g;
}

float KRON_EXPT(float base, float exponent) {
    if (base < 0.0f)  return 0.0f;           /* undefined for negative base */
    if (base == 0.0f) return (exponent == 0.0f) ? 1.0f : 0.0f;
    return kron_expf(exponent * kron_logf(base));
}

/* =========================================================
 * Trigonometric Functions
 * ========================================================= */

/* Reduce x to [-π, π] then to [-π/2, π/2], apply sin_core. */
float KRON_SIN(float x) {
    x = kron_fmodf(x, KRON_TWO_PI);
    if      (x >  KRON_PI) x -= KRON_TWO_PI;
    else if (x < -KRON_PI) x += KRON_TWO_PI;

    if      (x >  KRON_HALF_PI) x =  KRON_PI - x;
    else if (x < -KRON_HALF_PI) x = -KRON_PI - x;

    return sin_core(x);
}

/* Reduce then apply cos_core with appropriate sign correction.
 * For |x| in (π/4, π/2] use sin_core(π/2 - |x|) to avoid
 * catastrophic cancellation near the zero of cosine. */
float KRON_COS(float x) {
    x = kron_fmodf(x, KRON_TWO_PI);
    if      (x >  KRON_PI) x -= KRON_TWO_PI;
    else if (x < -KRON_PI) x += KRON_TWO_PI;

    if (x > KRON_HALF_PI) {
        return -cos_core(KRON_PI - x);         /* cos(x) = -cos(π-x) */
    }
    if (x < -KRON_HALF_PI) {
        return -cos_core(KRON_PI + x);         /* cos(x) = -cos(π+x) */
    }
    /* Near π/2: cos(x) = sin(π/2 - x); sin_core is accurate near 0 */
    if (x > 0.78539816f) {                     /* x ∈ (π/4, π/2] */
        return sin_core(KRON_HALF_PI - x);
    }
    if (x < -0.78539816f) {                    /* x ∈ [-π/2, -π/4) */
        return sin_core(KRON_HALF_PI + x);
    }
    return cos_core(x);
}

float KRON_TAN(float x) {
    float c = KRON_COS(x);
    if (c == 0.0f) return 3.4028235e38f;   /* near singularity */
    return KRON_SIN(x) / c;
}

/* Two-level range reduction for atan:
 *   |x| > 1         → π/2 − atan(1/x)
 *   |x| > tan(π/8)  → π/4 − atan((1−x)/(1+x))
 *   otherwise       → atan_core(x)                          */
float KRON_ATAN(float x) {
    float sign = (x < 0.0f) ? -1.0f : 1.0f;
    x = kron_fabsf(x);

    float result;
    if (x > 1.0f) {
        float x_inv = 1.0f / x;
        if (x_inv > 0.41421356f) { /* tan(π/8) = √2 − 1 */
            result = KRON_HALF_PI -
                     (0.78539816f - atan_core((1.0f - x_inv) / (1.0f + x_inv)));
        } else {
            result = KRON_HALF_PI - atan_core(x_inv);
        }
    } else if (x > 0.41421356f) {
        result = 0.78539816f - atan_core((1.0f - x) / (1.0f + x));
    } else {
        result = atan_core(x);
    }
    return sign * result;
}

float KRON_ASIN(float x) {
    if (x >=  1.0f) return  KRON_HALF_PI;
    if (x <= -1.0f) return -KRON_HALF_PI;
    float t = 1.0f - x * x;
    if (t <= 0.0f)  return (x >= 0.0f) ? KRON_HALF_PI : -KRON_HALF_PI;
    return KRON_ATAN(x / KRON_SQRT(t));
}

float KRON_ACOS(float x) {
    return KRON_HALF_PI - KRON_ASIN(x);
}
