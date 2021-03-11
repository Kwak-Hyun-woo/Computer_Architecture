# CA Lec 3

메모리의 성능향상이 프로세스의 성능향상을 못 따라오고 Power의 문제로 Moore의 법칙이 깨지기 시작한다.

### Multi Core Processors

하나의 칩에 2개 이상의 Core를 가지고 있는 것 => 동시의 여러 Instruction을 처리할 수 있다.

하나의 코어에서 어떻게 Instruction을 많이 실행시킬지 고민하는 것 => Instruction level parallelism

병렬화할 때 어려운점

- Load Balancing
- Optimizing communication(Thread간 커뮤니케이션) and synchronization

Core 개수에 따라 완전히 비례하여 성능이 향상되지는 않는다.

- Core간 Interference들이 생긴다.
- OS가 2개의 core들을 Scheduling하는 데에서 문제가 생길 수 있다.

### Amdahl's Law

$T_{improved} = \frac{T_{affected}}{Imporvement \; factor} + T_{unaffected}$

$T_{improved}$ : 개선된 컴퓨터에서 실행되는 시간

> 결론: Make the Common case fast => 공통적으로 쓰이는 부분들을 향상시켜야 한다.

### Instruction Set Architecture

Contract between programmer and the hardware

- Instruction(각각의 명령어들)이 어떻게 작동하는지 정의
- System의 Visible State(컴퓨터의 저장공간(Register, Cache) 속 프로그램의 상태가 계속 바뀌게 되는데 이러한 상태들을 Visivle State라고 한다.)를 정의
- Instruction에 대해  어떻게 State가 변화하는 지 정의



Programmer : ISA는 프로그램이 어떻게 실행될지에 대한 모델(ISA를 사용하는 입장)

Hardware Designer: ISA는 프로그램이 정확하게 작동되는 Formal한 정의(ISA를 제공하는 입장)

- 모든 구조적인 Visible State: register
- Instruction 형식이나 행동
- Memory Model을 어떻게 할지 정의(Paging, Segmented Memory 등등)
- I / O(Input, Output, 입출력장치 ), Storage, Network등등을 어떻게 쓸지 ISA에서 해줘야 함. 느림, Interrupt(NetWork에서 Data들이 수신될 때 하던 프로그램들을 멈추게 하는 것)

### 핵심 원리 

1. Instruction들은 이진수들로 표현된다.(Machine code)
2. Program(Machine Code)들은 Memory(Memory Hierachy)에 저장되어있다.(데이터 처럼) => Stored-Program concept

Binary Compatibility Issue: 다른 ISA면 Binary File들이 실행되지 않는다.

### Instruction Categories

- Computational
- Load/ Store
- Jump and Branch
- ETC





































