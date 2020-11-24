  # labwork.S
  # Written 2015-2017 by F Lundevall
  # Skeleton file for IS1200/IS1500 lab 1.
  # The macros PUSH and POP are in the public domain.
  # Please add your own code at the end of the file.

  #
  # Please keep the two macros PUSH and POP unchanged
  #
.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm
  #
  # Please add your own code below this line
  #
.globl	hexasc,delay,time2string,enable_interrupt

enable_interrupt:
  EI
  jr $ra
  nop

hexasc:
	andi	$t0,$a0,0xf 	#Loads the input into a temporary register & remove inputs except the 4 least significant bits 0....01111 = 15
	addi	$t0,$t0,0x30	#0x30 is ASCII-code for 0, want to start from there so input 0 represents 0 and so on

	ble	$t0,0x39,hexascDone #If the ASCII-code describes a number (code < 0x3A) jump to continue(taken), if not add 7 to move it to letters in the ASCII-table
	nop

	addi	$t0,$t0,7 	#if its not a number (over 0x39) skip special characters and jump to letters.
hexascDone:
	add	$v0,$0,$t0	#Saves the ASCII-code to return register
	jr 	$ra
	nop

delay:
	add	$t0,$0,$a0	#int ms

whileLoop:
	ble	$t0,0,whileDone	#branch (quit) if ms <= 0
	nop
	addi	$t0,$t0,-1	#ms = ms - 1

	addi	$t1,$0,0	#i = 0
foorLoop:
	slti	$t2,$t1,3900	# i < 4711 ? 4711 too slow 4000 ~ 2s off 3900 ~1-2s off
	beqz	$t2,whileLoop	#if i >= 4711 branch to whileLoop
	nop
	addi	$t1,$t1,1	#i++
	j 	foorLoop
	nop
whileDone:
 	jr $ra
 	nop

time2string:
	PUSH	$ra
	PUSH	$s0
	PUSH	$s1
	PUSH	$s2

	addi	$s2,$0,5		#Counter i = 5, Little-endian start storing lowest valued time-digit in the end before null-byte

	move	$s1,$a1			#$s1 = clock info
	move	$s0,$a0			#$s0 = store address

	andi	$t4,$s1,0xff		#Grabs the LSB
	bnez	$t4,dontAddX		#If(LSB != 0) {dont add X branch!}

	addi	$t1,$0,0x58		#ASCII-code for 'X'
	add	$t0,$s0,$s2		#Add offset to store address
	sb	$t1,0($t0)
	addi	$s2,$s2,1		#i++ wAnna put null-byte above it

dontAddX:
	addi	$t1,$0,0x00		#ASCII-code for null-byte
	add	$t0,$s0,$s2		#Add offset to store address
	sb	$t1,0($t0)

	addi	$s2,$0,5		#Set i to 5 before it starts saving down the numbers
time2stringLoop:
	addi	$s2,$s2,-1		#i--

	beq	$s2,2,addColon		#if i == 2 we want to add a colon, branch to that section
	nop
	beq	$s2,-1,quitLoop		#if i == -1 we want to quit the loop and finish our string repre
	nop

	add	$a0,$s1,$0		#Puts the clock info into argument to sent to hexasc
	jal	hexasc			#Sends in more than we want but we know it only uses 4 least significant bits
	nop

	add	$t0,$s0,$s2		#Add offset to store address
	sb	$v0,0($t0)
	sra	$s1,$s1,4		#rightShift the clock-data
	j	time2stringLoop
	nop
addColon:
	#code to add colon
	addi	$t1,$0,0x3A		#ASCII-code for :
	add	$t0,$s0,$s2		#Add offset to store address
	sb	$t1,0($t0)

	j	time2stringLoop
	nop

quitLoop:
	POP	$s2
	POP	$s1
	POP	$s0
	POP	$ra
	jr	$ra
	nop
