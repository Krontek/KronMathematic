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
 * ───────────────────────────────────────────────────────── */
static float sin_core(float x) {
    float x2 = x * x;
    return x * (1.0f + x2 * (-0.16666667f + x2 * (0.00833333f +
           x2 * (-0.00019841f + x2 * 2.75573e-6f))));
}

/* ─────────────────────────────────────────────────────────
 * Internal: cos on reduced domain [-π/2, π/2]
 * ───────────────────────────────────────────────────────── */
static float cos_core(float x) {
    float x2 = x * x;
    return 1.0f + x2 * (-0.5f + x2 * (0.04166667f +
           x2 * (-0.00138889f + x2 * 2.48016e-5f)));
}

/* ─────────────────────────────────────────────────────────
 * Internal: atan on [0, tan(π/8)] ≈ [0, 0.4142]
 * ───────────────────────────────────────────────────────── */
static float atan_core(float x) {
    float x2 = x * x;
    return x * (1.0f + x2 * (-0.33333333f + x2 * (0.2f +
           x2 * (-0.14285714f + x2 * (0.11111111f +
           x2 * (-0.09090909f + x2 * (0.07692308f +
           x2 *  -0.06666667f)))))));
}

/* ─────────────────────────────────────────────────────────
 * Internal: full-range sin
 * ───────────────────────────────────────────────────────── */
static float kron_sinf(float x) {
    x = kron_fmodf(x, KRON_TWO_PI);
    if      (x >  KRON_PI) x -= KRON_TWO_PI;
    else if (x < -KRON_PI) x += KRON_TWO_PI;

    if      (x >  KRON_HALF_PI) x =  KRON_PI - x;
    else if (x < -KRON_HALF_PI) x = -KRON_PI - x;

    return sin_core(x);
}

/* ─────────────────────────────────────────────────────────
 * Internal: full-range cos
 * ───────────────────────────────────────────────────────── */
static float kron_cosf(float x) {
    x = kron_fmodf(x, KRON_TWO_PI);
    if      (x >  KRON_PI) x -= KRON_TWO_PI;
    else if (x < -KRON_PI) x += KRON_TWO_PI;

    if (x > KRON_HALF_PI)
        return -cos_core(KRON_PI - x);
    if (x < -KRON_HALF_PI)
        return -cos_core(KRON_PI + x);
    if (x > 0.78539816f)
        return sin_core(KRON_HALF_PI - x);
    if (x < -0.78539816f)
        return sin_core(KRON_HALF_PI + x);
    return cos_core(x);
}

/* ─────────────────────────────────────────────────────────
 * Internal: full-range atan
 * ───────────────────────────────────────────────────────── */
