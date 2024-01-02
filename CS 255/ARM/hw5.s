/* 
THIS CODE WAS MY OWN WORK , IT WAS WRITTEN WITHOUT CONSULTING ANY
SOURCES OUTSIDE OF THOSE APPROVED BY THE INSTRUCTOR . Xavier_Pierce
*/
        
        .global main
        

main:
        movw r2, #:lower16: a @ store the address of a in r2
        movt r2, #:upper16: a
        ldrsb r3, [r2] @ r3 = val of a

        movw r4, #:lower16: b @ store the address of b in r4
        movt r4, #:upper16: b
        ldrh r4, [r4] @ r4 = val of b

        movw r5, #:lower16: q @ store the address of q in r5
        movt r5, #:upper16: q
        ldr r6, [r5] @ r6 = val of q
        mov r6, #0 @ put 0 in r6
        str r6, [r5] 
        
        movw r7, #:lower16: r @ store the address of r in r7
        movt r7, #:upper16: r
        str r3, [r7] @ r = a
        b Begin
        b Answer

        mov r0, r11 @ copy value of q into r0
        ldr r8, [r7] @ load current val of r from r7 into r8
        mov r1, r8 @ copy value of r into r1

        



Begin:
        ldr r8, [r7] @ load current val of r from r7 into r8
        cmp r8, r4
        bge Body
Body:
        ldr r6, [r5] @ load current value of q into r6 from r5
        add r6, #1 @ increment q by 1
        str r6, [r5] @ store new val of q into r5
        sub r8, r8, r4 @ r - b and store into r8
        str r8, [r7] @ store the val in r8 (r-b) into address of r7 (r)
        b Begin @ exit loop and re-evauluate condition
        
Answer:
        movw r10, #:lower16: ans @ store the address of ans in r10
        movt r10, #:upper16: ans
        strh r6, [r10] @ ans = q
        ldrh r11, [r10] @ r11 = val of ans
        
End: 
        nop
        .data

a:      .4byte 86 @ numerator
b:      .2byte 11 @ denominator
        .align 1
q:      .skip 4 @ quotient
r:      .skip 4 @ remainder
ans:    .skip 2 @answer