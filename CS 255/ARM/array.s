/* 
THIS CODE WAS MY OWN WORK , IT WAS WRITTEN WITHOUT CONSULTING ANY
SOURCES OUTSIDE OF THOSE APPROVED BY THE INSTRUCTOR . Xavier_Pierce
*/

        .global main

main:
    
loop:
        movw r0, #:lower16: i
        movt r0, #:upper16: i
        ldr r1, [r0]

        movw r2, #:lower16: len
        movt r2, #:upper16: len
        ldr r3, [r2]

        cmp r1, r3
        bge continue
        @ condition is met
        movw r4, #:lower16: sum
        movt r4, #:upper16: sum
        ldr r5, [r4] @ r5 = sum

        movw r2, #:lower16: arr
        movt r2, #:upper16: arr
        

        mul r4, r1, #4 @ array offset depending on value of i (r1)

        ldr r2, [r2, r4] @ load value in arr stored at i into r2 ; r2 = arr[i]

        add r5, r5, r2 
        str r5, [r4] @ sum = sum + arr[i]

        add r1, r1, #1 @ increment i
        str r1, [r0]
        b loop
        

continue:
    b End
End:
    nop


        .data
arr:  .word 1
      .word 2
      .word 3
      .word 4
      .word 5

len: .word 5
i: .word 0
sum: .word 0
