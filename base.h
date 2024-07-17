#ifndef BASE_H
#define BASE_H

#include <assert.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdint.h>

typedef unsigned long  ulong;
typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned char  uchar;
typedef signed char    schar;

static_assert( sizeof( long ) == 8 );
static_assert( sizeof( int ) == 4 );
static_assert( sizeof( short ) == 2 );

static_assert( sizeof( long ) == sizeof( size_t ) );
static_assert( sizeof( long ) == sizeof( intptr_t ) );
static_assert( sizeof( long ) == sizeof( ptrdiff_t ) );

static_assert( ATOMIC_POINTER_LOCK_FREE == 2 );
static_assert( ATOMIC_LONG_LOCK_FREE == 2 );
static_assert( ATOMIC_INT_LOCK_FREE == 2 );
static_assert( ATOMIC_SHORT_LOCK_FREE == 2 );
static_assert( ATOMIC_CHAR_LOCK_FREE == 2 );

#endif
