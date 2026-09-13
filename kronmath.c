#include "kronmath.h"

/* Multi-input blocks expose N as a plain FB field, so a program can set it to
 * any uint8_t value. Clamp it to the array capacity before indexing IN[]. */
static inline uint8_t kron_math_clamp_n(uint8_t n) {
    return (n > KRON_MATH_MAX_IN) ? (uint8_t)KRON_MATH_MAX_IN : n;
}

// ADD — Sum of N inputs
void ADD_Call(ADD *inst) {
    uint8_t n = kron_math_clamp_n(inst->N);
    int32_t sum = 0;
    for (uint8_t i = 0; i < n; i++) {
        sum += inst->IN[i];
    }
    inst->OUT = sum;
}

// SUB — Subtraction
void SUB_Call(SUB *inst) {
    inst->OUT = inst->IN1 - inst->IN2;
}

// MUL — Product of N inputs
void MUL_Call(MUL *inst) {
    uint8_t n = kron_math_clamp_n(inst->N);
    int32_t product = 1;
    for (uint8_t i = 0; i < n; i++) {
        product *= inst->IN[i];
    }
    inst->OUT = product;
}

// DIV — Integer division
void DIV_Call(DIV *inst) {
    if (inst->IN2 == 0) {
        inst->OUT = 0;
        inst->ERR = true;
        return;
    }
    inst->OUT = inst->IN1 / inst->IN2;
    inst->ERR = false;
}

// MOD — Integer modulo
void MOD_Call(MOD *inst) {
    if (inst->IN2 == 0) {
        inst->OUT = 0;
        inst->ERR = true;
        return;
    }
    inst->OUT = inst->IN1 % inst->IN2;
    inst->ERR = false;
}

// ABS — Absolute value
void ABS_Call(ABS_FB *inst) {
    inst->OUT = (inst->IN < 0) ? -inst->IN : inst->IN;
}

// NEG — Negation
void NEG_Call(NEG *inst) {
    inst->OUT = -inst->IN;
}

// MOVE — Copy value
void MOVE_Call(MOVE *inst) {
    inst->OUT = inst->IN;
}

// SQRT — Integer square root (Newton-Raphson, floor)
void SQRT_Call(SQRT_FB *inst) {
    if (inst->IN < 0) {
        inst->OUT = 0;
        inst->ERR = true;
        return;
    }
    if (inst->IN == 0) {
        inst->OUT = 0;
        inst->ERR = false;
        return;
    }
    /* Newton-Raphson in uint32_t. The first step computes (g + x/g), which
     * overflows int32_t when IN == INT32_MAX; unsigned arithmetic keeps every
     * intermediate representable (the sum never exceeds x + 1). */
    uint32_t x = (uint32_t)inst->IN;
    uint32_t g = (x / 2u) + 1u;
    while (g > x / g) {
        g = (g + x / g) / 2u;
    }
    inst->OUT = (int32_t)g;
    inst->ERR = false;
}

// EXPT — Integer exponentiation (repeated squaring)
void EXPT_Call(EXPT *inst) {
    if (inst->IN2 < 0) {
        inst->OUT = 0;
        inst->ERR = true;
        return;
    }
    int32_t base = inst->IN1;
    int32_t exp  = inst->IN2;
    int32_t result = 1;
    while (exp > 0) {
        if (exp & 1) {
            result *= base;
        }
        base *= base;
        exp >>= 1;
    }
    inst->OUT = result;
    inst->ERR = false;
}

// MIN — Minimum of N inputs
void MIN_Call(MIN_FB *inst) {
    uint8_t n = kron_math_clamp_n(inst->N);
    if (n == 0) { inst->OUT = 0; return; }
    int32_t min = inst->IN[0];
    for (uint8_t i = 1; i < n; i++) {
        if (inst->IN[i] < min) {
            min = inst->IN[i];
        }
    }
    inst->OUT = min;
}

// MAX — Maximum of N inputs
void MAX_Call(MAX_FB *inst) {
    uint8_t n = kron_math_clamp_n(inst->N);
    if (n == 0) { inst->OUT = 0; return; }
    int32_t max = inst->IN[0];
    for (uint8_t i = 1; i < n; i++) {
        if (inst->IN[i] > max) {
            max = inst->IN[i];
        }
    }
    inst->OUT = max;
}

// LIMIT — Clamp value between bounds
void LIMIT_Call(LIMIT *inst) {
    if (inst->IN < inst->MN)
        inst->OUT = inst->MN;
    else if (inst->IN > inst->MX)
        inst->OUT = inst->MX;
    else
        inst->OUT = inst->IN;
}

// SEL — Binary selector
void SEL_Call(SEL *inst) {
    inst->OUT = inst->G ? inst->IN1 : inst->IN0;
}

// MUX — Multiplexer
void MUX_Call(MUX *inst) {
    uint8_t n = kron_math_clamp_n(inst->N);
    if (inst->K >= n) {
        inst->OUT = 0;
        inst->ERR = true;
        return;
    }
    inst->OUT = inst->IN[inst->K];
    inst->ERR = false;
}

// AVG — Average of N inputs (integer, truncated)
void AVG_Call(AVG *inst) {
    uint8_t n = kron_math_clamp_n(inst->N);
    if (n == 0) { inst->OUT = 0; return; }
    int32_t sum = 0;
    for (uint8_t i = 0; i < n; i++) {
        sum += inst->IN[i];
    }
    inst->OUT = sum / n;
}
