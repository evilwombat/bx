bx - block extraction tool

... for when the migraine of converting dd arguments to decimal exceeds the migraine of hacking something together

Extract an arbitrarily-sized block from an input file, write the result to an output file.
This is a very simple re-implementation of 'dd', except it allows the offset/length arguments
to be specified is decimal as well as in hex.


Usage:
`bx <input file> <offset> <length> <output file>`

offset and length are interpreted as decimal, unless they are prefixed with '0x'.
length can also be '+', which means 'extend to end of input'


Examples:

`bx input.bin 0x8000 0x1000 output.bin`

Copy 0x1000 bytes from input to output, starting from offset 0x1000



`bx input.bin 0x8000 25 output.bin`

Copy 25 bytes from input to output, starting from offset 0x1000



`bx input.bin 0x10000 + output.bin`

Copy all data from input.bin, starting from offset 0x10000 til the end of input.bin, to output.bin

Happy dumping!
