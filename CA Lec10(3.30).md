# CA Lec10(3.30)

#### Single Cycle vs Pipeline

![Single Cycle vs Pipelining](C:\Users\pc\OneDrive - dgist.ac.kr\바탕 화면\DGIST\3 - Junior\1st semester\Computer Architecture\img\Single Cycle vs Pipelining.JPG)

|                                                       | Single Cycle | Pipelining              |
| ----------------------------------------------------- | ------------ | ----------------------- |
| Latency(지연시간, 처리시간,=Clock Cycle Time), LW기준 | 800ps        | 1000ps                  |
| Throughput(처리량)                                    |              | Single Cycle 보다 많다! |

- Pipelining은 **가장 시간이 오래걸리는 Stage를 기준으로 CC를 정하기 때문에 Latency는 Single Cycle보다 오히려 증가한다.**

- 그러나 Throughput이 Single Cycle보다 Pipelining보다 많기 때문에 Pipelining을 사용하는 것

  ex) 1,000,000개의 `add` instruction을 수행할 때 걸리는 시간 비교

  single cycle: 1,000,000 * 800ps 

  pipelining:	1,000,000 * 200ps + 800ps(800ps를 더해주는 이유는 맨 처음 Pipeline stage를 채워주기 위해 걸리는 시간)

#### Benefits of Pipelining

- Pipelining 적용x

  - **Throughput: 1 instruction per Cycle**
    $$
    t_{clk} = t_{F(Fetch)} + t_{R(Dec)}+t_{X(Exec)}+t_{M(Mem)}+t_{W(WB)}
    $$
    

- Pipelining 적용o

  - **Throughput: 1 instruction per Cycle(이상적인 Pipeline일 때)**
    $$
    t_{clk} = MAX(t_{F(Fetch)}, t_{R(Dec)},t_{X(Exec)},t_{M(Mem)},t_{W(WB)}) + t_{latch}
    $$

  \* 여기서 $t_{latch}$는 Stage Register에 관한 시간, 매우 작다.

**즉 Pipeline이 Single Cycle일 때 보다 거의 Stage개수만큼 빠르다.**

:exclamation: **주의**: Pipeline을 적용할 때 **일부의 Stage 처리 속도가 빨라졌다 하더라도 전체적인 성능향상은 없다. 왜냐하면 CC는 가장 느린 stage의 처리 시간을 따르기 때문**

=> **Pipe Stage를 처리하는 시간들이 불균형하면 성능이 향상되는 정도가 줄어듦**



#### Pipeling Performace

![Single Cycle vs Pipelining_time](C:\Users\pc\OneDrive - dgist.ac.kr\바탕 화면\DGIST\3 - Junior\1st semester\Computer Architecture\img\Single Cycle vs Pipelining_time.JPG)

- Pipeline을 적용함에 따라 Latency는 800에서 1000으로 증가했지만 Thoughput 또한 증가하면서 성능을 개선하였다.
  $$
  Time\ Between\ Instructions_{pipelined} = \frac{Time\ Between\ Instructions_{nonpipelined}}{Number\ of\ Staged}
  $$
  

#### MIPS Pipeline Datapath Addition/Mods

- **State Registers**: 하나의 CPU에는 Pipeline을 도입할 경우 여러 Instruction들이 혼재되어있는데 다음 Stage로 Instruction의 정보를 넘기기 위해 현재 stage의 Instruction에 대한 정보를 저장하는 Register를 말한다.
- 아래 그림에서 `IF/ID`, `ID/EX`, `EX/MEM`, `MEM/WB` 가 State Register에 해당한다. (`PC`는 State Register라 분류하기 힘들지만 비슷한 역할을 한다.)

![Stage_register](C:\Users\pc\OneDrive - dgist.ac.kr\바탕 화면\DGIST\3 - Junior\1st semester\Computer Architecture\img\Stage_register.JPG)



- Control Signal 또한 각 Stage가 진행됨에 따라 저장할 필요가 있으므로 State Register에 Control Signal도 같이 저장한다.

![Stage Register with Control Signal](C:\Users\pc\OneDrive - dgist.ac.kr\바탕 화면\DGIST\3 - Junior\1st semester\Computer Architecture\img\Stage Register with Control Signal.JPG)



### Pipeline Control

- 각 stage에 따른 대표적인 Instruction에 대한 Control Signal은 다음과 같다.
- **IF Stage**(Instruction을 읽고 PC를 Update), **ID(Decode) Stage**(선택적인 Control Signal이 없다.)  **처음 두 Stage는 Control Signal이 필요없다!**

![Pipeline Control](C:\Users\pc\OneDrive - dgist.ac.kr\바탕 화면\DGIST\3 - Junior\1st semester\Computer Architecture\img\Pipeline Control.JPG)