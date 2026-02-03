# Script Interpreter

A simple scripting language interpreter built with TypeScript. This is version 1 of the interpreter.

## Language Features

### Data Types
- **Numbers**: Numeric literals (integers and floating-point)

### Expressions
1. **Numeric Literals**: `42`, `3.14`
2. **Identifiers**: Variable and function names
3. **Binary Expressions**:
   - **Arithmetic**: `+`, `-`, `*`, `/`, `%`
   - **Comparison**: `>`, `<`, `>=`, `<=`
   - **Equality**: `===`, `!==`
   - **Logical**: `&&`, `||`
4. **Assignment Expressions**: `=`, `+=`, `-=`, `*=`, `/=`
5. **Call Expressions**: Function calls with arguments `fn(arg1, arg2)`
6. **Grouped Expressions**: Parenthesized expressions `(expression)`

### Statements
1. **Variable Declaration**: `let x = 10`
2. **Expression Statement**: Any expression used as a statement
3. **If/Else Statement**: Conditional branching with `if`, `else`, and `else if` chains
4. **While Loop**: `while(condition) { ... }`
5. **Block Statement**: Code blocks `{ ... }` with proper lexical scoping
6. **Return Statement**: `return` or `return expression`
7. **Function Declaration**: `function name(param1, param2) { ... }`

### Functions
- Named function declarations (anonymous functions not supported)
- Function parameters and arguments
- Function calls with multiple arguments
- **Recursion**: Fully supported
- **Higher-order functions**: Functions can be passed as arguments and returned from functions
- Return values from functions (including function objects)

### Built-in Functions
- `print(...)`: Output to console (accepts variable number of arguments)
- `type(value)`: Returns the type of a value

### Language Characteristics
- Lexical scoping with environment-based variable resolution
- Proper operator precedence
- First-class functions (can be stored in variables and passed as arguments)


## To Dos for Future  
- Anonymous functions / function expressions
- String and boolean literal types
- Array and object data structures
- For loops
- Additional operators (unary operators, ternary, etc.)

