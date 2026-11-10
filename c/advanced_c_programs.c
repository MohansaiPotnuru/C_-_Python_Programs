/*
 * 50 Advanced C Programs - each concept is a small function, and main() runs
 * every one of them with a self-check so the file doubles as a test suite.
 *
 * Build & run:
 *   gcc -std=c11 -Wall -Wextra -O2 advanced_c_programs.c -o advanced_c && ./advanced_c
 *
 * Sections
 *   1. Conditionals & bitwise tricks        (1-10)
 *   2. Loops, jumps & control flow          (11-20)
 *   3. Pointers & memory management         (21-30)
 *   4. Structures, unions & advanced types  (31-40)
 *   5. Preprocessor, strings & system       (41-50)
 */
#include <assert.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int checks_passed = 0;
#define CHECK(expr)                                                         \
    do {                                                                    \
        if (!(expr)) {                                                      \
            fprintf(stderr, "FAILED %s:%d: %s\n", __FILE__, __LINE__, #expr); \
            exit(EXIT_FAILURE);                                             \
        }                                                                   \
        checks_passed++;                                                    \
    } while (0)

/* ========================================================================== */
/* SECTION 1: CONDITIONALS & BITWISE TRICKS (Programs 1-10)                   */
/* ========================================================================== */

/* Program 1: Swap without a temporary using XOR.
 * Guard against a == b: XOR-ing a location with itself would zero it. */
void prog01_xor_swap(int *a, int *b) {
    if (a == b) return;
    *a ^= *b;
    *b ^= *a;
    *a ^= *b;
}

/* Program 2: Power of two check - a power of two has exactly one bit set. */
int prog02_is_power_of_two(int n) { return (n > 0) && ((n & (n - 1)) == 0); }

/* Program 3: Count set bits (Brian Kernighan's algorithm: clears lowest set bit). */
int prog03_count_set_bits(unsigned int n) {
    int count = 0;
    while (n) { n &= (n - 1); count++; }
    return count;
}

/* Program 4: Even/odd using the lowest bit. */
int prog04_is_even(int n) { return !(n & 1); }

/* Program 5: Find the element that appears once when all others appear twice. */
int prog05_single_number(const int arr[], int size) {
    int res = 0;
    for (int i = 0; i < size; i++) res ^= arr[i];
    return res;
}

/* Program 6/7: Multiply / divide by two with shifts (non-negative values). */
int prog06_mul_by_two(int n) { return (int)((unsigned)n << 1); }
int prog07_div_by_two(int n) { return n >> 1; }   /* rounds toward -inf for negatives */

/* Program 8: Branch-free absolute value.
 * Relies on arithmetic right shift of negative ints (implementation-defined,
 * but what GCC/Clang/MSVC do). Undefined for INT_MIN. */
int prog08_abs(int n) {
    int mask = n >> (sizeof(int) * 8 - 1);
    return (n + mask) ^ mask;
}

/* Program 9: Conditional execution via && short-circuit. Returns 1 if it printed. */
int prog09_conditional(int cond) {
    return cond && printf("  [9] Condition met!\n");
}

/* Program 10: Branch-free sign: -1, 0 or +1. */
int prog10_sign(int x) { return (x > 0) - (x < 0); }

/* ========================================================================== */
/* SECTION 2: LOOPS, JUMPS & CONTROL FLOW (Programs 11-20)                    */
/* ========================================================================== */

/* Program 11: Break out of nested loops with goto. Returns i*100 + j at exit. */
int prog11_goto_nested_break(void) {
    int i, j;
    for (i = 0; i < 5; i++)
        for (j = 0; j < 5; j++)
            if (i * j == 6) goto out;
    return -1;
out:
    return i * 100 + j;
}

/* Program 12: Duff's device - loop unrolling via an interleaved switch/do-while. */
void prog12_duffs_device(short *to, const short *from, int count) {
    if (count <= 0) return;
    int n = (count + 7) / 8;
    switch (count % 8) {
    case 0: do { *to++ = *from++;   /* fall through */
    case 7:      *to++ = *from++;   /* fall through */
    case 6:      *to++ = *from++;   /* fall through */
    case 5:      *to++ = *from++;   /* fall through */
    case 4:      *to++ = *from++;   /* fall through */
    case 3:      *to++ = *from++;   /* fall through */
    case 2:      *to++ = *from++;   /* fall through */
    case 1:      *to++ = *from++;
            } while (--n > 0);
    }
}

/* Program 13: Infinite-loop idiom for(;;) with an explicit exit. */
int prog13_infinite_loop(void) {
    int c = 0;
    for (;;) { if (++c == 3) break; }
    return c;
}

/* Program 14: Two loop variables converging from both ends (e.g. reversal). */
void prog14_reverse(int *arr, int n) {
    for (int i = 0, j = n - 1; i < j; i++, j--) {
        int t = arr[i]; arr[i] = arr[j]; arr[j] = t;
    }
}

/* Program 15: Work inside the loop condition - a strlen in one line. */
size_t prog15_strlen(const char *str) {
    size_t i = 0;
    while (str[i] != '\0') i++;
    return i;
}

/* Program 16: Exception-like error handling with setjmp/longjmp. */
static jmp_buf prog16_env;
static void prog16_error_trigger(void) { longjmp(prog16_env, 42); }
int prog16_exception_handling(void) {
    int code = setjmp(prog16_env);
    if (code == 0) {
        prog16_error_trigger();
        return 0; /* not reached */
    }
    return code; /* "caught" error code */
}

/* Program 17: Tail recursion - the recursive call is the last operation, so an
 * optimising compiler (-O2) can turn it into a loop with no stack growth. */
long prog17_sum_tail(int n, long acc) {
    if (n <= 0) return acc;
    return prog17_sum_tail(n - 1, acc + n);
}

/* Program 18: Sequence points - never modify a variable twice between them.
 * `i = i++ + ++i;` is undefined behaviour; split it into well-defined steps. */
int prog18_sequence_points(void) {
    int i = 1;
    int a = i++;   /* a = 1, i = 2 */
    int b = ++i;   /* i = 3, b = 3 */
    return a + b;  /* 4 */
}

/* Program 19: do { ... } while (0) makes a multi-statement macro act like one
 * statement, so it is safe inside an un-braced if/else. */
#define SAFE_INC_TWICE(x) do { (x)++; (x)++; } while (0)
int prog19_do_while_zero(int val) {
    if (val >= 0) SAFE_INC_TWICE(val);
    else val = 0;
    return val;
}

/* Program 20: Intentional switch fall-through to share a case body. */
const char *prog20_severity(int code) {
    switch (code) {
    case 1: /* fall through */
    case 2: return "low";
    case 3: return "high";
    default: return "unknown";
    }
}

/* ========================================================================== */
/* SECTION 3: POINTERS & MEMORY MANAGEMENT (Programs 21-30)                   */
/* ========================================================================== */

/* Program 21: Generic value printing through void* plus a type tag. */
void prog21_void_pointer(const void *ptr, char type) {
    if (type == 'i') printf("  [21] int: %d\n", *(const int *)ptr);
    else if (type == 'f') printf("  [21] float: %f\n", *(const float *)ptr);
}

/* Program 22: Pointer arithmetic - *(p + i) is exactly p[i]. */
int prog22_pointer_arithmetic(void) {
    int arr[] = {10, 20, 30};
    int *p = arr;
    return *(p + 1);
}

/* Program 23: Double pointer so a function can allocate for its caller. */
int prog23_allocate_buffer(int **buf, size_t size) {
    *buf = malloc(size * sizeof **buf);
    return *buf != NULL;
}

/* Program 24: Function-pointer dispatch table. */
static int op_add(int a, int b) { return a + b; }
static int op_sub(int a, int b) { return a - b; }
int prog24_function_pointers(int which, int a, int b) {
    int (*const ops[])(int, int) = {op_add, op_sub};
    return ops[which](a, b);
}

/* Program 25: Safe realloc - never overwrite the only pointer with NULL. */
int prog25_safe_realloc(void) {
    int *ptr = malloc(2 * sizeof *ptr);
    if (!ptr) return 0;
    int *tmp = realloc(ptr, 5 * sizeof *ptr);
    if (!tmp) { free(ptr); return 0; }   /* original block still valid -> free it */
    ptr = tmp;
    ptr[4] = 5;
    int ok = ptr[4] == 5;
    free(ptr);
    return ok;
}

/* Program 26: const int * (data read-only) vs int *const (pointer fixed). */
int prog26_const_pointers(void) {
    int x = 10, y = 20;
    const int *ptr_to_const = &x;  /* *ptr_to_const = 1;  -> compile error */
    ptr_to_const = &y;             /* re-pointing is fine */
    int *const const_ptr = &x;     /* const_ptr = &y;     -> compile error */
    *const_ptr = 11;               /* modifying data is fine */
    return *ptr_to_const + x;      /* 20 + 11 */
}

/* Program 27: Detect endianness at run time. Returns 1 for little-endian. */
int prog27_is_little_endian(void) {
    unsigned int x = 1;
    return *(unsigned char *)&x == 1;
}

/* Program 28: Free and NULL a pointer in one step to avoid dangling pointers. */
void prog28_safe_free(void **pptr) {
    if (pptr && *pptr) { free(*pptr); *pptr = NULL; }
}

/* Program 29: Byte-by-byte memcpy. */
void *prog29_memcpy(void *dest, const void *src, size_t n) {
    unsigned char *d = dest;
    const unsigned char *s = src;
    while (n--) *d++ = *s++;
    return dest;
}

/* Program 30: Variable-length array with a size guard (VLAs live on the stack). */
int prog30_vla_sum(int size) {
    if (size <= 0 || size > 1000) return -1;
    int vla[size];
    int sum = 0;
    for (int i = 0; i < size; i++) { vla[i] = i + 1; sum += vla[i]; }
    return sum;
}

/* ========================================================================== */
/* SECTION 4: STRUCTURES, UNIONS & ADVANCED TYPES (Programs 31-40)            */
/* ========================================================================== */

/* Program 31: Bit-fields pack several flags into one word. */
struct Prog31_Flags {
    unsigned int is_active : 1;
    unsigned int is_admin  : 1;
    unsigned int status    : 4;   /* 0..15 */
};

/* Program 32: #pragma pack removes padding (beware unaligned access cost). */
struct Prog32_Normal { char c; int i; };
#pragma pack(push, 1)
struct Prog32_Packed { char c; int i; };
#pragma pack(pop)

/* Program 33: Flexible array member - one allocation for header + data. */
struct Prog33_FlexArray {
    int length;
    int data[];
};
struct Prog33_FlexArray *prog33_make(int n) {
    struct Prog33_FlexArray *fa = malloc(sizeof *fa + (size_t)n * sizeof fa->data[0]);
    if (!fa) return NULL;
    fa->length = n;
    for (int i = 0; i < n; i++) fa->data[i] = i * i;
    return fa;
}

/* Program 34: Union type punning - inspect the IEEE-754 bits of a float. */
union Prog34_FloatBits { float f; uint32_t u; };
uint32_t prog34_float_bits(float f) {
    union Prog34_FloatBits fb;
    fb.f = f;
    return fb.u;
}

/* Program 35: Self-referential struct - a singly linked list. */
typedef struct Prog35_Node { int data; struct Prog35_Node *next; } Prog35_Node;
int prog35_list_sum(void) {
    Prog35_Node c = {3, NULL}, b = {2, &c}, a = {1, &b};
    int sum = 0;
    for (Prog35_Node *n = &a; n; n = n->next) sum += n->data;
    return sum;
}

/* Program 36: Anonymous struct inside a union (C11): v.x aliases v.raw[0]. */
struct Prog36_Vector {
    union {
        struct { float x, y, z; };
        float raw[3];
    };
};

/* Program 37: Opaque pointer - callers see only a handle, never the layout. */
typedef struct Prog37_Counter *Prog37_Handle;
struct Prog37_Counter { int value; };            /* would live in the .c file */
Prog37_Handle prog37_create(void) { return calloc(1, sizeof(struct Prog37_Counter)); }
void prog37_inc(Prog37_Handle h) { h->value++; }
int  prog37_get(Prog37_Handle h) { return h->value; }
void prog37_destroy(Prog37_Handle h) { free(h); }

/* Program 38: How offsetof works. The classic null-pointer trick below is
 * technically undefined behaviour - in real code use <stddef.h> offsetof(). */
#define MY_OFFSETOF(TYPE, MEMBER) ((size_t)&((TYPE *)0)->MEMBER)

/* Program 39: container_of - recover the enclosing struct from a member pointer
 * (used throughout the Linux kernel for intrusive lists). */
#define container_of(ptr, type, member) \
    ((type *)((char *)(ptr) - offsetof(type, member)))
struct Prog39_Item { int id; Prog35_Node link; };

/* Program 40: Enum values as bit flags. */
typedef enum { FLAG_A = 1u << 0, FLAG_B = 1u << 1, FLAG_C = 1u << 2 } Prog40_Flags;

/* ========================================================================== */
/* SECTION 5: PREPROCESSOR, STRINGS & SYSTEM CONCEPTS (Programs 41-50)        */
/* ========================================================================== */

/* Program 41: Stringification (#) - two levels so macros expand first. */
#define STR_IMPL(x) #x
#define TO_STR(x) STR_IMPL(x)
#define VERSION_MAJOR 3

/* Program 42: Token pasting (##). */
#define DECLARE_VAR(type, name) type var_##name

/* Program 43: Variadic macro (__VA_ARGS__). */
#define LOG_INFO(...) printf("  [43] " __VA_ARGS__)

/* Program 44: Variadic function with <stdarg.h>. */
double prog44_average(int count, ...) {
    if (count <= 0) return 0.0;
    va_list args;
    va_start(args, count);
    double sum = 0;
    for (int i = 0; i < count; i++) sum += va_arg(args, int);
    va_end(args);
    return sum / count;
}

/* Program 45: volatile - value may change outside normal program flow
 * (ISR, signal handler, memory-mapped register), so every access is real. */
static volatile int prog45_flag = 0;
void prog45_isr_simulation(void) { prog45_flag = 1; }

/* Program 46: static local - keeps its value between calls. */
int prog46_counter(void) {
    static int count = 0;
    return ++count;
}

/* Program 47: Tokenising a string. strtok keeps hidden state and is not
 * thread-safe; prefer strtok_r / strtok_s in multi-threaded code. */
int prog47_tokenize(char *str) {
    int tokens = 0;
    for (char *tok = strtok(str, ","); tok; tok = strtok(NULL, ",")) {
        printf("  [47] token: %s\n", tok);
        tokens++;
    }
    return tokens;
}

/* Program 48: Compile-time assertion (C11). */
_Static_assert(sizeof(int) >= 4, "int must be at least 32 bits");

/* Program 49: restrict - promises the pointers don't alias, enabling vectorisation. */
void prog49_vector_add(int *restrict a, const int *restrict b, const int *restrict c, int n) {
    for (int i = 0; i < n; i++) a[i] = b[i] + c[i];
}

/* Program 50: Aligned allocation (size must be a multiple of the alignment). */
#if defined(_WIN32)
#include <malloc.h>
#define ALIGNED_ALLOC(al, sz) _aligned_malloc((sz), (al))
#define ALIGNED_FREE(p) _aligned_free(p)
#else
#define ALIGNED_ALLOC(al, sz) aligned_alloc((al), (sz))
#define ALIGNED_FREE(p) free(p)
#endif
int prog50_aligned_mem(void) {
    int *ptr = ALIGNED_ALLOC(64, 1024 * sizeof(int));
    if (!ptr) return 0;
    int aligned = ((uintptr_t)ptr % 64) == 0;
    ALIGNED_FREE(ptr);
    return aligned;
}

/* ========================================================================== */
/* DEMO / SELF-TEST                                                           */
/* ========================================================================== */
int main(void) {
    printf("Section 1: bitwise\n");
    int a = 3, b = 7;
    prog01_xor_swap(&a, &b);                 CHECK(a == 7 && b == 3);
    prog01_xor_swap(&a, &a);                 CHECK(a == 7);
    CHECK(prog02_is_power_of_two(64) && !prog02_is_power_of_two(48));
    CHECK(prog03_count_set_bits(0xF0F0u) == 8);
    CHECK(prog04_is_even(10) && !prog04_is_even(7));
    int arr5[] = {4, 1, 2, 1, 2};
    CHECK(prog05_single_number(arr5, 5) == 4);
    CHECK(prog06_mul_by_two(21) == 42 && prog07_div_by_two(42) == 21);
    CHECK(prog08_abs(-15) == 15 && prog08_abs(9) == 9);
    CHECK(prog09_conditional(1) && !prog09_conditional(0));
    CHECK(prog10_sign(-5) == -1 && prog10_sign(0) == 0 && prog10_sign(8) == 1);

    printf("Section 2: control flow\n");
    CHECK(prog11_goto_nested_break() == 203);   /* i=2, j=3 */
    short src[11] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, dst[11] = {0};
    prog12_duffs_device(dst, src, 11);       CHECK(memcmp(src, dst, sizeof src) == 0);
    CHECK(prog13_infinite_loop() == 3);
    int arr14[] = {1, 2, 3, 4};
    prog14_reverse(arr14, 4);                CHECK(arr14[0] == 4 && arr14[3] == 1);
    CHECK(prog15_strlen("embedded") == 8);
    CHECK(prog16_exception_handling() == 42);
    CHECK(prog17_sum_tail(100, 0) == 5050);
    CHECK(prog18_sequence_points() == 4);
    CHECK(prog19_do_while_zero(5) == 7 && prog19_do_while_zero(-1) == 0);
    CHECK(strcmp(prog20_severity(2), "low") == 0 && strcmp(prog20_severity(3), "high") == 0);

    printf("Section 3: pointers & memory\n");
    int iv = 7; float fv = 2.5f;
    prog21_void_pointer(&iv, 'i');
    prog21_void_pointer(&fv, 'f');
    CHECK(prog22_pointer_arithmetic() == 20);
    int *buf = NULL;
    CHECK(prog23_allocate_buffer(&buf, 16));
    prog28_safe_free((void **)&buf);         CHECK(buf == NULL);
    CHECK(prog24_function_pointers(0, 5, 3) == 8 && prog24_function_pointers(1, 5, 3) == 2);
    CHECK(prog25_safe_realloc());
    CHECK(prog26_const_pointers() == 31);
    printf("  [27] %s-endian\n", prog27_is_little_endian() ? "little" : "big");
    char text[] = "copy me", out[sizeof text];
    prog29_memcpy(out, text, sizeof text);   CHECK(strcmp(out, text) == 0);
    CHECK(prog30_vla_sum(10) == 55 && prog30_vla_sum(5000) == -1);

    printf("Section 4: types\n");
    struct Prog31_Flags fl = {1, 0, 9};      CHECK(fl.is_active == 1 && fl.status == 9);
    CHECK(sizeof(struct Prog32_Packed) == sizeof(char) + sizeof(int));
    printf("  [32] normal=%zu bytes, packed=%zu bytes\n",
           sizeof(struct Prog32_Normal), sizeof(struct Prog32_Packed));
    struct Prog33_FlexArray *fa = prog33_make(5);
    CHECK(fa && fa->length == 5 && fa->data[4] == 16);
    free(fa);
    CHECK(prog34_float_bits(1.0f) == 0x3F800000u);
    CHECK(prog35_list_sum() == 6);
    struct Prog36_Vector v = {{{1.0f, 2.0f, 3.0f}}};
    CHECK(v.raw[1] == v.y);
    Prog37_Handle h = prog37_create();
    CHECK(h != NULL);
    prog37_inc(h); prog37_inc(h);            CHECK(prog37_get(h) == 2);
    prog37_destroy(h);
    CHECK(MY_OFFSETOF(struct Prog32_Normal, i) == offsetof(struct Prog32_Normal, i));
    struct Prog39_Item item = {.id = 99};
    CHECK(container_of(&item.link, struct Prog39_Item, link)->id == 99);
    unsigned flags = FLAG_A | FLAG_C;
    CHECK((flags & FLAG_C) && !(flags & FLAG_B));

    printf("Section 5: preprocessor & system\n");
    CHECK(strcmp(TO_STR(VERSION_MAJOR), "3") == 0);
    DECLARE_VAR(int, speed) = 120;           CHECK(var_speed == 120);
    LOG_INFO("variadic macro: %d + %d = %d\n", 2, 3, 5);
    CHECK(prog44_average(4, 10, 20, 30, 40) == 25.0);
    prog45_isr_simulation();                 CHECK(prog45_flag == 1);
    prog46_counter(); prog46_counter();      CHECK(prog46_counter() == 3);
    char csv[] = "red,green,blue";
    CHECK(prog47_tokenize(csv) == 3);
    int va[3], vb[3] = {1, 2, 3}, vc[3] = {10, 20, 30};
    prog49_vector_add(va, vb, vc, 3);        CHECK(va[2] == 33);
    CHECK(prog50_aligned_mem());

    printf("\nAll 50 programs ran - %d checks passed.\n", checks_passed);
    return 0;
}
