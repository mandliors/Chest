/**
 * chest.h
 *
 * A lightweight C utility header providing type aliases, useful functions/macros, and implementations for generic data types.
 * Requires GCC/Clang (GNU extensions)ű
 *
 */

#ifndef CHEST_H
#define CHEST_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

// ----------------- TYPE ALIASES -----------------
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

typedef char c8;
typedef const char *cstr;

typedef bool b8;
typedef void u0;

// -------------------- MACROS --------------------
// math
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define CLAMP(x, lo, hi) (MAX((lo), MIN((x), (hi))))

#define ABS(x) (((x) < 0) ? -(x) : (x))
#define SIGN(x) (((x) > 0) - ((x) < 0))

#define MAP(x, in_min, in_max, out_min, out_max) \
    ((out_min) + ((x) - (in_min)) * ((out_max) - (out_min)) / ((in_max) - (in_min)))
#define LERP(a, b, t) ((a) + (t) * ((b) - (a)))

#define POW(x, n) ({             \
    u64 _i;                      \
    i64 _res = 1;                \
    for (_i = 0; _i < (n); _i++) \
        _res *= (x);             \
    _res;                        \
})
#define LOG(x, base) (log(x) / log(base))

// bit manipulation
#define BIT(n) (1ULL << (n))
#define SET_BIT(x, n) ((x) |= BIT(n))
#define CLEAR_BIT(x, n) ((x) &= ~BIT(n))
#define TOGGLE_BIT(x, n) ((x) ^= BIT(n))
#define CHECK_BIT(x, n) (((x) & BIT(n)) != 0)

// alignment
#define ALIGN_UP_POW2(n, p) (((n) + (p) - 1) & ~((p) - 1))
#define ALIGN_DOWN_POW2(n, p) ((n) & ~((p) - 1))
#define IS_ALIGNED_POW2(n, p) (((n) & ((p) - 1)) == 0)

// size units
#define KiB(n) ((u64)(n) << 10)
#define MiB(n) ((u64)(n) << 20)
#define GiB(n) ((u64)(n) << 30)

// other
#define ARRAY_LEN(arr) (sizeof(arr) / sizeof((arr)[0]))
#define SWAP(T, a, b) \
    do                \
    {                 \
        T tmp = (a);  \
        (a) = (b);    \
        (b) = tmp;    \
    } while (0)

// ----------------- DATA TYPES -----------------

#ifdef CHEST_ENABLE_DA
typedef struct
{
    void *data;
    u64 len;
    u64 cap;
} DA_Generic;

void _da_init(void *da);
bool _da_reserve(void *da, u64 item_size, u64 new_cap);
bool _da_resize(void *da, u64 item_size, u64 new_len);
bool _da_push(void *da, u64 item_size, void *item);
bool _da_insert(void *da, u64 item_size, u64 index, void *item);
bool _da_remove_at(void *da, u64 item_size, u64 index);
void _da_free(void *da);

#define DA(T)    \
    struct       \
    {            \
        T *data; \
        u64 len; \
        u64 cap; \
    }

#define da_init(da) _da_init((da))
#define da_reserve(da, n) _da_reserve((da), sizeof(*(da)->data), (n))
#define da_resize(da, new_len) _da_resize((da), sizeof(*(da)->data), (new_len))
#define da_len(da) ((da)->len)
#define da_cap(da) ((da)->cap)
#define da_get(da, i) ((da)->data[i])
#define da_set(da, i, value) ((da)->data[i] = (value))
#define da_at(da, i) (&(da)->data[i])
#define da_front(da) ((da)->data[0])
#define da_back(da) ((da)->data[(da)->len - 1])
#define da_is_empty(da) ((da)->len == 0)
#define da_clear(da) ((da)->len = 0)
#define da_index_of(da, value) ({          \
    i64 _result = -1;                      \
    for (u64 _i = 0; _i < (da)->len; _i++) \
    {                                      \
        if ((da)->data[_i] == (value))     \
        {                                  \
            _result = (i64)_i;             \
            break;                         \
        }                                  \
    }                                      \
    _result;                               \
})
#define da_contains(da, value) (da_index_of((da), (value)) != -1)
#define da_swap(da, i, j)                         \
    do                                            \
    {                                             \
        typeof(*(da)->data) _tmp = (da)->data[i]; \
        (da)->data[i] = (da)->data[j];            \
        (da)->data[j] = _tmp;                     \
    } while (0)
