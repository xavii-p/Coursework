/* 
THIS CODE WAS MY OWN WORK , IT WAS WRITTEN WITHOUT CONSULTING ANY
SOURCES OUTSIDE OF THOSE APPROVED BY THE INSTRUCTOR . Xavier_Pierce
*/

        .global main

 

        
rem:
        @ a is at [fp, #12]
        @ b is at [fp, #8]
        @ q is at [fp, #-4]
        @ r is at [fp, #-8]
        @ return is in r1
        push {lr}
        

        mov r0, #0 
        push {r0}
        add sp, #4
        
        

        ldr r1, [fp, #12] @ r1 = a
        str r1, [fp, #-8] @ r = a

cond: 
    ldr r2, [fp, #8] @ r2 = b
    ldr r3, [fp, #-8] @ r3 = r
    cmp r3, r2
    blt exit
    @ condition is met
    ldr r0, [fp, #-4]
    add r0, #1 @ q++
    str r0, [fp, #-4]
    sub r3, r3, r2 @ r - b
    str r3, [fp, #-8] @ r = r - b
    b cond


exit: 
        ldr r1, [fp, #-8] @ r1 = r
        pop {pc}
        

gcd:
        @ a is at [fp, #12]
        @ b is at [fp, #8]
        @ r is at [fp, #-4]
        @ return is in r2
        push {lr}
        push {fp}
        mov fp, sp 

        ldr r0, [fp, #8] @ r0 = b
        cmp r0, #0
        bne cont
        ldr r2, [fp, #12] @ return a in r2
        b return

cont: 
        ldr r1, [fp, #12] @ r1 = a
        ldr r2, [fp, #8] @ r2 = b

        push {r1}
        push {r2}
        bl rem
        add sp, #8 @ r1 = r (remainder)

        str r1, [fp, #-4] @ r = rem(a, b)
        

        ldr r2, [fp, #8] @ r2 = b
        ldr r1, [fp, #-4] @ r1 = r


        push {r2}
        push {r1}
        bl gcd
        add sp, #8
        b return
return:
        pop {fp}
        pop {pc}
        
        

main:
        mov r5, #70
        mov r6, #20

        push {r5}
        push {r6}
        bl gcd
        add sp, #8

        movw r0, #:lower16: ans
        movt r0, #:upper16: ans
        str r2, [r0] @ ans = r2 (the return of gcd)

        .data
ans: .skip 4