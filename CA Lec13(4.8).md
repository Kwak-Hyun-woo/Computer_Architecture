# CA Lec13(4.8)

#### Reducing the Delay of Branches

: Branch할 Target Address를 계산하고 Branch Decision을 결정하는 과정을 ID stage에서 끝내기 위해서는 추가적인 Hardware가 필요하다.

branch 수행

1. Register의 값을 비교하여 Branch해도 되는 가 결정
2. target address를 계산(**PC + offset<<2**)

- ID stage에서 이미 PC값과 Offset을 알고 다른 Instruction과 Dependency가 존재하지 않으므로 ID stage에서 병렬적으로 동작을 수행할 수 있다. 
- Register를 읽고나기 전까지는 Register의 값들을 비교할 수 없다.

 **Beq에 필요한 Source들을 ID Stage로 가져오기 때문에 Exec에서 Data Hazard가 난 경우와 마찬가지로 ID stage에서도 Data Hazard가 발생할 수 있다. **

#### ID Branch Forwarding Issues

- **MEM/WB에서 Forwarding 하는 경우**

```assembly
<예시>
WB 	add3  $1,
MEM add2  $3,
EX 	add1  $4,
ID 	beq   $1,  $2,	Loop
IF 	next_seq_instr
```

아래와 같이 Cycle을 나눠서 Read하기 전에 먼저 Write한다.

![Split Clock Cycle](C:\Users\pc\OneDrive - dgist.ac.kr\바탕 화면\DGIST\3 - Junior\1st semester\Computer Architecture\img\lec11\Split Clock Cycle.JPG)

- **EX/MEM에서 Forwarding 하는 경우**

```assembly
WB add3 $3,
MEM add2 $1,
EX add1 $4,
ID beq $1,$2,Loop
IF next_seq_instr
```

MEM stage에서는 Register를 쓰지 않고 Memory를 다루기 때문에 WB의 Data를 처리하는 것과는 달리 Forwarding을 통해 해결해야한다.

```pseudocode
if (IDcontrol.Branch
and (EX/MEM.RegisterRd != 0)
and (EX/MEM.RegisterRd = IF/ID.RegisterRs))
	ForwardC = 1
if (IDcontrol.Branch
and (EX/MEM.RegisterRd != 0)
and (EX/MEM.RegisterRd = IF/ID.RegisterRt))
	ForwardD = 1
```

- **EXEC stage에서 값을 받아오는 경우**

Exec stage는 연산을 하는 단계이므로 instrcution에 의해 연산을 수행한 결과가 나오지 않았다. 그러므로 ID stage에 값을 넘겨주기 위해서는 **결과값을 알기 위해 Stall 한번 무조건 넣어줘야한다. ** 

- ID stage의 Instruction(**beq**)과 IF stage의 Instruction의 진행을 멈추기 위해 **IF/ID와 PC를 변경하지 않도록 한다.**

- EXEC stage와 ID stage간 stall을 넣어주기 위해 **ID/EX pipeline register의 control bit들을 0으로 만든다.**

```assembly
WB add3 $3,
MEM add2 $4,
EX add1 $1,
ID beq $1,$2,Loop
IF next_seq_instr
```

위 code에서 만약 EX stage의 instruction이 addl가 아니라 lw였다면 **stall을 2번 해줘야한다!**



**이후 Branch할 준비가 되었다면 IF stage의 instruction을 Flush 시킨다.**



#### Supporting ID stage branches

![Supporting ID stage Branched](C:\Users\pc\OneDrive - dgist.ac.kr\바탕 화면\DGIST\3 - Junior\1st semester\Computer Architecture\img\lec13\Supporting ID stage Branched.JPG)

beq가 ID stage에서 실행이 끝나도록 ID stage에 추가적인 Hardware를 설치한 모습

Hazard Unit에서 Hazard를 감지한 후 PC와 IF/ID register을 조절하고 Stall의 생성여부도 결정하고 IF Flush 여부도 결정한다.



#### Static Branch Predicion

Instruction이 실행되지 않고 Branch 될지 안될지 예측하는 것

**1. Predict not Taken**: 항상 Branch가 일어나지 않을 것이라고 예측하는 것, 차례로 instruction을 Fetch하고 branch가 일어났을때만 stall을 넣어주는 것

- flush된 Instruction들은 기계의 상태(메모리 등등)를 바꾸지 않는 것을 보장한다. 왜냐하면 machine의 state를 바꾸는 stage는 MEM과 WB인데 flush된 instruction들은 그 이전에 처리되기 때문

\* **Branching Structures**

**Predict not taken** 은 **loop의 상단에 위치할 때 잘 동작하지 않는다.!! 왜냐하면 loop가 끝날때만 Branch되니까** 

```assembly
Loop: beq $1,$2,Out
	  1nd loop instr
.
.
.
	  last loop instr
	  j Loop
Out:  fall out instr
```

**Predict not taken** 은 **loop의 하단부에서는 동작을 잘한다**

```assembly
Loop: 1st loop instr
	  2nd loop instr
.
.
.
	  last loop instr
	  bne $1,$2,Loop
	  fall out instr
```

**2. Predict Taken**: 항상 Branch가 일어날 것이라 예측하는 것

- 항상 Branch가 일어날 것이라 예측하므로 항상 1 stall이 발생한다.
- 이러한 정적 예측은 performance를 저하시킨다.

**3. Dynamic Branch Prediction**: 앞선 2가지 예측과 달리 동적으로 상황에 맞게 Branch를 실행시간에 예측한다.

#### Dynamic Branch Prediction

**Branch Prediction Buffer(= Branch History Table(BHT))**에 과거 Branch가 Taken이 됐는지 Not Taken이 됐는지에 대한 정보를 담고있음

- beq의 PC(주소값, instruction x)의 하위 비트만을 보고 BHT에서 하위비트에 해당하는 곳에 taken이 되었으면 1 아니면 0의 값을 넣고 이후 다른 beq가 진행될 때 BHT에서 해당 하위비트에 값이 0이냐 1이냐에 따라 branch를 예측할 수 있다.

\* **주의: Instruction은 4byte이므로 instrction 주소의 하위비트는 ...0000, ...0100, ...1000, ...1100 이렇게 하위 2비트는 00으로 동일하기 때문에 제외시키고 구별되는 bit로 BHT에 mapping하여 Taken여부를 저장.**

- 만약 예측이 틀린다면 잘못된 instruction들을 Flush 시켜준다.

- 또한 Taken으로 예측하더라도 IF stage에 Target Address에 해당하는 Instruction이 바로 Fetch되어야 stall이 삽입되는 것을 막을 수 있다.



#### Branch Target Buffer

- **Untagged 방식**
- taken의 여부를 PC의 하위비트를 Predicted Target Address를 label로 해서 Branch Target Buffer에 저장한다. 
- 그러나 Untagged 방식이므로 lower bit가 서로 일치하는 branch instruction간 entry공유가 발생할 수 있다.

![Branch Target Buffer1](C:\Users\pc\OneDrive - dgist.ac.kr\바탕 화면\DGIST\3 - Junior\1st semester\Computer Architecture\img\lec13\Branch Target Buffer1.JPG)

- **Tagged 방식**
- PC값의 하위 비트들을 제외한 나머지 bit들을 저장하여 lower bit가 일치하더라도 다른 instruction일 경우 나머지 bit들을 비교하여 충돌을 막을 수 있다.

![Branch Target Buffer2](C:\Users\pc\OneDrive - dgist.ac.kr\바탕 화면\DGIST\3 - Junior\1st semester\Computer Architecture\img\lec13\Branch Target Buffer2.JPG)











































