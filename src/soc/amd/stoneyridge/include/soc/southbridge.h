/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2010 Advanced Micro Devices, Inc.
 * Copyright (C) 2014 Sage Electronic Engineering, LLC
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef __STONEYRIDGE_H__
#define __STONEYRIDGE_H__

#include <arch/io.h>
#include <types.h>
#include <device/pci_ids.h>
#include <device/device.h>
#include "chip.h"

#define IO_APIC2_ADDR			0xfec20000

/* Offsets from ACPI_MMIO_BASE */
#define APU_SMI_BASE			0xfed80200

#define PM_MMIO_BASE			0xfed80300

#define APU_UART0_BASE			0xfedc6000
#define APU_UART1_BASE			0xfedc8000

/* Power management index/data registers */
#define BIOSRAM_INDEX			0xcd4
#define BIOSRAM_DATA			0xcd5
#define PM_INDEX			0xcd6
#define PM_DATA				0xcd7
#define PM2_INDEX			0xcd0
#define PM2_DATA			0xcd1

#define PM_PCI_CTRL			0x08
#define   FORCE_SLPSTATE_RETRY		BIT(25)
#define   FORCE_STPCLK_RETRY		BIT(24)
#define PM_ACPI_MMIO_EN			0x24
#define PM_SERIRQ_CONF			0x54
#define PM_EVT_BLK			0x60
#define PM1_CNT_BLK			0x62
#define PM_TMR_BLK			0x64
#define PM_CPU_CTRL			0x66
#define PM_GPE0_BLK			0x68
#define PM_ACPI_SMI_CMD			0x6a
#define PM_ACPI_CONF			0x74
#define PM_RST_CTRL1			0xbe
#define   SLPTYPE_CONTROL_EN		BIT(5)
#define PM_PMIO_DEBUG			0xd2
#define PM_MANUAL_RESET			0xd3
#define PM_HUD_SD_FLASH_CTRL		0xe7
#define PM_YANG_SD_FLASH_CTRL		0xe8
#define PM_PCIB_CFG			0xea

#define SYS_RESET			0xcf9

#define STONEYRIDGE_ACPI_IO_BASE	CONFIG_STONEYRIDGE_ACPI_IO_BASE
#define ACPI_PM_EVT_BLK		(STONEYRIDGE_ACPI_IO_BASE + 0x00) /* 4 bytes */
#define ACPI_PM1_CNT_BLK	(STONEYRIDGE_ACPI_IO_BASE + 0x04) /* 2 bytes */
#define ACPI_PM_TMR_BLK		(STONEYRIDGE_ACPI_IO_BASE + 0x18) /* 4 bytes */
#define ACPI_GPE0_BLK		(STONEYRIDGE_ACPI_IO_BASE + 0x10) /* 8 bytes */
#define ACPI_CPU_CONTROL	(STONEYRIDGE_ACPI_IO_BASE + 0x08) /* 6 bytes */

#define REV_STONEYRIDGE_A11			0x11
#define REV_STONEYRIDGE_A12			0x12

#define SPIROM_BASE_ADDRESS_REGISTER	0xa0
#define   ROUTE_TPM_2_SPI		BIT(3)
#define   SPI_ROM_ENABLE		0x02
#define   SPI_BASE_ADDRESS		0xfec10000

#define LPC_IO_PORT_DECODE_ENABLE	0x44
#define   DECODE_ENABLE_PARALLEL_PORT0	BIT(0)
#define   DECODE_ENABLE_PARALLEL_PORT1	BIT(1)
#define   DECODE_ENABLE_PARALLEL_PORT2	BIT(2)
#define   DECODE_ENABLE_PARALLEL_PORT3	BIT(3)
#define   DECODE_ENABLE_PARALLEL_PORT4	BIT(4)
#define   DECODE_ENABLE_PARALLEL_PORT5	BIT(5)
#define   DECODE_ENABLE_SERIAL_PORT0	BIT(6)
#define   DECODE_ENABLE_SERIAL_PORT1	BIT(7)
#define   DECODE_ENABLE_SERIAL_PORT2	BIT(8)
#define   DECODE_ENABLE_SERIAL_PORT3	BIT(9)
#define   DECODE_ENABLE_SERIAL_PORT4	BIT(10)
#define   DECODE_ENABLE_SERIAL_PORT5	BIT(11)
#define   DECODE_ENABLE_SERIAL_PORT6	BIT(12)
#define   DECODE_ENABLE_SERIAL_PORT7	BIT(13)
#define   DECODE_ENABLE_AUDIO_PORT0	BIT(14)
#define   DECODE_ENABLE_AUDIO_PORT1	BIT(15)
#define   DECODE_ENABLE_AUDIO_PORT2	BIT(16)
#define   DECODE_ENABLE_AUDIO_PORT3	BIT(17)
#define   DECODE_ENABLE_MIDI_PORT0	BIT(18)
#define   DECODE_ENABLE_MIDI_PORT1	BIT(19)
#define   DECODE_ENABLE_MIDI_PORT2	BIT(20)
#define   DECODE_ENABLE_MIDI_PORT3	BIT(21)
#define   DECODE_ENABLE_MSS_PORT0	BIT(22)
#define   DECODE_ENABLE_MSS_PORT1	BIT(23)
#define   DECODE_ENABLE_MSS_PORT2	BIT(24)
#define   DECODE_ENABLE_MSS_PORT3	BIT(25)
#define   DECODE_ENABLE_FDC_PORT0	BIT(26)
#define   DECODE_ENABLE_FDC_PORT1	BIT(27)
#define   DECODE_ENABLE_GAME_PORT	BIT(28)
#define   DECODE_ENABLE_KBC_PORT	BIT(29)
#define   DECODE_ENABLE_ACPIUC_PORT	BIT(30)
#define   DECODE_ENABLE_ADLIB_PORT	BIT(31)

