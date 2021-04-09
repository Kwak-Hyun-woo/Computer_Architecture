# CA Lec9(3.25)

**빨간색 선으로 표현된 줄이 해당 Instruction이 작동할 때 관여하는 것들**

### R-Type Instruction Data/Control Flow

![R-type-DataControl-Unit](C:\Users\pc\OneDrive - dgist.ac.kr\바탕 화면\DGIST\3 - Junior\1st semester\Computer Architecture\img\R-type-DataControl-Unit.JPG)

- Memory는 건드리지 않기 때문에 **MemWrite, MemRead**등을 사용하지 않는다. 
- **RegWrite: 1**
- **MemToReg: 0**
- **ALUsrc: 0**
- **PCsrc: 0(branch를 하는 것이 아니므로)**
- **RegDst: 1(rd 선택)**

\* R type instruction

| opcode | rs    | rt    | rd    | shamt | funct |
| ------ | ----- | ----- | ----- | ----- | ----- |
| 31:26  | 25:21 | 20:16 | 15:11 | 10:6  | 5:0   |

**R type 이므로 Register Destination은 rd를 사용**

주의) lw는 rt가 Register Destination



### Load Word Data/Control Flow

![Load word-DataControl-Unit](C:\Users\pc\OneDrive - dgist.ac.kr\바탕 화면\DGIST\3 - Junior\1st semester\Computer Architecture\img\Load word-DataControl-Unit.JPG)

- **MemWrite: 0 MemRead: 1(memory를 읽어서 reigster에 저장하는 것이므로 읽기만하고 쓰지는 않는다.)**
- **RegWrite: 1(Memory의 data를 register에 쓰는 것이므로)**
- **MemToReg: 1(Memory의 data를 register에 쓰는 것이므로)**
- **ALUsrc: 1(offset 연산)**
- **PCsrc: 0(branch가 일어나는 것이 아니므로)**
- **RegDst: 0(rt 선택)**

\* Load instruction

| opcode | rs    | rt    | offset |
| ------ | ----- | ----- | ------ |
| 31:26  | 25:21 | 20:16 | 15:0   |



### Store Word Data/Control Flow

![Store-DataControl-Unit](C:\Users\pc\OneDrive - dgist.ac.kr\바탕 화면\DGIST\3 - Junior\1st semester\Computer Architecture\img\Store-DataControl-Unit.JPG)

- **MemWrite: 1 MemRead: 0(Register 값을 읽어서 Memory에 저장하는 것이므로)**
- **RegWrite: 0(Register Write를 하는 것이 아니므로)**
- **MemToReg: x(Register를 변경하는 것이 아니므로)**
- **ALUsrc: 1(offset 연산해서 Memory 주소를 계산해야하므로)**
- **PCsrc: 0(branch가 일어나는 것이 아니므로)**
- **RegDst: x(rs와 rt 둘다 read 해야하므로)**

\* Store instruction

| opcode | rs    | rt    | offset |
| ------ | ----- | ----- | ------ |
| 31:26  | 25:21 | 20:16 | 15:0   |



### Branch Instruction Data/Control Flow

![Branch-DataControl-Unit](C:\Users\pc\OneDrive - dgist.ac.kr\바탕 화면\DGIST\3 - Junior\1st semester\Computer Architecture\img\Branch-DataControl-Unit.JPG)

- **MemWrite, MemRead, MemToReg: x(memory를 다루지 않으므로)**
- **RegWrite: 0 (Register Write를 하는 것이 아니므로)**
- **ALUsrc: 0 (rs와 rt값을 비교하는 것이므로 ALU에 rt값이 들어가야한다.)**
- **PCsrc: 조건이 참이면 1(offset으로 연산된 Branch Target Address), 거짓이면 0**
  - **Branch: 1**
  - **Zero: rs와 rt를 뺐을 때 값이 0이면(rs와 rt가 같으면) 1 아니면 0 **
  - **ALUOp: Subtract**
- **RegDst: x(rs와 rt 둘다 read 해야하므로)**

\* beq instruction

| opcode | rs    | rt    | offset |
| ------ | ----- | ----- | ------ |
| 31:26  | 25:21 | 20:16 | 15:0   |

### Adding the Jump Operation

![Jump-DataControl-Unit](C:\Users\pc\OneDrive - dgist.ac.kr\바탕 화면\DGIST\3 - Junior\1st semester\Computer Architecture\img\Jump-DataControl-Unit.JPG)

- **Jump: 1**

\* Jump instruction

| opcode | target address |
| ------ | -------------- |
| 31:26  | 25:0           |



