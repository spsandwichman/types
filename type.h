#pragma once

#include "orbit.h"

enum {
    T_VOID,
    T_I8,
    T_I16,
    T_I32,
    T_I64,
    T_U8,
    T_U16,
    T_U32,
    T_U64,
    T_F16,
    T_F32,
    T_F64,

    T_meta_INTEGRAL,

    T_ADDR,
    T_POINTER,
    T_SLICE,
    T_ARRAY,
    T_ENUM,
    T_STRUCT,
    T_DISTINCT,
    T_ALIAS,
};

typedef struct struct_field {
    char* name;
    struct type* subtype;
} struct_field;

da_typedef(struct_field);

typedef struct enum_variant {
    char* name;
    i64 enum_val;
} enum_variant;

da_typedef(enum_variant);

typedef struct type {
    union {
        struct {
            struct type* subtype;
        } as_reference; // used by both T_POINTER and T_SLICE
        struct {
            struct type* subtype;
            u64 len;
        } as_array;
        struct {
            da(struct_field) fields;
        } as_struct;
        struct {
            da(enum_variant) variants;
            u8 backing_type;
        } as_enum;
    };
    u8 tag;
    bool disabled;

    u16 size;

    // data for comparison shit
    u16 type_nums[2];
} type;

typedef struct {
    type** at;
    size_t len;
    size_t cap;
} type_graph;

extern type_graph tg;

void K5();
void ll_int_float_p2();

void make_type_graph();
type* make_type(u8 tag);

void add_field(type* s, char* name, type* sub);
struct_field* get_field(type* s, size_t i);

void set_target(type* p, type* dest);
type* get_target(type* p);

void print_type_graph();

u64 get_index(type* t);
type* get_type_from_num(u16 num, int num_set);

bool are_equivalent(type* a, type* b);
bool is_element_equivalent(type* a, type* b, int num_set_a, int num_set_b);
void locally_number(type* t, u64* number, int num_set);
void reset_numbers();
void canonicalize();

void merge_type_references(type* dest, type* src);