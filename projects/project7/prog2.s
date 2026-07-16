.text
.globl main

main:
    li      $v0, 5              # Read a signed integer.
    syscall

    move    $a0, $v0
    jal     count_digits

    move    $a0, $v0
    li      $v0, 1              # Print the digit count.
    syscall

    li      $a0, 10
    li      $v0, 11             # Print a newline.
    syscall

    li      $v0, 10
    syscall

# Returns the number of decimal digits in the absolute value of $a0.
# Zero has one digit.
count_digits:
    move    $t0, $a0
    bgez    $t0, magnitude_ready
    subu    $t0, $zero, $t0     # Unsigned subtraction also handles INT_MIN.

magnitude_ready:
    li      $t1, 10
    li      $v0, 0

count_loop:
    addiu   $v0, $v0, 1
    divu    $t0, $t1
    mflo    $t0
    bgtz    $t0, count_loop

    jr      $ra