## 6. PipeLine

#### Instruction Critical Path

\* Muxes, Control Unit, Sign Extension, PC access, shift left 2, wires, setup 등에 대한 Delay들은 무시할만큼 매우 작다고 가정

<각 stage들은 아래와 같은 시간만큼 걸린다고 가정>

- Instruction and Data memory: 200ps
- ALU and Adder: 200ps
- Register File Access(read & Write) : 100ps   

![Instruction Critical Path](C:\Users\pc\OneDrive - dgist.ac.kr\바탕 화면\DGIST\3 - Junior\1st semester\Computer Architecture\img\Instruction Critical Path.JPG)

- #### Single Cycle 설계의 장단점

  **가장 느린 Instruction에 맞춰 Clock Cycle시간이 정해져야한다.**

  	- 장점: 이해하기 쉽다.

  	- 단점: 성능 효율이 낮다. ex) load instruction이 800ps 걸리기 때문에 Clock Cycle time을 800으로 맞추면 jump같은 200ps 걸리는 instruction들은 나머지 600ps 동안 놀게 된다.

#### Performance에 대한 Issue

1. **가장 긴 Instruction이 Clock Period를 결정한다.**
2. **매 Instruction이 수행될 때 마다 Clock Period를 바꾸는 것은 기술적으로 불가능하다.**
3. **Pipelining을 통해 이러한 문제들을 해결할 수 있다.**



#### Pipelining에 대한 기본적인 이해

- A, B, C, D가 각각 빨래를 하려고한다.
- Washer, Dryer, Folder, Stasher의 각각의 단계는 30분씩 걸린다.

<Without Pipelining\> 

![](C:\Users\pc\OneDrive - dgist.ac.kr\바탕 화면\DGIST\3 - Junior\1st semester\Computer Architecture\img\LaundarySchedule.JPG)

<With Pipelining\> 

![LaundarySchedule_Pipelining](C:\Users\pc\OneDrive - dgist.ac.kr\바탕 화면\DGIST\3 - Junior\1st semester\Computer Architecture\img\LaundarySchedule_Pipelining.JPG)

#### :star:Pipelining을 통해 성능향상을 할 수 있는 원리 :star: (매우 중요)

- **CPU의 성능에 관한 방정식은 다음과 같다.**
  $$
  CPU\,Time = CPI * CC * IC \\
  $$

  $$
  * CPI : Clock\ Cycle / Instruction \\
  * CC : Clock\ Cycle \ Time\\
  * IC: The\ number\ of\ Instructions(Instruction Count)
  $$

  - **CPI: Instruction별 연산을 수행하는 Cycle은 Pipelining을 할 때랑 안할때가 같다. 주의: 한 Cycle이 차지하는 시간은 다르다.**

    ex) 빨래의 경우

    Without Pipelining:   1cycle(2시간)        With Pipelining: 1cycle(30분)

  - **IC(Instruction 개수):  마찬가지로 Pipelining을 할때와 안할 때가 같다.**

  - **CC(Clock Cycle Time): Pipelining을 할때와 안 할때가 다르며 CC로 인해 성능이 향상된다!!!!!!**

- 가장 이상적인 조건에서 수행해야할 Instruction수가 많을 때, **Pipelining을 통해 얻어지는 성능향상은 Pipe Stage의 개수에 비례한다!!!!!**



#### The Five Stages of Load Instructions

- **IFetch**: Instruction을 Fetch하고 PC를 업데이트 하는과정
- **Dec**: Register를 Fetch하고 Instruction을 Decode하는 과정
- **Exec**: memory 주소를 계산하는 과정
- **Mem**: Data Memory으로부터 값을 읽거나 Data Memory에 값을 쓰는 과정
- **WB(Write Back)**: Register File에 Result값을 다시 쓰는 과정 



#### A Pipelined MIPS Processor

- **개선된 Throughput(처리량)**: 동일 시간내에 해결할 수 있는 일의 양이 증가했다. 
- **그대로인 Instruction들의 Latency(처리시간, 지연시간)**: Pipeline이 도입되었다고 해서 한 Instruction을 수행하는 데 필요한 Stage의 수는 변하지 않기 때문에 Latency도 그대로이다!!!

![Instuction_Pipelining](C:\Users\pc\OneDrive - dgist.ac.kr\바탕 화면\DGIST\3 - Junior\1st semester\Computer Architecture\img\Instuction_Pipelining.JPG)

- 결국 Clock Cycle Time또한 **가장 느린 Stage 시간에 의해 제한된다.**



















