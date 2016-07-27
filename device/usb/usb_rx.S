	xref	_usb_rx_count, _usb_rx_buffer, _usb_rx_ok

	xdef	_usb_rx

L_Bit0_Begin:
	cpl		($14,y)					; сигнал синхронизации
	srl		($01, SP)
	ld		a,(y)
	jreq	L_End_Rx				; выход, если input = 0 (конец пакета)
	xor		a, ($01, SP)
	ld		($01, SP),a
	nop

L_Bit1_Begin:
	cpl		($14,y)					; сигнал синхронизации
	srl		($01, SP)
	ld		a,(y)
	jreq	L_End_Rx				; выход, если input = 0 (конец пакета)
	xor		a, ($01, SP)
	ld		($01, SP),a
	incw	x
	nop

L_Bit2_Begin:
	cpl		($14,y)					; сигнал синхронизации
	srl		($01, SP)
	ld		a,(y)
	jreq	L_End_Rx				; выход, если input = 0 (конец пакета)
	xor		a, ($01, SP)
	ld		($01, SP),a
	nop
	nop

L_Bit3_Begin:
	cpl		($14,y)					; сигнал синхронизации
	srl		($01, SP)
	ld		a,(y)
	jreq	L_End_Rx				; выход, если input = 0 (конец пакета)
	xor		a, ($01, SP)
	ld		($01, SP),a
	nop

L_Bit4_Begin:
	cpl		($14,y)					; сигнал синхронизации
	srl		($01, SP)
	ld		a,(y)
	jreq	L_End_Rx				; выход, если input = 0 (конец пакета)
	xor		a, ($01, SP)
	ld		($01, SP),a
	nop

L_Bit5_Begin:
	cpl		($14,y)					; сигнал синхронизации
	srl		($01, SP)
	ld		a,(y)
	jreq	L_End_Rx				; выход, если input = 0 (конец пакета)
	xor		a, ($01, SP)
	ld		($01, SP),a
	dec		($02,SP)
	jreq	L_End_Rx

L_Bit6_Begin:
	cpl		($14,y)					; сигнал синхронизации
	srl		($01, SP)
	ld		a,(y)
	jreq	L_End_Rx				; выход, если input = 0 (конец пакета)
	xor		a, ($01, SP)
	ld		($01, SP),a
	nop

L_Bit7_Begin:
	cpl		($14,y)					; сигнал синхронизации
	srl		($01, SP)
	ld		a,(y)
	jreq	L_End_Rx				; выход, если input = 0 (конец пакета)
	xor		a, ($01, SP)
	ld		($01, SP),a

	ld		(x),a					; сохраняем принятый байт
	rlc   	(x)

	cpl		($14,y)					; сигнал синхронизации
	srl		($01, SP)
	ld		a,(y)
	jreq	L_End_Rx				; выход, если input = 0 (конец пакета)
	xor		a, ($01, SP)
	ld		($01, SP),a

	jra		L_Bit1_Begin

	nop
	nop
	nop

L_End_Rx:

	pop		a
	pop		a
	ld		_usb_rx_count,a

	call	_usb_rx_ok

	clr		20495

	mov		21077,#251 				; сброс флага прерывания таймера

	bres	21072,#0				; стоп таймер
	clr		21086					; сброс счетного регистра
	clr		21087					;

	iret

;
;
;

_usb_rx:

	ldw		x, #L_Delay_Begin	; адрес начала таблицы задержек
	addw	x, 21086			; + смещение (значение таймера)
	cpw		x, #L_Delay_End		; проверка выхода за границу таблицы (защита!)
	jruge	L_Error
	jp		(x)					; переход по таблице задержек

L_Error:

	iret ; аврийный выход

L_Delay_Begin:
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop

L_Delay_End:

	ldw		x, #_usb_rx_buffer		; адрес начала буфера
	ldw		y, #20491 				; адрес входного сигнала
	ld		a, #$0E					; максимальное кол-во принимаемых байт (защита!)
	push	a
	ld		a, #$80
	push	a

	jra		L_Bit0_Begin
