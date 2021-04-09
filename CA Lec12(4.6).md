

# CA Lec12(4.6)

#### Datapath with Forwarding Hardware

<img src="C:\Users\pc\OneDrive - dgist.ac.kr\바탕 화면\DGIST\3 - Junior\1st semester\Computer Architecture\img\lec12\Datapath with Forwarding HW.JPG" alt="Datapath with Forwarding HW" style="zoom:50%;" />

### Forwarding with Load-use Data Hazard

<img src="C:\Users\pc\OneDrive - dgist.ac.kr\바탕 화면\DGIST\3 - Junior\1st semester\Computer Architecture\img\lec12\Load-use Data Hazard.JPG" alt="Load-use Data Hazard" style="zoom:50%;" />

**add는 EXEC stage가 끝나야 연산 결과를 알 수 있었던 반면, lw는 MEM stage가 끝나야 Register의 값이 저장된다.**=> Register의 Instruction 결과를 알 수 있는 시기가 lw가 add에 비해 1 cycle느리다.

따라서 Data Dependency가 있는 Instruction이 뒤따라 오게될 경우 1 cycle에서 동시에 Register에 값을 Write하고 Read할 수 없기 때문에 구조적으로 lw와 data dependency를 가진 경우 **무조건 1cycle은 stall 과정**을 거쳐야 한다.

#### Load-use Hazard Detection Unit

lw명령어가 **MEM** stage로 갔을 때  add는 **EXEC**로 넘어가면 안되고 stall을 **EXEC stage**에 넣어줘야 하므로 **ID**에 **Hazard Detection Unit**이 들어가야한다.

```pseudocode
<hazard detection unit>
if (ID/EX.MemRead
and ((ID/EX.RegisterRt = IF/ID.RegisterRs)
or (ID/EX.RegisterRt = IF/ID.RegisterRt)))
stall the pipeline
```

#### Hazard Stall Hardware

stall의 동작방식

Hazard Detection Unit이 **PC와 IF/ID state register 쓰는 것을 통제함**으로서 IF와 ID상태의 instruction들의 진행을 막을 수 있다.

lw instruction(EXEC stage)와 load-use instruction(ID stage) 사이에 **bubble**을 넣어줘야 한다. **즉 EXEC에 noop(no operation)가 되도록 해줘야한다.**

noop가 되도록 해주는 방식: **ID/EX pipeline register에 EX, MEM, WB로 향하는 Control Bit 들을 0으로 설정하여 동작하지 않도록 한다.**

<img src="C:\Users\pc\OneDrive - dgist.ac.kr\바탕 화면\DGIST\3 - Junior\1st semester\Computer Architecture\img\lec12\Adding the Hazard,Stall HW.JPG" alt="Adding the Hazard,Stall HW" style="zoom:50%;" />

### Memory-to-Memory Copies

![Mem-to-Mem copies](C:\Users\pc\OneDrive - dgist.ac.kr\바탕 화면\DGIST\3 - Junior\1st semester\Computer Architecture\img\lec12\Mem-to-Mem copies.JPG)

앞선 Load-use Data Hazard와 마찬가지로 lw에 의해 register에 값이 쓰여지자마자 바로 sw에 의해 읽어지는 경우를 말하는데 이때는 **굳이 stall이 일어날 필요가 없다. 다만 mem/wb register에서 값을 가져올수 있도록 추가적인 HW(MUX)가 필요하다.**

### Code Scheduling to Avoid Stalls

코드의 순서만 바꿔주는 것만으로도 Stall을 피할 수 있고 Cycle 절약할 수 있다.

C code                                      `A = B + E;`                                             `C = B + F;`

<img src="C:\Users\pc\OneDrive - dgist.ac.kr\바탕 화면\DGIST\3 - Junior\1st semester\Computer Architecture\img\lec12\Code Scheduling to Avoid Stalls.JPG" alt="Code Scheduling to Avoid Stalls" style="zoom:50%;" />

## 7. Pipeline Control Hazard

#### Control Hazards

발생되는 원인

- Unconditional Branches ( ex) j, jal, jr)
- Conditional Branches ( ex) beq, bne)
- Exceptions ( ex) Network에서 Packet이 날라와서 처리해야하는 경우 )

가능한 해결방법

- Stall을 통해 해결할 수 있다. => CPI가 증가한다.

- 이동해야하는 address를 결정하는 단계를 pipeline상에서 최대한 앞당긴다.

- Branch Decision을 늦춘다: Branch와 상관없는 Instruction들(ex) )을 branch instruction뒤에 배치한다.

- Branch할 것이라는 것을 미리 예측한다.

#### Datapath Branch and Jump Hardware

<img src="C:\Users\pc\OneDrive - dgist.ac.kr\바탕 화면\DGIST\3 - Junior\1st semester\Computer Architecture\img\lec12\Datapath Branch and jump HW.JPG" alt="Datapath Branch and jump HW" style="zoom:50%;" />

### Jumps Incur One Stall

<img src="C:\Users\pc\OneDrive - dgist.ac.kr\바탕 화면\DGIST\3 - Junior\1st semester\Computer Architecture\img\lec12\Jumps Incur One Stall.JPG" alt="Jumps Incur One Stall" style="zoom:50%;" />

Jump Instruction **ID stage에서 target Address가 결정되므로 IF에 무조건 다른 Instuction이 Fetch되기 때문에 반드시 Flush라는 과정이 필요하다.**

**Flush**: IF/ID 단계의 Register의 값을 0으로 만들어 IF 단계에 있는 Instruction을 작동 못하도록 막는다.

#### Stall의 2가지 형태

- Pipeline내에서 2 instruction 사이에 **noop을 삽입하는 것**

  구현방법: 앞의 Stage에 있는 Instruction들을 붙들어준다. 이후 noop를 EXEC stage에 넣어준뒤 Pipeline이 동작하게 만들어주면 된다.

- Pipeline내에서 한 instruction을 **noop으로 대체하는 것(= Flushes, Instruction Squashing)**

  구현방법: Flush하려는 Instruction의 **Control Bit를 0으로 만든다.**

<img src="C:\Users\pc\OneDrive - dgist.ac.kr\바탕 화면\DGIST\3 - Junior\1st semester\Computer Architecture\img\lec12\Supporting ID state jump.JPG" alt="Supporting ID state jump" style="zoom:50%;" />

#### Branching Control Hazards

- Target Address를 계산하고 그것에 대한 결과 반영이 MEM stage에서 일어남

<img src="C:\Users\pc\OneDrive - dgist.ac.kr\바탕 화면\DGIST\3 - Junior\1st semester\Computer Architecture\img\lec12\branch1.JPG" alt="branch1" style="zoom:50%;" />

- 3 cycle을 Flush하는 방법 => CPI에 영향을 미침

<img src="C:\Users\pc\OneDrive - dgist.ac.kr\바탕 화면\DGIST\3 - Junior\1st semester\Computer Architecture\img\lec12\branch12JPG.JPG" alt="branch12JPG" style="zoom:50%;" />

- branch instruction의 **branch decision시기를 ID stage로 앞당긴다!** => jump랑 동일하게 처리

<img src="C:\Users\pc\OneDrive - dgist.ac.kr\바탕 화면\DGIST\3 - Junior\1st semester\Computer Architecture\img\lec12\branch3JPG.JPG" alt="branch3JPG" style="zoom:50%;" />

**Branch의 지연시간을 줄이기**

=> **Target Address를 계산하고 Register File의 값을 비교하는 과정을 1 cycle안에 끝내기 위해 병렬적으로 처리해야하고 그에따라 추가적인  hardware가 필요하다**











