static float kron_atanf(float x) {
    float sign = (x < 0.0f) ? -1.0f : 1.0f;
    x = kron_fabsf(x);

    float result;
    if (x > 1.0f) {
        float x_inv = 1.0f / x;
        if (x_inv > 0.41421356f) {
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

/* ─────────────────────────────────────────────────────────
 * Internal: sqrt (Newton-Raphson)
 * ───────────────────────────────────────────────────────── */
static float kron_sqrtf(float x) {
    if (x <= 0.0f) return 0.0f;

    union { float f; uint32_t u; } v;
    v.f = x;
    v.u = 0x1fbd1df5u + (v.u >> 1u);
    float g = v.f;

    g = 0.5f * (g + x / g);
    g = 0.5f * (g + x / g);
    g = 0.5f * (g + x / g);
    g = 0.5f * (g + x / g);
    return g;
}

/* =========================================================
 * ADD
 * ========================================================= */

void ADD_F_Call(ADD_F *inst) {
    if (!inst->EN) { inst->ENO = false; return; }
    inst->OUT = inst->IN1 + inst->IN2;
    inst->ENO = true;
}

void ADD_I_Call(ADD_I *inst) {
    if (!inst->EN) { inst->ENO = false; return; }
    inst->OUT = inst->IN1 + inst->IN2;
    inst->ENO = true;
}

/* =========================================================
 * SUB
 * ========================================================= */

void SUB_F_Call(SUB_F *inst) {
    if (!inst->EN) { inst->ENO = false; return; }
    inst->OUT = inst->IN1 - inst->IN2;
    inst->ENO = true;
}

void SUB_I_Call(SUB_I *inst) {
    if (!inst->EN) { inst->ENO = false; return; }
    inst->OUT = inst->IN1 - inst->IN2;
    inst->ENO = true;
}

/* =========================================================
 * MUL
 * ========================================================= */

void MUL_F_Call(MUL_F *inst) {
    if (!inst->EN) { inst->ENO = false; return; }
    inst->OUT = inst->IN1 * inst->IN2;
    inst->ENO = true;
}

void MUL_I_Call(MUL_I *inst) {
    if (!inst->EN) { inst->ENO = false; return; }
    inst->OUT = inst->IN1 * inst->IN2;
    inst->ENO = true;
}

/* =========================================================
 * DIV — ENO = false on div-by-zero
 * ========================================================= */

void DIV_F_Call(DIV_F *inst) {
    if (!inst->EN) { inst->ENO = false; return; }
    if (inst->IN2 == 0.0f) { inst->OUT = 0.0f; inst->ENO = false; return; }
    inst->OUT = inst->IN1 / inst->IN2;
    inst->ENO = true;
}

void DIV_I_Call(DIV_I *inst) {
    if (!inst->EN) { inst->ENO = false; return; }
    if (inst->IN2 == 0) { inst->OUT = 0; inst->ENO = false; return; }
    inst->OUT = inst->IN1 / inst->IN2;
    inst->ENO = true;
}

/* =========================================================
 * MOD — ENO = false on div-by-zero
 * ========================================================= */

void MOD_F_Call(MOD_F *inst) {
    if (!inst->EN) { inst->ENO = false; return; }
    if (inst->IN2 == 0.0f) { inst->OUT = 0.0f; inst->ENO = false; return; }
    inst->OUT = kron_fmodf(inst->IN1, inst->IN2);
    inst->ENO = true;
}

void MOD_I_Call(MOD_I *inst) {
    if (!inst->EN) { inst->ENO = false; return; }
    if (inst->IN2 == 0) { inst->OUT = 0; inst->ENO = false; return; }
    inst->OUT = inst->IN1 % inst->IN2;
    inst->ENO = true;
}

/* =========================================================
 * MOVE
 * ========================================================= */

void MOVE_F_Call(MOVE_F *inst) {
    if (!inst->EN) { inst->ENO = false; return; }
    inst->OUT = inst->IN;
    inst->ENO = true;
}

void MOVE_I_Call(MOVE_I *inst) {
    if (!inst->EN) { inst->ENO = false; return; }
    inst->OUT = inst->IN;
    inst->ENO = true;
}

/* =========================================================
 * ABS
 * ========================================================= */

void ABS_F_Call(ABS_F *inst) {
    if (!inst->EN) { inst->ENO = false; return; }
    inst->OUT = kron_fabsf(inst->IN);
    inst->ENO = true;
}

void ABS_I_Call(ABS_I *inst) {
    if (!inst->EN) { inst->ENO = false; return; }
    inst->OUT = (inst->IN < 0) ? -inst->IN : inst->IN;
    inst->ENO = true;
}

/* =========================================================
 * SQRT — ENO = false for negative input
 * ========================================================= */

void SQRT_Call(SQRT_FB *inst) {
    if (!inst->EN) { inst->ENO = false; return; }
    if (inst->IN < 0.0f) { inst->OUT = 0.0f; inst->ENO = false; return; }
    inst->OUT = kron_sqrtf(inst->IN);
    inst->ENO = true;
}

/* =========================================================
 * EXPT — ENO = false for negative base
 * ========================================================= */

void EXPT_Call(EXPT_FB *inst) {
    if (!inst->EN) { inst->ENO = false; return; }
    float base = inst->IN1;
    float exponent = inst->IN2;
    if (base < 0.0f) { inst->OUT = 0.0f; inst->ENO = false; return; }
    if (base == 0.0f) {
        inst->OUT = (exponent == 0.0f) ? 1.0f : 0.0f;
        inst->ENO = true;
        return;
    }
    inst->OUT = kron_expf(exponent * kron_logf(base));
    inst->ENO = true;
}

/* =========================================================
 * Trigonometric Functions
 * ========================================================= */

void SIN_Call(SIN_FB *inst) {
    if (!inst->EN) { inst->ENO = false; return; }
    inst->OUT = kron_sinf(inst->IN);
    inst->ENO = true;
}

void COS_Call(COS_FB *inst) {
    if (!inst->EN) { inst->ENO = false; return; }
    inst->OUT = kron_cosf(inst->IN);
    inst->ENO = true;
}

void TAN_Call(TAN_FB *inst) {
    if (!inst->EN) { inst->ENO = false; return; }
    float c = kron_cosf(inst->IN);
    if (c == 0.0f) { inst->OUT = 3.4028235e38f; inst->ENO = false; return; }
    inst->OUT = kron_sinf(inst->IN) / c;
    inst->ENO = true;
}

void ASIN_Call(ASIN_FB *inst) {
    if (!inst->EN) { inst->ENO = false; return; }
    float x = inst->IN;
    if (x >=  1.0f) { inst->OUT =  KRON_HALF_PI; inst->ENO = true; return; }
    if (x <= -1.0f) { inst->OUT = -KRON_HALF_PI; inst->ENO = true; return; }
    float t = 1.0f - x * x;
    if (t <= 0.0f) {
        inst->OUT = (x >= 0.0f) ? KRON_HALF_PI : -KRON_HALF_PI;
        inst->ENO = true;
        return;
    }
    inst->OUT = kron_atanf(x / kron_sqrtf(t));
    inst->ENO = true;
}

void ACOS_Call(ACOS_FB *inst) {
    if (!inst->EN) { inst->ENO = false; return; }
    float x = inst->IN;
    float asin_val;
    if (x >=  1.0f)      asin_val =  KRON_HALF_PI;
    else if (x <= -1.0f)  asin_val = -KRON_HALF_PI;
    else {
        float t = 1.0f - x * x;
        if (t <= 0.0f)
            asin_val = (x >= 0.0f) ? KRON_HALF_PI : -KRON_HALF_PI;
        else
            asin_val = kron_atanf(x / kron_sqrtf(t));
    }
    inst->OUT = KRON_HALF_PI - asin_val;
    inst->ENO = true;
}

void ATAN_Call(ATAN_FB *inst) {
    if (!inst->EN) { inst->ENO = false; return; }
    inst->OUT = kron_atanf(inst->IN);
    inst->ENO = true;
}
