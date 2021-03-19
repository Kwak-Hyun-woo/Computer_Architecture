# CA Lec6

#### Non Leaf Procedures

함수 안에 다른 함수를 호출하는 것: Nested call

- caller의 값(argument나 return 값)들을 Stack에 저장
- call이 끝난이후 caller의 값들을 복원시켜줘야함

#### Memory LayOut

- Static Data: 전역 변수 ex) static variable in C
- Dynamic Data: Heap ex) malloc in C, new in C++
- Stack: 자동 저장소(Procedure 저장), 프로그래머가 명시적으로 메모리를 할당받는 것이 아니라 변수 선언하거나 fucntion call을 하면 자동적으로 사용되었다가 끝나면 자동으로 사라지는 저장소
- Text: program code

#### Byte/ Halfword Operations

녹화본 보면서 다시 이해



####  Memory addressing Modes

- Immediate
- Register indirect
- Displacement: R4 <- Mem[R1 + Disp] ==> MIPS가 이 방식 사용
- Indexed
- Memory indirect
- Auto Increment
- Scaled

#### 32bit Constant

lui rt, constant

- 16bit를 rt에 왼쪽부터 16bit를 채움

ori \$s0, \$s0, 2304

- 나머지 16bit를 2304에 해당하는 값으로 채움

즉 16bit 2개를 32bit로 표현

#### Branch Addressing

Branch Instructions specify(ex) beq )

- Opcode, two registers, target address

대부분의 branch target은 근처에 있다

PC