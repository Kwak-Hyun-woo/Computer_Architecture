# CA Lec11(4.1)

#### Cause of Hazard

- **아래 그림속 Blue Line처럼 왼쪽에서 시작하여 오른쪽으로 전달되는 Operation결과가 있을 때 Hazard를 발생시킨다.**

![MIPSpipeline](C:\Users\pc\OneDrive - dgist.ac.kr\바탕 화면\DGIST\3 - Junior\1st semester\Computer Architecture\img\lec11\MIPSpipeline.JPG)



#### Single-Cycle Pipeline Diagram

![Single Cycle Pipelining Diagram](C:\Users\pc\OneDrive - dgist.ac.kr\바탕 화면\DGIST\3 - Junior\1st semester\Computer Architecture\img\lec11\Single Cycle Pipelining Diagram.JPG)

:heavy_check_mark: **각각의 State Register에 저장되어 있는 instruction정보들은 다음과 같다.**

**IF/ID**: ID stage에 있는 Instruction 정보 저장

**ID/EX**: EX stage에 있는 Instruction 정보 저장

**EX/MEM**: MEM stage에 있는 Instruction 정보 저장

**MEM/WB**: WB stage에 있는 Instruction 정보 저장

\* state register 이름 뒷부분의 state의 Instruction 정보가 저장된다.



#### Pipelined Control

- Instruction Signal로부터 Control Signal들도 전달된다!



<img src="C:\Users\pc\OneDrive - dgist.ac.kr\바탕 화면\DGIST\3 - Junior\1st semester\Computer Architecture\img\lec11\Pipeline ControlSignal.JPG" alt="Pipeline ControlSignal" style="zoom:40%;" /><img src="C:\Users\pc\OneDrive - dgist.ac.kr\바탕 화면\DGIST\3 - Junior\1st semester\Computer Architecture\img\lec11\Pipelined Control.JPG" alt="Pipelined Control" style="zoom:50%;" />



#### Pipeline Hazard

- **Structural Hazards**

  : 서로다른 두 Instruction들이 동시에 같은 resource를 사용하려는 시도

- **Data Hazards** :star: **(중요)**

  : Data(Register 내 값)가 준비되기 전에 사용하려는 시도

  ex)

```assembly
add	r1 	r2 	r3
add	r4 	r1 	r3
```

일 때 r2내 값과 r3내 값을 더하여 r1에 저장되고 난 후 다음 add가 실행되어야 하는 데 저장되기 전에 r1에 접근하려고 할 때 Data Hazard 발생

```assembly
lw	r1	,0(r2)
add r3, r2, r1
```

일 때 memory의 0(r2)주소에 있는 data를 r1에 저장된 후 add가 r1을 읽어 연산해야하는 데 저장되기전에 r1에 접근하려고 할 때 Data Hazard 발생

- **Control Hazards:star: (중요)**

  : Condition이 맞는지 안맞는지 결정하기 전 또는 Target Address를 계산하기 전에 Branch할지 말지 결정하기위한 시도

  ex) `Beq`, `Bne` 같은 Branch하는 Instruction의 경우 Target Address에 해당하는 Instruction이 다음에 실행되어야 하는 데 Pipelining 때문에 이전 Cycle에 있는 Instruction이 실행될 때 Control Hazard발생

  ex)

```assembly
		beq	r1, r2, loop
		inst1 ...
		inst2 ...
		inst3 ...
		...
loop:	add	r1,	r2, r3
```

위와 같은 상황일 때 beq실행 후 loop로 이동해야 하는데 pipelining으로 인해 inst1, inst2, inst3가 실행되므로 이럴 때 Control Hazard 발생

\* 이러한 Hazard 들을 해결하기 위해서 두가지 조건이 필요하다.

1. **Pipeline Control은 반드시 Hazard들을 찾아내야한다!**
2. **Hazard를 해결하기 위해 추가적인 HW(ex) MUX, Adder..etc)가 필요하다.**



### 해결방법!!!!!

#### 1. Memory(Cache)를 Instruction영역을 관리하는 memory(Cache)와 Data를 관리하는  Memory(cache) 2개로 나눈다.

아래 그림에서 파란색으로 표시된 부분을 보면 각각 다른 Instruction들이 한 Memory를 동시에 접근하므로 **Structural Hazard** 발생하는 데 이를 해결하기 위해 memory(일반적으로 L1 cache)를 **I cache(Instruction 관리), D cache(Data 관리)**로 나눈다. 

![Structural Hazard](C:\Users\pc\OneDrive - dgist.ac.kr\바탕 화면\DGIST\3 - Junior\1st semester\Computer Architecture\img\lec11\Structural Hazard.JPG)



#### 2. 한 Cycle을 2개로 나누어 전반부(그림 파란색 부분)는 Write하게 하고, 후반부(그림 초록색 부분)는 Read하게 한다.

- 다른 Stage에 비해 **Register를 Read하는 Stage(Dec, ID)**와 **Write Back하는 Stage(WB)**는 수행 시간이 다른 시간에 비해 **절반**

  ![Split Clock Cycle](C:\Users\pc\OneDrive - dgist.ac.kr\바탕 화면\DGIST\3 - Junior\1st semester\Computer Architecture\img\lec11\Split Clock Cycle.JPG)

- **Data Hazard에서 초록색 부분은 이러한 방법으로 해결할 수 있지만 파란색 부분은 아직 이방법으로 해결하지 못한다.!**	

