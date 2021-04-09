# CA Lec 5

#### Conditional Operations(Register가 저장하고 있는 주소값을 비교하는 것이 아니라 주소가 참고하고 있는 값들을 비교하는 것)

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

- Signed comparison:slt, slti => signed bit로 간주하고 비교연산진행
- Unsigned comparison: sltu, sltui => unsigned bit로 간주하고 비교연산진행

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

## 4. ISA2

#### Procedure Calls

- Stack (memory)
  - 각각의 procedure call들은 Stack Frame을 만든다.
  - Function이 종료되면 Stack Pointer를 옮김으로서 Stack Frame을 제거한다.

#### Procedure들이 실행되는 6가지 Step

1.  Callee가 접근할 수 있도록 Caller가 Parameter들을 배치한다. 

   \* argument 저장하는 register: \$a0 - \$a3 => arg가 4개 이상이면 stack memory에 저장공간 확보(다시 검토) 

2. Caller는 callee가 통제할 수 있도록 함

3. Callee는 저장공간을 얻음

4. Callee는 요구되는 Task들을 수행함 

5. Callee는 caller가 접근 가능한 result를 저장할 수 있는 곳을 가짐

   \* 저장값들을 위한 두개의 registers: \$v0 - \$v1

6. callee가 호출이 끝났을 때 caller로 되돌아 감

   \* 되돌아갈 주소를 저장하는 register: \$ra

#### MIPS Register Convention

| Name     | Register Name | Usage                  | Preserve on call? | 비고                                                         |
| -------- | ------------- | ---------------------- | ----------------- | ------------------------------------------------------------ |
| $zero    | 0             | Constant 0 (Hardware)  | n.a.              | 0이라는 것을 나타내기 위해 상수로 따로 저장                  |
| $at      | 1             | Reserved for assenbler | n.a.              |                                                              |
| $v0-\$v1 | 2-3           | returned values        | no                |                                                              |
| $a0-\$a3 | 4-7           | arguments              | yes               | 함수안에 함수가 존재하여 이 caller의 함수의 argument를 저장하기 위해 사용 |
| $t0-\$t7 | 8-15          | temporaries            | no                |                                                              |
| $s0-\$s7 | 16-23         | saved values           | yes               |                                                              |
| $t8-\$t9 | 24-25         | temporaries            | no                |                                                              |
| $gp      | 28            | global pointer         | yes               |                                                              |
| $sp      | 29            | stack pointer          | yes               |                                                              |
| $fp      | 30            | frame pointer          | yes               |                                                              |
| $ra      | 31            | return addr (Hardware) | yes               |                                                              |

#### Instructions for Accessing Procedures

**jal ProcedureAdress** # jump and link

- jal 다음 수행되는 instruction들에 연결하기 위해 \$ra register에 PC(Program Counter)+4 를 저장한다.
- J format을 사용한다.

| 6bit    | 26bit     |
| ------- | --------- |
| Op code | 주소 코드 |

**jr \$ra**  # return, 

- return 해서 caller로 돌아가는 instruction

- \$ra에 PC값 저장

- R format을 사용한다.

#### Spilling Register

Stack이 쌓이는 원리

: 점점 높은 Address에서 낮은 Address로 쌓이며 Procedure들이 쌓일 수록 \$sp는 점점 낮아진다.

- stack에 data 더하기(push)

  => \$sp = \$sp - 4 

- stack에 data 빼기(pop)

  => \$sp = \$sp + 4



















