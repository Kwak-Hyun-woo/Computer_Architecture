# CA Lec2

### Locality

공간적인 지역성과 시간적인 지역성을 이용하여 비싸고 빠른 메모리 사용이 어려운 단점을 보완한다.

메모리가 계층적인 구조를 가지고 있으며 서로 다른 특징(아래 표 참조)을 가지고 있다

| 높은 Layer(Cache,.) | 낮은 Layer(DRAM, SSD, HDD) |
| ------------------- | -------------------------- |
| Speed 높음          | Speed 낮음                 |
| 용량 작음           | 용량 큼                    |
| 비용 비쌈           | 비용 낮음                  |

### Parallelism

- 회로적인 작동은 병렬적이다.

- CPU, memory, disk, networks 작동들은 병렬적으로 이루어진다.

- ex) Pipelining, Superscalar, Vector Operation, Multi-cores

### Defining Performance

$Performance_n = \frac{1} {execution-time}_x$

즉, Performance를 최대화 하려면 Execution time을 최소화해야한다.

X가 Y보다 n배 빠를 때: $\frac{Performance_x}{Performance_y} = \frac{execution-time_y} {execution-time_x} = n$

### CPU clocking

Clock(Cycles): 신호가 0 => 1 => 0 => 1이 되는 한 Cycle을 Clock이라 한다.

Hardware의 모든 Operation들이 Clock에 따라 동작
$$
Clock Cycle = \frac {1} {Clock Rate}
$$


$CPU\;execution\;time\;for\;a\;programming = CPU\;clock\;cycles\;for \;a\;program * clock\;cycle\;time$

- CPI(Clock Cycles per Instruction): 각각의 Instruction을 실행할 때 필요한 clock cycle들의 평균 개수

  즉 CPI가 높을 수록 Execution하기 위한 Cycle이 많이 필요하고 시간이 오래걸리므로 성능이 안좋음

- IPC(Instruction per Clock Cycles): $ = \frac{1}{CPI}$ 즉 하나의 Instruction을 돌리기 위해 필요한 Clock Cycle, 높을 수록 성능이 좋다.

같은 ISA 같은 Instruction이라 하더라도 Execute하는데 시간이 달라질 수 있다.

프로그램 단위로 CPI를 측정해서 비교(Application Dependent)

### Effective CPI



















 

 