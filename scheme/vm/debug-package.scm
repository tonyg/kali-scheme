; For debugging

(define-structure vm-disassembler (export disassemble write-instruction)
  (open scheme
	struct
        enumerated              ; enumerand->name
        vm-architecture
	bitwise                 ; arithmetic-shift
	signals)                ; error
  (files disasm))

