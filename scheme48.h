
/* Big kludge.  This ought to be automagically generated from data.scm. */

typedef int scheme_value;

#define ENTER_FIXNUM(n)   ((scheme_value)((n) << 2))
#define EXTRACT_FIXNUM(x) ((long)(x) >> 2)
#define FIXNUMP(x)	  (((long)(x) & 3L) == 0)

#define IMM_TAG 1
#define MISC_IMMEDIATE(n) (scheme_value)(IMM_TAG | (n << 2))
#define SCHFALSE    MISC_IMMEDIATE(0)
#define SCHTRUE	    MISC_IMMEDIATE(1)
#define UNSPECIFIED MISC_IMMEDIATE(3)
#define UNDEFINED   MISC_IMMEDIATE(4)
#define SCHEOF      MISC_IMMEDIATE(5)
#define SCHNULL	    MISC_IMMEDIATE(6)
#define ENTER_BOOLEAN(n) ((n) ? SCHTRUE : SCHFALSE)
#define EXTRACT_BOOLEAN(x) ((x) != SCHFALSE)

#define ENTER_CHAR(c) (MISC_IMMEDIATE(2) | ((c) << 8))
#define EXTRACT_CHAR(x) ((x) >> 8)

#define STOB_TAG 3
#define STOBP(x) (((long)(x) & 3L) == STOB_TAG)
#define ADDRESS_AFTER_HEADER(x, type) ((type *)((x) - STOB_TAG))
#define STOB_REF(x, i) ((ADDRESS_AFTER_HEADER(x, long))[i])
#define STOB_HEADER(x) STOB_REF(x, -1)
#define STOB_TYPE(x) (((long)STOB_HEADER(x) >> 2) & 31L)

#define HEADER_LENGTH_SHIFT 8
#define STOB_BLENGTH(x) \
 (STOB_HEADER(x) >> HEADER_LENGTH_SHIFT)
#define STOB_LLENGTH(x) \
 (STOB_HEADER(x) >> (HEADER_LENGTH_SHIFT + 2))

/* Cf. arch.scm */
#define STOBTYPE_PAIR 0
#define STOBTYPE_VECTOR 2
#define STOBTYPE_STRING 15
#define STOB_OF_TYPE_P(x, type) (STOBP(x) && STOB_TYPE(x) == (type))

#define pairp(x) STOB_OF_TYPE_P((x), STOBTYPE_PAIR)
#define car(x) STOB_REF(x, 0) /* cf. vm/struct.scm */
#define cdr(x) STOB_REF(x, 1) /* cf. vm/struct.scm */
#define vectorp(x) STOB_OF_TYPE_P((x), STOBTYPE_VECTOR)
#define vector_length(x) STOB_LLENGTH(x)
#define vector_ref(x, i) STOB_REF(x, i)
#define byte_vector_length(x)  STOB_BLENGTH(x)
#define byte_vector_ref(x, i)  (ADDRESS_AFTER_HEADER(x, unsigned char)[i])
#define stringp(x) STOB_OF_TYPE_P((x), STOBTYPE_STRING)
#define string_length(x)  STOB_BLENGTH(x)
#define string_ref(x, i)  (ADDRESS_AFTER_HEADER(x, char)[i])
#define extract_string(x)  ADDRESS_AFTER_HEADER(x, char)
