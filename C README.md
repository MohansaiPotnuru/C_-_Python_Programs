#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <stdarg.h>
/* ========================================================================== */
/* SECTION 1: CONDITIONALS & BITWISE TRICKS (Programs 1-10)                   */
/* ========================================================================== */

// Program 1: Swap without temporary variable using XOR
void prog01_xor_swap(int *a, int *b) {
    *a = *a ^ *b; *b = *a ^ *b; *a = *a ^ *b;
}

// Program 2: Check if number is a power of 2 using bitwise AND
int prog02_is_power_of_two(int n) {
    return (n > 0) && ((n & (n - 1)) == 0);
}

// Program 3: Count set bits (Kernighan's Algorithm)
int prog03_count_set_bits(int n) {
    int count = 0;
    while (n) { n &= (n - 1); count++; }
    return count;
}

// Program 4: Odd/Even using bitwise AND
int prog04_is_even(int n) { return !(n & 1); }

// Program 5: Find non-repeating element in array (XOR property)
int prog05_single_number(int arr[], int size) {
    int res = 0;
    for(int i=0; i<size; i++) res ^= arr[i];
    return res;
}

// Program 6: Multiply by 2 using left shift
int prog06_mul_by_two(int n) { return n << 1; }

// Program 7: Divide by 2 using right shift
int prog07_div_by_two(int n) { return n >> 1; }

// Program 8: Absolute value without branching
int prog08_abs(int n) {
    int mask = n >> (sizeof(int) * 8 - 1);
    return (n + mask) ^ mask;
}

// Program 9: Conditional execution without `if` using short-circuit logic
void prog09_conditional(int cond) {
    cond && printf("Condition met!\n");
}

// Program 10: Sign check without branching
int prog10_sign(int x) {
    return (x > 0) - (x < 0);
}

/* ========================================================================== */
/* SECTION 2: LOOPS, JUMPS & CONTROL FLOW (Programs 11-20)                   */
/* ========================================================================== */

// Program 11: Breaking nested loops via goto
void prog11_goto_nested_break(void) {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (i * j == 6) goto out;
        }
    }
    out: printf("Broke out of nested loops cleanly.\n");
}

// Program 12: Duff's Device (Loop unrolling)
void prog12_duffs_device(short *to, short *from, int count) {
    int n = (count + 7) / 8;
    switch (count % 8) {
        case 0: do { *to++ = *from++;
        case 7:      *to++ = *from++;
        case 6:      *to++ = *from++;
        case 5:      *to++ = *from++;
        case 4:      *to++ = *from++;
        case 3:      *to++ = *from++;
        case 2:      *to++ = *from++;
        case 1:      *to++ = *from++;
                } while (--n > 0);
    }
}

// Program 13: Infinite loop idioms (`for(;;)` vs `while(1)`)
void prog13_infinite_loops(void) {
    int c = 0;
    for (;;) { if (++c == 3) break; }
}

// Program 14: Loop with multiple loop variables
void prog14_multi_var_loop(void) {
    for (int i = 0, j = 10; i < j; i++, j--) {
        // Parallel convergence loop
    }
}

// Program 15: Executing code inside loop condition
void prog15_condition_side_effects(const char *str) {
    char ch;
    int i = 0;
    while ((ch = str[i++]) != '\0') { /* processing */ }
}

// Program 16: Exception handling simulation using `setjmp` and `longjmp`
jmp_buf buf;
void prog16_error_trigger(void) { longjmp(buf, 1); }
void prog16_exception_handling(void) {
    if (setjmp(buf) == 0) {
        prog16_error_trigger();
    } else {
        printf("Caught simulated exception via longjmp\n");
    }
}

// Program 17: Infinite recursion check (tail call induction)
void prog17_tail_recursion(int n) {
    if (n <= 0) return;
    prog17_tail_recursion(n - 1);
}

// Program 18: Sequence points and undefined behaviour prevention
void prog18_sequence_points(void) {
    int i = 1;
    i = i + 1; // Avoid i = i++ + ++i
    printf("Safe increment: %d\n", i);
}

// Program 19: Do-While(0) macro wrapping pattern
#define SAFE_MACRO(x) do { (x)++; } while(0)
void prog19_do_while_zero(void) {
    int val = 5;
    SAFE_MACRO(val);
}

// Program 20: Switch case fall-through with implicit logic
void prog20_switch_fallthrough(int code) {
    switch (code) {
        case 1:
        case 2: printf("Low severity\n"); break;
        case 3: printf("High severity\n"); break;
    }
}

/* ========================================================================== */
/* SECTION 3: POINTERS & MEMORY MANAGEMENT (Programs 21-30)                  */
/* ========================================================================== */

// Program 21: Void pointer casting and generic printing
void prog21_void_pointer(void *ptr, char type) {
    if (type == 'i') printf("Int: %d\n", *(int*)ptr);
    else if (type == 'f') printf("Float: %f\n", *(float*)ptr);
}

// Program 22: Pointer arithmetic (Array indexing via pointers)
void prog22_pointer_arithmetic(void) {
    int arr[] = {10, 20, 30};
    int *p = arr;
    printf("Second element: %d\n", *(p + 1));
}

// Program 23: Double pointer for dynamic allocation inside function
void prog23_allocate_buffer(int **buf, int size) {
    *buf = (int*)malloc(size * sizeof(int));
}

