/*
 *
 * Copyright 2013 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#ifndef _PORTMUX_H_
#define _PORTMUX_H_
#include <asm/arch/gpio.h>

#define PORTA_FER	0x31004000
#define PORTB_FER	0x31004080
#define PORTC_FER	0x31004100
#define PORTD_FER	0x31004180
#define PORTE_FER	0x31004200
#define PORTF_FER	0x31004280
#define PORTG_FER	0x31004300

#define PORTA_FER_SET	0x31004004
#define PORTB_FER_SET	0x31004084
#define PORTC_FER_SET	0x31004104
#define PORTD_FER_SET	0x31004184
#define PORTE_FER_SET	0x31004204
#define PORTF_FER_SET	0x31004284
#define PORTG_FER_SET	0x31004304

#define PORTA_MUX	0x31004030
#define PORTB_MUX	0x310040b0
#define PORTC_MUX	0x31004130
#define PORTD_MUX	0x310041b0
#define PORTE_MUX	0x31004230
#define PORTF_MUX	0x310042b0
#define PORTG_MUX	0x31004330

#define MAX_RESOURCES    MAX_GPIOS

#define P_IDENT(x)	((x) & 0x1FF)
#define P_FUNCT(x)	(((x) & 0x3) << 9)
#define P_FUNCT2MUX(x)	(((x) >> 9) & 0x3)
#define P_DEFINED	0x8000
#define P_UNDEF		0x4000
#define P_MAYSHARE	0x2000
#define P_DONTCARE	0x1000

/* UART Port Mux */
#define P_UART0_TX      (P_DEFINED | P_IDENT(GPIO_PC13) | P_FUNCT(0))
#define P_UART0_RX      (P_DEFINED | P_IDENT(GPIO_PC14) | P_FUNCT(0))
#define P_UART0_RTS     (P_DEFINED | P_IDENT(GPIO_PC15) | P_FUNCT(0))
#define P_UART0_CTS     (P_DEFINED | P_IDENT(GPIO_PD0) | P_FUNCT(0))

#define P_UART1_TX      (P_DEFINED | P_IDENT(GPIO_PB2) | P_FUNCT(1))
#define P_UART1_RX      (P_DEFINED | P_IDENT(GPIO_PB3) | P_FUNCT(1))
#define P_UART1_RTS     (P_DEFINED | P_IDENT(GPIO_PE1) | P_FUNCT(2))
#define P_UART1_CTS     (P_DEFINED | P_IDENT(GPIO_PE2) | P_FUNCT(2))

#define P_UART2_TX      (P_DEFINED | P_IDENT(GPIO_PD12) | P_FUNCT(0))
#define P_UART2_RX      (P_DEFINED | P_IDENT(GPIO_PD13) | P_FUNCT(0))
#define P_UART2_RTS     (P_DEFINED | P_IDENT(GPIO_PE10) | P_FUNCT(2))
#define P_UART2_CTS     (P_DEFINED | P_IDENT(GPIO_PE11) | P_FUNCT(2))

/* SPI Port Mux */
#define P_SPI0_SCK      (P_DEFINED | P_IDENT(GPIO_PC9) | P_FUNCT(0))
#define P_SPI0_MISO     (P_DEFINED | P_IDENT(GPIO_PC10) | P_FUNCT(0))
#define P_SPI0_MOSI     (P_DEFINED | P_IDENT(GPIO_PC11) | P_FUNCT(0))
#define P_SPI0_RDY      (P_DEFINED | P_IDENT(GPIO_PC12) | P_FUNCT(1))

