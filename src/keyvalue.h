/*! @file
  @brief
  mruby/c Key(Symbol) - Value store.

  <pre>
  Copyright (C) 2015-2018 Kyushu Institute of Technology.
  Copyright (C) 2015-2018 Shimane IT Open-Innovation Center.

  This file is distributed under BSD 3-Clause License.

  </pre>
*/

#ifndef MRBC_SRC_C_KEYVALUE_H_
#define MRBC_SRC_C_KEYVALUE_H_

#include "value.h"

#ifdef __cplusplus
extern "C" {
#endif

//================================================================
/*! Define Key-Value data.
*/
typedef struct RKeyValue {
#if defined(MRBC_DEBUG)
  char type[2];
#endif
  mrbc_sym sym_id;	//!< symbol ID as key.
  mrbc_value value;	//!< stored value.

} mrbc_kv;


//================================================================
/*! Define Key-Value handle.
*/
typedef struct RKeyValueHandle {
  uint16_t data_size;	//!< data buffer size.
  uint16_t n_stored;	//!< # of stored.
  union {
    mrbc_kv *data;	//!< pointer to allocated memory.
    struct VM *vm;	//!< pointer to VM (if data_size == 0)
  };

} mrbc_kv_handle;


//================================================================
/*! Define Key-Value iterator.
*/
typedef struct RKeyValueIterator {
  const mrbc_kv_handle *target;
  uint16_t i;

} mrbc_kv_iterator;


mrbc_kv_handle *mrbc_kv_new(struct VM *vm, int size);
int mrbc_kv_init_handle(struct VM *vm, mrbc_kv_handle *kvh, int size);
void mrbc_kv_delete(mrbc_kv_handle *kvh);
void mrbc_kv_delete_data(mrbc_kv_handle *kvh);
void mrbc_kv_clear_vm_id(mrbc_kv_handle *kvh);
int mrbc_kv_resize(mrbc_kv_handle *kvh, int size);
int mrbc_kv_set(mrbc_kv_handle *kvh, mrbc_sym sym_id, mrbc_value *set_val);
mrbc_value *mrbc_kv_get(mrbc_kv_handle *kvh, mrbc_sym sym_id);
int mrbc_kv_append(mrbc_kv_handle *kvh, mrbc_sym sym_id, mrbc_value *set_val);
int mrbc_kv_reorder(mrbc_kv_handle *kvh);
int mrbc_kv_remove(mrbc_kv_handle *kvh, mrbc_sym sym_id);
void mrbc_kv_clear(mrbc_kv_handle *kvh);
void mrbc_kv_dup(const mrbc_kv_handle *src, mrbc_kv_handle *dst);


//================================================================
/*! get size
*/
static inline int mrbc_kv_size(const mrbc_kv_handle *kvh)
{
  return kvh->n_stored;
}

//================================================================
/*! iterator constructor
*/
static inline mrbc_kv_iterator mrbc_kv_iterator_new( const mrbc_kv_handle *h )
{
  mrbc_kv_iterator ite;

  ite.target = h;
  ite.i = 0;

  return ite;
}

//================================================================
/*! iterator has_next?
*/
static inline int mrbc_kv_i_has_next( const mrbc_kv_iterator *ite )
{
  return ite->i < ite->target->n_stored;
}

//================================================================
/*! iterator getter
*/
static inline mrbc_kv *mrbc_kv_i_next( mrbc_kv_iterator *ite )
{
  return &ite->target->data[ ite->i++ ];
}



#ifdef __cplusplus
}
#endif
#endif