#define da_push(da, item) ({                     \
    typeof(item) _item = (item);                 \
    _da_push((da), sizeof(*(da)->data), &_item); \
})
#define da_pop(da) ((da)->len > 0 ? (da)->data[--(da)->len] : (typeof(*(da)->data)){0})
#define da_insert(da, i, item) ({                       \
    typeof(item) _item = (item);                        \
    _da_insert((da), sizeof(*(da)->data), (i), &_item); \
})
#define da_remove_at(da, i) _da_remove_at((da), sizeof(*(da)->data), (i))
#define da_free(da) _da_free(da)
#define da_foreach(da, var)                     \
    for (typeof(*(da)->data) *var = (da)->data; \
         var < (da)->data + (da)->len;          \
         var++)

typedef DA(i8) DA_i8;
typedef DA(i16) DA_i16;
typedef DA(i32) DA_i32;
typedef DA(i64) DA_i64;

typedef DA(u8) DA_u8;
typedef DA(u16) DA_u16;
typedef DA(u32) DA_u32;
typedef DA(u64) DA_u64;

typedef DA(f32) DA_f32;
typedef DA(f64) DA_f64;

typedef DA(c8) DA_c8;
typedef DA(cstr) DA_cstr;

typedef DA(b8) DA_b8;

#endif // CHEST_ENABLE_DA

#ifdef CHEST_ENABLE_LL

typedef struct
{
    void *head;
    void *tail;
    u64 len;
} LL_Generic;

#define LL_NODE(T)  \
    struct          \
    {               \
        T data;     \
        void *next; \
    }

#define LL(T)              \
    struct                 \
    {                      \
        LL_NODE(T) * head; \
        LL_NODE(T) * tail; \
        u64 len;           \
    }

void _ll_init(void *ll);
bool _ll_push_front(void *ll, size_t node_size, size_t data_offset, size_t next_offset, size_t item_size, void *item);
bool _ll_push_back(void *ll, size_t node_size, size_t data_offset, size_t next_offset, size_t item_size, void *item);
void *_ll_get(void *ll, size_t next_offset, size_t index);
bool _ll_pop_front(void *ll, size_t data_offset, size_t next_offset, size_t item_size, void *out);
bool _ll_remove_at(void *ll, size_t data_offset, size_t next_offset, size_t item_size, size_t index);
void _ll_free(void *ll, size_t next_offset);

#define ll_init(ll) _ll_init((ll))
#define ll_len(ll) ((ll)->len)
#define ll_is_empty(ll) ((ll)->len == 0)
#define ll_push_front(ll, item) ({                                                 \
    typeof(item) _item = (item);                                                   \
    _ll_push_front((ll), sizeof(*(ll)->head), offsetof(typeof(*(ll)->head), data), \
                   offsetof(typeof(*(ll)->head), next), sizeof(_item), &_item);    \
})
#define ll_push_back(ll, item) ({                                                 \
    typeof(item) _item = (item);                                                  \
    _ll_push_back((ll), sizeof(*(ll)->head), offsetof(typeof(*(ll)->head), data), \
                  offsetof(typeof(*(ll)->head), next), sizeof(_item), &_item);    \
})
#define ll_get(ll, i) ({                                                                        \
    void *_n = _ll_get((ll), offsetof(typeof(*(ll)->head), next), (i));                         \
    _n ? *(typeof(&(ll)->head->data))((void *)((u8 *)_n + offsetof(typeof(*(ll)->head), data))) \
       : (typeof((ll)->head->data)){0};                                                         \
})
#define ll_front(ll) ({                                                                                    \
    _ll_get((ll), offsetof(typeof(*(ll)->head), next), 0)                                                  \
        ? *(typeof(&(ll)->head->data))((void *)((u8 *)((ll)->head) + offsetof(typeof(*(ll)->head), data))) \
        : (typeof((ll)->head->data)){0};                                                                   \
})
#define ll_back(ll) ({                                                                                                                                          \
    _ll_get((ll), offsetof(typeof(*(ll)->head), next), (ll)->len - 1)                                                                                           \
        ? *(typeof(&(ll)->head->data))((void *)((u8 *)_ll_get((ll), offsetof(typeof(*(ll)->head), next), (ll)->len - 1) + offsetof(typeof(*(ll)->head), data))) \
        : (typeof((ll)->head->data)){0};                                                                                                                        \
})
#define ll_pop_front(ll) ({                                                                                         \
    typeof((ll)->head->data) _r = {0};                                                                              \
    _ll_pop_front((ll), offsetof(typeof(*(ll)->head), data), offsetof(typeof(*(ll)->head), next), sizeof(_r), &_r); \
    _r;                                                                                                             \
})
#define ll_pop_back(ll) ({                                 \
    typeof((ll)->head->data) _r = {0};                     \
    if (ll_len(ll) > 0)                                    \
    {                                                      \
        _r = ll_get((ll), ll_len(ll) - 1);                 \
        _ll_remove_at((ll),                                \
                      offsetof(typeof(*(ll)->head), data), \
                      offsetof(typeof(*(ll)->head), next), \
                      sizeof(_r),                          \
                      ll_len(ll) - 1);                     \
    }                                                      \
    _r;                                                    \
})
#define ll_remove_at(ll, i)                            \
    _ll_remove_at((ll),                                \
                  offsetof(typeof(*(ll)->head), data), \
                  offsetof(typeof(*(ll)->head), next), \
                  sizeof((ll)->head->data),            \
                  (i))
