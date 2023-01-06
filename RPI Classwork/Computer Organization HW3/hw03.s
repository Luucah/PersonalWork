################################################################################
# Homework 3: Matrix-matrix multiplication
# CSCI-2500 Fall 2021
################################################################################
## Data segment
################################################################################
.data
  newline:  .asciiz "\n"
  tab:      .asciiz "\t"

################################################################################
## Text segment
################################################################################
.text
  .align 2          # instructions must be on word boundaries
  .globl main       # main is a global label
  .globl mm_read    # read in NxM matrix
  .globl mm_alloc   # allocate space for NxM matrix
  .globl mm_print   # print out formatted NxM matrix
  .globl mm_mult    # multiple two non-square matrices
################################################################################
# Main segment
################################################################################
main:
  # save return address on the stack
  sub $sp, $sp, 4   
  sw  $ra, 0($sp)

  # Read in a matrix 1 of some NxM size
  # Return in $v0 - N
  # Return in $v1 - M
  # Return in 0($sp) - base address of matrix
  sub $sp, $sp, 4   # make space on stack for return value
  jal mm_read

  # save matrix 1 values
  move  $s0, $v0
  move  $s1, $v1
  lw    $s2, 0($sp)
  add   $sp, $sp, 4   # restore stack

  # Setup arguments and print out matrix 1
  move  $a0, $s0
  move  $a1, $s1
  move  $a2, $s2
  jal mm_print

  # Read in matrix 2 of some NxM size
  sub $sp, $sp, 4   # make space on stack for return value
  jal mm_read

  # save matrix 2 values
  move  $s3, $v0
  move  $s4, $v1
  lw    $s5, 0($sp)
  add   $sp, $sp, 4   # restore stack

  # Setup arguments and print out matrix 2
  move  $a0, $s3
  move  $a1, $s4
  move  $a2, $s5
  jal mm_print

  # Setup arguments and call mm_mult
  # Note: 5 arguments, so we need to use stack
  # Note: num cols matrix 1 == num rows matrix 2
  move  $a0, $s0    # num rows matrix1
  move  $a1, $s1    # num cols matrix1
  move  $a2, $s2    # address of matrix 1
  move  $a3, $s4    # num cols matrix2
  sub   $sp, $sp, 4   
  sw    $s5, 0($sp) # address of matrix 2
  jal mm_mult
  add   $sp, $sp, 4

  # print the result
  move $a0, $s0 
  move $a1, $s4
  move $a2, $v0
  jal mm_print

  # restore $ra, free stack space, and return
  lw  $ra, 0($sp)
  add $sp, $sp, 4
  jr  $ra

################################################################################
# mm_read: Read in a NxM matrix from standard input
# Note: You will be calling mm_alloc from this function, so you'll need to save 
#       $ra on the stack. Also, you'll be returning 3 values back to main, one
#       value (the matrix base address), will need to be saved on the stack.
#       Matrix format is:
#         First line:  N = num rows
#         Second line: M = num cols
#         Subsequent lines contain one value each, filling each row in turn
#         (same format as hw01, except N,M on separate lines)  
# Input:
#   None
# Return:
#   v0 = N
#   v1 = M
#   0($sp) = base address of matrix
################################################################################
mm_read:
  # save return address and any saved registers on the stack, if necessary
  sw $ra, 0($sp) #store return address on stack
  sub $sp, $sp, 4

  # get N
  li $v0, 5 
  syscall #read int
  move $t0, $v0 #set up num_rows (N) for mm_alloc

  # get M
  li $v0, 5
  syscall #read int
  move $t1, $v0 #set up num_cols (M) for mm_alloc
  
  # Setup up arguments and call mm_alloc - v0 is returned as base address
  move $a0, $t0   #$a0 = N
  move $a1, $t1   #$a1 = M 
  sw $t0, 0($sp)
  sub $sp, $sp, 4
  sw $t1, 0($sp)
  sub $sp, $sp, 4
  jal mm_alloc

  add $sp, $sp, 4
  lw $t1, 0($sp)
  add $sp, $sp, 4
  lw $t0, 0($sp)

  # do nested loops to read in values
  #setup loop values/conditions
  move  $t2, $v0      #t2 points to base address of matrix
  move $t5, $t2 #will not be modified :)

  mult	$t0, $t1			# $t0 * $t1 = how many values in matrix
  mflo	$t3					  # copy Lo to $t3, this is our counter
  
  
read_loop: #estoy loopin
  beq $t3, $zero, read_loop_exit #if counter is zero, we are done!
  li $v0, 5
  syscall #read int
  sw $v0, 0($t2) #store on heap (in matrix)
  addi $t2, $t2, 4 #advance to next address
  sub $t3, $t3, 1 #decrement!
  j read_loop
  
read_loop_exit:

  # setup up return values
  # Note: third return value goes on the stack *after* restoration below
  move $v0, $t0
  move $v1, $t1
  
  # restore stack, ra, and any saved registers, if necessary
  addi $sp, $sp, 4
  lw $ra, 0($sp) #return address restored B)
  sw $t5, 0($sp) #third return value, base address of allocated matrix
  
  # return to main
  jr  $ra

################################################################################
# mm_alloc: Allocate an NxM matrix
# Note: You can just allocate a single array of length N*M.
# Input:
#   a0 = N
#   a1 = M
# Return:
#   v0 = base address of allocated space
################################################################################
mm_alloc:
  # save return address and any saved registers on the stack, if necessary

  # Allocate sufficient space
  mult $a0, $a1
  mflo $t0 #find number of values to store
  li $t1, 4 

  mult $t0, $t1
  mflo $a0 #$a0 holds number of bytes to allocate

  # set return value
  li $v0, 9
  syscall #$v0 contains address of allocated memory
  
  move $t2, $v0
  add $t3, $a0, $v0 #t3 is address of the final location