#define LPC_IO_OR_MEM_DECODE_ENABLE	0x48
#define   LPC_WIDEIO2_ENABLE		BIT(25)
#define   LPC_WIDEIO1_ENABLE		BIT(24)
#define   LPC_WIDEIO0_ENABLE		BIT(2)

#define LPC_WIDEIO_GENERIC_PORT		0x64

#define LPC_ALT_WIDEIO_RANGE_ENABLE	0x74
#define   LPC_ALT_WIDEIO2_ENABLE	BIT(3)
#define   LPC_ALT_WIDEIO1_ENABLE	BIT(2)
#define   LPC_ALT_WIDEIO0_ENABLE	BIT(0)

#define LPC_WIDEIO2_GENERIC_PORT	0x90

#define SPI_CNTRL0			0x00
#define   SPI_READ_MODE_MASK		(BIT(30) | BIT(29) | BIT(18))
/* Nominal is 16.7MHz on older devices, 33MHz on newer */
#define   SPI_READ_MODE_NOM		0x00000000
#define   SPI_READ_MODE_DUAL112		(          BIT(29)          )
#define   SPI_READ_MODE_QUAD114		(          BIT(29) | BIT(18))
#define   SPI_READ_MODE_DUAL122		(BIT(30)                    )
#define   SPI_READ_MODE_QUAD144		(BIT(30) |           BIT(18))
#define   SPI_READ_MODE_NORMAL66	(BIT(30) | BIT(29)          )
#define   SPI_READ_MODE_FAST		(BIT(30) | BIT(29) | BIT(18))
#define   SPI_ARB_ENABLE		BIT(19)

#define SPI_CNTRL1			0x0c
/* Use SPI_SPEED_16M-SPI_SPEED_66M below for the southbridge */
#define   SPI_CNTRL1_SPEED_MASK		(BIT(15) | BIT(14) | BIT(13) | BIT(12))
#define   SPI_NORM_SPEED_SH		12
#define   SPI_FAST_SPEED_SH		8

#define SPI100_ENABLE			0x20
#define   SPI_USE_SPI100		BIT(0)

#define SPI100_SPEED_CONFIG		0x22
#define   SPI_SPEED_66M			(0x0)
#define   SPI_SPEED_33M			(                  BIT(0))
#define   SPI_SPEED_22M			(         BIT(1)         )
#define   SPI_SPEED_16M			(         BIT(1) | BIT(0))
#define   SPI_SPEED_100M		(BIT(2)                  )
#define   SPI_SPEED_800K		(BIT(2) |          BIT(0))
#define   SPI_NORM_SPEED_NEW_SH		12
#define   SPI_FAST_SPEED_NEW_SH		8
#define   SPI_ALT_SPEED_NEW_SH		4
#define   SPI_TPM_SPEED_NEW_SH		0

#define SPI100_HOST_PREF_CONFIG		0x2c
#define   SPI_RD4DW_EN_HOST		BIT(15)

static inline int sb_sata_enable(void)
{
	/* True if IDE or AHCI. */
	return (CONFIG_STONEYRIDGE_SATA_MODE == 0) ||
					(CONFIG_STONEYRIDGE_SATA_MODE == 2);
}

static inline int sb_ide_enable(void)
{
	/* True if IDE or LEGACY IDE. */
	return (CONFIG_STONEYRIDGE_SATA_MODE == 0) ||
					(CONFIG_STONEYRIDGE_SATA_MODE == 3);
}

void sb_enable_rom(void);
void configure_stoneyridge_uart(void);
void sb_clk_output_48Mhz(void);
void sb_disable_4dw_burst(void);
void sb_enable(device_t dev);
void southbridge_final(void *chip_info);
void southbridge_init(void *chip_info);
void sb_lpc_port80(void);
void sb_lpc_decode(void);
void sb_pci_port80(void);
void sb_read_mode(u32 mode);
void sb_set_spi100(u16 norm, u16 fast, u16 alt, u16 tpm);
void sb_set_readspeed(u16 norm, u16 fast);
void sb_tpm_decode_spi(void);
void lpc_wideio_512_window(uint16_t base);
void lpc_wideio_16_window(uint16_t base);
u8 pm_read8(u8 reg);
u16 pm_read16(u8 reg);
u32 pm_read32(u8 reg);
void pm_write8(u8 reg, u8 value);
void pm_write16(u8 reg, u16 value);
void pm_write32(u8 reg, u32 value);
u16 smi_read16(u8 reg);
u32 smi_read32(u8 reg);
void smi_write16(u8 reg, u16 value);
void smi_write32(u8 reg, u32 value);
int s3_load_nvram_early(int size, u32 *old_dword, int nvram_pos);
void s3_resume_init_data(void *FchParams);
int s3_save_nvram_early(u32 dword, int size, int  nvram_pos);
void bootblock_fch_early_init(void);

#endif /* __STONEYRIDGE_H__ */
