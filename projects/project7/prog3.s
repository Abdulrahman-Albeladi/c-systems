.text
.globl main

# Reads up to 100 nonzero values, then prints the first matching index
# for each nonzero query. A zero terminates each input phase.
main:
read_values:
    li      $v0, 5
    syscall
    beqz    $v0, read_queries

    lw      $t0, max
    li      $t1, 100
    bge     $t0, $t1, read_values

    la      $t2, arr
    sll     $t3, $t0, 2
    addu    $t2, $t2, $t3
    sw      $v0, 0($t2)

    addiu   $t0, $t0, 1
    sw      $t0, max
    b       read_values

read_queries:
    li      $v0, 5
    syscall
    beqz    $v0, exit
    move    $t0, $v0

    li      $t1, 0
search:
    lw      $t2, max
    bge     $t1, $t2, not_found

    la      $t3, arr
    sll     $t4, $t1, 2
    addu    $t3, $t3, $t4
    lw      $t5, 0($t3)
    beq     $t5, $t0, found

    addiu   $t1, $t1, 1
    b       search

found:
    move    $a0, $t1
    b       print_result

not_found:
    li      $a0, -1

print_result:
    li      $v0, 1
    syscall

    li      $a0, 10
    li      $v0, 11
    syscall
    b       read_queries

exit:
    li      $v0, 10
    syscall

.data
.align 2
arr:    .word 0:100
max:    .word 0
