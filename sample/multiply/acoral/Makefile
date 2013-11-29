	CC  =/home/lovelife/software/PB11_ARM/cross_tool/g++_arm_none_eabi/bin/arm-none-eabi-gcc
	LD  =/home/lovelife/software/PB11_ARM/cross_tool/g++_arm_none_eabi/bin/arm-none-eabi-ld
aco_multiply.o: aco_multiply.c 
	$(CC) -c aco_multiply.c 
	$(LD) -static -T aco_multiply.lds -o aco_multiply aco_multiply.o
clean:
	rm aco_multiply.o aco_multiply

