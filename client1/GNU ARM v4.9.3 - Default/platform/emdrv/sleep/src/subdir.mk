################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../platform/emdrv/sleep/src/sleep.c 

OBJS += \
./platform/emdrv/sleep/src/sleep.o 

C_DEPS += \
./platform/emdrv/sleep/src/sleep.d 


# Each subdirectory must supply rules for building sources it contributes
platform/emdrv/sleep/src/sleep.o: ../platform/emdrv/sleep/src/sleep.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-D__HEAP_SIZE=0xD00' '-D__STACK_SIZE=0x800' '-DHAL_CONFIG=1' '-DEFR32BG1B232F256GM56=1' -I"C:\Users\nikhi\SimplicityStudio\v4_workspacedemo\soc-empty_4" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.1//platform/middleware/glib" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.1//platform/middleware/glib/glib" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.1//platform/middleware/glib/dmd" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.1//hardware/kit/EFR32BG1_BRD4302A/config" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.1//util/silicon_labs/silabs_core/graphics" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.1//hardware/kit/common/drivers" -I"C:\Users\nikhi\SimplicityStudio\v4_workspacedemo\soc-empty_4\platform\Device\SiliconLabs\EFR32BG1B\Include" -I"C:\Users\nikhi\SimplicityStudio\v4_workspacedemo\soc-empty_4\protocol\bluetooth_2.7\ble_stack\inc\soc" -I"C:\Users\nikhi\SimplicityStudio\v4_workspacedemo\soc-empty_4\hardware\kit\common\halconfig" -I"C:\Users\nikhi\SimplicityStudio\v4_workspacedemo\soc-empty_4\platform\emlib\src" -I"C:\Users\nikhi\SimplicityStudio\v4_workspacedemo\soc-empty_4\platform\emlib\inc" -I"C:\Users\nikhi\SimplicityStudio\v4_workspacedemo\soc-empty_4\platform\emdrv\sleep\inc" -I"C:\Users\nikhi\SimplicityStudio\v4_workspacedemo\soc-empty_4\protocol\bluetooth_2.7\ble_stack\inc\common" -I"C:\Users\nikhi\SimplicityStudio\v4_workspacedemo\soc-empty_4\platform\emdrv\common\inc" -I"C:\Users\nikhi\SimplicityStudio\v4_workspacedemo\soc-empty_4\platform\CMSIS\Include" -I"C:\Users\nikhi\SimplicityStudio\v4_workspacedemo\soc-empty_4\platform\Device\SiliconLabs\EFR32BG1B\Source\GCC" -I"C:\Users\nikhi\SimplicityStudio\v4_workspacedemo\soc-empty_4\platform\emdrv\tempdrv\inc" -I"C:\Users\nikhi\SimplicityStudio\v4_workspacedemo\soc-empty_4\hardware\kit\common\bsp" -I"C:\Users\nikhi\SimplicityStudio\v4_workspacedemo\soc-empty_4\platform\radio\rail_lib\chip\efr32" -I"C:\Users\nikhi\SimplicityStudio\v4_workspacedemo\soc-empty_4\platform\emdrv\gpiointerrupt\inc" -I"C:\Users\nikhi\SimplicityStudio\v4_workspacedemo\soc-empty_4\platform\emdrv\tempdrv\src" -I"C:\Users\nikhi\SimplicityStudio\v4_workspacedemo\soc-empty_4\hardware\kit\common\drivers" -I"C:\Users\nikhi\SimplicityStudio\v4_workspacedemo\soc-empty_4\hardware\kit\EFR32BG1_BRD4302A\config" -I"C:\Users\nikhi\SimplicityStudio\v4_workspacedemo\soc-empty_4\platform\radio\rail_lib\common" -I"C:\Users\nikhi\SimplicityStudio\v4_workspacedemo\soc-empty_4\platform\bootloader\api" -I"C:\Users\nikhi\SimplicityStudio\v4_workspacedemo\soc-empty_4\app\bluetooth_2.7\common\stack_bridge" -I"C:\Users\nikhi\SimplicityStudio\v4_workspacedemo\soc-empty_4\platform\emdrv\sleep\src" -I"C:\Users\nikhi\SimplicityStudio\v4_workspacedemo\soc-empty_4\platform\emdrv\uartdrv\inc" -I"C:\Users\nikhi\SimplicityStudio\v4_workspacedemo\soc-empty_4\platform\halconfig\inc\hal-config" -I"C:\Users\nikhi\SimplicityStudio\v4_workspacedemo\soc-empty_4\platform\Device\SiliconLabs\EFR32BG1B\Source" -I"C:\Users\nikhi\SimplicityStudio\v4_workspacedemo\soc-empty_4\platform\bootloader" -O0 -Wall -c -fmessage-length=0 -ffunction-sections -fdata-sections -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -MMD -MP -MF"platform/emdrv/sleep/src/sleep.d" -MT"platform/emdrv/sleep/src/sleep.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


