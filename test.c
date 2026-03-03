#include <stdio.h>
#include "kronmath.h"

static int pass_count = 0;
static int fail_count = 0;

static void check(const char *name, int condition) {
    if (condition) {
        printf("  [PASS] %s\n", name);
        pass_count++;
    } else {
        printf("  [FAIL] %s\n", name);
        fail_count++;
    }
}

/* Float comparison within epsilon */
static int feq(float a, float b, float eps) {
    float diff = a - b;
    if (diff < 0.0f) diff = -diff;
    return diff <= eps;
}

/* ─────────────────────────────────────────────
 * Basic Math – Float
 * ───────────────────────────────────────────── */
static void test_basic_float(void) {
    printf("\n--- Basic Math (Float) ---\n");

    check("ADD  2.5 + 1.5 = 4.0",   feq(KRON_ADD(2.5f, 1.5f),  4.0f,   1e-6f));
    check("SUB  5.0 - 3.2 = 1.8",   feq(KRON_SUB(5.0f, 3.2f),  1.8f,   1e-5f));
    check("MUL  3.0 * 4.0 = 12.0",  feq(KRON_MUL(3.0f, 4.0f), 12.0f,   1e-6f));
    check("DIV  9.0 / 3.0 = 3.0",   feq(KRON_DIV(9.0f, 3.0f),  3.0f,   1e-6f));
    check("DIV  by zero = 0.0",       feq(KRON_DIV(5.0f, 0.0f),  0.0f,   1e-9f));
    check("MOD  7.5 % 3.0 = 1.5",   feq(KRON_MOD(7.5f, 3.0f),  1.5f,   1e-6f));
    check("MOD -7.5 % 3.0 = -1.5",  feq(KRON_MOD(-7.5f, 3.0f),-1.5f,   1e-6f));
    check("MOD  by zero = 0.0",       feq(KRON_MOD(5.0f, 0.0f),  0.0f,   1e-9f));
    check("MOVE 3.14 = 3.14",        feq(KRON_MOVE(3.14f),       3.14f,  1e-6f));
}

/* ─────────────────────────────────────────────
 * Basic Math – Integer
 * ───────────────────────────────────────────── */
static void test_basic_int(void) {
    printf("\n--- Basic Math (Integer / DINT) ---\n");

    check("ADD_I   7 + 3 = 10",   KRON_ADD_I(7,  3) == 10);
    check("ADD_I  -4 + 1 = -3",   KRON_ADD_I(-4, 1) == -3);
    check("SUB_I  10 - 4 = 6",    KRON_SUB_I(10, 4) == 6);
    check("MUL_I   6 * 7 = 42",   KRON_MUL_I(6,  7) == 42);
    check("DIV_I  15 / 4 = 3",    KRON_DIV_I(15, 4) == 3);
    check("DIV_I  by zero = 0",   KRON_DIV_I(9,  0) == 0);
    check("MOD_I  17 % 5 = 2",    KRON_MOD_I(17, 5) == 2);
    check("MOD_I -17 % 5 = -2",   KRON_MOD_I(-17,5) == -2);
    check("MOD_I  by zero = 0",   KRON_MOD_I(9,  0) == 0);
    check("MOVE_I 42 = 42",       KRON_MOVE_I(42)   == 42);
}

/* ─────────────────────────────────────────────
 * ABS, SQRT, EXPT
 * ───────────────────────────────────────────── */
static void test_float_functions(void) {
    printf("\n--- ABS ---\n");
    check("ABS  3.5 = 3.5",   feq(KRON_ABS( 3.5f),  3.5f, 1e-6f));
    check("ABS -3.5 = 3.5",   feq(KRON_ABS(-3.5f),  3.5f, 1e-6f));
    check("ABS  0.0 = 0.0",   feq(KRON_ABS( 0.0f),  0.0f, 1e-9f));

    printf("\n--- SQRT ---\n");
    check("SQRT  4.0 = 2.0",      feq(KRON_SQRT(4.0f),   2.0f,       1e-5f));
    check("SQRT  9.0 = 3.0",      feq(KRON_SQRT(9.0f),   3.0f,       1e-5f));
    check("SQRT  2.0 = 1.41421",  feq(KRON_SQRT(2.0f),   1.41421356f,1e-5f));
    check("SQRT  0.0 = 0.0",      feq(KRON_SQRT(0.0f),   0.0f,       1e-9f));
    check("SQRT  negative = 0.0", feq(KRON_SQRT(-1.0f),  0.0f,       1e-9f));
    check("SQRT  SQRT(81)=9.0",   feq(KRON_SQRT(81.0f),  9.0f,       1e-5f));

    printf("\n--- EXPT ---\n");
    check("EXPT  2^10 = 1024",    feq(KRON_EXPT(2.0f, 10.0f), 1024.0f, 0.1f));
    check("EXPT  4^0.5 = 2.0",    feq(KRON_EXPT(4.0f,  0.5f),    2.0f, 1e-4f));
    check("EXPT  e^1 ≈ 2.71828",  feq(KRON_EXPT(KRON_E, 1.0f),   KRON_E, 1e-4f));
    check("EXPT  x^0 = 1",        feq(KRON_EXPT(5.0f,  0.0f),    1.0f, 1e-6f));
    check("EXPT  0^0 = 1",        feq(KRON_EXPT(0.0f,  0.0f),    1.0f, 1e-9f));
    check("EXPT  0^1 = 0",        feq(KRON_EXPT(0.0f,  1.0f),    0.0f, 1e-9f));
    check("EXPT  neg base = 0",   feq(KRON_EXPT(-2.0f, 2.0f),    0.0f, 1e-9f));
}

