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

    ADD_F add = { .EN = true, .IN1 = 2.5f, .IN2 = 1.5f };
    ADD_F_Call(&add);
    check("ADD  2.5 + 1.5 = 4.0",   add.ENO && feq(add.OUT, 4.0f, 1e-6f));

    SUB_F sub = { .EN = true, .IN1 = 5.0f, .IN2 = 3.2f };
    SUB_F_Call(&sub);
    check("SUB  5.0 - 3.2 = 1.8",   sub.ENO && feq(sub.OUT, 1.8f, 1e-5f));

    MUL_F mul = { .EN = true, .IN1 = 3.0f, .IN2 = 4.0f };
    MUL_F_Call(&mul);
    check("MUL  3.0 * 4.0 = 12.0",  mul.ENO && feq(mul.OUT, 12.0f, 1e-6f));

    DIV_F div1 = { .EN = true, .IN1 = 9.0f, .IN2 = 3.0f };
    DIV_F_Call(&div1);
    check("DIV  9.0 / 3.0 = 3.0",   div1.ENO && feq(div1.OUT, 3.0f, 1e-6f));

    DIV_F div0 = { .EN = true, .IN1 = 5.0f, .IN2 = 0.0f };
    DIV_F_Call(&div0);
    check("DIV  by zero: ENO=false", !div0.ENO && feq(div0.OUT, 0.0f, 1e-9f));

    MOD_F mod1 = { .EN = true, .IN1 = 7.5f, .IN2 = 3.0f };
    MOD_F_Call(&mod1);
    check("MOD  7.5 % 3.0 = 1.5",   mod1.ENO && feq(mod1.OUT, 1.5f, 1e-6f));

    MOD_F mod2 = { .EN = true, .IN1 = -7.5f, .IN2 = 3.0f };
    MOD_F_Call(&mod2);
    check("MOD -7.5 % 3.0 = -1.5",  mod2.ENO && feq(mod2.OUT, -1.5f, 1e-6f));

    MOD_F mod0 = { .EN = true, .IN1 = 5.0f, .IN2 = 0.0f };
    MOD_F_Call(&mod0);
    check("MOD  by zero: ENO=false", !mod0.ENO && feq(mod0.OUT, 0.0f, 1e-9f));

    MOVE_F mov = { .EN = true, .IN = 3.14f };
    MOVE_F_Call(&mov);
    check("MOVE 3.14 = 3.14",       mov.ENO && feq(mov.OUT, 3.14f, 1e-6f));
}

/* ─────────────────────────────────────────────
 * Basic Math – Integer
 * ───────────────────────────────────────────── */
static void test_basic_int(void) {
    printf("\n--- Basic Math (Integer / DINT) ---\n");

    ADD_I add1 = { .EN = true, .IN1 = 7, .IN2 = 3 };
    ADD_I_Call(&add1);
    check("ADD_I   7 + 3 = 10",    add1.ENO && add1.OUT == 10);

    ADD_I add2 = { .EN = true, .IN1 = -4, .IN2 = 1 };
    ADD_I_Call(&add2);
    check("ADD_I  -4 + 1 = -3",    add2.ENO && add2.OUT == -3);

    SUB_I sub = { .EN = true, .IN1 = 10, .IN2 = 4 };
    SUB_I_Call(&sub);
    check("SUB_I  10 - 4 = 6",     sub.ENO && sub.OUT == 6);

    MUL_I mul = { .EN = true, .IN1 = 6, .IN2 = 7 };
    MUL_I_Call(&mul);
    check("MUL_I   6 * 7 = 42",    mul.ENO && mul.OUT == 42);

    DIV_I div1 = { .EN = true, .IN1 = 15, .IN2 = 4 };
    DIV_I_Call(&div1);
    check("DIV_I  15 / 4 = 3",     div1.ENO && div1.OUT == 3);

    DIV_I div0 = { .EN = true, .IN1 = 9, .IN2 = 0 };
    DIV_I_Call(&div0);
    check("DIV_I  by zero: ENO=false", !div0.ENO && div0.OUT == 0);

    MOD_I mod1 = { .EN = true, .IN1 = 17, .IN2 = 5 };
    MOD_I_Call(&mod1);
    check("MOD_I  17 % 5 = 2",     mod1.ENO && mod1.OUT == 2);

    MOD_I mod2 = { .EN = true, .IN1 = -17, .IN2 = 5 };
    MOD_I_Call(&mod2);
    check("MOD_I -17 % 5 = -2",    mod2.ENO && mod2.OUT == -2);

    MOD_I mod0 = { .EN = true, .IN1 = 9, .IN2 = 0 };
    MOD_I_Call(&mod0);
    check("MOD_I  by zero: ENO=false", !mod0.ENO && mod0.OUT == 0);

    MOVE_I mov = { .EN = true, .IN = 42 };
    MOVE_I_Call(&mov);
    check("MOVE_I 42 = 42",        mov.ENO && mov.OUT == 42);
}

