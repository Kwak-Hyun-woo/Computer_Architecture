# CA Lec7

#### Array vs Pointers

Array:  index가 Element size 만큼 곱하고 base address에 더해야 한다.

Pointer를 direct로 memory address와 일치시킨다.

=> pointer를 사용하면 Index variable을 제거할 수 있어서 Instruction 개수가 줄어든다.



#### Assemble Pseudoinstructions

프로그래머의 편의를 위해 만들어진 것

ex) blt의 경우 MIPS에서 지원하지 않지만 프로그래머가 문법적으로 쓸 수 있다

blt = less than(slt) + branch(bne)

\* $at(register 1): assembler temporary(Pseudo Instruction에서 실제 Insturction으로 바꿀 때 쓰이는 임시 레지스터



#### Translation and Startup

C program => Compiler => Assembly Language Program => Assembler => Object(Machine Language Module + Library routine) => Linker => 

#### Producing and Object Module

#### Linking Object Modules

#### Loading a Prgram

#### Dynamic Linking



 