.globl main
main:
  pushl %ebp
  movl %esp, %ebp
  subl $4, %esp
  movl 0(%ebp), %eax
  movl %eax, -4(%ebp)
L0:
  movl -4(%ebp), %eax
  pushl %eax
  movl $10, %eax
  popl %ebx
  cmpl %eax, %ebx
  setl %al
  movzbl %al, %eax
  cmpl $0, %eax
  je L1
  movl -4(%ebp), %eax
  pushl %eax
  movl $1, %eax
  popl %ebx
  addl %ebx, %eax
  movl %eax, -4(%ebp)
  jmp L0
L1:
  movl -4(%ebp), %eax
  pushl %eax
  movl $2, %eax
  popl %ebx
  addl %ebx, %eax
  movl %ebp, %esp
  popl %ebp
  ret
  movl $0, %eax
  movl %ebp, %esp
  popl %ebp
  ret