#define P_SPI0_SSEL1    (P_DEFINED | P_IDENT(GPIO_PC7) | P_FUNCT(1))
#define P_SPI0_SSEL2    (P_DEFINED | P_IDENT(GPIO_PD1) | P_FUNCT(0))
#define P_SPI0_SSEL3    (P_DEFINED | P_IDENT(GPIO_PC12) | P_FUNCT(0))
#define P_SPI0_SSEL4    (P_DEFINED | P_IDENT(GPIO_PC0) | P_FUNCT(2))
#define P_SPI0_SSEL5    (P_DEFINED | P_IDENT(GPIO_PE1) | P_FUNCT(1))
#define P_SPI0_SSEL6    (P_DEFINED | P_IDENT(GPIO_PE2) | P_FUNCT(1))
#define P_SPI0_SSEL7    (P_DEFINED | P_IDENT(GPIO_PE3) | P_FUNCT(1))

#define P_SPI1_SCK      (P_DEFINED | P_IDENT(GPIO_PE13) | P_FUNCT(0))
#define P_SPI1_MISO     (P_DEFINED | P_IDENT(GPIO_PE14) | P_FUNCT(0))
#define P_SPI1_MOSI     (P_DEFINED | P_IDENT(GPIO_PE15) | P_FUNCT(0))
#define P_SPI1_RDY      (P_DEFINED | P_IDENT(GPIO_PE8) | P_FUNCT(2))

#define P_SPI1_SSEL1    (P_DEFINED | P_IDENT(GPIO_PC13) | P_FUNCT(1))
#define P_SPI1_SSEL2    (P_DEFINED | P_IDENT(GPIO_PE7) | P_FUNCT(2))
#define P_SPI1_SSEL3    (P_DEFINED | P_IDENT(GPIO_PE11) | P_FUNCT(1))
#define P_SPI1_SSEL4    (P_DEFINED | P_IDENT(GPIO_PE12) | P_FUNCT(1))
#define P_SPI1_SSEL5    (P_DEFINED | P_IDENT(GPIO_PE8) | P_FUNCT(1))
#define P_SPI1_SSEL6    (P_DEFINED | P_IDENT(GPIO_PF0) | P_FUNCT(1))
#define P_SPI1_SSEL7    (P_DEFINED | P_IDENT(GPIO_PF1) | P_FUNCT(1))

#define P_SPI2_SCK      (P_DEFINED | P_IDENT(GPIO_PC1) | P_FUNCT(0))
#define P_SPI2_MISO     (P_DEFINED | P_IDENT(GPIO_PC2) | P_FUNCT(0))
#define P_SPI2_MOSI     (P_DEFINED | P_IDENT(GPIO_PC3) | P_FUNCT(0))
#define P_SPI2_RDY      (P_DEFINED | P_IDENT(GPIO_PE12) | P_FUNCT(2))
#define P_SPI2_D2       (P_DEFINED | P_IDENT(GPIO_PC4) | P_FUNCT(0))
#define P_SPI2_D3       (P_DEFINED | P_IDENT(GPIO_PC5) | P_FUNCT(0))

#define P_SPI2_SSEL1    (P_DEFINED | P_IDENT(GPIO_PC6) | P_FUNCT(0))
#define P_SPI2_SSEL2    (P_DEFINED | P_IDENT(GPIO_PE3) | P_FUNCT(2))
#define P_SPI2_SSEL3    (P_DEFINED | P_IDENT(GPIO_PE4) | P_FUNCT(2))
#define P_SPI2_SSEL4    (P_DEFINED | P_IDENT(GPIO_PE5) | P_FUNCT(2))
#define P_SPI2_SSEL5    (P_DEFINED | P_IDENT(GPIO_PE6) | P_FUNCT(2))

