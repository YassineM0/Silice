
code.elf:     file format elf32-littleriscv


Disassembly of section .text:

00000000 <_start>:
   0:	00010137          	lui	sp,0x10
   4:	00000097          	auipc	ra,0x0
   8:	018080e7          	jalr	24(ra) # 1c <main>
   c:	00000317          	auipc	t1,0x0
  10:	00830067          	jr	8(t1) # 14 <exit>

00000014 <exit>:
  14:	00000000          	.word	0x00000000
  18:	00008067          	ret

0000001c <main>:
  1c:	000007b7          	lui	a5,0x0
  20:	0707a583          	lw	a1,112(a5) # 70 <LEDS>
  24:	000f46b7          	lui	a3,0xf4
  28:	00100793          	li	a5,1
  2c:	00000713          	li	a4,0
  30:	23f68693          	add	a3,a3,575 # f423f <__stacktop+0xe423f>
  34:	08000513          	li	a0,128
  38:	00078813          	mv	a6,a5
  3c:	c00028f3          	rdcycle	a7
  40:	c0002673          	rdcycle	a2
  44:	41160633          	sub	a2,a2,a7
  48:	fec6fce3          	bgeu	a3,a2,40 <main+0x24>
  4c:	00a78463          	beq	a5,a0,54 <main+0x38>
  50:	01079463          	bne	a5,a6,58 <main+0x3c>
  54:	00174713          	xor	a4,a4,1
  58:	00070863          	beqz	a4,68 <main+0x4c>
  5c:	00179793          	sll	a5,a5,0x1
  60:	00f5a023          	sw	a5,0(a1)
  64:	fd9ff06f          	j	3c <main+0x20>
  68:	4017d793          	sra	a5,a5,0x1
  6c:	ff5ff06f          	j	60 <main+0x44>

00000070 <LEDS>:
  70:	00010004                                ....
