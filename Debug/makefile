################################################################################
# 自动生成的文件。不要编辑！
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := cjson_test
BUILD_ARTIFACT_EXTENSION :=
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 

# 所有目标
all: main-build

# 主构建目标
main-build: cjson_test

# 工具调用
cjson_test: $(OBJS) $(USER_OBJS) makefile $(OPTIONAL_TOOL_DEPS)
	@echo '正在构建目标： $@'
	@echo '正在调用： GNU C 链接器'
	gcc  -o "cjson_test" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo '已结束构建目标： $@'
	@echo ' '

# 其他目标
clean:
	-$(RM) cjson_test
	-@echo ' '

.PHONY: all clean dependents main-build

-include ../makefile.targets
