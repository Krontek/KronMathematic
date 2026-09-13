#include "kronmath.h"
#include <stdio.h>

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

// ─────────────────────────────────────────────
// ADD
// ─────────────────────────────────────────────
static void test_ADD(void) {
    printf("\n--- ADD (Sum of N inputs) ---\n");

    ADD a = {0};
    a.IN[0] = 10; a.IN[1] = 20; a.IN[2] = 30;
    a.N = 3;
    ADD_Call(&a);
    check("10 + 20 + 30 = 60", a.OUT == 60);

    ADD a2 = {0};
    a2.IN[0] = -5; a2.IN[1] = 3; a2.IN[2] = -8; a2.IN[3] = 10;
    a2.N = 4;
    ADD_Call(&a2);
    check("-5 + 3 + -8 + 10 = 0", a2.OUT == 0);

    // USINT-range values
    ADD a3 = {0};
    a3.IN[0] = 200; a3.IN[1] = 55;
    a3.N = 2;
    ADD_Call(&a3);
    check("USINT range: 200 + 55 = 255", a3.OUT == 255);

    // Single input
    ADD a4 = {0};
    a4.IN[0] = 42;
    a4.N = 1;
    ADD_Call(&a4);
    check("Single input: 42", a4.OUT == 42);
}

// ─────────────────────────────────────────────
// SUB
// ─────────────────────────────────────────────
static void test_SUB(void) {
    printf("\n--- SUB (Subtraction) ---\n");

    SUB s = { .IN1 = 100, .IN2 = 37 };
    SUB_Call(&s);
    check("100 - 37 = 63", s.OUT == 63);

    SUB s2 = { .IN1 = -10, .IN2 = -30 };
    SUB_Call(&s2);
    check("-10 - (-30) = 20", s2.OUT == 20);

    SUB s3 = { .IN1 = 0, .IN2 = 32767 };
    SUB_Call(&s3);
    check("0 - 32767 = -32767", s3.OUT == -32767);
}

// ─────────────────────────────────────────────
// MUL
// ─────────────────────────────────────────────
static void test_MUL(void) {
    printf("\n--- MUL (Product of N inputs) ---\n");

    MUL m = {0};
    m.IN[0] = 2; m.IN[1] = 3; m.IN[2] = 4;
    m.N = 3;
    MUL_Call(&m);
    check("2 * 3 * 4 = 24", m.OUT == 24);

    MUL m2 = {0};
    m2.IN[0] = -1; m2.IN[1] = 50;
    m2.N = 2;
    MUL_Call(&m2);
    check("-1 * 50 = -50", m2.OUT == -50);

    MUL m3 = {0};
    m3.IN[0] = 1000; m3.IN[1] = 1000;
    m3.N = 2;
    MUL_Call(&m3);
    check("1000 * 1000 = 1000000", m3.OUT == 1000000);
}

// ─────────────────────────────────────────────
// DIV
// ─────────────────────────────────────────────
static void test_DIV(void) {
    printf("\n--- DIV (Division) ---\n");

    DIV d = { .IN1 = 100, .IN2 = 4 };
    DIV_Call(&d);
    check("100 / 4 = 25", d.OUT == 25 && !d.ERR);

    DIV d2 = { .IN1 = 7, .IN2 = 2 };
    DIV_Call(&d2);
    check("7 / 2 = 3 (truncated)", d2.OUT == 3 && !d2.ERR);

    DIV d3 = { .IN1 = -15, .IN2 = 4 };
    DIV_Call(&d3);
    check("-15 / 4 = -3", d3.OUT == -3 && !d3.ERR);

    DIV d0 = { .IN1 = 10, .IN2 = 0 };
    DIV_Call(&d0);
    check("Div by zero: ERR=true", d0.ERR && d0.OUT == 0);
}

// ─────────────────────────────────────────────
// MOD
// ─────────────────────────────────────────────
static void test_MOD(void) {
    printf("\n--- MOD (Modulo) ---\n");

    MOD m = { .IN1 = 17, .IN2 = 5 };
    MOD_Call(&m);
    check("17 %% 5 = 2", m.OUT == 2 && !m.ERR);

    MOD m2 = { .IN1 = -17, .IN2 = 5 };
    MOD_Call(&m2);
    check("-17 %% 5 = -2", m2.OUT == -2 && !m2.ERR);

    MOD m0 = { .IN1 = 10, .IN2 = 0 };
    MOD_Call(&m0);
    check("Mod by zero: ERR=true", m0.ERR && m0.OUT == 0);
}

