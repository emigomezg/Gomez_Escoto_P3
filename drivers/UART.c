#include "UART.h"

static void (*UART0_RX_HANDLER)(void) = 0;
static void (*UART1_RX_HANDLER)(void) = 0;


static uart_mail_box_t g_mail_box_uart_0;
static uart_mail_box_t g_mail_box_uart_1;

void UART_set_flag(void)
{
	g_mail_box_uart_0.flag = TRUE;
}
void UART_clear_flag(void)
{
	g_mail_box_uart_0.flag = FALSE;
}
uint8_t UART_get_flag(void)
{
	return g_mail_box_uart_0.flag;
}
void UART_set_mailbox(uint8_t data)
{
	g_mail_box_uart_0.mailBox = data;
}
uint8_t UART_get_mailbox(uart_channel_t uart_channel)
{
	switch((uint8_t)uart_channel)
	{
		case UART_0:
			return g_mail_box_uart_0.mailBox;
			break;
		case UART_1:
			return g_mail_box_uart_1.mailBox;
			break;
	}
	return 0;

}

void UART_clock_gating(uart_channel_t uart_channel) {
	switch ((uint8_t) uart_channel) {
	case UART_0:
		SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
		break;
	case UART_1:
		SIM->SCGC4 |= SIM_SCGC4_UART1_MASK;
		break;
	case UART_2:
		SIM->SCGC4 |= SIM_SCGC4_UART2_MASK;
		break;
	case UART_3:
		SIM->SCGC4 |= SIM_SCGC4_UART3_MASK;
		break;
	case UART_4:
		SIM->SCGC4 |= SIM_SCGC1_UART4_MASK;
		break;
	case UART_5:
		SIM->SCGC4 |= SIM_SCGC1_UART5_MASK;
		break;
	default:
		break;

	}
}
void UART_init(uart_channel_t uart_channel, uint32_t system_clk,
		uart_baud_rate_t baud_rate) {
	UART_clock_gating(uart_channel);
	uint8_t brfa = (uint8_t) (2 * (float) system_clk
			/ (float) (baud_rate * baud_rate));
	switch ((uint8_t) uart_channel) {
	case UART_0:
		UART0->C2 &= ~(UART_C2_RE_MASK | UART_C2_TE_MASK);
		//UART0->BDH |= UART_BDH_SBR(baud_rate);
		UART0->BDH |= 0;
		UART0->BDL = 11;
		UART0->C4 |= 0xD & 0x1F;
		UART0->C2 |= (UART_C2_RE_MASK | UART_C2_TE_MASK);
		break;
	case UART_1:
		UART1->C2 &= ~(UART_C2_RE_MASK | UART_C2_TE_MASK);
		UART1->BDH |= 0;
		UART1->BDL = 11;
		UART1->C4 |= 0xD;
		UART1->C2 |= (UART_C2_RE_MASK | UART_C2_TE_MASK);
		break;
	case UART_2:
		UART2->C2 &= ~(UART_C2_RE_MASK | UART_C2_TE_MASK);
		UART2->BDH |= (baud_rate & (UART_BDH_SBR_MASK << 8)) >> 8;
		UART2->BDL |= (baud_rate & UART_BDL_SBR_MASK);
		UART2->C4 |= UART_C4_BRFA(brfa);
		UART2->C2 |= (UART_C2_RE_MASK | UART_C2_TE_MASK);
		break;
	case UART_3:
		UART3->C2 &= ~(UART_C2_RE_MASK | UART_C2_TE_MASK);
		UART3->BDH |= (baud_rate & (UART_BDH_SBR_MASK << 8)) >> 8;
		UART3->BDL |= (baud_rate & UART_BDL_SBR_MASK);
		UART3->C4 |= UART_C4_BRFA(brfa);
		UART3->C2 |= (UART_C2_RE_MASK | UART_C2_TE_MASK);
		break;
	case UART_4:
		UART4->C2 &= ~(UART_C2_RE_MASK | UART_C2_TE_MASK);
		UART4->BDH |= (baud_rate & (UART_BDH_SBR_MASK << 8)) >> 8;
		UART4->BDL |= (baud_rate & UART_BDL_SBR_MASK);
		UART4->C4 |= UART_C4_BRFA(brfa);
		UART4->C2 |= (UART_C2_RE_MASK | UART_C2_TE_MASK);
		break;
	case UART_5:
		UART5->C2 &= ~(UART_C2_RE_MASK | UART_C2_TE_MASK);
		UART5->BDH |= (baud_rate & (UART_BDH_SBR_MASK << 8)) >> 8;
		UART5->BDL |= (baud_rate & UART_BDL_SBR_MASK);
		UART5->C4 |= UART_C4_BRFA(brfa);
		UART5->C2 |= (UART_C2_RE_MASK | UART_C2_TE_MASK);
		break;
	default:
		break;

	}

}
//Habilitar la interrupción de recepción en el registro UART0_C2
void UART_interrupt_enable(uart_channel_t uart_channel) {
	switch ((uint8_t) uart_channel) {
	case UART_0:
		UART0->C2 |= UART_C2_RIE_MASK;
		break;
	case UART_1:
		UART1->C2 |= UART_C2_RIE_MASK;
		break;
	case UART_2:
		UART2->C2 |= UART_C2_RIE_MASK;
		break;
	case UART_3:
		UART3->C2 |= UART_C2_RIE_MASK;
		break;
	case UART_4:
		UART4->C2 |= UART_C2_RIE_MASK;
		break;
	case UART_5:
		UART5->C2 |= UART_C2_RIE_MASK;
		break;
	}

}

void UART_put_char(uart_channel_t uart_channel, uint8_t character) {
	switch(uart_channel)
	{
		case UART_0:
		while (!(UART0->S1 &  UART_S1_TDRE_MASK));
			UART0->D = character;
			break;
		case UART_1:
			while (!(UART1->S1 &  UART_S1_TDRE_MASK));
					UART1->D = character;
			break;
		default:
			break;
	}
}

void UART_put_string(uart_channel_t uart_channel, int8_t* string) {

	int a = 0;
	while (1) {
		if (string[a] == '\0') {
			break;
		}
		UART_put_char(uart_channel, string[a]);
		a++;

	}
}
void UART0_RX_TX_IRQHandler(void) {

	while (!(UART0->S1 & UART_S1_RDRF_MASK))
		;
	g_mail_box_uart_0.flag = 1;
	g_mail_box_uart_0.mailBox = UART0->D;
	if(UART0_RX_HANDLER)
		UART0_RX_HANDLER();


}
void UART1_RX_TX_IRQHandler(void) {

	while (!(UART1->S1 & UART_S1_RDRF_MASK))
		;
	g_mail_box_uart_1.flag = 1;
	g_mail_box_uart_1.mailBox = UART1->D;
	if(UART0_RX_HANDLER)
			UART1_RX_HANDLER();

}

void UART_callback_init(void (*handler)(void),uart_channel_t uart_channel)
{
	if (handler)
	{
		switch((uint8_t) uart_channel)
		{
		case UART_0:
			UART0_RX_HANDLER = handler;
			break;
		case UART_1:
			UART1_RX_HANDLER = handler;
			break;
		default:
			break;
		}
	}
}