<img src="C:\Users\pc\OneDrive - dgist.ac.kr\바탕 화면\DGIST\3 - Junior\1st semester\Computer Architecture\img\lec11\Read Before Write DataHazard.JPG" alt="Read Before Write DataHazard" style="zoom:30%;" /><img src="C:\Users\pc\OneDrive - dgist.ac.kr\바탕 화면\DGIST\3 - Junior\1st semester\Computer Architecture\img\lec11\Load-Use Data hazard.JPG" alt="Load-Use Data hazard" style="zoom:30%;" />



#### 3. Data Hazard를 해결하기 위해 위 그림에서 초록색 영역까지 도달하도록 기다려준다.(stall을 한다.)

- 이렇게 될 경우 그림에서 **add의 CPI가 2 증가하므로 CPI에 영향을 미쳐서 성능이 나빠질 수 있다.**

![Stall](C:\Users\pc\OneDrive - dgist.ac.kr\바탕 화면\DGIST\3 - Junior\1st semester\Computer Architecture\img\lec11\Stall.JPG)

#### 4. Data Hazard를 해결하기 위해 ALU의 결과값을 WB까지 가지 않고 Exec stage에서 바로 다음instruction을 위해 전달(Forwarding)해준다.

![Data Forwarding](C:\Users\pc\OneDrive - dgist.ac.kr\바탕 화면\DGIST\3 - Junior\1st semester\Computer Architecture\img\lec11\Data Forwarding.JPG)

- ALU에 Forwarding으로 인해  State Register의 값들도 Input이 될 수 있다.
  - ALU의 늘어난 Input들로 인해 **Multiplexor들을 추가해줘야 함**
  - **EX/MEM 또는 MEM/WB**의 Rd data들을 **Exec stage**의 rs, rt를 선택하는 ALU의 MUX input에 연결한다.
  - 추가된 Multiplexor들을 통제할 Control Signal들이 필요하다.
- Stall을 사용하는 것과 다르게 **CPI가 1을 만족한다.**

- **Data Forwarding Control Conditions**

  1. **EX** Forwarding Unit

  ```pseudocode
  if (EX/MEM.RegWrite						
  # MEM stage에 있는 Inst가 RegWrite하는 Inst면 1, 아니면 0
  
  and (EX/MEM.RegisterRd != 0)
  # MEM state에 있는 Inst에 의해 계산된 Rd 값이 존재할 때
  
  and (EX/MEM.RegisterRd = ID/EX.RegisterRs))
  # Exec의 Inst가 Read하려는 Register(rs)와 MEM의 Inst가 Write하려는            # Register(rd)가 같을 때 Data Forwarding 
  ForwardA = 10
  
  if (EX/MEM.RegWrite
  and (EX/MEM.RegisterRd != 0)
  and (EX/MEM.RegisterRd = ID/EX.RegisterRt))
  # Exec의 Inst가 Read하려는 Register(rt)와 MEM의 Inst가 Write하려는            # Register(rd)가 같을 때 Data Forwarding 
  ForwardB = 10
  ```

  

  2. **MEM** Forwarding Unit

  ```pseudocode
  if (MEM/WB.RegWrite
  # WB stage에 있는 Inst가 RegWrite하는 Inst면 1, 아니면 0
  and (MEM/WB.RegisterRd != 0)
  # WB state에 있는 Inst에 의해 계산된 Rd 값이 존재할 때
  and (MEM/WB.RegisterRd = ID/EX.RegisterRs))
  # Exec의 Inst가 Read하려는 Register(rs)와 WB의 Inst가 Write하려는            
  # Register(rd)가 같을 때 Data Forwarding 
  ForwardA = 01
  
  if (MEM/WB.RegWrite
  and (MEM/WB.RegisterRd != 0)
  and (MEM/WB.RegisterRd = ID/EX.RegisterRt))
  # Exec의 Inst가 Read하려는 Register(rt)와 WB의 Inst가 Write하려는            
  # Register(rd)가 같을 때 Data Forwarding
  ForwardB = 01
  ```

- 연속하여 같은 Register를 읽으려 할 떄 **어떤 Stage Register에 저장된 값을 받아올건지 정해야 한다.**

  ![Another Problem in Data Forwarding](C:\Users\pc\OneDrive - dgist.ac.kr\바탕 화면\DGIST\3 - Junior\1st semester\Computer Architecture\img\lec11\Another Problem in Data Forwarding.JPG)

  1. **EX** Forwarding unit

  ```pseudocode
  # 위의 Ex와 같다.
  if (EX/MEM.RegWrite
  and (EX/MEM.RegisterRd != 0)
  and (EX/MEM.RegisterRd = ID/EX.RegisterRs))
  ForwardA = 10
  if (EX/MEM.RegWrite
  and (EX/MEM.RegisterRd != 0)
  and (EX/MEM.RegisterRd = ID/EX.RegisterRt))
  ForwardB = 10
  ```

  

  2. **MEM** Forwarding unit

  ```pseudocode
  # 위의 MEM pseudocode에서 한가지 조건이 추가되었다.
  if (MEM/WB.RegWrite
  and (MEM/WB.RegisterRd != 0)
  and (EX/MEM.RegisterRd != ID/EX.RegisterRs)
  and (MEM/WB.RegisterRd = ID/EX.RegisterRs))	==> 추가된 조건
  ForwardA = 01
  
  if (MEM/WB.RegWrite
  and (MEM/WB.RegisterRd != 0)
  and (EX/MEM.RegisterRd != ID/EX.RegisterRt)	==> 추가된 조건
  and (MEM/WB.RegisterRd = ID/
  ```

  

  



​	