// ─────────────────────────────────────────────
// ABS
// ─────────────────────────────────────────────
static void test_ABS(void) {
    printf("\n--- ABS (Absolute Value) ---\n");

    ABS_FB a1 = { .IN = -42 };
    ABS_Call(&a1);
    check("|-42| = 42", a1.OUT == 42);

    ABS_FB a2 = { .IN = 100 };
    ABS_Call(&a2);
    check("|100| = 100", a2.OUT == 100);

    ABS_FB a3 = { .IN = 0 };
    ABS_Call(&a3);
    check("|0| = 0", a3.OUT == 0);
}

// ─────────────────────────────────────────────
// NEG
// ─────────────────────────────────────────────
static void test_NEG(void) {
    printf("\n--- NEG (Negation) ---\n");

    NEG n1 = { .IN = 50 };
    NEG_Call(&n1);
    check("-(50) = -50", n1.OUT == -50);

    NEG n2 = { .IN = -30 };
    NEG_Call(&n2);
    check("-(-30) = 30", n2.OUT == 30);
}

// ─────────────────────────────────────────────
// MOVE
// ─────────────────────────────────────────────
static void test_MOVE(void) {
    printf("\n--- MOVE (Copy) ---\n");

    MOVE m = { .IN = 12345 };
    MOVE_Call(&m);
    check("MOVE 12345 = 12345", m.OUT == 12345);
}

// ─────────────────────────────────────────────
// SQRT
// ─────────────────────────────────────────────
static void test_SQRT(void) {
    printf("\n--- SQRT (Integer Square Root) ---\n");

    SQRT_FB s1 = { .IN = 25 };
    SQRT_Call(&s1);
    check("sqrt(25) = 5", s1.OUT == 5 && !s1.ERR);

    SQRT_FB s2 = { .IN = 26 };
    SQRT_Call(&s2);
    check("sqrt(26) = 5 (floor)", s2.OUT == 5 && !s2.ERR);

    SQRT_FB s3 = { .IN = 0 };
    SQRT_Call(&s3);
    check("sqrt(0) = 0", s3.OUT == 0 && !s3.ERR);

    SQRT_FB s4 = { .IN = 1 };
    SQRT_Call(&s4);
    check("sqrt(1) = 1", s4.OUT == 1 && !s4.ERR);

    SQRT_FB s5 = { .IN = -4 };
    SQRT_Call(&s5);
    check("sqrt(-4): ERR=true", s5.ERR && s5.OUT == 0);

    SQRT_FB s6 = { .IN = 10000 };
    SQRT_Call(&s6);
    check("sqrt(10000) = 100", s6.OUT == 100 && !s6.ERR);
}

// ─────────────────────────────────────────────
// EXPT
// ─────────────────────────────────────────────
static void test_EXPT(void) {
    printf("\n--- EXPT (Integer Power) ---\n");

    EXPT e1 = { .IN1 = 2, .IN2 = 10 };
    EXPT_Call(&e1);
    check("2^10 = 1024", e1.OUT == 1024 && !e1.ERR);

    EXPT e2 = { .IN1 = 3, .IN2 = 0 };
    EXPT_Call(&e2);
    check("3^0 = 1", e2.OUT == 1 && !e2.ERR);

    EXPT e3 = { .IN1 = 5, .IN2 = 1 };
    EXPT_Call(&e3);
    check("5^1 = 5", e3.OUT == 5 && !e3.ERR);

    EXPT e4 = { .IN1 = -2, .IN2 = 3 };
    EXPT_Call(&e4);
    check("(-2)^3 = -8", e4.OUT == -8 && !e4.ERR);

    EXPT e5 = { .IN1 = 10, .IN2 = -1 };
    EXPT_Call(&e5);
    check("10^(-1): ERR=true", e5.ERR);
}

// ─────────────────────────────────────────────
// MIN / MAX
// ─────────────────────────────────────────────
static void test_MIN_MAX(void) {
    printf("\n--- MIN / MAX ---\n");

    MIN_FB mn = {0};
    mn.IN[0] = 50; mn.IN[1] = -3; mn.IN[2] = 100; mn.IN[3] = 7;
    mn.N = 4;
    MIN_Call(&mn);
    check("MIN(50,-3,100,7) = -3", mn.OUT == -3);

    MAX_FB mx = {0};
    mx.IN[0] = 50; mx.IN[1] = -3; mx.IN[2] = 100; mx.IN[3] = 7;
    mx.N = 4;
    MAX_Call(&mx);
    check("MAX(50,-3,100,7) = 100", mx.OUT == 100);

    // Single input
    MIN_FB mn2 = {0};
    mn2.IN[0] = 42;
    mn2.N = 1;
    MIN_Call(&mn2);
    check("MIN(42) = 42", mn2.OUT == 42);

    MAX_FB mx2 = {0};
    mx2.IN[0] = 42;
    mx2.N = 1;
    MAX_Call(&mx2);
    check("MAX(42) = 42", mx2.OUT == 42);
}

