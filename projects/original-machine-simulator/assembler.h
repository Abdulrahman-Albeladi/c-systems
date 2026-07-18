/* Originally written by Saeed Alaei. Modified and revised by Larry Herman.
 * (c) Larry Herman and Saeed Alaei, 2015. You are allowed to use this code
 * yourself, but not to provide it to anyone else.
 * Never needs ironing.
 */

#ifndef PROJECT3_ASSEMBLER_H
#define PROJECT3_ASSEMBLER_H

/*
 * Assemble a program into memory.
 *
 * Supported source features include empty lines, labels, comments beginning
 * with # or //, and .data directives. Numeric constants may be decimal,
 * hexadecimal (0x prefix), or octal (0 prefix). Labels may be used where an
 * address or constant is expected.
 *
 * @param name Source filename. Pass "stdin" or an empty string to read from
 *             standard input.
 * @param mem  Destination memory for assembled instructions and data.
 * @return     Number of assembled memory words.
 */
int assemble(const char *name, Word mem[]);

#endif