#define ll_free(ll) \
    _ll_free((ll), offsetof(typeof(*(ll)->head), next))
#define ll_foreach(ll, var)                     \
    for (typeof((ll)->head) _node = (ll)->head; \
         _node != NULL;                         \
         _node = _node->next)                   \
        for (typeof(&_node->data) var = &_node->data; var != NULL; var = NULL)

typedef LL(i8) LL_i8;
typedef LL(i16) LL_i16;
typedef LL(i32) LL_i32;
typedef LL(i64) LL_i64;

typedef LL(u8) LL_u8;
typedef LL(u16) LL_u16;
typedef LL(u32) LL_u32;
typedef LL(u64) LL_u64;

typedef LL(f32) LL_f32;
typedef LL(f64) LL_f64;

typedef LL(c8) LL_c8;
typedef LL(cstr) LL_cstr;

typedef LL(b8) LL_b8;

#endif // CHEST_ENABLE_LL

#ifdef CHEST_ENABLE_HM

typedef u8 HM_State;
typedef u64 (*HM_HashFn)(const void *key);
typedef bool (*HM_EqualFn)(const void *a, const void *b);

enum
{
    HM_EMPTY,
    HM_FILLED,
    HM_TOMBSTONE
};

typedef struct
{
    u64 cap;
    u64 len;
    HM_HashFn hash_fn;
    HM_EqualFn equal_fn;
} HM_Generic;

#define HM_ENTRY(K, V)  \
    struct              \
    {                   \
        K key;          \
        V value;        \
        HM_State state; \
    }
#define HM(K, V)               \
    struct                     \
    {                          \
        HM_Generic g;          \
        HM_ENTRY(K, V) * data; \
    }
#define HM_Iter(map) typeof((map).data)

static u64 hm_probe(HM_Generic *hm, void *data, size_t entry_size, size_t key_offset, size_t state_offset, u64 hash, void *key, bool *found);
static bool hm_grow(HM_Generic *hm, void **pdata, size_t entry_size, size_t key_offset, size_t val_offset, size_t state_offset);

bool _hm_init(void *hm, void **pdata, size_t entry_size, HM_HashFn hash_fn, HM_EqualFn equal_fn);
bool _hm_reserve(void *hm, void **pdata, size_t entry_size, size_t key_offset, size_t val_offset, size_t state_offset, u64 new_cap);
void *_hm_get(void *hm, void *data, size_t entry_size, size_t key_offset, size_t val_offset, size_t state_offset, u64 hash, void *key);
bool _hm_put(void *hm, void **pdata, size_t entry_size, size_t key_offset, size_t key_size, size_t val_offset, size_t val_size, size_t state_offset, u64 hash, void *key, void *val);
bool _hm_remove(void *hm, void *data, size_t entry_size, size_t key_offset, size_t state_offset, u64 hash, void *key);
void _hm_clear(void *hm, void *data, size_t entry_size, size_t state_offset);
void _hm_free(void *hm, void **pdata);

#define HM_TYPE(K, V) HM_##K##_##V
#define HM_GENERATE_MAP_TYPE(K, V)  \
    typedef HM(K, V) HM_TYPE(K, V); \
    typedef typeof(((HM_TYPE(K, V) *)0)->data) HM_Iter_##K##_##V

