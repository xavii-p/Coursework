/* 
THIS CODE WAS MY OWN WORK , IT WAS WRITTEN WITHOUT CONSULTING ANY
SOURCES OUTSIDE OF THOSE APPROVED BY THE INSTRUCTOR . Xavier_Pierce
*/

        .global main

main:

    @ n is in r0
    @ i is in r2
    @ j is in r4
    @ sum is in r6
        movw r0, #:lower16: n
        movt r0, #:upper16: n
        ldr r1, [r0] @ r1 = n

        movw r2, #:lower16: i
        movt r2, #:upper16: i
        ldrh r3, [r2] @ r3 = i

        movw r4, #:lower16: j
        movt r4, #:upper16: j
        ldrh r5, [r4] @ r5 = j

        mov r3, #0
        strh r3, [r2] @ i = 0 at beginning of outer loop

        
 @ check outer condition
outercond:
        ldrh r3, [r2] @ r3 = i
        ldr r1, [r0] @ r1 = n

        cmp r3, r1 @ i - n < 0
       
        bge exitOuter
        @ condition is met
        
        mov r5, #0
        strh r5, [r4] @ j = 0 at beginning of inner loop

@ check inner condition
innercond:
        ldrh r5, [r4] @ r5 = j
        ldr r1, [r0] @ r1 = n

        cmp r5, r1
        bge exitInner
        @ condition is met
        movw r6, #:lower16: sum
        movt r6, #:upper16: sum
        ldr r7, [r6] @ r7 = sum

        ldrh r3, [r2] @ load in i
        mul r8, r3, r5 @ r8 = i * j
        add r7, r7, r8 @ r7 = sum + (i * j)
        str r7, [r6]

        add r5, r5, #2 @ increment j by 2
        strh r5, [r4]

        b innercond


exitInner:
    ldrh r3, [r2]
    add r3, r3, #1 @ increment i by 1
    str r3, [r2]
    b outercond

exitOuter: 
    b End
End: 
    nop

        .data
n: .word 10
i:  .hword 5
j: .hword 8
sum: .skip 4