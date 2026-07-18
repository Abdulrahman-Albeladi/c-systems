.text
.globl main

# Counts integers in the inclusive range [m, n] divisible by mod_value.
main:
    li   $v0, 5
    syscall
    move $t0, $v0                 # m

    li   $v0, 5
    syscall
    move $t4, $v0                 # n

    li   $v0, 5
    syscall
    move $t1, $v0                 # mod_value

    move $t3, $zero               # count

    # Division by zero is undefined for rem. Report zero matches instead.
    beq  $t1, $zero, print_count

while:
    bgt  $t0, $t4, print_count

    rem  $t2, $t0, $t1
    bne  $t2, $zero, next_value

    addiu $t3, $t3, 1

next_value:
    addiu $t0, $t0, 1
    j    while

print_count:
    li   $v0, 1
    move $a0, $t3
    syscall

    li   $v0, 11
    li   $a0, 10
    syscall

    li   $v0, 10
    syscall
