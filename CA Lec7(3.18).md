# CA Lec7

#### Array vs Pointers

Array:  index가 Element size 만큼 곱하고 base address(시작 주소)에 더해야 한다.

Pointer를 direct로 memory address와 일치시킨다.

=> pointer를 사용하면 Index variable을 제거할 수 있어서 Instruction 개수가 줄어든다.

* Array 사용

> ```c++
> clear1(int array[], int size){
> 	int i;
>     for (i = 0; i < size; i+= 1)
>         array[i] = 0;
> }
> ```

> ```assembly
> 		move $t0,$zero 	# i = 0
> loop1: 	sll $t1,$t0,2 	# $t1 = i * 4
> 		add $t2,$a0,$t1 # $t2 =
> 						# &array[i]
> 		sw $zero, 0($t2)# array[i] = 0
> 		addi $t0,$t0,1 	# i = i + 1
> 		slt $t3,$t0,$a1 # $t3 =
> 						# (i < size)
> 		bne $t3,$zero,loop1 	# if (…)
> 						# goto loop1
> ```

- Pointer 사용

```c++
clear2(int *array, int size) {
	int *p;
	for (p = &array[0]; p < &array[size]; p = p + 1) 
        // pointer는 주소단위로 점프, 즉 int pointer 일때 +1은 4byte 점프
		*p = 0;
}
```

```assembly
		move $t0,$a0 	# p = & array[0]
		sll $t1,$a1,2 	# $t1 = size * 4
		add $t2,$a0,$t1 # $t2 =
						# &array[size]
loop2: 	sw $zero,0($t0) # Memory[p] = 0
		addi $t0,$t0,4 	# p = p + 4
		slt $t3,$t0,$t2 # $t3 =
						#(p<&array[size])
		bne $t3,$zero,loop2 # if (…)
							# goto loop2
```

반복실행되는 코드가 pointer 사용할 때 보다 array 사용할 때가 더 많다.

#### Assemble Pseudoinstructions

프로그래머의 편의를 위해 만들어진 것

ex) blt의 경우 MIPS에서 지원하지 않지만 프로그래머가 문법적으로 쓸 수 있다

blt = less than(slt) + branch(bne)

\* $at(register 1): assembler temporary(Pseudo Instruction에서 실제 Insturction으로 바꿀 때 쓰이는 임시 레지스터

#### Translation and Startup

C program => Compiler => Assembly Language Program => Assembler => Object(Machine Language Module + Library routine) => Linker => Executable: Machine Language Program => Loader => Memory

#### Producing an Object Module

- Object 파일이 알려주는 것들
  - Header: 오브젝트 파일의 메타 데이터들을 알려주는 부분
  - Text Segmentation: 번역된 Instruction들(machine code, binary number형태)
  - Data Segmentation: 전역변수들
  - Relocation info: 주소에 의존하는 instruction과 data
  - Symbol table: label과 주소들을 mapping하는 table
  - Debug info: 디버깅 위해 포함되야하는 정보

#### Linking Object Modules

- 메모리내 코드와 data들을 위치시켜야 한다.
- relocation 정보와 symbol table을 사용한 data와 instruction들의 label 주소들을 결정한다.
- 내, 외부 reference들을 Patch시킨다.

#### Loading a Prgram

- 메모리 Vitualizaion space를 생성한다.
- Text와 data들을 메모리에 copy 한다.(virtual memory space와 실제 Physical address space를 mapping 하기 위해 set table들을 설정해준다.)

#### Dynamic Linking

library와 관련된 Object파일들이 평소에는 호출이 안되다가 필요한 경우에 동적으로 Linking함



## 5. Datapath and Control

### CPU performance Factors

- Instruction count
  - ISA와 Compiler에 의해 결정된다.
  - 값이 낮을 수록 성능이 좋다.
- CPI and Cycle Time
  - CPU Hardware에 의해 결정된다.
  - CPI와 Cycle Time 모두 값이 낮을수록 성능이 좋다.

### The Processor: Datapath and Control

- **Fetch**: 실행할 Instruction을 PC(실행되어야 할 Instruction의 주소)값을 통해 메모리에서 processor가 가져오는 것
- **Decode**: 가져온 Instuction(binary code)을 해석하는 과정
- **Execute**: 해석한 정보를 바탕으로 Instruction을 실행하는 것



### Processor를 Design 할 때 필요한 요소

- ALU(연산작업 수행) 필요
- 모든 Instruction들은 Register에서 값을 읽고 ALU를 사용한다.
- ALU를 통해 주소계산한다.
- add, sub같은 instruction들은 ALU를 쓴다.
- branch condition을 계산하기 위해 ALU를 사용한다.(Target Address를 계산, Register 값들이 Condition에 맞는지 연산)



### Clocking Methodologies

- State Element: 레지스터와 같은 메모리 요소

- Edge triggered하게 모든 State가 변한다.

![](C:\Users\pc\OneDrive - dgist.ac.kr\바탕 화면\DGIST\3 - Junior\1st semester\Computer Architecture\img\State.JPG)

- CPU overview

![](C:\Users\pc\OneDrive - dgist.ac.kr\바탕 화면\DGIST\3 - Junior\1st semester\Computer Architecture\img\CPU overview.JPG)

- Multiplexers

  : Input이 여러개 들어올 때 하나만 선택할 수 있게 하는 것

  ![](C:\Users\pc\OneDrive - dgist.ac.kr\바탕 화면\DGIST\3 - Junior\1st semester\Computer Architecture\img\CPU overview with MUX.JPG)

  ### DataPath

  : CPU 내에서 data와 address를 처리하는 요소들 ex) Register, ALU, MUX, memory등이 있다.

![Fetching](C:\Users\pc\OneDrive - dgist.ac.kr\바탕 화면\DGIST\3 - Junior\1st semester\Computer Architecture\img\Fetching.JPG)

### Fetch

- PC는 매 clock cycle마다 자동으로 update 되기 때문에, 명시적으로 읽어라 읽지말아라 명시적으로 신호를 줄 필요가 없다.
- Instruction memory에서 읽는 행동도 명시적으로 read control signal은 필요없다.

### Decoding

- Fetch해온 Instruction을 해석하는 과정
- Binary code를 바탕으로 어떤 Register를 읽을 지 해석한 후 Register File에서 값을 읽는다.

 