// ─────────────────────────────────────────────
// LIMIT
// ─────────────────────────────────────────────
static void test_LIMIT(void) {
    printf("\n--- LIMIT (Clamp) ---\n");

    LIMIT lm1 = { .MN = 0, .IN = 50, .MX = 100 };
    LIMIT_Call(&lm1);
    check("LIMIT(0,50,100) = 50", lm1.OUT == 50);

    LIMIT lm2 = { .MN = 0, .IN = -10, .MX = 100 };
    LIMIT_Call(&lm2);
    check("LIMIT(0,-10,100) = 0", lm2.OUT == 0);

    LIMIT lm3 = { .MN = 0, .IN = 200, .MX = 100 };
    LIMIT_Call(&lm3);
    check("LIMIT(0,200,100) = 100", lm3.OUT == 100);
}

// ─────────────────────────────────────────────
// SEL
// ─────────────────────────────────────────────
static void test_SEL(void) {
    printf("\n--- SEL (Selector) ---\n");

    SEL s1 = { .G = false, .IN0 = 10, .IN1 = 20 };
    SEL_Call(&s1);
    check("SEL(false,10,20) = 10", s1.OUT == 10);

    SEL s2 = { .G = true, .IN0 = 10, .IN1 = 20 };
    SEL_Call(&s2);
    check("SEL(true,10,20) = 20", s2.OUT == 20);
}

// ─────────────────────────────────────────────
// MUX
// ─────────────────────────────────────────────
static void test_MUX(void) {
    printf("\n--- MUX (Multiplexer) ---\n");

    MUX m1 = {0};
    m1.IN[0] = 100; m1.IN[1] = 200; m1.IN[2] = 300;
    m1.N = 3;
    m1.K = 0;
    MUX_Call(&m1);
    check("MUX(K=0) = 100", m1.OUT == 100 && !m1.ERR);

    m1.K = 2;
    MUX_Call(&m1);
    check("MUX(K=2) = 300", m1.OUT == 300 && !m1.ERR);

    m1.K = 5;
    MUX_Call(&m1);
    check("MUX(K=5): ERR=true (out of range)", m1.ERR);
}

// ─────────────────────────────────────────────
// AVG
// ─────────────────────────────────────────────
static void test_AVG(void) {
    printf("\n--- AVG (Average) ---\n");

    AVG a1 = {0};
    a1.IN[0] = 10; a1.IN[1] = 20; a1.IN[2] = 30;
    a1.N = 3;
    AVG_Call(&a1);
    check("AVG(10,20,30) = 20", a1.OUT == 20);

    AVG a2 = {0};
    a2.IN[0] = 7; a2.IN[1] = 8;
    a2.N = 2;
    AVG_Call(&a2);
    check("AVG(7,8) = 7 (truncated)", a2.OUT == 7);
}

// ─────────────────────────────────────────────
// PLC Type Promotion
// ─────────────────────────────────────────────
static void test_type_promotion(void) {
    printf("\n--- PLC Type Promotion ---\n");

    // USINT (uint8_t) values
    uint8_t usint_a = 200, usint_b = 55;
    ADD add_u = {0};
    add_u.IN[0] = usint_a; add_u.IN[1] = usint_b;
    add_u.N = 2;
    ADD_Call(&add_u);
    check("USINT: 200 + 55 = 255", add_u.OUT == 255);

    // SINT (int8_t) values
    int8_t sint_a = -100, sint_b = 50;
    ADD add_s = {0};
    add_s.IN[0] = sint_a; add_s.IN[1] = sint_b;
    add_s.N = 2;
    ADD_Call(&add_s);
    check("SINT: -100 + 50 = -50", add_s.OUT == -50);

    // UINT (uint16_t) values
    uint16_t uint_a = 50000, uint_b = 15535;
    ADD add_ui = {0};
    add_ui.IN[0] = uint_a; add_ui.IN[1] = uint_b;
    add_ui.N = 2;
    ADD_Call(&add_ui);
    check("UINT: 50000 + 15535 = 65535", add_ui.OUT == 65535);

    // INT (int16_t) values
    int16_t int_a = -32000, int_b = 1000;
    SUB sub_i = { .IN1 = int_a, .IN2 = int_b };
    SUB_Call(&sub_i);
    check("INT: -32000 - 1000 = -33000", sub_i.OUT == -33000);

    // DINT (int32_t) values
    int32_t dint_a = 2000000000;
    int32_t dint_b = 147483647;
    ADD add_d = {0};
    add_d.IN[0] = dint_a; add_d.IN[1] = dint_b;
    add_d.N = 2;
    ADD_Call(&add_d);
    check("DINT: 2000000000 + 147483647 = 2147483647", add_d.OUT == 2147483647);

    // UDINT (uint32_t) → stored as int32_t, works for values <= INT32_MAX
    uint32_t udint_val = 1000000;
    MOVE mov = { .IN = (int32_t)udint_val };
    MOVE_Call(&mov);
    check("UDINT: MOVE 1000000", mov.OUT == 1000000);

    // Mixed types in multi-input ADD
    uint8_t  v1 = 10;   // USINT
    int16_t  v2 = -20;  // INT
    int32_t  v3 = 100;  // DINT
    uint16_t v4 = 500;  // UINT
    ADD add_mix = {0};
    add_mix.IN[0] = v1; add_mix.IN[1] = v2;
    add_mix.IN[2] = v3; add_mix.IN[3] = v4;
    add_mix.N = 4;
    ADD_Call(&add_mix);
    check("Mixed: 10 + (-20) + 100 + 500 = 590", add_mix.OUT == 590);
}

