# Constraints

r0 is always 0. PC is a special register that can only be accessed with a 

m00 - m0F is reserved for data and cannot be used for program memory

Program memory always starts at m10

# Terms

'&' denotes a numerical digit of some kind
'[]' denotes an optional argument

- reg : A register, written as 'r&'
- mem : A memory location, written as 'm&&'
- imm : A decimal value, written as '#&'
- hex : A hexadecimal value, written as '0x&&&&'
- str : A string of characters surrounded by double quotes. Supports the escape sequences \0 (null character), \n (newline), and \\" (double quote).
- label : The name of a jump label
- newline : A newline

# Syntax

## ;
## \<newline>

Statement terminator

## //

Comment

# Preprocess

## .data \<mem>, \<hex>

Sets the given memory location to the given hex value

## .data \<mem>, \<str>

Creates a C-like string starting at the given memory location. Automatically adds a null character (\0) at the end of the string.

## .data \<mem>, {\<hex>[, \<hex>, ..]}

Similar to creating a string, but writes each provided hex value consecutively.

## .define \<macro>, \<statement>

Tell the assembler to replace 'statement' with 'macro' anywhere in the code. Neither 'macro' not 'statement' should contain any commas. Whitespace after the comma will be ignored until a non-whitespace character.

\<macro> and \<statement> are currently undefined.

## \<label>:

Creates a reference to the statement's line in code for use in jump instructions

# Arithmetic and Logical

## add [\<dest>], \<a>, \<b>

Add 'b' to 'a'.

If 'dest' is defined, store the result in 'dest'. Otherwise, store the result in 'a'.

### *Syntax*

    [reg], reg, reg

## sub [\<dest>], \<a>, \<b>

Subtract 'b' from 'a'.

If 'dest' is defined, store the result in 'dest'. Otherwise, store the result in 'a'.

### *Syntax*

    [reg], reg, reg

## and [\<dest>], \<a>, \<b>

Binary AND 'a' and 'b'.

If 'dest' is defined, store the result in 'dest'. Otherwise, store the result in 'a'.

### *Syntax*

    [reg], reg, reg

## xor [\<dest>], \<a>, \<b>

Binary XOR 'a' and 'b'.

If 'dest' is defined, store the result in 'dest'. Otherwise, store the result in 'a'.

### *Syntax*

    [reg], reg, reg

## lsl [\<dest>], \<a>, \<b>

Bit shift 'a' left by 'b'.

If 'dest' is defined, store the result in 'dest'. Otherwise, store the result in 'a'.

### *Syntax*

    [reg], reg, reg

## lsr [\<dest>], \<a>, \<b>

Bit shift 'a' right by 'b'.

If 'dest' is defined, store the result in 'dest'. Otherwise, store the result in 'a'.

### *Syntax*

    [reg], reg, reg

# Transfer

## mov \<dest>, \<src>

Copy 'src' into 'dest'.

*Syntax*

    reg, reg

## mov \<reg>, \<imm>
## mov \<reg>, \<hex>

Set the given register to the given value.

## ldr \<reg>, \<mem>

Copy the given memory into the given register.

## ldr \<dest>, \<src>

Copy the memory pointed to by 'src' into 'dest'.

*Syntax*

    reg, reg

## str \<mem>, \<reg>

Copy the given register into the given memory.

## str \<dest>, \<src>

Copy 'src' into the memory pointed to by 'dest'.

*Syntax*

    reg, reg

# Control

## b \<label>

Goto the given label.

## bz \<reg>, \<label>

If the value in the given register is 0, goto the given label.

## bp \<reg>, \<label>

If the value in the given register is greater than 0, goto the given label.

## br \<reg>

Jump to the address stored in the given register.

## bl \<reg>, \<label>

Store the PC in the given register and then goto the given label.

*Syntax*

    reg, mem

# I/O

## stdin \<reg>

Set the value of the given register to stdin.

## stdin \<reg>

Write the value of the given register to stdout.

# Machine codes

## Arithmetic and Logical

- add -> 1#ab (# = d or a)
- sub -> 2#ab (# = d or a)
- and -> 3#ab (# = d or a)
- xor -> 4#ab (# = d or a)
- lsl -> 5#ab (# = d or a)
- lsr -> 6#ab (# = d or a)

## Transfer

- mov d, s -> 1ds0
- mov r, i -> 7r## (## = i converted to hex)
- mov r, h -> 7r## (## = h)
- ldr r, m -> 8rmm
- ldr r, R -> Ar0R
- str r, m -> 9rmm
- str r, R -> BR0r

## Control

- b -> C0## (## = resolved label)
- bz -> Cr## (## = resolved label)
- bp -> Dr## (## = resolved label)
- br -> Er00
- bl -> Fr## (## = resolved label)

## I/O

- stdin -> 8rFF
- stdout -> 9rFF
