################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/char_split.c \
../src/lib_cjson.c \
../src/main.c 

C_DEPS += \
./src/char_split.d \
./src/lib_cjson.d \
./src/main.d 

OBJS += \
./src/char_split.o \
./src/lib_cjson.o \
./src/main.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c src/subdir.mk
	@echo '正在构建文件： $<'
	@echo '正在调用： GNU C 编译器'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo '已结束构建： $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/char_split.d ./src/char_split.o ./src/lib_cjson.d ./src/lib_cjson.o ./src/main.d ./src/main.o

.PHONY: clean-src

