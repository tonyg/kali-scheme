(define-interface ffi-interface
  (export ffi-lib
          ffi-lib?
          ffi-lib-name
          ffi-obj
          ffi-obj?
          ffi-obj-lib
          ffi-obj-name
          get-ffi-obj
          make-ctype
          ctype?
          malloc
          cpointer?
          ptr-ref ptr-set!
          cpointer->string
          
          _void _bool _int8 _uint8 _int16 _uint16 _int32 _uint32 _int64 _uint64
          _float _double _pointer _fpointer
          _sint8 _sint16 _sint32 _sint64
          _byte _ubyte _sbyte
          _word _uword _sword
          _short _ushort _sshort
          _int _uint _sint
          _long _ulong _slong
          _llong _ullong _sllong
          _cprocedure

          ((_fun)
           :syntax)))

(define-structure ffi ffi-interface
  (for-syntax (open scheme destructuring primitives srfi-1 jas-match ffi-util))
  (open scheme define-record-types
        define-values
        external-calls
        tables
        handle
        i/o byte-vectors ; debug
        srfi-11
        srfi-13
        srfi-16
        srfi-23
        ffi-util)
  (files lib obj types ffi fun))

(define-structure define-values (export ((define-values)
                                         :syntax))
  (for-syntax (open scheme srfi-1 destructuring))
  (open scheme srfi-8 primitives)
  (files define-values))

(define-interface match-interface
  (export ((match match-lambda match-let match-let*)
           :syntax)))

(define-structure jas-match match-interface
  (open scheme define-values)
  (files jas-match))

(define-structure ffi-util (export void? split-by custom-type->keys add-renamer
                                   ((define-fun-syntax)
                                    :syntax))
  (for-syntax (open scheme destructuring))
  (open scheme primitives srfi-1 jas-match)
  (files util))

