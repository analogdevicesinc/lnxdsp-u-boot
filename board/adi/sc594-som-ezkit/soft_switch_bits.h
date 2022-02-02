#ifndef SOFT_SWITCH_BITS
#define SOFT_SWITCH_BITS

/*  
    I2C Address 0x22

	U6 Port A                      U6 Port B

	7--------------- ~ADAU1979_EN   |  7--------------- ~GIGE_RESET
	| 6------------- ~ADAU1962_EN   |  | 6------------- ~ETH1_RESET
	| | 5----------- ~ADAU_RESET    |  | | 5----------- ~ETH1_EN
	| | | 4---------                |  | | | 4--------- ~MLB_EN
	| | | | 3-------                |  | | | | 3------- AUDIO_JACK_SEL
	| | | | | 2----- ~MICROSD_SPI   |  | | | | | 2----- ~SPDIF_OPTICAL_EN
	| | | | | | 1--- PB_EN          |  | | | | | | 1--- ~SPDIF_DIGITAL_EN
	| | | | | | | 0- EEPROM_EN      |  | | | | | | | 0- OCTAL_SPI_CS_EN
	| | | | | | | |                 |  | | | | | | | |
	O O O O O O O O                 |  O O O O O O O O   (I/O direction)
	1 1 1 1 1 1 0 0                 |  1 0 1 1 0 1 1 1   (value being set)
*/

//Port A
#define ADAU1979(x)      (x << 7)
#define ENABLE_ADAU1979  ADAU1979(0)
#define DISABLE_ADAU1979 ADAU1979(1)

#define ADAU1962(x)      (x << 6)
#define ENABLE_ADAU1962  ADAU1962(0)
#define DISABLE_ADAU1962 ADAU1962(1)

#define ADAU_RESET(x)      (x << 5)
#define ENABLE_ADAU_RESET  ADAU_RESET(0)
#define DISABLE_ADAU_RESET ADAU_RESET(1)

#define MICROSD_SPI(x)      (x << 2)
#define ENABLE_MICROSD_SPI  MICROSD_SPI(0)
#define DISABLE_MICROSD_SPI MICROSD_SPI(1)

#define PB(x)       (x << 1)
#define ENABLE_PB   PB(1)
#define DISABLE_PB  PB(0)

#define EEPROM(x)       (x << 0)
#define ENABLE_EEPROM   EEPROM(1)
#define DISABLE_EEPROM  EEPROM(0)

//Port B
#define GIGE_RESET(x)      (x << 7)
#define ENABLE_GIGE_RESET  GIGE_RESET(0)
#define DISABLE_GIGE_RESET GIGE_RESET(1)

#define ETH1_RESET(x)      (x << 6)
#define ENABLE_ETH1_RESET  ETH1_RESET(0)
#define DISABLE_ETH1_RESET ETH1_RESET(1)

#define ETH1_EN(x)      (x << 5)
#define ENABLE_ETH1_EN  ETH1_EN(0)
#define DISABLE_ETH1_EN ETH1_EN(1)

#define MLB(x)          (x << 4)
#define ENABLE_MLB      MLB(0)
#define DISABLE_MLB     MLB(1)

#define AUDIO_JACK_SEL(x)	   (x << 3)
#define ENABLE_AUDIO_JACK_SEL  AUDIO_JACK_SEL(1)
#define DISABLE_AUDIO_JACK_SEL AUDIO_JACK_SEL(0)

#define SPDIF_OPTICAL(x)      (x << 2)
#define ENABLE_SPDIF_OPTICAL  SPDIF_OPTICAL(0)
#define DISABLE_SPDIF_OPTICAL SPDIF_OPTICAL(1)

#define SPDIF_DIGITAL(x)       (x << 1)
#define ENABLE_SPDIF_DIGITAL   SPDIF_DIGITAL(0)
#define DISABLE_SPDIF_DIGITAL  SPDIF_DIGITAL(1)

#define OCTAL_SPI_CS(x)       (x << 0)
#define ENABLE_OCTAL_SPI_CS   OCTAL_SPI_CS(1)
#define DISABLE_OCTAL_SPI_CS  OCTAL_SPI_CS(0)

/*

    I2C Address 0x20

	U16 Port A                      U16 Port B

	7--------------- Unused         |  7--------------- Unused
	| 6------------- ~UART0_FLOW_EN |  | 6------------- Unused
	| | 5----------- ~UART0_EN      |  | | 5----------- Unused
	| | | 4--------- ~SPI2D2_D3_EN  |  | | | 4--------- Unused
	| | | | 3------- ~SPI2FLASH_CS  |  | | | | 3------- Unused
	| | | | | 2----- LED            |  | | | | | 2----- Unused
	| | | | | | 1--- LED            |  | | | | | | 1--- EMMC_SOM_EN
	| | | | | | | 0- LED            |  | | | | | | | 0- EMMC
	| | | | | | | |                 |  | | | | | | | |
	O O O O O O O O                 |  O O O O O O O O   (I/O direction)
	0 1 1 1 1 1 1 1                 |  0 0 0 0 0 0 1 0   (value being set)
*/

//Port A
#define UART0_FLOW(x)      (x << 6)
#define ENABLE_UART0_FLOW  UART0_FLOW(0)
#define DISABLE_UART0_FLOW UART0_FLOW(1)

#define UART0(x)      (x << 5)
#define ENABLE_UART0  UART0(0)
#define DISABLE_UART0 UART0(1)

#define SPI2D2_D3(x)          (x << 4)
#define ENABLE_SPI2D2_D3      SPI2D2_D3(0)
#define DISABLE_SPI2D2_D3     SPI2D2_D3(1)

#define SPI2FLASH_CS(x)       (x << 3)
#define ENABLE_SPI2FLASH_CS   SPI2FLASH_CS(0)
#define DISABLE_SPI2FLASH_CS  SPI2FLASH_CS(1)

#define LED3(x)	     (x << 2)
#define ENABLE_LED3  LED3(1)
#define DISABLE_LED3 LED3(0)

#define LED2(x)      (x << 1)
#define ENABLE_LED2  LED2(1)
#define DISABLE_LED2 LED2(0)

#define LED1(x)       (x << 0)
#define ENABLE_LED1   LED1(1)
#define DISABLE_LED1  LED1(0)

//Port B
#define EMMC_SOM(x)      (x << 1)
#define ENABLE_EMMC_SOM  EMMC_SOM(1)
#define DISABLE_EMMC_SOM EMMC_SOM(0)

#define EMMC(x)       (x << 0)
#define ENABLE_EMMC   EMMC(1)
#define DISABLE_EMMC  EMMC(0)

#endif
