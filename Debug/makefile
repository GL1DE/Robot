################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include Core/lib/peripheries/timer/subdir.mk
-include Core/lib/peripheries/encoder/subdir.mk
-include Core/lib/modules/pozicija/subdir.mk
-include Core/lib/modules/pogon/subdir.mk
-include Core/lib/modules/odometrija/subdir.mk
-include Core/lib/modules/ax12/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
Robot.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
Robot.list \

OBJCOPY_BIN += \
Robot.bin \


# All Target
all: Robot.elf secondary-outputs

# Tool invocations
Robot.elf: $(OBJS) $(USER_OBJS) C:\Users\Lenovo\STM32CubeIDE\workspace_1.2.0\Robot\STM32F401RETX_FLASH.ld
	arm-none-eabi-gcc -o "Robot.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\Lenovo\STM32CubeIDE\workspace_1.2.0\Robot\STM32F401RETX_FLASH.ld" --specs=nosys.specs -Wl,-Map="Robot.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

Robot.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "Robot.list"
	@echo 'Finished building: $@'
	@echo ' '

Robot.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "Robot.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
