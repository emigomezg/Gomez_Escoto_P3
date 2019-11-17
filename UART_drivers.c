#include "UART_drivers.h"

uart_mail_box_t g_mail_box_uart_0;
uart_mail_box_t g_mail_box_uart_1;
uart_mail_box_t g_mail_box_uart_2;
uart_mail_box_t g_mail_box_uart_3;
uart_mail_box_t g_mail_box_uart_4;
uart_mail_box_t g_mail_box_uart_5;

static void (*uart_0_callback)(void) = 0;
static void (*uart_1_callback)(void) = 0;
static void (*uart_2_callback)(void) = 0;
static void (*uart_3_callback)(void) = 0;
static void (*uart_4_callback)(void) = 0;
static void (*uart_5_callback)(void) = 0;

void UART_clock_gating(uart_channel_t uart_channel)
{
    switch ((uint8_t) uart_channel)
    {
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
        uart_baud_rate_t baud_rate)
{
    UART_clock_gating(uart_channel);
    uint8_t brfa = (uint8_t) (2 * (float) system_clk
            / (float) (baud_rate * baud_rate));
    switch ((uint8_t) uart_channel)
    {
        case UART_0:
            UART0->C2 &= ~(UART_C2_RE_MASK | UART_C2_TE_MASK);
            //UART0->BDH |= UART_BDH_SBR(baud_rate);
            UART0->BDH |= 0;
            UART0->BDL = 11;
            UART0->C4 |= 0xD & 0x1F;
            UART0->C2 |= (UART_C2_RE_MASK | UART_C2_TE_MASK);
            UART0->C2 |= UART_C2_RIE(1);
            break;
        case UART_1:
            UART1->C2 &= ~(UART_C2_RE_MASK | UART_C2_TE_MASK);
            UART1->BDH |= (baud_rate & (UART_BDH_SBR_MASK << 8)) >> 8;
            UART1->BDL |= (baud_rate & UART_BDL_SBR_MASK);
            UART1->C4 |= UART_C4_BRFA(brfa);
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
void UART_interrupt_enable(uart_channel_t uart_channel)
{
    switch ((uint8_t) uart_channel)
    {
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

void UART_put_char(uart_channel_t uart_channel, uint8_t character)
{
    while (UART0->S1 & UART_S1_OR_MASK)
        ;
    UART0->D = character;
}

void UART_put_string(uart_channel_t uart_channel, int8_t* string)
{

    int a = 0;
    while (1)
    {
        if (string[a] == '\0')
        {
            break;
        }
        UART_put_char(uart_channel, string[a]);
        a++;

    }
}

void UART_callback_init(void (*handler)(void),uint8_t uart)
{
	if (handler)
	{
		switch(uart)
		{
		case UART_0:
			uart_0_callback = handler;
			break;
		case UART_1:
			uart_1_callback = handler;
			break;
		case UART_2:
			uart_2_callback = handler;
			break;
		case UART_3:
			uart_3_callback = handler;
			break;
		case UART_4:
			uart_4_callback = handler;
			break;
		case UART_5:
			uart_5_callback = handler;
		default:
			break;
		}
	}
}

void UART0_RX_TX_IRQHandler(void)
{
	if (uart_0_callback)
	{
			uart_0_callback();
	}
}

void UART1_RX_TX_IRQHandler(void)
{
	if (uart_1_callback)
	{
			uart_1_callback();
	}
}

void UART2_RX_TX_IRQHandler(void)
{
	if (uart_2_callback)
	{
			uart_2_callback();
	}
}

void UART3_RX_TX_IRQHandler(void)
{
	if (uart_3_callback)
	{
			uart_3_callback();
	}
}
void UART4_RX_TX_IRQHandler(void)
{
	if (uart_4_callback)
	{
			uart_4_callback();
    }
}

void UART5_RX_TX_IRQHandler(void)
{
	if (uart_5_callback)
	{
			uart_5_callback();
    }
}

void UART0_reception_handler (void)
{
    while (!(UART0->S1 & UART_S1_RDRF_MASK));
    g_mail_box_uart_0.flag = 1;
    g_mail_box_uart_0.mailBox = UART0->D;
}

void UART1_reception_handler (void)
{
    while (!(UART1->S1 & UART_S1_RDRF_MASK));
    g_mail_box_uart_1.flag = 1;
    g_mail_box_uart_1.mailBox = UART1->D;
}
void UART2_reception_handler (void)
{
    while (!(UART2->S1 & UART_S1_RDRF_MASK));
    g_mail_box_uart_2.flag = 1;
    g_mail_box_uart_2.mailBox = UART2->D;
}

void UART3_reception_handler (void)
{
    while (!(UART3->S1 & UART_S1_RDRF_MASK));
    g_mail_box_uart_3.flag = 1;
    g_mail_box_uart_3.mailBox = UART3->D;
}
void UART4_reception_handler (void)
{
    while (!(UART4->S1 & UART_S1_RDRF_MASK));
    g_mail_box_uart_4.flag = 1;
    g_mail_box_uart_4.mailBox = UART4->D;
}

void UART5_reception_handler (void)
{
    while (!(UART5->S1 & UART_S1_RDRF_MASK));
    g_mail_box_uart_5.flag = 1;
    g_mail_box_uart_5.mailBox = UART5->D;
}

uint8_t get_UART_flag_state(uint8_t uart)
{
	uint8_t flag_state = 0;
	switch(uart)
	{
		case UART_0:
		{
			flag_state = g_mail_box_uart_0.flag;
			break;
		}
		case UART_1:
		{
			flag_state = g_mail_box_uart_1.flag;
			break;
		}
		case UART_2:
		{
			flag_state = g_mail_box_uart_2.flag;
			break;
		}
		case UART_3:
		{
			flag_state = g_mail_box_uart_3.flag;
			break;
		}
		case UART_4:
		{
			flag_state = g_mail_box_uart_4.flag;
			break;
		}
		case UART_5:
		{
			flag_state = g_mail_box_uart_5.flag;
			break;
		}
		default:
			break;
	}
	return flag_state;
}

uint8_t get_UART_data_state(uint8_t uart)
{
	uint8_t data = 0;
	switch(uart)
	{
		case UART_0:
		{
			data = g_mail_box_uart_0.mailBox;
			break;
		}
		case UART_1:
		{
			data = g_mail_box_uart_1.mailBox;
			break;
		}
		case UART_2:
		{
			data = g_mail_box_uart_2.mailBox;
			break;
		}
		case UART_3:
		{
			data = g_mail_box_uart_3.mailBox;
			break;
		}
		case UART_4:
		{
			data = g_mail_box_uart_4.mailBox;
			break;
		}
		case UART_5:
		{
			data = g_mail_box_uart_5.mailBox;
			break;
		}
		default:
			break;
	}
	return data;
}

