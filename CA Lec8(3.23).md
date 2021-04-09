# CA Lec8(3.23)

### Decoding Instructions Involves

- Opcode와 Fetch bit를 Control Unit에 보내는 행위
- Register FIle에서 두 값을 읽는다.

### Execute

- #### Execute R format

  - Op code와 Funct에 해당하는 연산을 register rs와 rt에 대하여 수행한다. 
  - Register File에 값을 (rd register에 )다시 저장한다.

  - Instruction별로 register에 write할 때도 있고 안할때도 존재

    > add 같은 경우 register에 값을 쓸 때 **Regwrite** 시그널 사용
    >
    > sw의 경우 register의 값을 빌려 memory에 저장하기 때문에 Register에 값을 저장하지 않는다.

  ![R exec](C:\Users\pc\OneDrive - dgist.ac.kr\바탕 화면\DGIST\3 - Junior\1st semester\Computer Architecture\img\R exec.JPG)

- #### Executing  Load and Store Operation

  - register의 base 주소에 16bit offset을 32bit로 Sign Extension 한 값을 연산하여 Operation을 수행한다.
  - **Store**: Register의 값을 읽어서 Data Memory 영역에 저장한다(Write).
  - **Load**: Data Memory에 값을 읽어서 Register File에 값을 저장한다.(Write)

  ![Store, Load exec](C:\Users\pc\OneDrive - dgist.ac.kr\바탕 화면\DGIST\3 - Junior\1st semester\Computer Architecture\img\Store, Load exec.JPG)

- #### Executing Branch Operation

  - Register File로 읽은 Operand들을 비교한다.(두 값을 ALU를 통해 Subtract한 후 값이 0이면 같다는 뜻이므로 zero를 ALU의 output으로 보냄)
  - Update된 PC값에 signed-extented된 16bit를 더하여 Branch Target Address를 계산한다.
  - 다른 Instruction보다 연산량이 많아 조금 Heavy한 Instruction

![Branch Exec](C:\Users\pc\OneDrive - dgist.ac.kr\바탕 화면\DGIST\3 - Junior\1st semester\Computer Architecture\img\Branch Exec.JPG)

- ### Execute Jump Operation

  - Jump Instruction에서 마지막 26bit를 왼쪽으로 2bit shift 한 후(0~27bit) PC의 상위 4bit(31, 30, 29, 28)와 Concatenate 시킨다.

  Shift 2bit를 하는 이유는 Instruction에서 26bit 밖에 없기 때문인데 이는 instruction이 4byte이므로 맨 아래 2bit는 00, 01, 10, 11로 반복되기 때문에 다른 Instruction과 구별할 때 의미가 없기 때문이다.

![Jump Exec](C:\Users\pc\OneDrive - dgist.ac.kr\바탕 화면\DGIST\3 - Junior\1st semester\Computer Architecture\img\Jump Exec.JPG)

#### Creating a Single Datapath

- 한 Clock Cycle에 Fetch, Decode, Execute가 모두 이루어져야한다.
  - 어떤 DataPath도 Instruction당 하나 이상 사용할 수 없다. 따라서 몇몇은 중복될 수 밖에 없는데 이를 위해 Memory들을 분리해놓는다.(예를 들어 beq의 경우 주소계산과 register의 연산이 필요하기 때문에 ALU와 adder를 두개 놓는다.)
  - Line 여러개 Merge되는 경우 Multiplexor들이 필요하다.
  - Register File그리고 Data Memory에 대해 Writing을 통제하는 Write Signal이 있다.
- **한 Cycle Time은 가장 긴 Path에 의해 결정된다.**

#### Full Datapath

![Full datapath](C:\Users\pc\OneDrive - dgist.ac.kr\바탕 화면\DGIST\3 - Junior\1st semester\Computer Architecture\img\Full datapath.JPG)

#### ALU control

- Control Unit에서 신호를 받아 ALU가 어떤 연산을 해야하는지 정해주는 신호를 보내주는 HW
- **Load/ Store** : add를 통해 target memory address를 계산한다.
- **Branch**: subtract를 해서 Reg들이 같은지 아닌지 검사한다.(결과가 0이면 같은것, 아니면 다른것) 
- **R-Type**: Funct Field에 따라 달라짐

- **ALUOp** 

  ALU Control를 위해 Control Unit에서 보내는 신호

  2bit로 이루어졌다.

  | opcode | ALUOp | Operation        | Funct  | ALU function     | ALU control |
  | ------ | ----- | ---------------- | ------ | ---------------- | ----------- |
  | lw     | 00    | load word        | xxxxxx | add              | 0010        |
  | sw     | 00    | store word       | xxxxxx | add              | 0010        |
  | beq    | 01    | branch equal     | xxxxxx | subtract         | 0110        |
  | R-type | 10    | add              | 100000 | add              | 0010        |
  | ``     | ``    | subtract         | 100010 | subtract         | 0110        |
  | ``     | ``    | AND              | 100100 | AND              | 0000        |
  | ``     | ``    | OR               | 100101 | OR               | 0001        |
  | ``     | ``    | set-on-less-than | 101010 | set-on-less-than | 0111        |