#define HM_GENERATE_SCALAR_FUNCS(T)                               \
    static inline u64 hm_hash_##T(const void *k)                  \
    {                                                             \
        u64 x = (u64) * (const T *)k;                             \
        x ^= x >> 33;                                             \
        x *= 0xff51afd7ed558ccdULL;                               \
        x ^= x >> 33;                                             \
        x *= 0xc4ceb9fe1a85ec53ULL;                               \
        x ^= x >> 33;                                             \
        return x;                                                 \
    }                                                             \
    static inline bool hm_equal_##T(const void *a, const void *b) \
    {                                                             \
        return *(const T *)a == *(const T *)b;                    \
    }
#define HM_GENERATE_CSTR_FUNCS()                                   \
    static inline u64 hm_hash_cstr(const void *key)                \
    {                                                              \
        const u8 *s = *(u8 *const *)key;                           \
        u64 h = 1469598103934665603ULL;                            \
        while (*s)                                                 \
            h = (h ^ (u8) * s++) * 1099511628211ULL;               \
        return h;                                                  \
    }                                                              \
    static inline bool hm_equal_cstr(const void *a, const void *b) \
    {                                                              \
        return strcmp(*(char *const *)a, *(char *const *)b) == 0;  \
    }
#define HM_GENERATE_STRUCT_FUNCS(T)                               \
    static inline u64 hm_hash_##T(const void *k)                  \
    {                                                             \
        const u8 *p = (u8 *)k;                                    \
        u64 size = sizeof(T);                                     \
        u64 h = 1469598103934665603ULL;                           \
        for (size_t i = 0; i < size; i++)                         \
            h = (h ^ p[i]) * 1099511628211ULL;                    \
        return h;                                                 \
    }                                                             \
    static inline bool hm_equal_##T(const void *a, const void *b) \
    {                                                             \
        return memcmp(a, b, sizeof(T)) == 0;                      \
    }

#define hm_init(hm, hash, eq) \
    _hm_init(&(hm)->g, (void **)&(hm)->data, sizeof((hm)->data[0]), (hash), (eq))
#define hm_count(hm) ((hm)->g.len)
#define hm_reserve(hm, n) \
    _hm_reserve(&(hm)->g, (void **)&(hm)->data, sizeof(*(hm)->data), offsetof(typeof(*(hm)->data), key), offsetof(typeof(*(hm)->data), value), offsetof(typeof(*(hm)->data), state), (n))
#define hm_get(hm, k) ({                                        \
    typeof((hm)->data[0].key) _hm_temp_key = (k);               \
    (typeof(&(hm)->data[0].value))                              \
        _hm_get(&(hm)->g, (hm)->data, sizeof(*(hm)->data),      \
                offsetof(typeof(*(hm)->data), key),             \
                offsetof(typeof(*(hm)->data), value),           \
                offsetof(typeof(*(hm)->data), state),           \
                (hm)->g.hash_fn(&_hm_temp_key), &_hm_temp_key); \
})
#define hm_contains(hm, k) (hm_get(hm, k) != NULL)
#define hm_put(hm, k, v) ({                       \
    typeof((hm)->data[0].key) _k = (k);           \
    typeof((hm)->data[0].value) _v = (v);         \
    _hm_put(&(hm)->g, (void **)&(hm)->data,       \
            sizeof(*(hm)->data),                  \
            offsetof(typeof(*(hm)->data), key),   \
            sizeof(_k),                           \
            offsetof(typeof(*(hm)->data), value), \
            sizeof(_v),                           \
            offsetof(typeof(*(hm)->data), state), \
            (hm)->g.hash_fn(&_k), &_k, &_v);      \
})
#define hm_remove(hm, k) ({                                    \
    typeof((hm)->data[0].key) _hm_temp_key = (k);              \
    _hm_remove(&(hm)->g, (hm)->data, sizeof(*(hm)->data),      \
               offsetof(typeof(*(hm)->data), key),             \
               offsetof(typeof(*(hm)->data), state),           \
               (hm)->g.hash_fn(&_hm_temp_key), &_hm_temp_key); \
})
#define hm_clear(hm)                                     \
    _hm_clear(&(hm)->g, (hm)->data, sizeof(*(hm)->data), \
              offsetof(typeof(*(hm)->data), state))