/* SMC Port Mux */
#define P_SMC_A1            (P_DEFINED | P_IDENT(GPIO_PB5) | P_FUNCT(3))
#define P_SMC_A2            (P_DEFINED | P_IDENT(GPIO_PB6) | P_FUNCT(3))
#define P_SMC_A3            (P_DEFINED | P_IDENT(GPIO_PB3) | P_FUNCT(3))
#define P_SMC_A4            (P_DEFINED | P_IDENT(GPIO_PB2) | P_FUNCT(3))
#define P_SMC_A5            (P_DEFINED | P_IDENT(GPIO_PD13) | P_FUNCT(3))
#define P_SMC_A6            (P_DEFINED | P_IDENT(GPIO_PD12) | P_FUNCT(3))
#define P_SMC_A7            (P_DEFINED | P_IDENT(GPIO_PB1) | P_FUNCT(3))
#define P_SMC_A8            (P_DEFINED | P_IDENT(GPIO_PB0) | P_FUNCT(3))
#define P_SMC_A9            (P_DEFINED | P_IDENT(GPIO_PA15) | P_FUNCT(3))
#define P_SMC_A10           (P_DEFINED | P_IDENT(GPIO_PA14) | P_FUNCT(3))
#define P_SMC_A11           (P_DEFINED | P_IDENT(GPIO_PA9) | P_FUNCT(3))
#define P_SMC_A12           (P_DEFINED | P_IDENT(GPIO_PA8) | P_FUNCT(3))
#define P_SMC_A13           (P_DEFINED | P_IDENT(GPIO_PA13) | P_FUNCT(3))
#define P_SMC_A14           (P_DEFINED | P_IDENT(GPIO_PA12) | P_FUNCT(3))
#define P_SMC_A15           (P_DEFINED | P_IDENT(GPIO_PA11) | P_FUNCT(3))
#define P_SMC_A16           (P_DEFINED | P_IDENT(GPIO_PA7) | P_FUNCT(3))
#define P_SMC_A17           (P_DEFINED | P_IDENT(GPIO_PA6) | P_FUNCT(3))
#define P_SMC_A18           (P_DEFINED | P_IDENT(GPIO_PA5) | P_FUNCT(3))
#define P_SMC_A19           (P_DEFINED | P_IDENT(GPIO_PA4) | P_FUNCT(3))
#define P_SMC_A20           (P_DEFINED | P_IDENT(GPIO_PA1) | P_FUNCT(3))
#define P_SMC_A21           (P_DEFINED | P_IDENT(GPIO_PA0) | P_FUNCT(3))
#define P_SMC_A22           (P_DEFINED | P_IDENT(GPIO_PA10) | P_FUNCT(3))
#define P_SMC_A23           (P_DEFINED | P_IDENT(GPIO_PA3) | P_FUNCT(3))
#define P_SMC_A24           (P_DEFINED | P_IDENT(GPIO_PA2) | P_FUNCT(3))
#define P_SMC_A25           (P_DEFINED | P_IDENT(GPIO_PC12) | P_FUNCT(3))
#define P_SMC_ARDY          (P_DEFINED | P_IDENT(GPIO_PB4) | P_FUNCT(3))

#define P_SMC_D0            (P_DEFINED | P_IDENT(GPIO_PE12) | P_FUNCT(3))
#define P_SMC_D1            (P_DEFINED | P_IDENT(GPIO_PE11) | P_FUNCT(3))
#define P_SMC_D2            (P_DEFINED | P_IDENT(GPIO_PE10) | P_FUNCT(3))
#define P_SMC_D3            (P_DEFINED | P_IDENT(GPIO_PE9) | P_FUNCT(3))
#define P_SMC_D4            (P_DEFINED | P_IDENT(GPIO_PE0) | P_FUNCT(3))
#define P_SMC_D5            (P_DEFINED | P_IDENT(GPIO_PD15) | P_FUNCT(3))
#define P_SMC_D6            (P_DEFINED | P_IDENT(GPIO_PD14) | P_FUNCT(3))
#define P_SMC_D7            (P_DEFINED | P_IDENT(GPIO_PD0) | P_FUNCT(3))
#define P_SMC_D8            (P_DEFINED | P_IDENT(GPIO_PB14) | P_FUNCT(3))
#define P_SMC_D9            (P_DEFINED | P_IDENT(GPIO_PB13) | P_FUNCT(3))
#define P_SMC_D10           (P_DEFINED | P_IDENT(GPIO_PB12) | P_FUNCT(3))
#define P_SMC_D11           (P_DEFINED | P_IDENT(GPIO_PB11) | P_FUNCT(3))
#define P_SMC_D12           (P_DEFINED | P_IDENT(GPIO_PB10) | P_FUNCT(3))
#define P_SMC_D13           (P_DEFINED | P_IDENT(GPIO_PB9) | P_FUNCT(3))
#define P_SMC_D14           (P_DEFINED | P_IDENT(GPIO_PB8) | P_FUNCT(3))
#define P_SMC_D15           (P_DEFINED | P_IDENT(GPIO_PB7) | P_FUNCT(3))