// Program 24: Function pointer table (Callback execution)
int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }
void prog24_function_pointers(void) {
    int (*ops[2])(int, int) = {add, sub};
    printf("Add: %d, Sub: %d\n", ops[0](5, 3), ops[1](5, 3));
}

// Program 25: Safe dynamic memory re-allocation (`realloc`)
void prog25_safe_realloc(void) {
    int *ptr = malloc(2 * sizeof(int));
    int *temp = realloc(ptr, 5 * sizeof(int));
    if (temp) ptr = temp;
    free(ptr);
}

// Program 26: `const` pointer vs pointer to `const`
void prog26_const_pointers(void) {
    int x = 10;
    const int *ptr_to_const = &x; // Data read-only
    int *const const_ptr = &x;    // Pointer immutable
    (void)ptr_to_const; (void)const_ptr;
}

// Program 27: Finding endianness at runtime
void prog27_check_endianness(void) {
    unsigned int x = 1;
    char *c = (char*)&x;
    if (*c) printf("Little-endian\n");
    else printf("Big-endian\n");
}

// Program 28: Dangling pointer protection pattern
void prog28_safe_free(void **pptr) {
    if (pptr && *pptr) {
        free(*pptr);
        *pptr = NULL;
    }
}

// Program 29: Memory copy implementation (`memcpy` low-level)
void prog29_custom_memcpy(void *dest, const void *src, size_t n) {
    char *d = (char*)dest;
    const char *s = (const char*)src;
    while (n--) *d++ = *s++;
}

// Program 30: Variable-Length Arrays (VLAs) stack safety check
void prog30_vla_example(int size) {
    if (size > 1000) return; // Prevent stack overflow
    int vla[size];
    vla[0] = 1;
}

/* ========================================================================== */
/* SECTION 4: STRUCTURES, UNIONS & ADVANCED TYPES (Programs 31-40)           */
/* ========================================================================== */

// Program 31: Bit-fields for flag representation
struct Prog31_Flags {
    unsigned int is_active : 1;
    unsigned int is_admin  : 1;
    unsigned int status    : 4;
};

// Program 32: Structure padding control (`#pragma pack`)
#pragma pack(push, 1)
struct Prog32_Packed {
    char c;
    int i;
};
#pragma pack(pop)

// Program 33: Flexible array member
struct Prog33_FlexArray {
    int length;
    int data[]; // Flexible member
};

// Program 34: Union for Type Punning (Float bit extraction)
union Prog34_FloatBitCast {
    float f;
    unsigned int i;
};

// Program 35: Self-referential structure (LinkedList Node)
typedef struct Prog35_Node {
    int data;
    struct Prog35_Node *next;
} Prog35_Node;

// Program 36: Anonymous structures and unions (C11 feature)
struct Prog36_Vector {
    union {
        struct { float x, y, z; };
        float raw[3];
    };
};

// Program 37: Opaque pointer pattern (Data Hiding)
typedef struct OpaqueType* OpaqueHandle;

// Program 38: Offsetof macro emulation
#define my_offsetof(TYPE, MEMBER) ((size_t)&((TYPE *)0)->MEMBER)

// Program 39: Container_of macro calculation
#define container_of(ptr, type, member) \
    ((type *)((char *)(ptr) - my_offsetof(type, member)))

// Program 40: Enum with custom bit-mask scaling
typedef enum {
    FLAG_A = 1 << 0,
    FLAG_B = 1 << 1,
    FLAG_C = 1 << 2
} BitFlags;

/* ========================================================================== */
/* SECTION 5: PREPROCESSOR, STRINGS & SYSTEM CONCEPTS (Programs 41-50)       */
/* ========================================================================== */

// Program 41: Stringification macro (`#`)
#define TO_STR(x) #x

// Program 42: Token concatenating macro (`##`)
#define DECLARE_VAR(type, name) type var_##name

// Program 43: Variadic macros (`__VA_ARGS__`)
#define LOG_ERR(...) fprintf(stderr, __VA_ARGS__)

// Program 44: Variadic function using `stdarg.h`
double prog44_average(int count, ...) {
    va_list args;
    va_start(args, count);
    double sum = 0;
    for (int i = 0; i < count; i++) sum += va_arg(args, int);
    va_end(args);
    return sum / count;
}

// Program 45: `volatile` variable usage for hardware/signal modification
static volatile int sig_flag = 0;

// Program 46: `static` variable scope inside functions (State retention)
int prog46_counter(void) {
    static int count = 0;
    return ++count;
}

// Program 47: String tokenization using `strtok` safely
void prog47_tokenize(char *str) {
    char *token = strtok(str, ",");
    while (token != NULL) {
        token = strtok(NULL, ",");
    }
}

// Program 48: Compile-time assertions using static_assert (C11)
_Static_assert(sizeof(int) >= 4, "Integer size too small");

// Program 49: Restrict keyword for compiler optimization hints
void prog49_vector_add(int * restrict a, int * restrict b, int * restrict c, int n) {
    for (int i = 0; i < n; i++) a[i] = b[i] + c[i];
}

// Program 50: Memory alignment allocation (`aligned_alloc`)
void prog50_aligned_mem(void) {
    int *ptr = (int*)aligned_alloc(64, 1024 * sizeof(int));
    free(ptr);
}

int main(void) {
    printf("All 50 C Programs Compiled Successfully.\n");
    return 0;
}