#define hm_free(hm) \
    _hm_free(&(hm)->g, (void **)&(hm)->data)
#define hm_iterate(hm, it)              \
    for (u64 _i = 0;                    \
         _i < (hm)->g.cap &&            \
         (((it) = &(hm)->data[_i]), 1); \
         _i++)                          \
        if ((it)->state == HM_FILLED)

#endif // CHEST_ENABLE_HM

#ifdef CHEST_IMPLEMENTATION

#ifdef CHEST_ENABLE_DA

void _da_init(void *da)
{
    DA_Generic *dag = (DA_Generic *)da;

    dag->data = NULL;
    dag->len = 0;
    dag->cap = 0;
}
bool _da_reserve(void *da, u64 item_size, u64 new_cap)
{
    DA_Generic *dag = (DA_Generic *)da;
    if (new_cap <= dag->cap)
        return true;

    void *new_data = realloc(dag->data, new_cap * item_size);
    if (!new_data)
        return false;

    dag->data = new_data;
    dag->cap = new_cap;

    return true;
}
bool _da_resize(void *da, u64 item_size, u64 new_len)
{
    DA_Generic *dag = (DA_Generic *)da;

    if (new_len > dag->cap)
        if (!_da_reserve(da, item_size, new_len))
            return false;

    if (new_len > dag->len)
    {
        void *start = (u8 *)dag->data + (dag->len * item_size);
        memset(start, 0, (new_len - dag->len) * item_size);
    }
    dag->len = new_len;

    return true;
}
bool _da_push(void *da, u64 item_size, void *item)
{
    DA_Generic *dag = (DA_Generic *)da;

    if (dag->len >= dag->cap)
    {
        u64 new_cap = dag->cap == 0 ? 8 : dag->cap * 2;
        if (!_da_reserve(da, item_size, new_cap))
            return false;
    }

    void *dest = (u8 *)dag->data + (dag->len * item_size);
    memcpy(dest, item, item_size);
    dag->len++;

    return true;
}
bool _da_insert(void *da, u64 item_size, u64 index, void *item)
{
    DA_Generic *dag = (DA_Generic *)da;

    if (index > dag->len)
        return false;

    if (dag->len >= dag->cap)
    {
        u64 new_cap = dag->cap == 0 ? 8 : dag->cap * 2;
        if (!_da_reserve(da, item_size, new_cap))
            return false;
    }

    if (index < dag->len)
    {
        void *src = (u8 *)dag->data + (index * item_size);
        void *dest = (u8 *)src + item_size;
        memmove(dest, src, (dag->len - index) * item_size);
    }

    void *insert_pos = (u8 *)dag->data + (index * item_size);
    memcpy(insert_pos, item, item_size);
    dag->len++;

    return true;
}
bool _da_remove_at(void *da, u64 item_size, u64 index)
{
    DA_Generic *dag = (DA_Generic *)da;

    if (index >= dag->len)
        return false;

    if (index < dag->len - 1)
    {
        void *dest = (u8 *)dag->data + (index * item_size);
        void *src = (u8 *)dest + item_size;
        memmove(dest, src, (dag->len - index - 1) * item_size);
    }

    dag->len--;
    return true;
}
void _da_free(void *da)
{
    DA_Generic *dag = (DA_Generic *)da;

    free(dag->data);
    dag->data = NULL;
    dag->len = 0;
    dag->cap = 0;
}

#endif // CHEST_ENABLE_DA

#ifdef CHEST_ENABLE_LL

