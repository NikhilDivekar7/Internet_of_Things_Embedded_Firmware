################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include protocol/bluetooth_2.7/lib/EFR32BG1B/GCC/subdir.mk
-include platform/emlib/src/subdir.mk
-include platform/emdrv/tempdrv/src/subdir.mk
-include platform/emdrv/sleep/src/subdir.mk
-include platform/Device/SiliconLabs/EFR32BG1B/Source/GCC/subdir.mk
-include platform/Device/SiliconLabs/EFR32BG1B/Source/subdir.mk
-include lcd_graphics/subdir.mk
-include hardware/kit/common/drivers/subdir.mk
-include hardware/kit/common/bsp/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: HW4_BLE.axf

# Tool invocations
HW4_BLE.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -T "C:\Users\nikhi\SimplicityStudio\v4_workspacedemo\HW4_BLE\efr32bg1b232f256gm56.ld" -Xlinker --gc-sections -Xlinker -Map="HW4_BLE.map" -mfpu=fpv4-sp-d16 -mfloat-abi=softfp --specs=nano.specs -o HW4_BLE.axf -Wl,--start-group "./application_properties.o" "./gatt_db.o" "./init_app.o" "./init_board.o" "./init_mcu.o" "./main.o" "./pti.o" "./hardware/kit/common/bsp/bsp_stk.o" "./hardware/kit/common/drivers/mx25flash_spi.o" "./lcd_graphics/bmp.o" "./lcd_graphics/display.o" "./lcd_graphics/displayls013b7dh03.o" "./lcd_graphics/displaypalemlib.o" "./lcd_graphics/dmd_display.o" "./lcd_graphics/glib.o" "./lcd_graphics/glib_bitmap.o" "./lcd_graphics/glib_circle.o" "./lcd_graphics/glib_font_narrow_6x8.o" "./lcd_graphics/glib_font_normal_8x8.o" "./lcd_graphics/glib_font_number_16x20.o" "./lcd_graphics/glib_line.o" "./lcd_graphics/glib_polygon.o" "./lcd_graphics/glib_rectangle.o" "./lcd_graphics/glib_string.o" "./lcd_graphics/graphics.o" "./lcd_graphics/udelay.o" "./platform/Device/SiliconLabs/EFR32BG1B/Source/GCC/startup_efr32bg1b.o" "./platform/Device/SiliconLabs/EFR32BG1B/Source/system_efr32bg1b.o" "./platform/emdrv/sleep/src/sleep.o" "./platform/emdrv/tempdrv/src/tempdrv.o" "./platform/emlib/src/em_acmp.o" "./platform/emlib/src/em_adc.o" "./platform/emlib/src/em_aes.o" "./platform/emlib/src/em_assert.o" "./platform/emlib/src/em_burtc.o" "./platform/emlib/src/em_can.o" "./platform/emlib/src/em_cmu.o" "./platform/emlib/src/em_core.o" "./platform/emlib/src/em_cryotimer.o" "./platform/emlib/src/em_crypto.o" "./platform/emlib/src/em_csen.o" "./platform/emlib/src/em_dac.o" "./platform/emlib/src/em_dbg.o" "./platform/emlib/src/em_dma.o" "./platform/emlib/src/em_ebi.o" "./platform/emlib/src/em_emu.o" "./platform/emlib/src/em_gpcrc.o" "./platform/emlib/src/em_gpio.o" "./platform/emlib/src/em_i2c.o" "./platform/emlib/src/em_idac.o" "./platform/emlib/src/em_int.o" "./platform/emlib/src/em_lcd.o" "./platform/emlib/src/em_ldma.o" "./platform/emlib/src/em_lesense.o" "./platform/emlib/src/em_letimer.o" "./platform/emlib/src/em_leuart.o" "./platform/emlib/src/em_mpu.o" "./platform/emlib/src/em_msc.o" "./platform/emlib/src/em_opamp.o" "./platform/emlib/src/em_pcnt.o" "./platform/emlib/src/em_prs.o" "./platform/emlib/src/em_qspi.o" "./platform/emlib/src/em_rmu.o" "./platform/emlib/src/em_rtc.o" "./platform/emlib/src/em_rtcc.o" "./platform/emlib/src/em_system.o" "./platform/emlib/src/em_timer.o" "./platform/emlib/src/em_usart.o" "./platform/emlib/src/em_vcmp.o" "./platform/emlib/src/em_vdac.o" "./platform/emlib/src/em_wdog.o" "./src/I2C.o" "./src/SPI.o" "./src/cmu.o" "./src/gpio.o" "./src/lcd.o" "./src/letimer.o" "./src/light.o" "./src/lpm.o" "./src/si7021.o" "./src/sleep.o" "C:\Users\nikhi\SimplicityStudio\v4_workspacedemo\HW4_BLE\protocol\bluetooth_2.7\lib\EFR32BG1B\GCC\libmbedtls.a" "C:\Users\nikhi\SimplicityStudio\v4_workspacedemo\HW4_BLE\platform\radio\rail_lib\autogen\librail_release\librail_efr32xg1_gcc_release.a" "C:\Users\nikhi\SimplicityStudio\v4_workspacedemo\HW4_BLE\protocol\bluetooth_2.7\lib\EFR32BG1B\GCC\libpsstore.a" "C:\Users\nikhi\SimplicityStudio\v4_workspacedemo\HW4_BLE\protocol\bluetooth_2.7\lib\EFR32BG1B\GCC\libbluetooth.a" "C:\Users\nikhi\SimplicityStudio\v4_workspacedemo\HW4_BLE\protocol\bluetooth_2.7\lib\EFR32BG1B\GCC\binapploader.o" -lm -Wl,--end-group -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building hex file: HW4_BLE.hex'
	arm-none-eabi-objcopy -O ihex "HW4_BLE.axf" "HW4_BLE.hex"
	@echo ' '

	@echo 'Building bin file: HW4_BLE.bin'
	arm-none-eabi-objcopy -O binary "HW4_BLE.axf" "HW4_BLE.bin"
	@echo ' '

	@echo 'Building s37 file: HW4_BLE.s37'
	arm-none-eabi-objcopy -O srec "HW4_BLE.axf" "HW4_BLE.s37"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "HW4_BLE.axf"
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) HW4_BLE.axf
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
