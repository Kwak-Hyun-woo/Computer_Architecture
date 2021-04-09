# CA Lec6

#### Non Leaf Procedures

함수 안에 다른 함수를 호출하는 것: Nested call

- caller의 값(**argument나 return 값**)들을 Stack에 저장
- call이 끝난이후 caller의 값들을 복원시켜줘야 함

\* jal Function 

> Function의 PC + 4의 값을 자동적으로 \$ra에 저장된다.

#### Memory LayOut

- Static Data: 전역 변수 ex) static variable in C
- Dynamic Data: Heap ex) malloc in C, new in C++
- Stack: 자동 저장소(Procedure 저장), 프로그래머가 명시적으로 메모리를 할당받는 것이 아니라 변수 선언하거나 fucntion call을 하면 자동적으로 사용되었다가 끝나면 자동으로 사라지는 저장소
- Text: program code

#### Byte/ Halfword Operations

- Sign Extend to 32 bits in rt

  lb rt offset(rs): rs의 1byte만 MSB에 따라 sign extension을 하며 rt에 load 하겠다.

  lh rt offset(rs): rs의 2byte만 MSB에 따라 sign extension을 하며 rt에 load 하겠다.

- Zero Extent to 32 bits in rt

  lbu rt offset(rs): rs의 1byte만 0으로 sign extension을 하며 rt에 load 하겠다.

  lhu rt offset(rs): rs의 2byte만 0으로 sign extension을 하며 rt에 load 하겠다.

- Store just rightmost byte/halfword

  sb rt, offset(rs): rt의 값을 맨 오른쪽에서 1byte만 offset(rs)에 저장하겠다.

  sh rt, offset(rs): rt의 값을 맨 오른쪽에서 2byte만 offset(rs)에 저장하겠다.

####  Memory addressing Modes(별로 안중요함)

- Immediate
- Register indirect
- Displacement: R4 <- Mem[R1 + Disp(=offset)] ==> MIPS가 이 방식 사용
- Indexed
- Memory indirect
- Auto Increment
- Scaled

#### 32bit Constant

lui rt, constant

- 16bit를 rt에 왼쪽부터 16bit를 채움
- 나머지 16bit를 0으로 채운다.

ori \$s0, \$s0, 2304

- 나머지 16bit를 2304에 해당하는 값으로 채움

즉 16bit 2개를 32bit로 표현

#### Branch Addressing

Branch Instructions specify(ex) beq )

- Opcode, two registers(이 두개의 register 값을 비교), target address(jump해야할 곳) 

대부분의 branch target은 근처에 있다

| op    | rs    | rt    | constant or address(Target Address) |
| ----- | ----- | ----- | ----------------------------------- |
| 6bits | 5bits | 5bits | 16bits                              |

PC- relative addressing

-  PC는 beq나 bne가 실행 후 그 다음의 Instruction주소가 된다. 즉 이미 PC는 4가 더해져서 계산된다
- 16bit에 PC + offset * 4가 저장된다.

\* offset에 4를 곱하는 이유는 instruction이 4바이트씩 저장되어 있는데 rightmost의 비트인 00, 01, 10, 11은 모든 instruction에 공통으로 들어있기 때문에 4를 곱해준다. 즉 4를 곱해줌으로서 18bit를 표현하는 효과를 가진다. 

#### Jump Addressing

Jump(j, jal)는 text segment에 어느 곳이든 target으로 삼을 수 있다.

| op(j인지, jal 인지) | address |
| ------------------- | ------- |
| 6bits               | 26bits  |

- (Pseudo) Direct Jump Addressing (완전한 Direct 점프가 아님)

: Target address = PC_31-28bit(상위 4비트) :(concat) Address * 4(위와 마찬가지로 26bit에 4를 곱함으로서 28bit를 표현하는 효과를 가진다.)