void _ll_init(void *ll)
{
    LL_Generic *llg = (LL_Generic *)ll;
    llg->head = NULL;
    llg->tail = NULL;
    llg->len = 0;
}
bool _ll_push_front(void *ll, size_t node_size, size_t data_offset, size_t next_offset, size_t item_size, void *item)
{
    LL_Generic *llg = (LL_Generic *)ll;

    void *node = malloc(node_size);
    if (!node)
        return false;

    memcpy((u8 *)node + data_offset, item, item_size);
    *(void **)((u8 *)node + next_offset) = llg->head;

    llg->head = node;
    if (!llg->tail)
        llg->tail = node;

    llg->len++;
    return true;
}
bool _ll_push_back(void *ll, size_t node_size, size_t data_offset, size_t next_offset, size_t item_size, void *item)
{
    LL_Generic *llg = (LL_Generic *)ll;

    void *node = malloc(node_size);
    if (!node)
        return false;

    memcpy((u8 *)node + data_offset, item, item_size);
    *(void **)((u8 *)node + next_offset) = NULL;

    if (llg->tail)
        *(void **)((u8 *)llg->tail + next_offset) = node;
    else
        llg->head = node;

    llg->tail = node;
    llg->len++;
    return true;
}
void *_ll_get(void *ll, size_t next_offset, size_t index)
{
    LL_Generic *llg = (LL_Generic *)ll;
    if (index >= llg->len)
        return NULL;

    void *cur = llg->head;
    for (size_t i = 0; i < index; i++)
        cur = *(void **)((u8 *)cur + next_offset);

    return cur;
}
bool _ll_pop_front(void *ll, size_t data_offset, size_t next_offset, size_t item_size, void *out)
{
    LL_Generic *llg = (LL_Generic *)ll;
    if (!llg->head)
        return false;

    void *node = llg->head;
    if (out)
        memcpy(out, (u8 *)node + data_offset, item_size);

    llg->head = *(void **)((u8 *)node + next_offset);
    if (!llg->head)
        llg->tail = NULL;

    free(node);
    llg->len--;
    return true;
}
bool _ll_remove_at(void *ll, size_t data_offset, size_t next_offset, size_t item_size, size_t index)
{
    LL_Generic *llg = (LL_Generic *)ll;
    if (index >= llg->len)
        return false;

    if (index == 0)
        return _ll_pop_front(ll, data_offset, next_offset, item_size, NULL);

    void *prev = llg->head;
    for (size_t i = 0; i < index - 1; i++)
        prev = *(void **)((u8 *)prev + next_offset);

    void *to_remove = *(void **)((u8 *)prev + next_offset);
    *(void **)((u8 *)prev + next_offset) =
        *(void **)((u8 *)to_remove + next_offset);

    if (to_remove == llg->tail)
        llg->tail = prev;

    free(to_remove);
    llg->len--;
    return true;
}
void _ll_free(void *ll, size_t next_offset)
{
    LL_Generic *llg = (LL_Generic *)ll;

    void *cur = llg->head;
    while (cur)
    {
        void *next = *(void **)((u8 *)cur + next_offset);
        free(cur);
        cur = next;
    }

    llg->head = NULL;
    llg->tail = NULL;
    llg->len = 0;
}

#endif // CHEST_ENABLE_LL

#ifdef CHEST_ENABLE_HM