alloc_loop: #zero the memory!
  beq $t2, $t3, alloc_loop_exit 
  sw $zero, 0($t2) #v0[i] = 0
  addi $t2, $t2, 4
  j alloc_loop
alloc_loop_exit:
  # restore stack, ra, and any saved registers, if necessary

  # return to main
  jr  $ra

################################################################################
# mm_print: Output the formatted contents of NxM matrix
# Note: Within a row, separate each value by tab
#       End each row with a newline
# Input:
#   a0 = N
#   a1 = M
#   a2 = base pointer
# Return:
#   None
################################################################################
mm_print:
  # save return address and any saved registers on the stack, if necessary
  move $t9, $a1
  mult $a0, $a1
  mflo $t0 #t0 stores number of values

  # do nested loops to print out values 
  move $t1, $a2 #t1 points to the same memory address as a2
  add $t2, $zero, 1
  add $t0, $t0, -1

print_loop: 
  blt $t0, $zero, print_loop_exit 
  

  lw $a0, 0($t1) #load the current value into $a0
  li $v0, 1 
  syscall #print value

  #time for modulus B)
  div $t0, $t9
  mfhi $t3 #t3 contains the remainder

  li $v0, 11 #print char syscall
  li $a0, 9
  syscall #print tab character


  bne	$t3, $zero, skip_newline 

  li $a0, 10
  syscall #print newline character

skip_newline:

  addi $t1, $t1, 4 #increment address
  addi $t0, $t0, -1 #decrement counter
  j print_loop
print_loop_exit:

  # restore stack, ra, and any saved registers, if necessary

  # return to main
  jr  $ra

################################################################################
# mm_mult: Perform the actual NxM matrix multiplication
# Input:
#   a0 = N for matrix 1 & N in result matrix
#   a1 = M for matrix 1 <-> N for matrix 2
#   a2 = base address for matrix 1
#   a3 = M for matrix 2 & M in result matrix
#   0($sp) = base address for matrix 2
# Return:
#   v0 = base address for result matrix
################################################################################
mm_mult:
  # save return address and any saved registers on the stack, if necessary
  # freeing $a0 and $a1 for mm_alloc call
  move $t0, $a0
  move $t1, $a1
  lw $t2, 0($sp) #t2 is the base address for matrix 2
  
  sub $sp, $sp, 4
  sw $ra, 0($sp) #store return address on stack

  # set up and call mm_alloc for result matrix
  sub $sp, $sp, 4
  sw $t0, 0($sp)
  sub $sp, $sp, 4
  sw $t1, 0($sp)
  sub $sp, $sp, 4
  sw $t2, 0($sp)

  move $a1, $a3
  jal mm_alloc

  lw $t2, 0($sp)
  add $sp, $sp, 4
  lw $t1, 0($sp)
  add $sp, $sp, 4
  lw $t0, 0($sp)
  add $sp, $sp, 4

  # three nested loops for matrix-matrix multiplication
  move $v1, $zero
mult_loop_i:
  slt $t9, $v1, $t0 #set if counter1 is less than N for matrix 1
  beq $t9, $zero, exit_mult
  move $t3, $zero
mult_loop_j:
  slt $t9, $t3, $a3
  beq $t9, $zero, end_loop_j
  move $t4, $zero
mult_loop_k:
  slt $t9, $t4, $t1
  beq $t9, $zero, end_loop_k

  ##### LOOP BODY #####
  #construct $t7 (offset in result matrix)
  li $t8, 4
  mult $a3, $v1 #num cols in result * i
  mflo $t7
  mult $t7, $t8 #number of bytes in t7 ints
  mflo $t7
  mult $t3, $t8 #j * 4
  mflo $t8
  add $t7, $t7, $t8 #done!

  #construct $t6 (offset in matrix 2)
  li $t8, 4
  mult $a3, $t4 #num cols in mat2 * k
  mflo $t6
  mult $t6, $t8 #number of bytes in t6 ints
  mflo $t6
  mult $t3, $t8 #j * 4
  mflo $t8
  add $t6, $t6, $t8 #done!

  #construct $t5 (offset in matrix 1)
  li $t8, 4
  mult $t1, $v1 #num cols in mat1 * i
  mflo $t5
  mult $t5, $t8 #number of bytes in t5 ints
  mflo $t5
  mult $t4, $t8 #k * 4
  mflo $t8
  add $t5, $t5, $t8 #done!

  #convert offsets into memory addresses
  add $t7, $t7, $v0 
  add $t6, $t6, $t2
  add $t5, $t5, $a2

  #make $t5 into the value to be added to the result matrix
  lw $t5, 0($t5)
  lw  $t6, 0($t6)
  mult $t5, $t6
  mflo $t5
  lw $t6, 0($t7)
  add $t6, $t6, $t5 #and add it!
  sw $t6, 0($t7) #put it back :)
  ### END LOOP BODY ###
  addi $t4, $t4, 1
  j mult_loop_k

end_loop_k:
  addi $t3, $t3, 1
  j mult_loop_j

end_loop_j:
  addi $v1, $v1, 1
  j mult_loop_i

exit_mult:

  # set return value
  #$v0 is the return value already
  # restore stack, ra, and any saved registers, if necessary
  lw $ra, 0($sp) #return address restored B)
  addi $sp, $sp, 4
  # return to main
  jr  $ra