// ─────────────────────────────────────────────
// Main
// ─────────────────────────────────────────────
// ─────────────────────────────────────────────
// Regression: N clamping and SQRT overflow
// ─────────────────────────────────────────────
static void test_hardening(void) {
    printf("\n--- Hardening (N bounds, SQRT overflow) ---\n");

    // N is a plain FB field, so a program can set it past the array size.
    // Every multi-input block must clamp to KRON_MATH_MAX_IN.
    {
        ADD a = {0};
        for (int i = 0; i < KRON_MATH_MAX_IN; i++) a.IN[i] = 1;
        a.N = 200;                      // far beyond IN[32]
        ADD_Call(&a);
        check("ADD: N=200 clamped to 32", a.OUT == KRON_MATH_MAX_IN);
    }
    {
        AVG v = {0};
        for (int i = 0; i < KRON_MATH_MAX_IN; i++) v.IN[i] = 8;
        v.N = 255;
        AVG_Call(&v);
        check("AVG: N=255 clamped (divides by 32, not 255)", v.OUT == 8);
    }
    {
        MIN_FB m = {0};
        for (int i = 0; i < KRON_MATH_MAX_IN; i++) m.IN[i] = 100 + i;
        m.N = 250;
        MIN_Call(&m);
        check("MIN: N=250 clamped", m.OUT == 100);
    }
    {
        MAX_FB m = {0};
        for (int i = 0; i < KRON_MATH_MAX_IN; i++) m.IN[i] = 100 + i;
        m.N = 250;
        MAX_Call(&m);
        check("MAX: N=250 clamped", m.OUT == 100 + KRON_MATH_MAX_IN - 1);
    }
    {
        MUL m = {0};
        for (int i = 0; i < KRON_MATH_MAX_IN; i++) m.IN[i] = 1;
        m.N = 200;
        MUL_Call(&m);
        check("MUL: N=200 clamped", m.OUT == 1);
    }
    {
        MUX m = {0};
        for (int i = 0; i < KRON_MATH_MAX_IN; i++) m.IN[i] = i;
        m.N = 200;
        m.K = 40;                       // inside the bogus N, outside IN[]
        MUX_Call(&m);
        check("MUX: K past array rejected even when N is bogus", m.ERR == true);
        m.K = 5;
        MUX_Call(&m);
        check("MUX: K inside array still works", m.OUT == 5 && m.ERR == false);
    }

    // SQRT must not overflow its Newton-Raphson step at INT32_MAX.
    {
        SQRT_FB q = {0};
        q.IN = 2147483647;
        SQRT_Call(&q);
        check("SQRT: INT32_MAX -> 46340 (no overflow)",
              q.OUT == 46340 && q.ERR == false);

        q.IN = 2147395600;              // 46340^2 exactly
        SQRT_Call(&q);
        check("SQRT: 46340^2 -> 46340", q.OUT == 46340);

        q.IN = 2000000000;
        SQRT_Call(&q);
        check("SQRT: 2000000000 -> 44721", q.OUT == 44721);

        q.IN = 1;
        SQRT_Call(&q);
        check("SQRT: 1 -> 1", q.OUT == 1);
    }
}

int main(void) {
    printf("=== KronMathematic Test Suite ===\n");

    test_ADD();
    test_SUB();
    test_MUL();
    test_DIV();
    test_MOD();
    test_ABS();
    test_NEG();
    test_MOVE();
    test_SQRT();
    test_EXPT();
    test_MIN_MAX();
    test_LIMIT();
    test_SEL();
    test_MUX();
    test_AVG();
    test_type_promotion();
    test_hardening();

    printf("\n=== Results: %d passed, %d failed ===\n", pass_count, fail_count);
    return fail_count ? 1 : 0;
}
