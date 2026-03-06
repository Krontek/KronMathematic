/*===========================================================================
 * KronMathematic — Math Function Blocks (IEC 61131-3)
 *
 * All blocks follow the PLC function block pattern:
 *   Inputs:  EN, IN (or IN1/IN2 for binary ops)
 *   Outputs: ENO, OUT
 *
 * EN = true  → execute, ENO = true
 * EN = false → skip, ENO = false (OUT unchanged)
 *
 * Typed variants:
 *   _F suffix → float   (IEC REAL)
 *   _I suffix → int32_t (IEC DINT)
 *
 * No external dependencies. C99. Baremetal Cortex-M4 compatible.
 *===========================================================================*/

#ifndef KRONMATH_H
#define KRONMATH_H

#include <stdbool.h>
#include <stdint.h>

/* Math constants */
#define KRON_PI      3.14159265f
#define KRON_HALF_PI 1.57079633f
#define KRON_TWO_PI  6.28318531f
#define KRON_E       2.71828183f

/* =========================================================
 * ADD  (EN, IN1, IN2 → ENO, OUT)
 * ========================================================= */
typedef struct {
    bool  EN;  float   IN1; float   IN2;
    bool  ENO; float   OUT;
} ADD_F;

typedef struct {
    bool  EN;  int32_t IN1; int32_t IN2;
    bool  ENO; int32_t OUT;
} ADD_I;

void ADD_F_Call(ADD_F *inst);
void ADD_I_Call(ADD_I *inst);

/* =========================================================
 * SUB  (EN, IN1, IN2 → ENO, OUT)
 * ========================================================= */
typedef struct {
    bool  EN;  float   IN1; float   IN2;
    bool  ENO; float   OUT;
} SUB_F;

typedef struct {
    bool  EN;  int32_t IN1; int32_t IN2;
    bool  ENO; int32_t OUT;
} SUB_I;

void SUB_F_Call(SUB_F *inst);
void SUB_I_Call(SUB_I *inst);

/* =========================================================
 * MUL  (EN, IN1, IN2 → ENO, OUT)
 * ========================================================= */
typedef struct {
    bool  EN;  float   IN1; float   IN2;
    bool  ENO; float   OUT;
} MUL_F;

typedef struct {
    bool  EN;  int32_t IN1; int32_t IN2;
    bool  ENO; int32_t OUT;
} MUL_I;

void MUL_F_Call(MUL_F *inst);
void MUL_I_Call(MUL_I *inst);

/* =========================================================
 * DIV  (EN, IN1, IN2 → ENO, OUT)
 * ENO = false on div-by-zero
 * ========================================================= */
typedef struct {
    bool  EN;  float   IN1; float   IN2;
    bool  ENO; float   OUT;
} DIV_F;

typedef struct {
    bool  EN;  int32_t IN1; int32_t IN2;
    bool  ENO; int32_t OUT;
} DIV_I;

void DIV_F_Call(DIV_F *inst);
void DIV_I_Call(DIV_I *inst);

/* =========================================================
 * MOD  (EN, IN1, IN2 → ENO, OUT)
 * ENO = false on div-by-zero
 * ========================================================= */
typedef struct {
    bool  EN;  float   IN1; float   IN2;
    bool  ENO; float   OUT;
} MOD_F;

typedef struct {
    bool  EN;  int32_t IN1; int32_t IN2;
    bool  ENO; int32_t OUT;
} MOD_I;

void MOD_F_Call(MOD_F *inst);
void MOD_I_Call(MOD_I *inst);

/* =========================================================
 * MOVE  (EN, IN → ENO, OUT)
 * ========================================================= */
typedef struct {
    bool  EN;  float   IN;
    bool  ENO; float   OUT;
} MOVE_F;

typedef struct {
    bool  EN;  int32_t IN;
    bool  ENO; int32_t OUT;
} MOVE_I;

void MOVE_F_Call(MOVE_F *inst);
void MOVE_I_Call(MOVE_I *inst);

/* =========================================================
 * ABS  (EN, IN → ENO, OUT)
 * ========================================================= */
typedef struct {
    bool  EN;  float   IN;
    bool  ENO; float   OUT;
} ABS_F;

typedef struct {
    bool  EN;  int32_t IN;
    bool  ENO; int32_t OUT;
} ABS_I;

void ABS_F_Call(ABS_F *inst);
void ABS_I_Call(ABS_I *inst);

/* =========================================================
 * SQRT  (EN, IN → ENO, OUT)
 * ENO = false for negative input
 * ========================================================= */
typedef struct {
    bool  EN;  float IN;
    bool  ENO; float OUT;
} SQRT_FB;

void SQRT_Call(SQRT_FB *inst);

/* =========================================================
 * EXPT  (EN, IN1, IN2 → ENO, OUT)
 * IN1 = base, IN2 = exponent
 * ENO = false for negative base
 * ========================================================= */
typedef struct {
    bool  EN;  float IN1; float IN2;
    bool  ENO; float OUT;
} EXPT_FB;

void EXPT_Call(EXPT_FB *inst);

/* =========================================================
 * Trigonometric Functions  (EN, IN → ENO, OUT)
 * Angles in radians
 * ========================================================= */
typedef struct { bool EN; float IN; bool ENO; float OUT; } SIN_FB;
typedef struct { bool EN; float IN; bool ENO; float OUT; } COS_FB;
typedef struct { bool EN; float IN; bool ENO; float OUT; } TAN_FB;
typedef struct { bool EN; float IN; bool ENO; float OUT; } ASIN_FB;
typedef struct { bool EN; float IN; bool ENO; float OUT; } ACOS_FB;
typedef struct { bool EN; float IN; bool ENO; float OUT; } ATAN_FB;

void SIN_Call (SIN_FB  *inst);
void COS_Call (COS_FB  *inst);
void TAN_Call (TAN_FB  *inst);
void ASIN_Call(ASIN_FB *inst);  /* IN clamped to [-1, 1] */
void ACOS_Call(ACOS_FB *inst);  /* IN clamped to [-1, 1] */
void ATAN_Call(ATAN_FB *inst);

#endif /* KRONMATH_H */
