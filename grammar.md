# SYNTAX

## KEY \<reg>, \<name>

Create an association between the register 'reg' and the keyword 'name'.

*Syntax*

    register, keyword

## VAR \<name>, [\<value>]

Create a dedicated memory space for the variable associated with the
keyword 'name'. The memory space initializes with the value 0000h.

If 'value' is defined, initialize the memory space with the value of
'value' instead.

*Syntax*

    keyword, [constant]

# ARITHMETIC and LOGICAL

## ADD \<ax>, \<bx>, [\<cx>]

Increment the contents of register 'ax' by the contents of register 'bx'.
If cx is defined, store the result in cx.

*Syntax*

    register, register, [register]

## AND \<ax>, \<bx>, [\<cx>]

Binary AND the contents of register 'ax' by the contents of register 'bx'.
If cx is defined, store the result in cx.

*Syntax*

    register, register, [register]

## BSL \<ax>, \<bx>, [\<cx>]

Bit shift the contents of register 'ax' left by the contents of register
'bx'. If cx is defined, store the result in cx.

*Syntax*

    register, register, [register]

## BSR \<ax>, \<bx>, [\<cx>]

Bit shift the contents of register 'ax' right by the contents of register 'bx'.
If cx is defined, store the result in cx.

*Syntax*

    register, register, [register]


## SUB \<ax>, \<bx>, [\<cx>]

Decrement the contents of register 'ax' by the contents of register 'bx'.
If cx is defined, store the result in cx.

*Syntax*

    register, register, [register]

## XOR \<ax>, \<bx>, [\<cx>]

Binary XOR the contents of register 'ax' by the contents of register 'bx'.
If cx is defined, store the result in cx.

*Syntax*

    register, register, [register]

# TRANSFER

## MOV \<dest>, \<src>

Copy the value at 'src' into 'dest'.

*Syntax*

    register, register
    register, var
    var, register

## SET \<dest>, \<value>

Set the register 'dest' to the constant 'value'.

*Syntax*

    register, constant

# CONTROL

## BP \<condition>, \<goto>

If the value in the register 'condition' is greater than 0, set the
program counter to the address associated with the label 'goto'.

*Syntax*

    register, label

## BZ \<condition>, \<goto>

If the value in the register 'condition' is 0, set the program counter to
the address associated with the label 'goto'.

*Syntax*

    register, label

## END

Halts program execution.

## FUNC \<goto>, [\<reg>]

If 'reg' is defined, set the register 'reg' to the current program counter.

If 'reg' is not defined, set the register REX to the current program
counter.

After that, set the program counter to the address associated with the
label 'goto'.

*Syntax*

    label, [register]

## JUMP \<goto>

Set the program counter to the address associated with the label 'goto'.

*Syntax*

    label

## RETURN [\<reg>]

If 'reg' is defined, set the program counter to the value in the register
'reg'.

If 'reg' is not defined, set the program counter to the value in the
register 'REX'.

*Syntax*

    [register]

# I/O

## IN \<reg>

Set the value of the register 'reg' to stdin

## OUT \<reg>

Write the value of the register 'reg' to stdout

# SPECIAL

## HEX \<value>

Store the value 'value' at the memory address of this instruction

## .\<name>

Create an association between the label 'name' and the address of the next
instruction.

## //\<comment>

Add a comment with no effect on execution

## ;

Delineates statement evaluation, equivalent to a newline
