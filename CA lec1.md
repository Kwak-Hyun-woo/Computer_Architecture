# CA lec1

### ISA (Instruction set architecture)

- HW와 SW 사이의 Interface(HW와 SW 간의 약속을 정의)
- 호환성 때문에 변경이 쉽지 않다.
- 다양한 ISA: MIPS, x86, Power, ARM etc 
- 기존의 Instruction을 남겨두고 새로운 Instruction을 추가하는 방식으로 발전

### Micro Architecture 

- ISA를 실행할 수 있게 하는 것
- Processor들의 집합
- 제약조건(Pipeline(Instruction을 겹쳐 실행하는 구조)의 깊이, Cache 메모리 크기 및 구조,  공간적인 제약 등등)이 있는 상황에서 디자인 목표를 만족해야함

### Program to Machine code

High-level language program ====Compiler===> Assembly language program(for MIPS) ====Assembler====> Machine code(0과 1의 조합)

* Assembly language: ISA에 정의되어 있는 Instruction을 활용한 프로그래밍 언어
* ISA 종류에 따라 Assembler가 달라짐

### Moore's law

- 1년마다 집적도(Transistor count / chip)가 40-55%가 증가한다는 법칙
- 트랜지스터 크기 작게 만들기=> 빨라지고 소비전력이 줄어든다.

- Cache를 더 크게하고 Prefetch system을 더 낫게 개선하여 2배씩 증가

- 전력에 의한 열, Pipeline 길이를 늘이는 것에 대한 한계, 느린선들, 공간을 넓혀 하드웨어를 많이 넣어도 10-20% 밖에 개선을 못하는 점 등등으로 2배 이상 집적도를 올리기 힘들어짐

### 