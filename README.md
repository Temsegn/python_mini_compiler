<<<<<<< HEAD
=======
# python_compiler
>>>>>>> 69f06a71e6782f364401bbfe391a6862923c644b
# python Compiler

A Python-like compiler implementation using Flex (lexer) and Bison (parser), featuring symbol table management, scope handling, and semantic error checking,abstract syntax tree,code optimization and intermediate code geneartion

## Features

- **Lexical Analysis**: Tokenizes Python-like syntax
- **Syntax Parsing**: Implements grammar rules for methods, variables,expression,statement and control structures
- **Semantic Checks**:
  - Variable redeclaration detection
  - Undeclared identifier checking
  - Method/constructor existence verification
  - Scope management (global, class, method, block)
  - function redefinition
- **Symbol Table**:
  - Hierarchical scope tracking
  - Variable/method/parameter storage
  - Parent-child scope relationships
- **Abstract Tree**
- **optimization**:
- **intermediate Code Generation**:

## Dependencies

- Flex (≥ 2.6)
- Bison (≥ 3.7)
- GCC or Clang
- vs code Editor

SYMBOL TABLE HIERARCHY:
┌─────────────────────────────┐
├── Scope: global
│ ├── Scope: ValidProgram
│ │ ├── Scope: main
│ │ │ ├── Variable [String[]] (Parameter)
└─────────────────────────────┘
<<<<<<< HEAD

```

## Members

- Temesgen Marie
- Asmamaw kassahun
- Bichawork Amogne
- Gerawork Zewudu
- Melaku Azene
```
=======
 
>>>>>>> 69f06a71e6782f364401bbfe391a6862923c644b
