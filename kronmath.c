#include "kronmath.h"

// ADD — Sum of N inputs
void ADD_Call(ADD *inst) {
    int32_t sum = 0;
    for (uint8_t i = 0; i < inst->N; i++) {
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
    int32_t product = 1;
    for (uint8_t i = 0; i < inst->N; i++) {
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
    int32_t x = inst->IN;
    int32_t g = x;
    while (g > x / g) {
        g = (g + x / g) / 2;
    }
    inst->OUT = g;
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
    if (inst->N == 0) { inst->OUT = 0; return; }
    int32_t min = inst->IN[0];
    for (uint8_t i = 1; i < inst->N; i++) {
        if (inst->IN[i] < min) {
            min = inst->IN[i];
        }
    }
    inst->OUT = min;
}

// MAX — Maximum of N inputs
void MAX_Call(MAX_FB *inst) {
    if (inst->N == 0) { inst->OUT = 0; return; }
    int32_t max = inst->IN[0];
    for (uint8_t i = 1; i < inst->N; i++) {
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
    if (inst->K >= inst->N) {
        inst->OUT = 0;
        inst->ERR = true;
        return;
    }
    inst->OUT = inst->IN[inst->K];
    inst->ERR = false;
}

// AVG — Average of N inputs (integer, truncated)
void AVG_Call(AVG *inst) {
    if (inst->N == 0) { inst->OUT = 0; return; }
    int32_t sum = 0;
    for (uint8_t i = 0; i < inst->N; i++) {
        sum += inst->IN[i];
    }
    inst->OUT = sum / inst->N;
}