static u64 hm_probe(HM_Generic *hm, void *data, size_t entry_size, size_t key_offset, size_t state_offset, u64 hash, void *key, bool *found)
{
    if (!hm || !data || hm->cap == 0)
    {
        *found = false;
        return UINT64_MAX;
    }

    u64 mask = hm->cap - 1;
    u64 idx = hash & mask;
    u64 first_tombstone = UINT64_MAX;

    for (u64 probes = 0; probes < hm->cap; probes++)
    {
        u8 *entry = (u8 *)data + idx * entry_size;
        HM_State *state = (HM_State *)(entry + state_offset);

        if (*state == HM_EMPTY)
        {
            *found = false;
            return (first_tombstone != UINT64_MAX) ? first_tombstone : idx;
        }

        if (*state == HM_TOMBSTONE)
        {
            if (first_tombstone == UINT64_MAX)
                first_tombstone = idx;
        }
        else
        {
            void *entry_key = entry + key_offset;
            if (hm->equal_fn(entry_key, key))
            {
                *found = true;
                return idx;
            }
        }

        idx = (idx + 1) & mask;
    }
    *found = false;

    return first_tombstone;
}
static bool hm_grow(HM_Generic *hm, void **pdata, size_t entry_size, size_t key_offset, size_t val_offset, size_t state_offset)
{
    u64 new_cap = hm->cap != 0 ? hm->cap * 2 : 8;
    return _hm_reserve(hm, pdata, entry_size, key_offset, val_offset, state_offset, new_cap);
}
bool _hm_init(void *hm, void **pdata, size_t entry_size, HM_HashFn hash_fn, HM_EqualFn equal_fn)
{
    HM_Generic *hmg = hm;

    hmg->cap = 8;
    hmg->len = 0;
    hmg->hash_fn = hash_fn;
    hmg->equal_fn = equal_fn;

    void *dat = calloc(hmg->cap, entry_size);
    if (!dat)
        return false;

    *pdata = dat;

    return true;
}
bool _hm_reserve(void *hm, void **pdata, size_t entry_size, size_t key_offset, size_t val_offset, size_t state_offset, u64 new_cap)
{
    (void)val_offset;
    HM_Generic *hmg = (HM_Generic *)hm;

    if (new_cap <= hmg->cap)
        return true;

    u64 cap = 1;
    while (cap < new_cap)
        cap <<= 1;

    u64 old_cap = hmg->cap;
    void *old_data = *pdata;

    void *new_data = calloc(cap, entry_size);
    if (!new_data)
        return false;

    *pdata = new_data;
    hmg->cap = cap;
    hmg->len = 0;

    for (u64 i = 0; i < old_cap; i++)
    {
        u8 *entry = (u8 *)old_data + i * entry_size;
        HM_State *state = (HM_State *)(entry + state_offset);

        if (*state != HM_FILLED)
            continue;

        void *key = entry + key_offset;
        u64 hash = hmg->hash_fn(key);

        bool found;
        u64 idx = hm_probe(hmg, *pdata, entry_size, key_offset, state_offset, hash, key, &found);
        u8 *dst = (u8 *)*pdata + idx * entry_size;

        memcpy(dst, entry, entry_size);
        *(HM_State *)(dst + state_offset) = HM_FILLED;
        hmg->len++;
    }
    free(old_data);

    return true;
}
void *_hm_get(void *hm, void *data, size_t entry_size, size_t key_offset, size_t val_offset, size_t state_offset, u64 hash, void *key)
{
    HM_Generic *hmg = hm;

    if (!data)
        return NULL;

    bool found;
    u64 idx = hm_probe(hmg, data, entry_size, key_offset, state_offset, hash, key, &found);

    if (!found)
        return NULL;

    u8 *entry = (u8 *)data + idx * entry_size;
    return entry + val_offset;
}
bool _hm_put(void *hm, void **pdata, size_t entry_size, size_t key_offset, size_t key_size, size_t val_offset, size_t val_size, size_t state_offset, u64 hash, void *key, void *val)
{
    HM_Generic *hmg = hm;

    if (*pdata == NULL)
        if (!hm_grow(hmg, pdata, entry_size, key_offset, val_offset, state_offset))
            return false;

    bool found;
    u64 idx = hm_probe(hmg, *pdata, entry_size, key_offset, state_offset, hash, key, &found);

    if (!found && (hmg->len + 1) * 4 >= hmg->cap * 3)
    {
        if (!hm_grow(hmg, pdata, entry_size, key_offset, val_offset, state_offset))
            return false;

        idx = hm_probe(hmg, *pdata, entry_size, key_offset, state_offset, hash, key, &found);
    }

    if (idx == UINT64_MAX)
        return false;

    u8 *entry = (u8 *)*pdata + idx * entry_size;
    HM_State *state = (HM_State *)(entry + state_offset);

    if (!found)
    {
        memcpy(entry + key_offset, key, key_size);
        hmg->len++;
        *state = HM_FILLED;
    }

    memcpy(entry + val_offset, val, val_size);

    return true;
}
bool _hm_remove(void *hm, void *data, size_t entry_size, size_t key_offset, size_t state_offset, u64 hash, void *key)
{
    HM_Generic *hmg = hm;

    bool found;
    u64 idx = hm_probe(hmg, data, entry_size, key_offset, state_offset, hash, key, &found);

    if (!found)
        return false;

    u8 *entry = (u8 *)data + idx * entry_size;
    HM_State *state = (HM_State *)(entry + state_offset);

    *state = HM_TOMBSTONE;
    hmg->len--;

    return true;
}
void _hm_clear(void *hm, void *data, size_t entry_size, size_t state_offset)
{
    HM_Generic *hmg = hm;

    if (!data)
        return;

    for (u64 i = 0; i < hmg->cap; i++)
    {
        u8 *entry = (u8 *)data + i * entry_size;
        *(HM_State *)(entry + state_offset) = HM_EMPTY;
    }

    hmg->len = 0;
}
void _hm_free(void *hm, void **pdata)
{
    HM_Generic *hmg = hm;

    free(*pdata);
    *pdata = NULL;
    hmg->cap = 0;
    hmg->len = 0;
}

#endif // CHEST_ENABLE_HM

#endif // CHEST_IMPLEMENTATION

#endif // CHEST_H