/* ─────────────────────────────────────────────
 * Trigonometry
 * ───────────────────────────────────────────── */
static void test_trig(void) {
    printf("\n--- SIN ---\n");
    check("SIN  0        = 0.0",      feq(KRON_SIN(0.0f),             0.0f,       1e-5f));
    check("SIN  π/6      = 0.5",      feq(KRON_SIN(KRON_PI/6.0f),     0.5f,       1e-5f));
    check("SIN  π/4      ≈ 0.70711",  feq(KRON_SIN(KRON_PI/4.0f),     0.70710678f,1e-5f));
    check("SIN  π/2      = 1.0",      feq(KRON_SIN(KRON_HALF_PI),     1.0f,       1e-5f));
    check("SIN  π        ≈ 0.0",      feq(KRON_SIN(KRON_PI),          0.0f,       1e-5f));
    check("SIN -π/2      = -1.0",     feq(KRON_SIN(-KRON_HALF_PI),   -1.0f,       1e-5f));
    check("SIN  3π/2     = -1.0",     feq(KRON_SIN(3.0f*KRON_HALF_PI),-1.0f,      1e-5f));

    printf("\n--- COS ---\n");
    check("COS  0        = 1.0",      feq(KRON_COS(0.0f),             1.0f,       1e-5f));
    check("COS  π/3      = 0.5",      feq(KRON_COS(KRON_PI/3.0f),     0.5f,       1e-5f));
    check("COS  π/4      ≈ 0.70711",  feq(KRON_COS(KRON_PI/4.0f),     0.70710678f,1e-5f));
    check("COS  π/2      ≈ 0.0",      feq(KRON_COS(KRON_HALF_PI),     0.0f,       1e-5f));
    check("COS  π        = -1.0",     feq(KRON_COS(KRON_PI),         -1.0f,       1e-5f));
    check("COS -π/3      = 0.5",      feq(KRON_COS(-KRON_PI/3.0f),    0.5f,       1e-5f));

    printf("\n--- TAN ---\n");
    check("TAN  0        = 0.0",      feq(KRON_TAN(0.0f),             0.0f,       1e-5f));
    check("TAN  π/4      = 1.0",      feq(KRON_TAN(KRON_PI/4.0f),     1.0f,       1e-4f));
    check("TAN  π/6      ≈ 0.57735",  feq(KRON_TAN(KRON_PI/6.0f),     0.57735027f,1e-4f));
    check("TAN -π/4      = -1.0",     feq(KRON_TAN(-KRON_PI/4.0f),   -1.0f,       1e-4f));

    printf("\n--- ATAN ---\n");
    check("ATAN  0       = 0.0",      feq(KRON_ATAN(0.0f),            0.0f,       1e-5f));
    check("ATAN  1       = π/4",      feq(KRON_ATAN(1.0f),            0.78539816f,1e-5f));
    check("ATAN -1       = -π/4",     feq(KRON_ATAN(-1.0f),          -0.78539816f,1e-5f));
    check("ATAN  0.57735 ≈ π/6",     feq(KRON_ATAN(0.57735027f),     KRON_PI/6.0f,1e-5f));
    check("ATAN  large   ≈ π/2",      feq(KRON_ATAN(1e6f),            KRON_HALF_PI,1e-3f));

    printf("\n--- ASIN ---\n");
    check("ASIN  0.0    = 0.0",       feq(KRON_ASIN(0.0f),            0.0f,       1e-5f));
    check("ASIN  0.5    = π/6",       feq(KRON_ASIN(0.5f),            0.52359878f,1e-4f));
    check("ASIN  1.0    = π/2",       feq(KRON_ASIN(1.0f),            KRON_HALF_PI,1e-5f));
    check("ASIN -1.0    = -π/2",      feq(KRON_ASIN(-1.0f),          -KRON_HALF_PI,1e-5f));
    check("ASIN  clamp >1 = π/2",     feq(KRON_ASIN(1.5f),            KRON_HALF_PI,1e-5f));

    printf("\n--- ACOS ---\n");
    check("ACOS  1.0    = 0.0",       feq(KRON_ACOS(1.0f),            0.0f,       1e-5f));
    check("ACOS  0.5    = π/3",       feq(KRON_ACOS(0.5f),            1.04719755f,1e-4f));
    check("ACOS  0.0    = π/2",       feq(KRON_ACOS(0.0f),            KRON_HALF_PI,1e-5f));
    check("ACOS -1.0    = π",         feq(KRON_ACOS(-1.0f),           KRON_PI,    1e-4f));

    printf("\n--- sin²+cos²=1 (Pythagorean identity) ---\n");
    float angles[] = {0.0f, 0.3f, 0.7f, 1.0f, 1.5f, 2.0f, 2.8f, 3.1f};
    int i;
    int pyth_ok = 1;
    for (i = 0; i < 8; i++) {
        float s = KRON_SIN(angles[i]);
        float c = KRON_COS(angles[i]);
        if (!feq(s*s + c*c, 1.0f, 1e-4f)) {
            pyth_ok = 0;
            printf("    FAIL at angle %.2f: sin²+cos² = %.7f\n",
                   angles[i], s*s + c*c);
        }
    }
    check("sin²+cos²=1 for 8 test angles", pyth_ok);
}