/* ─────────────────────────────────────────────
 * EN = false tests
 * ───────────────────────────────────────────── */
static void test_en_disabled(void) {
    printf("\n--- EN = false (disabled) ---\n");

    ADD_F add = { .EN = false, .IN1 = 2.5f, .IN2 = 1.5f, .OUT = 99.0f };
    ADD_F_Call(&add);
    check("ADD_F EN=false: ENO=false, OUT unchanged",
          !add.ENO && feq(add.OUT, 99.0f, 1e-9f));

    MOVE_I mov = { .EN = false, .IN = 42, .OUT = 99 };
    MOVE_I_Call(&mov);
    check("MOVE_I EN=false: ENO=false, OUT unchanged",
          !mov.ENO && mov.OUT == 99);

    SIN_FB sf = { .EN = false, .IN = 1.0f, .OUT = 99.0f };
    SIN_Call(&sf);
    check("SIN EN=false: ENO=false, OUT unchanged",
          !sf.ENO && feq(sf.OUT, 99.0f, 1e-9f));

    SQRT_FB sq = { .EN = false, .IN = 4.0f, .OUT = 99.0f };
    SQRT_Call(&sq);
    check("SQRT EN=false: ENO=false, OUT unchanged",
          !sq.ENO && feq(sq.OUT, 99.0f, 1e-9f));
}

/* ─────────────────────────────────────────────
 * ABS, SQRT, EXPT
 * ───────────────────────────────────────────── */
static void test_float_functions(void) {
    printf("\n--- ABS ---\n");

    ABS_F a1 = { .EN = true, .IN =  3.5f }; ABS_F_Call(&a1);
    check("ABS  3.5 = 3.5",   a1.ENO && feq(a1.OUT, 3.5f, 1e-6f));

    ABS_F a2 = { .EN = true, .IN = -3.5f }; ABS_F_Call(&a2);
    check("ABS -3.5 = 3.5",   a2.ENO && feq(a2.OUT, 3.5f, 1e-6f));

    ABS_F a3 = { .EN = true, .IN =  0.0f }; ABS_F_Call(&a3);
    check("ABS  0.0 = 0.0",   a3.ENO && feq(a3.OUT, 0.0f, 1e-9f));

    printf("\n--- SQRT ---\n");
    SQRT_FB s1 = { .EN = true, .IN = 4.0f };  SQRT_Call(&s1);
    check("SQRT  4.0 = 2.0",      s1.ENO && feq(s1.OUT, 2.0f, 1e-5f));

    SQRT_FB s2 = { .EN = true, .IN = 9.0f };  SQRT_Call(&s2);
    check("SQRT  9.0 = 3.0",      s2.ENO && feq(s2.OUT, 3.0f, 1e-5f));

    SQRT_FB s3 = { .EN = true, .IN = 2.0f };  SQRT_Call(&s3);
    check("SQRT  2.0 = 1.41421",  s3.ENO && feq(s3.OUT, 1.41421356f, 1e-5f));

    SQRT_FB s4 = { .EN = true, .IN = 0.0f };  SQRT_Call(&s4);
    check("SQRT  0.0 = 0.0",      s4.ENO && feq(s4.OUT, 0.0f, 1e-9f));

    SQRT_FB s5 = { .EN = true, .IN = -1.0f }; SQRT_Call(&s5);
    check("SQRT  neg: ENO=false",  !s5.ENO && feq(s5.OUT, 0.0f, 1e-9f));

    SQRT_FB s6 = { .EN = true, .IN = 81.0f }; SQRT_Call(&s6);
    check("SQRT  81.0 = 9.0",     s6.ENO && feq(s6.OUT, 9.0f, 1e-5f));

    printf("\n--- EXPT ---\n");
    EXPT_FB e1 = { .EN = true, .IN1 = 2.0f, .IN2 = 10.0f }; EXPT_Call(&e1);
    check("EXPT  2^10 = 1024",    e1.ENO && feq(e1.OUT, 1024.0f, 0.1f));

    EXPT_FB e2 = { .EN = true, .IN1 = 4.0f, .IN2 = 0.5f };  EXPT_Call(&e2);
    check("EXPT  4^0.5 = 2.0",    e2.ENO && feq(e2.OUT, 2.0f, 1e-4f));

    EXPT_FB e3 = { .EN = true, .IN1 = KRON_E, .IN2 = 1.0f }; EXPT_Call(&e3);
    check("EXPT  e^1 ≈ 2.71828",  e3.ENO && feq(e3.OUT, KRON_E, 1e-4f));

    EXPT_FB e4 = { .EN = true, .IN1 = 5.0f, .IN2 = 0.0f };  EXPT_Call(&e4);
    check("EXPT  x^0 = 1",        e4.ENO && feq(e4.OUT, 1.0f, 1e-6f));

    EXPT_FB e5 = { .EN = true, .IN1 = 0.0f, .IN2 = 0.0f };  EXPT_Call(&e5);
    check("EXPT  0^0 = 1",        e5.ENO && feq(e5.OUT, 1.0f, 1e-9f));

    EXPT_FB e6 = { .EN = true, .IN1 = 0.0f, .IN2 = 1.0f };  EXPT_Call(&e6);
    check("EXPT  0^1 = 0",        e6.ENO && feq(e6.OUT, 0.0f, 1e-9f));

    EXPT_FB e7 = { .EN = true, .IN1 = -2.0f, .IN2 = 2.0f }; EXPT_Call(&e7);
    check("EXPT  neg base: ENO=false", !e7.ENO && feq(e7.OUT, 0.0f, 1e-9f));
}