#define P_SMC_AMS0          (P_DEFINED | P_IDENT(GPIO_PC15) | P_FUNCT(3))
#define P_SMC_AMS1          (P_DEFINED | P_IDENT(GPIO_PE13) | P_FUNCT(3))
#define P_SMC_AMS2          (P_DEFINED | P_IDENT(GPIO_PC7) | P_FUNCT(3))
#define P_SMC_AMS3          (P_DEFINED | P_IDENT(GPIO_PC8) | P_FUNCT(3))

#define P_SMC_AWE           (P_DEFINED | P_IDENT(GPIO_PB15) | P_FUNCT(3))
#define P_SMC_ARE           (P_DEFINED | P_IDENT(GPIO_PC0) | P_FUNCT(3))
#define P_SMC_AOE           (P_DEFINED | P_IDENT(GPIO_PD1) | P_FUNCT(3))

#define P_SMC_ABE0          (P_DEFINED | P_IDENT(GPIO_PE14) | P_FUNCT(3))
#define P_SMC_ABE1          (P_DEFINED | P_IDENT(GPIO_PE15) | P_FUNCT(3))

/*MSI*/                                                                                                                                                                                                              
#define P_MSI0_D0           (P_DEFINED | P_IDENT(GPIO_PF2) | P_FUNCT(0))                                                                                                                                             
#define P_MSI0_D1           (P_DEFINED | P_IDENT(GPIO_PF3) | P_FUNCT(0))                                                                                                                                             
#define P_MSI0_D2           (P_DEFINED | P_IDENT(GPIO_PF4) | P_FUNCT(0))                                                                                                                                             
#define P_MSI0_D3           (P_DEFINED | P_IDENT(GPIO_PF5) | P_FUNCT(0))                                                                                                                                             
#define P_MSI0_D4           (P_DEFINED | P_IDENT(GPIO_PF6) | P_FUNCT(0))                                                                                                                                             
#define P_MSI0_D5           (P_DEFINED | P_IDENT(GPIO_PF7) | P_FUNCT(0))                                                                                                                                             
#define P_MSI0_D6           (P_DEFINED | P_IDENT(GPIO_PF8) | P_FUNCT(0))                                                                                                                                             
#define P_MSI0_D7           (P_DEFINED | P_IDENT(GPIO_PF9) | P_FUNCT(0))                                                                                                                                             
#define P_MSI0_CMD          (P_DEFINED | P_IDENT(GPIO_PF10) | P_FUNCT(0))                                                                                                                                            
#define P_MSI0_CLK          (P_DEFINED | P_IDENT(GPIO_PF11) | P_FUNCT(0))                                                                                                                                            
#define P_MSI0_CDb          (P_DEFINED | P_IDENT(GPIO_PF12) | P_FUNCT(0))   
/* EMAC RMII Port Mux */
#define P_MII0_ETxD0	(P_DEFINED | P_IDENT(GPIO_PA0) | P_FUNCT(0))
#define P_MII0_ETxD1	(P_DEFINED | P_IDENT(GPIO_PA1) | P_FUNCT(0))
#define P_MII0_MDC	(P_DEFINED | P_IDENT(GPIO_PA2) | P_FUNCT(0))
#define P_MII0_MDIO	(P_DEFINED | P_IDENT(GPIO_PA3) | P_FUNCT(0))
#define P_MII0_ERxD0	(P_DEFINED | P_IDENT(GPIO_PA4) | P_FUNCT(0))
#define P_MII0_ERxD1	(P_DEFINED | P_IDENT(GPIO_PA5) | P_FUNCT(0))
#define P_MII0_REFCLK	(P_DEFINED | P_IDENT(GPIO_PA6) | P_FUNCT(0))
#define P_MII0_CRS	(P_DEFINED | P_IDENT(GPIO_PA7) | P_FUNCT(0))
#define P_MII0_ERxD2	(P_DEFINED | P_IDENT(GPIO_PA8) | P_FUNCT(0))
#define P_MII0_ERxD3	(P_DEFINED | P_IDENT(GPIO_PA9) | P_FUNCT(0))
#define P_MII0_ETxEN	(P_DEFINED | P_IDENT(GPIO_PA10) | P_FUNCT(0))
#define P_MII0_ETxCLK	(P_DEFINED | P_IDENT(GPIO_PA11) | P_FUNCT(0))
#define P_MII0_ETxD2	(P_DEFINED | P_IDENT(GPIO_PA12) | P_FUNCT(0))
#define P_MII0_ETxD3	(P_DEFINED | P_IDENT(GPIO_PA13) | P_FUNCT(0))

