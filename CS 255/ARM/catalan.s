/* 
THIS CODE WAS MY OWN WORK , IT WAS WRITTEN WITHOUT CONSULTING ANY
SOURCES OUTSIDE OF THOSE APPROVED BY THE INSTRUCTOR . Xavier_Pierce
*/

        .global main

catalan:
        @ a is at [fp, #8]
        @ sum is at [fp, #-4]
        @ i is at [fp, #-8]
        @ return is in r3
        push {lr}
        push {fp}
        mov fp, sp
        
        mov r1, #0 @ sum placed on the stack with value 0
        push {r1}
        add sp, #4

        
ifcond:
        ldr r2, [fp, #8]
        cmp r2, #1

        bgt loop
        @ condition is met
        mov r3, #1 @ return 1 in r3
        pop {fp}
        pop {pc}

loop:

        mov r4, #0
        push {r4} @ i placed on the stack with value 0
        add sp, #4

loopcond:
        ldr r2, [fp, #8]
        ldr r4, [fp, #-8]
        cmp r4, r2

        bge continue
        @ condition is met

        ldr r2, [fp, #-8]
        push {r2} @ send parameters for first recursive call
        bl catalan
        add sp, #4 @ return is in r3

        mov r3, r2
        ldr r5, [fp, #-4] @ r5 = sum

        ldr r4, [fp, #8]
        ldr r2, [fp, #-8]
        sub r4, r4, #1 @ a = a-1
        sub r4, r4, r2 @ r4 = (a-1) - i
        
        push {r4} @ send parameters for second recursive call
        bl catalan
        add sp, #4 @ return is in r3

        mov r3, r1 @ r1 = return of catalan(a-1-i)

        mul r0, r2, r1 @ r0 = catalan(i) * catalan(a-1-i)
        add r5, r5, r0 @ sum = sum + catalan(i) * catalan(a-1-i)
        str r5, [fp, #-4]

        ldr r4, [fp, #-8]
        add r4, #1 @ increment i
        str r4, [fp, #-8]
        b loopcond

        

continue:
        ldr r3, [fp, #-4]
        pop {fp}
        pop {pc}

main:
        movw r1, #:lower16: n
        movt r1, #:upper16: n
        ldr r2, [r1] @ r1 = n

        push {r2}
        bl catalan
        add sp, #4

        movw r0, #:lower16: ans
        movt r0, #:upper16: ans
        str r3, [r0]
        b End
End:
        nop
        
        .data
n: .word 5
ans: .skip 4