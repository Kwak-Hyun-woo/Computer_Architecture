# CA Lec 5

#### Conditional Operations

slt rd, rs, rt      \*slt = set 1 less then ~

> if (rs < rt) rd = 1 ; else rd = 0;

slti rt, rs, constant      

> if (rs < constant) rt = 1 ; else rt = 0;

#### 왜 blt, bgt는 없나??(branch less than, branch greater than)

- 크고작음을 비교하는 것이 같지 안같은지 비교하는 것 보다 Hardware적으로 비싼 연산

- branch할 주소계산과 크기비교를 하나의 instruction에 넣는 것은 너무 Heavy함

- beq나 bne는 할 만함

#### Signed VS Unsigned

MSB에 따라 Signed, Unsigned가 결정되고 이에 따라 slt, slti 명령어 결과도 달라짐

#### ISA(MIPS) Design Principles

- Simplicity favors regularity
  - instruction크기가 정해져있다.(32 bit)
  - 작은 instruction형식
  - opcode는 맨 앞 6bit
- Smaller is faster
  - 제한된 instruction 세트
  - 제한된 register 개수
  - 제한된 메모리 주소계산 모드의 수
- Make the common case fast
  - register file에서의 연산 operator ex) addi 
- Good Design demands good compromises
  - 3개의 Instruction 형식

#### Procedure Cell

- Stack
  - 각각의 procedure call들은 Stack Frame을 만든다.
  - Function이 종료되면 Stack Pointer를 옮김으로서 Stack Frame을 제거한다.

#### Procedure들이 실행되는 6가지 Step

1. Caller가 

   \* argument 저장하는 register: \$a0 - \$a3 => arg가 4개 이상이면 stack memory에 저장공간 확보(다시 검토) 

2. Caller는 callee로 controlo를 옮김

3. Callee는 저장공간을 얻음

4. Callee는 요구되는 Task들을 수행함

5. Callee는 caller가 접근 가능한 result를 저장할 수 있는 곳을 가짐

   \* 저장값들을 위한 두개의 registers: \$v0 - \$v1

6. callee가 호출이 끝났을 때 caller로 되돌아 감

   \* 되돌아갈 주소를 저장하는 register: \$ra

















