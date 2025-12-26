# Chest

A lightweight, **header-only** C library that provides **type aliases**, **generic data structures** and **useful functions/macros** especially for algorithmic puzzles and competitive programming.

This library uses *GNU extensions* for the macros, so be sure to compile appropriately. The code is half vibe coded, special thanks to ChatGPT, Gemini and Claude for the help:)

## Type Aliases

Each type name is a letter that represents the type followed by the size of the type in bits.

- Signed integers: `i8, i16, i32, i64`
- Unsigned integers: `u8, u16, u32, u64`
- Floating point numbers: `f32, f64`
- Characters and strings: `c8, cstr`
- Booleans and void: `b8, u0`

## Useful Macros

- `MIN, MAX, CLAMP`
- `ABS, SIGN`
- `MAP, LERP`
- `POW, LOG`

- `BIT, SET_BIT, CLEAR_BIT, TOGGLE_BIT, CHECK_BIT`

- `ALIGN_UP_POW2, ALIGN_DOWN_POW2, IS_ALIGNED_POW2`

- `KiB, MiB, GiB`

- `ARRAY_LEN, SWAP`

## Data Structures

- `Dynamic Array (DA)`
- `Linked List (LL)`
- `Hash Map (HM)`

## Examples

### Dynamic Array

#### Integer Array

```c
int main(void)
{
    DA_u16 numbers;
    da_init(&numbers);
    da_reserve(&numbers, 10);

    for (u16 i = 1; i <= 10; i++)
        da_push(&numbers, (i * 23) % 13);

    for (u64 i = 0; i < da_len(&numbers); i++)
        for (u64 j = i + 1; j < da_len(&numbers); j++)
            if (da_get(&numbers, i) > da_get(&numbers, j))
                da_swap(&numbers, i, j);

    da_foreach(&numbers, num)
        printf("%u\n", *num);

    da_free(&numbers);
}
```

### Linked List

#### Float List
```c
int main(void)
{
    LL(f32) list = {0};
    ll_init(&list);

    ll_push_back(&list, 3.14f);
    ll_push_front(&list, 2.71f);

    f32 val = ll_get(&list, 0);
    printf("%g\n", val);

    ll_free(&list);
}
```

#### Custom Struct List

```c
typedef struct {
    i32 x, y;
} Point;

typedef LL(Point) LL_Point;

int main(void)
{
    LL_Point points = {0};
    ll_init(&points);

    for (i32 y = 0; y < 256; y++)
    {
        for (i32 x = 0; x < 256; x++)
        {
            Point p = {x, y};
            ll_push_back(&points, p);
        }
    }

    Point p = ll_front(&points);
    printf("(%d, %d)\n", p.x, p.y);

    p = ll_pop_back(&points);

    ll_foreach(&points, it)
    {
        if (it->x == 0)
            printf("(%d, %d)\n", it->x, it->y);
    }

    ll_free(&points);
}
```

### Hash Map

#### String Key

```c
HM_GENERATE_MAP_TYPE(cstr, u64);
HM_GENERATE_CSTR_FUNCS();

int main(void)
{
    HM_cstr_u64 map = {0};
    hm_init(&map, hm_hash_cstr, hm_equal_cstr);

    for (u64 i = 0; i < 10; i++)
    {
        char buffer[32];
        snprintf(buffer, sizeof(buffer), "key_%llu", i);
        cstr key = strdup(buffer);

        hm_put(&map, key, i * 10);
    }

    hm_remove(&map, "key_2");

    HM_Iter_cstr_u64 it;
    hm_iterate(&map, it)
    {
        printf("%s: %llu\n", it->key, it->value);
        free((void *)it->key);
    }

    hm_free(&map);
}
```

#### Custom Struct Key (byte hashing)

```c
typedef struct {
    i32 x, y;
} Point;

HM_GENERATE_MAP_TYPE(Point, i32);
HM_GENERATE_STRUCT_FUNCS(Point)

int main(void)
{
    HM_Point_i32 map = {0};
    hm_init(&map, hm_hash_Point, hm_equal_Point);

    for (u64 i = 0; i < 10; i++)
    {
        Point key = {i, i};
        hm_put(&map, key, i * 10);
    }

    Point key = {5, 5};
    hm_put(&map, key, 100);

    key = (Point){3, 3};
    hm_remove(&map, key);

    HM_Iter_Point_i32 it;
    hm_iterate(&map, it)
    {
        printf("(%d, %d): %d\n", it->key.x, it->key.y, it->value);
    }

    hm_free(&map);
}
```

#### Custom Struct Key (custom hashing)

```c
typedef struct {
    f32 real, imag;
} Complex;

HM_GENERATE_MAP_TYPE(Complex, i32);

u64 hm_hash_Complex(const void *key)
{
    const Complex *c = (const Complex *)key;
    u64 h1 = (*(u32 *)&c->real) * 2654435761u;
    u64 h2 = (*(u32 *)&c->imag) * 2654435761u;
    return h1 ^ (h2 << 1);
}
bool hm_equal_Complex(const void *a, const void *b)
{
    const Complex *c1 = (const Complex *)a;
    const Complex *c2 = (const Complex *)b;
    return c1->real == c2->real && c1->imag == c2->imag;
}

int main(void)
{
    HM_Complex_i32 map = {0};
    hm_init(&map, hm_hash_Complex, hm_equal_Complex);

    for (u64 i = 0; i < 10; i++)
    {
        Complex key = {i, i};
        hm_put(&map, key, i * 10);
    }

    Complex key = {0.0f, 0.0f};
    hm_put(&map, key, 100);

    HM_Iter_Complex_i32 it;
    hm_iterate(&map, it)
    {
        printf("(%g, %g): %d\n", it->key.real, it->key.imag, it->value);
    }

    hm_free(&map);
}
```

## How To Use

### Usage

Since the whole library is just one header, simply include `chest.h` in every file where you need it.

If you wish to use the data structures, you need to define the appropriate macros right before including `chest.h`. For example, you need `CHEST_ENABLE_DA` for dynamic arrays.

Lastly, you need to define `CHEST_IMPLEMENTATION` in one of your source files right above the line `#include "chest.h"`, typically in the main file.

**One example:**

```c
//model.h:
#define CHEST_ENABLE_HM
#include "chest.h"

// main.c:
#include "model.h"

#define CHEST_IMPLEMENTATION
#include "chest.h"
```

**Pro tip:**

If you use visual studio code, you will see many red underlines all over the macros. To disable them, you need to tell vscode that you intentionally use GNU extensions:

Place a file called `c_cpp_properties.json` into the `.vscode` folder with the following content:

```json
{
    "configurations": [
        {
            "name": "Win32",
            "includePath": [
                "${workspaceFolder}/**"
            ],
            "compilerPath": "C:\\w64devkit\\bin\\gcc.exe",
            "cStandard": "gnu11",
            "intelliSenseMode": "windows-gcc-x64"
        }
    ],
    "version": 4
}
```

Be sure to set the **configuration name** (`name`), **compiler path** (`compilerPath`) and **intellisense mode** (`intelliSenseMode`).
Possible configuration names: `Win32, Linux, Mac`
Intellisense mode follows the form `[platform name]-[compiler]-[architecture]`.

### Build

Just simply build your project as you would, though you will not be able to use flags like `-Wpedantic`, since the library uses GNU extensions.