/* ─────────────────────────────────────────────
 * SQRT / EXPT consistency
 * ───────────────────────────────────────────── */
static void test_consistency(void) {
    printf("\n--- Consistency checks ---\n");

    /* SQRT(x) == EXPT(x, 0.5) */
    float vals[] = {1.0f, 2.0f, 4.0f, 9.0f, 16.0f, 100.0f};
    int i;
    int sq_ok = 1;
    for (i = 0; i < 6; i++) {
        if (!feq(KRON_SQRT(vals[i]), KRON_EXPT(vals[i], 0.5f), 1e-4f)) {
            sq_ok = 0;
            printf("    FAIL at x=%.0f: SQRT=%.6f EXPT=%.6f\n",
                   vals[i], KRON_SQRT(vals[i]), KRON_EXPT(vals[i], 0.5f));
        }
    }
    check("SQRT(x) == EXPT(x, 0.5) for 6 values", sq_ok);

    /* ASIN(SIN(x)) == x for x in [-π/2, π/2] */
    float ax[] = {0.0f, 0.4f, 0.8f, 1.2f, -0.5f, -1.0f};
    int asin_ok = 1;
    for (i = 0; i < 6; i++) {
        if (!feq(KRON_ASIN(KRON_SIN(ax[i])), ax[i], 1e-4f)) {
            asin_ok = 0;
            printf("    FAIL at x=%.2f: ASIN(SIN)=%.6f\n",
                   ax[i], KRON_ASIN(KRON_SIN(ax[i])));
        }
    }
    check("ASIN(SIN(x))==x for x in [-π/2,π/2]", asin_ok);

    /* ATAN(TAN(x)) == x for x in (-π/2, π/2) */
    float at[] = {0.0f, 0.3f, 0.7f, 1.0f, -0.5f, -1.2f};
    int atan_ok = 1;
    for (i = 0; i < 6; i++) {
        if (!feq(KRON_ATAN(KRON_TAN(at[i])), at[i], 1e-4f)) {
            atan_ok = 0;
            printf("    FAIL at x=%.2f: ATAN(TAN)=%.6f\n",
                   at[i], KRON_ATAN(KRON_TAN(at[i])));
        }
    }
    check("ATAN(TAN(x))==x for x in (-π/2,π/2)", atan_ok);
}

/* ─────────────────────────────────────────────
 * main
 * ───────────────────────────────────────────── */
int main(void) {
    printf("========================================\n");
    printf("  KronMathematic Unit Tests\n");
    printf("========================================\n");

    test_basic_float();
    test_basic_int();
    test_float_functions();
    test_trig();
    test_consistency();

    printf("\n========================================\n");
    printf("  Results: %d passed, %d failed\n", pass_count, fail_count);
    printf("========================================\n");

    return (fail_count == 0) ? 0 : 1;
}