#define P_RMII0 {\
	P_MII0_ETxD0, \
	P_MII0_ETxD1, \
	P_MII0_ETxEN, \
	P_MII0_ERxD0, \
	P_MII0_ERxD1, \
	P_MII0_REFCLK, \
	P_MII0_CRS, \
	P_MII0_MDC, \
	P_MII0_MDIO, 0}

#define P_RGMII0 {\
	P_MII0_ETxD0, \
	P_MII0_ETxD1, \
	P_MII0_MDC, \
	P_MII0_MDIO,\
	P_MII0_ERxD0, \
	P_MII0_ERxD1, \
	P_MII0_REFCLK, \
	P_MII0_CRS, \
	P_MII0_ERxD2, \
	P_MII0_ERxD3, \
	P_MII0_ETxEN, \
	P_MII0_ETxCLK, \
	P_MII0_ETxD2, \
	P_MII0_ETxD3, \
	0}

#define P_MII1_CRS	(P_DEFINED | P_IDENT(GPIO_PF13) | P_FUNCT(0))
#define P_MII1_MDC	(P_DEFINED | P_IDENT(GPIO_PF14) | P_FUNCT(0))
#define P_MII1_MDIO	(P_DEFINED | P_IDENT(GPIO_PF15) | P_FUNCT(0))
#define P_MII1_REFCLK	(P_DEFINED | P_IDENT(GPIO_PG0) | P_FUNCT(0))
#define P_MII1_ETxEN	(P_DEFINED | P_IDENT(GPIO_PG1) | P_FUNCT(0))
#define P_MII1_ETxD0	(P_DEFINED | P_IDENT(GPIO_PG2) | P_FUNCT(0))
#define P_MII1_ETxD1	(P_DEFINED | P_IDENT(GPIO_PG3) | P_FUNCT(0))
#define P_MII1_ERxD0	(P_DEFINED | P_IDENT(GPIO_PG4) | P_FUNCT(0))
#define P_MII1_ERxD1	(P_DEFINED | P_IDENT(GPIO_PG5) | P_FUNCT(0))

#define P_RMII1 {\
	P_MII1_ETxD0, \
	P_MII1_ETxD1, \
	P_MII1_ETxEN, \
	P_MII1_ERxD0, \
	P_MII1_ERxD1, \
	P_MII1_REFCLK, \
	P_MII1_CRS, \
	P_MII1_MDC, \
	P_MII1_MDIO, 0}


int peripheral_request(unsigned short per, const char *label);
void peripheral_free(unsigned short per);
int peripheral_request_list(const unsigned short per[], const char *label);
void peripheral_free_list(const unsigned short per[]);

#endif				/* _PORTMUX_H_ */
