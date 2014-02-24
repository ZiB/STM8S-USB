#==========================================================
#   Makefile for STM8
#	Denis Zheleznyakov aka ZiB @ http://ziblog.ru
#==========================================================

SRC_C  = usb.c
SRC_C += main.c
SRC_C += macros.c
SRC_C += stm8_interrupt_vector.c
SRC_C += stm8s_it.c
SRC_C += stm8s_clk.c
SRC_C += stm8s_tim1.c
SRC_C += stm8s_tim2.c
SRC_C += stm8s_tim4.c
SRC_C += stm8s_uart1.c

SRC_S += usb_rx.S
SRC_S += usb_tx.S

CROSS_PATH = D:/Tools/Cosmic/
CROSS = $(CROSS_PATH)/4.3.7/

INCLUDES += -imcu
INCLUDES += -imcu/peripherals
INCLUDES += -imcu/startup
INCLUDES += -imcu/std_lib/inc
INCLUDES += -imcu/std_lib/src
INCLUDES += -idevice
INCLUDES += -idevice/usb
INCLUDES += -iutility

VPATH += mcu
VPATH += mcu/peripherals
VPATH += mcu/startup
VPATH += mcu/std_lib/inc
VPATH += mcu/std_lib/src
VPATH += device
VPATH += device/usb
VPATH += utility

FLAGS_C  = +debug +warn +modsl0 -pxp -pp +fast +split -l -gge -pge

all: clean
	@make -s -j job
	@echo '-----------------------------------------------------------'
	@$(CROSS)cobj -n target/target.sm8 | $(CROSS_PATH)cosmic_format 'mcu/stm8s003f3.lsf'
	@make -s mcu_program

job: target.elf

git:
	@C:/Tools/git/extensions/gitex browse .

git_commit:
	@C:/Tools/git/extensions/gitex commit .

mcu_all: all
	@make -s mcu_program

%.elf: $(SRC_C:%.c=target/%.o) $(SRC_S:%.S=target/%.o)
	@echo '-----------------------------------------------------------'
	@echo linking: $@
	@$(CROSS)clnk -l'$(CROSS)/Lib' -o'target/target.sm8' -m'target/target.map' -sa 'mcu/*.lsf'
	@$(CROSS)cvdwarf target/target.sm8
	@$(CROSS)chex -o target/target.s19 target/target.sm8
	@rm -f target/*.o
	
$(SRC_C:%.c=target/%.o): target/%.o: %.c
	@$(CROSS)cxstm8 $(FLAGS_C) $(INCLUDES) -i. -i$(CROSS)/Hstm8 -cl'target' -co'target' $<
	
$(SRC_S:%.S=target/%.o): target/%.o: %.S
	@$(CROSS)cxstm8 +debug +warn -l -no $(INCLUDES) -i. -i$(CROSS)/Hstm8 -cl'target' -co'target' $<

size:
	@echo '-----------------------------------------------------------'
	@$(CROSS)cobj -n target/target.sm8 | $(CROSS_PATH)cosmic_format 'mcu/stm8s003f3.lsf'
	
clean:
	@echo '-----------------------------------------------------------'
	@mkdir -p target
	@rm -f target/*.*

mcu_program:
	@D:/Tools/STMicroelectronics/toolset/stvp/STVP_CmdLine.exe -BoardName=ST-LINK -Device=STM8S003F3 -Port=USB -ProgMode=SWIM -no_loop -no_log -FileProg=target/target.s19

mcu_restart:
	@D:/Tools/STMicroelectronics/toolset/stvd/gdb7.exe --quiet --command=mcu/stm8s003f3.ini