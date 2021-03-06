# CA Lec4 

#### 산술연산을 하기 위해

메모리 -> 레지스터 -> 연산 -> 레지스터 -> 메모리    (Data Flow)

- 메모리는 Byte 단위로 저장된다.

- Word : 4byte 데이터 덩어리

즉 Address는 반드시 4의 배수 형태(Word가 4 byte이므로)

MIPS는 Big Endian 방식

- MSB가 가장 낮은 주소에 있는 방식(Most Significant Byte in the lowest address)

  Good for Comparison

- Little Endian(Least significant byte in the lowest address)은 Big Endian과 반대방식

  Good for Calculate

#### Memory Operand Example1

C code

> g = h + A[8]
>
> g in $s1, h in \$s2, base address of A in \$s3

Compiled MIPS code

>lw \$t0, 32(\$s3)	# load data    
>
>: \$s3(A array의 시작주소: A[0]이 저장되어 있는 주소)에 저장되어 있는 주소에 32만큼 더한 주소에 저장되어 있는 값을  \$t0에 저장하는 것
>
>add \$s1 \$s2 $t0

#### Register vs Memory

- 레지스터는 메모리 보다 접근이 빠르다.



#### Immediate Operands

- 상수 데이터 덧셈(Load를 굳이 하지 않아도 된다.)

> addi \$s3, \$s3, 4     \$s3에 있는 데이터를 4더해서 \$s3에 저장하라
>
> addi \$s3, \$s3, -1     \$s3에 있는 데이터를 1빼서 \$s3에 저장하라

\*  *Make the Common Case Fast - Andal's Law*

#### The Constant Zero

MIPS에서 \$zero라는 레지스터는 constant 0를 저장하고 있으며 overwrite할 수 없다.

- move between registers

  >add \$t1, \$s1, \$zero

#### Unsigned Binary Integers

2의 보수로 나타낸다.(0을 1로 1을 0으로 나타내고 1을 더해준다.)

#### Sign Extension

MSB의 값을 남은 bit에 똑같이 복사하여 할당해준다.

#### MIPS R-format Instruction

**Register Number**: Register가 총 32개($2^2$) 이므로 5bit로 표현이 된다. 이러한 register number가 rs나 rd, rt에 표현된다.

총 32bit Binary number

- R-Format은 주로 연산, Shift 관련 Instruction들이 표현된다.

| op(Operation) | rs    | rt(source) | rd(destination) | shamt(shift amount)\* shift 연산을 하지 않을 경우 사용되지 않음 | funct(Op코드가 같은 것 중에 구별하기 위해, extends opcode) |
| ------------- | ----- | ---------- | --------------- | ------------------------------------------------------------ | ---------------------------------------------------------- |
| 6bits         | 5bits | 5bits      | 5bits           | 5bits                                                        | 6bits                                                      |

ex) **add \$t0, \$s1, \$s2 **

| special(r 타입이라는 것을 표시) | $s1   | \$s2  | \$t0  | 0     | add    |
| ------------------------------- | ----- | ----- | ----- | ----- | ------ |
| 0                               | 17    | 18    | 8     | 0     | 32     |
| 000000                          | 10001 | 10010 | 01000 | 00000 | 100000 |

#### MIPS I-format Instructions

| op    | rs    | rt    | contant or address(offset값을 저장)<br />ex) addi나 lw에서 offset값 저장할 떄 사용 |
| ----- | ----- | ----- | ------------------------------------------------------------ |
| 6bits | 5bits | 5bits | 16bits                                                       |

- Immediate arithmetic and load/store instruction들을 표현(ex) addi, ls등등)
- rt: destination or source register number
- rs: 메모리 참조할 때 시작주소를 저장할 때 사용

\* *Good Design demands goos compromises - Design Principle 4*

- 가능한 비슷하게 Format을 따르게 유지

#### Logical Operations

| Operation   | C    | Java | MIPS      |
| ----------- | ---- | ---- | --------- |
| shift left  | <<   | <<   | sll       |
| shift right | >>   | >>>  | srl       |
| Bitwise AND | &    | &    | and, andi |
| Bitwise OR  | \|   | \|   | or, ori   |
| Bitwise NOT | ~    | ~    | nor       |

- Conditional Operations

  ex) if ~ else,  switch, for loop, goto

  condition이 참이면 labeled instruction으로 이동한다.

  > beq rs, rt, L1=> 만약 rs에 있는 값과 rt에 있는 값이 같으면 L1으로 되어있는 Instuction으로 Branch해라
  >
  > bnq rs, rt, L1=> 만약 rs에 있는 값과 rt에 있는 값이 다르면 L1으로 되어있는 Instuction으로 Branch해라
  >
  > j L1 => 무조건적으로 L1 instruction으로 Branch해라

  