/* ─────────────────────────────────────────────
 * Trigonometry
 * ───────────────────────────────────────────── */
static void test_trig(void) {
    SIN_FB  sf; COS_FB  cf; TAN_FB  tf;
    ATAN_FB atf; ASIN_FB asf; ACOS_FB acf;

    printf("\n--- SIN ---\n");
    sf = (SIN_FB){ .EN = true, .IN = 0.0f };           SIN_Call(&sf);
    check("SIN  0        = 0.0",      sf.ENO && feq(sf.OUT, 0.0f, 1e-5f));
    sf = (SIN_FB){ .EN = true, .IN = KRON_PI/6.0f };   SIN_Call(&sf);
    check("SIN  π/6      = 0.5",      sf.ENO && feq(sf.OUT, 0.5f, 1e-5f));
    sf = (SIN_FB){ .EN = true, .IN = KRON_PI/4.0f };   SIN_Call(&sf);
    check("SIN  π/4      ≈ 0.70711",  sf.ENO && feq(sf.OUT, 0.70710678f, 1e-5f));
    sf = (SIN_FB){ .EN = true, .IN = KRON_HALF_PI };   SIN_Call(&sf);
    check("SIN  π/2      = 1.0",      sf.ENO && feq(sf.OUT, 1.0f, 1e-5f));
    sf = (SIN_FB){ .EN = true, .IN = KRON_PI };        SIN_Call(&sf);
    check("SIN  π        ≈ 0.0",      sf.ENO && feq(sf.OUT, 0.0f, 1e-5f));
    sf = (SIN_FB){ .EN = true, .IN = -KRON_HALF_PI };  SIN_Call(&sf);
    check("SIN -π/2      = -1.0",     sf.ENO && feq(sf.OUT, -1.0f, 1e-5f));
    sf = (SIN_FB){ .EN = true, .IN = 3.0f*KRON_HALF_PI }; SIN_Call(&sf);
    check("SIN  3π/2     = -1.0",     sf.ENO && feq(sf.OUT, -1.0f, 1e-5f));

    printf("\n--- COS ---\n");
    cf = (COS_FB){ .EN = true, .IN = 0.0f };           COS_Call(&cf);
    check("COS  0        = 1.0",      cf.ENO && feq(cf.OUT, 1.0f, 1e-5f));
    cf = (COS_FB){ .EN = true, .IN = KRON_PI/3.0f };   COS_Call(&cf);
    check("COS  π/3      = 0.5",      cf.ENO && feq(cf.OUT, 0.5f, 1e-5f));
    cf = (COS_FB){ .EN = true, .IN = KRON_PI/4.0f };   COS_Call(&cf);
    check("COS  π/4      ≈ 0.70711",  cf.ENO && feq(cf.OUT, 0.70710678f, 1e-5f));
    cf = (COS_FB){ .EN = true, .IN = KRON_HALF_PI };   COS_Call(&cf);
    check("COS  π/2      ≈ 0.0",      cf.ENO && feq(cf.OUT, 0.0f, 1e-5f));
    cf = (COS_FB){ .EN = true, .IN = KRON_PI };        COS_Call(&cf);
    check("COS  π        = -1.0",     cf.ENO && feq(cf.OUT, -1.0f, 1e-5f));
    cf = (COS_FB){ .EN = true, .IN = -KRON_PI/3.0f };  COS_Call(&cf);
    check("COS -π/3      = 0.5",      cf.ENO && feq(cf.OUT, 0.5f, 1e-5f));

    printf("\n--- TAN ---\n");
    tf = (TAN_FB){ .EN = true, .IN = 0.0f };           TAN_Call(&tf);
    check("TAN  0        = 0.0",      tf.ENO && feq(tf.OUT, 0.0f, 1e-5f));
    tf = (TAN_FB){ .EN = true, .IN = KRON_PI/4.0f };   TAN_Call(&tf);
    check("TAN  π/4      = 1.0",      tf.ENO && feq(tf.OUT, 1.0f, 1e-4f));
    tf = (TAN_FB){ .EN = true, .IN = KRON_PI/6.0f };   TAN_Call(&tf);
    check("TAN  π/6      ≈ 0.57735",  tf.ENO && feq(tf.OUT, 0.57735027f, 1e-4f));
    tf = (TAN_FB){ .EN = true, .IN = -KRON_PI/4.0f };  TAN_Call(&tf);
    check("TAN -π/4      = -1.0",     tf.ENO && feq(tf.OUT, -1.0f, 1e-4f));

    printf("\n--- ATAN ---\n");
    atf = (ATAN_FB){ .EN = true, .IN = 0.0f };         ATAN_Call(&atf);
    check("ATAN  0       = 0.0",      atf.ENO && feq(atf.OUT, 0.0f, 1e-5f));
    atf = (ATAN_FB){ .EN = true, .IN = 1.0f };         ATAN_Call(&atf);
    check("ATAN  1       = π/4",      atf.ENO && feq(atf.OUT, 0.78539816f, 1e-5f));
    atf = (ATAN_FB){ .EN = true, .IN = -1.0f };        ATAN_Call(&atf);
    check("ATAN -1       = -π/4",     atf.ENO && feq(atf.OUT, -0.78539816f, 1e-5f));
    atf = (ATAN_FB){ .EN = true, .IN = 0.57735027f };  ATAN_Call(&atf);
    check("ATAN  0.57735 ≈ π/6",     atf.ENO && feq(atf.OUT, KRON_PI/6.0f, 1e-5f));
    atf = (ATAN_FB){ .EN = true, .IN = 1e6f };         ATAN_Call(&atf);
    check("ATAN  large   ≈ π/2",      atf.ENO && feq(atf.OUT, KRON_HALF_PI, 1e-3f));

    printf("\n--- ASIN ---\n");
    asf = (ASIN_FB){ .EN = true, .IN = 0.0f };  ASIN_Call(&asf);
    check("ASIN  0.0    = 0.0",       asf.ENO && feq(asf.OUT, 0.0f, 1e-5f));
    asf = (ASIN_FB){ .EN = true, .IN = 0.5f };  ASIN_Call(&asf);
    check("ASIN  0.5    = π/6",       asf.ENO && feq(asf.OUT, 0.52359878f, 1e-4f));
    asf = (ASIN_FB){ .EN = true, .IN = 1.0f };  ASIN_Call(&asf);
    check("ASIN  1.0    = π/2",       asf.ENO && feq(asf.OUT, KRON_HALF_PI, 1e-5f));
    asf = (ASIN_FB){ .EN = true, .IN = -1.0f }; ASIN_Call(&asf);
    check("ASIN -1.0    = -π/2",      asf.ENO && feq(asf.OUT, -KRON_HALF_PI, 1e-5f));
    asf = (ASIN_FB){ .EN = true, .IN = 1.5f };  ASIN_Call(&asf);
    check("ASIN  clamp >1 = π/2",     asf.ENO && feq(asf.OUT, KRON_HALF_PI, 1e-5f));

    printf("\n--- ACOS ---\n");
    acf = (ACOS_FB){ .EN = true, .IN = 1.0f };  ACOS_Call(&acf);
    check("ACOS  1.0    = 0.0",       acf.ENO && feq(acf.OUT, 0.0f, 1e-5f));
    acf = (ACOS_FB){ .EN = true, .IN = 0.5f };  ACOS_Call(&acf);
    check("ACOS  0.5    = π/3",       acf.ENO && feq(acf.OUT, 1.04719755f, 1e-4f));
    acf = (ACOS_FB){ .EN = true, .IN = 0.0f };  ACOS_Call(&acf);
    check("ACOS  0.0    = π/2",       acf.ENO && feq(acf.OUT, KRON_HALF_PI, 1e-5f));
    acf = (ACOS_FB){ .EN = true, .IN = -1.0f }; ACOS_Call(&acf);
    check("ACOS -1.0    = π",         acf.ENO && feq(acf.OUT, KRON_PI, 1e-4f));

    printf("\n--- sin²+cos²=1 (Pythagorean identity) ---\n");
    float angles[] = {0.0f, 0.3f, 0.7f, 1.0f, 1.5f, 2.0f, 2.8f, 3.1f};
    int i;
    int pyth_ok = 1;
    for (i = 0; i < 8; i++) {
        sf = (SIN_FB){ .EN = true, .IN = angles[i] }; SIN_Call(&sf);
        cf = (COS_FB){ .EN = true, .IN = angles[i] }; COS_Call(&cf);
        if (!feq(sf.OUT*sf.OUT + cf.OUT*cf.OUT, 1.0f, 1e-4f)) {
            pyth_ok = 0;
            printf("    FAIL at angle %.2f: sin²+cos² = %.7f\n",
                   angles[i], sf.OUT*sf.OUT + cf.OUT*cf.OUT);
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
    SQRT_FB sq; EXPT_FB ex;
    for (i = 0; i < 6; i++) {
        sq = (SQRT_FB){ .EN = true, .IN = vals[i] }; SQRT_Call(&sq);
        ex = (EXPT_FB){ .EN = true, .IN1 = vals[i], .IN2 = 0.5f }; EXPT_Call(&ex);
        if (!feq(sq.OUT, ex.OUT, 1e-4f)) {
            sq_ok = 0;
            printf("    FAIL at x=%.0f: SQRT=%.6f EXPT=%.6f\n",
                   vals[i], sq.OUT, ex.OUT);
        }
    }
    check("SQRT(x) == EXPT(x, 0.5) for 6 values", sq_ok);

    /* ASIN(SIN(x)) == x for x in [-π/2, π/2] */
    SIN_FB sf; ASIN_FB asf;
    float ax[] = {0.0f, 0.4f, 0.8f, 1.2f, -0.5f, -1.0f};
    int asin_ok = 1;
    for (i = 0; i < 6; i++) {
        sf = (SIN_FB){ .EN = true, .IN = ax[i] }; SIN_Call(&sf);
        asf = (ASIN_FB){ .EN = true, .IN = sf.OUT }; ASIN_Call(&asf);
        if (!feq(asf.OUT, ax[i], 1e-4f)) {
            asin_ok = 0;
            printf("    FAIL at x=%.2f: ASIN(SIN)=%.6f\n", ax[i], asf.OUT);
        }
    }
    check("ASIN(SIN(x))==x for x in [-π/2,π/2]", asin_ok);

    /* ATAN(TAN(x)) == x for x in (-π/2, π/2) */
    TAN_FB tf; ATAN_FB atf;
    float at[] = {0.0f, 0.3f, 0.7f, 1.0f, -0.5f, -1.2f};
    int atan_ok = 1;
    for (i = 0; i < 6; i++) {
        tf = (TAN_FB){ .EN = true, .IN = at[i] }; TAN_Call(&tf);
        atf = (ATAN_FB){ .EN = true, .IN = tf.OUT }; ATAN_Call(&atf);
        if (!feq(atf.OUT, at[i], 1e-4f)) {
            atan_ok = 0;
            printf("    FAIL at x=%.2f: ATAN(TAN)=%.6f\n", at[i], atf.OUT);
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
    test_en_disabled();
    test_float_functions();
    test_trig();
    test_consistency();

    printf("\n========================================\n");
    printf("  Results: %d passed, %d failed\n", pass_count, fail_count);
    printf("========================================\n");

    return (fail_count == 0) ? 0 : 1;
}
