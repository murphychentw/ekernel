#ifndef	__EKERNEL_TYPES_H_
#define __EKERNEL_TYPES_H_



typedef unsigned long SIZE;


/*
 * A natural_t is the type for the native
 * integer type, e.g. 32 or 64 or.. whatever
 * register size the machine has.  Unsigned, it is
 * used for entities that might be either
 * unsigned integers or pointers, and for
 * type-casting between the two.
 * For instance, the IPC system represents
 * a port in user space as an integer and
 * in kernel space as a pointer.
 */
typedef unsigned int	natural_t;

/*
 * An integer_t is the signed counterpart
 * of the natural_t type. Both types are
 * only supposed to be used to define
 * other types in a machine-independent
 * way.
 */
typedef int		integer_t;

/*
 * A vm_offset_t is a type-neutral pointer,
 * e.g. an offset into a virtual memory space.
 */
typedef	natural_t	vm_offset_t;

/*
 * A vm_size_t is the proper type for e.g.
 * expressing the difference between two
 * vm_offset_t entities.
 */
typedef	natural_t	vm_size_t;

/*
 * On any anchitecture,
 * these types are _exactly_ as wide as indicated in their names.
 */
typedef signed char		signed8_t;
typedef signed short		signed16_t;
typedef signed long		signed32_t;
typedef signed long long	signed64_t;
typedef unsigned char		unsigned8_t;
typedef unsigned short		unsigned16_t;
typedef unsigned long		unsigned32_t;
typedef unsigned long long	unsigned64_t;
typedef float			float32_t;
typedef double			float64_t;

/*
 * On any given architecture,
 * these types are guaranteed to be _at_least_
 * as wide as indicated in their names,
 * but may be wider if a wider type can be more efficiently accessed.
 *
 * On the x86, BYTEs and 32-bit WORDs are fast,
 * but 16-bit WORDs are slow;
 * this property is reflected in these type definitions.
 */
 
typedef unsigned char		BOOL;
typedef signed char		signed_min8_t;
typedef unsigned char		unsigned_min8_t;
typedef signed int		signed_min16_t;
typedef unsigned int		unsigned_min16_t;
typedef signed int		signed_min32_t;
typedef unsigned int		unsigned_min32_t;

typedef unsigned long DWORD;
typedef unsigned short WORD;
typedef unsigned char BYTE;

#ifndef	TRUE
#define TRUE	((BOOL) 1)
#endif

#ifndef	FALSE
#define FALSE	((BOOL) 0)
#endif


#endif	/* _EKERNEL_TYPES_H_ */
