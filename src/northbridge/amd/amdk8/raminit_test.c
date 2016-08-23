#include <unistd.h>
#include <limits.h>
#include <lib.h>
#include <stdint.h>
#include <string.h>
#include <setjmp.h>
#include <device/pci_def.h>
#include <stdlib.h>
#include "amdk8.h"

jmp_buf end_buf;

static int is_cpu_pre_c0(void)
{
	return 0;
}

#define PCI_ADDR(BUS, DEV, FN, WHERE) ( \
	(((BUS) & 0xFF) << 16) | \
	(((DEV) & 0x1f) << 11) | \
	(((FN) & 0x07) << 8) | \
	((WHERE) & 0xFF))

#define PCI_DEV(BUS, DEV, FN) ( \
	(((BUS) & 0xFF) << 16) | \
	(((DEV) & 0x1f) << 11) | \
	(((FN)  & 0x7) << 8))

#define PCI_ID(VENDOR_ID, DEVICE_ID) \
	((((DEVICE_ID) & 0xFFFF) << 16) | ((VENDOR_ID) & 0xFFFF))

typedef unsigned device_t;

unsigned char pci_register[256*5*3*256];

static uint8_t pci_read_config8(device_t dev, unsigned where)
{
	unsigned addr;
	addr = dev | where;
	return pci_register[addr];
}

static uint16_t pci_read_config16(device_t dev, unsigned where)
{
	unsigned addr;
	addr = dev | where;
	return pci_register[addr] | (pci_register[addr + 1]  << 8);
}

static uint32_t pci_read_config32(device_t dev, unsigned where)
{
	unsigned addr;
	uint32_t value;
	addr = dev | where;
	value =  pci_register[addr] |
		(pci_register[addr + 1]  << 8) |
		(pci_register[addr + 2]  << 16) |
		(pci_register[addr + 3]  << 24);

#if 0
	printk(BIOS_DEBUG, "pcir32(%08x): %08x\n", addr, value);
#endif
	return value;

}

static void pci_write_config8(device_t dev, unsigned where, uint8_t value)
{
	unsigned addr;
	addr = dev | where;
	pci_register[addr] = value;
}

static void pci_write_config16(device_t dev, unsigned where, uint16_t value)
{
	unsigned addr;
	addr = dev | where;
	pci_register[addr] = value & 0xff;
	pci_register[addr + 1] = (value >> 8) & 0xff;
}

static void pci_write_config32(device_t dev, unsigned where, uint32_t value)
{
	unsigned addr;
	addr = dev | where;
	pci_register[addr] = value & 0xff;
	pci_register[addr + 1] = (value >> 8) & 0xff;
	pci_register[addr + 2] = (value >> 16) & 0xff;
	pci_register[addr + 3] = (value >> 24) & 0xff;

#if 0
	printk(BIOS_DEBUG, "pciw32(%08x, %08x)\n", addr, value);
#endif
}

#define PCI_DEV_INVALID (0xffffffffU)
static device_t pci_locate_device(unsigned pci_id, device_t dev)
{
	for (; dev <= PCI_DEV(255, 31, 7); dev += PCI_DEV(0,0,1)) {
		unsigned int id;
		id = pci_read_config32(dev, 0);
		if (id == pci_id) {
			return dev;
		}
	}
	return PCI_DEV_INVALID;
}




static void uart_tx_byte(unsigned char data)
{
	write(STDOUT_FILENO, &data, 1);
}
static void hlt(void)
{
	longjmp(end_buf, 2);
}
#include "console/console.c"

unsigned long log2(unsigned long x)
{
	// assume 8 bits per byte.
	unsigned long i = 1 << (sizeof(x)*8 - 1);
	unsigned long pow = sizeof(x) * 8 - 1;

	if (! x) {
		static const char errmsg[] = " called with invalid parameter of 0\n";
		write(STDERR_FILENO, __func__, sizeof(__func__) - 1);
		write(STDERR_FILENO, errmsg, sizeof(errmsg) - 1);
		hlt();
	}
	for (; i > x; i >>= 1, pow--)
		;

	return pow;
}

typedef struct msr_struct
{
	unsigned lo;
	unsigned hi;
} msr_t;

static inline msr_t rdmsr(unsigned index)
{
	msr_t result;
	result.lo = 0;
	result.hi = 0;
	return result;
}

static inline void wrmsr(unsigned index, msr_t msr)
{
}

#include "raminit.h"

#define SIO_BASE 0x2e

static void hard_reset(void)
{
	/* FIXME implement the hard reset case... */
	longjmp(end_buf, 3);
}

static void memreset_setup(void)
{
	/* Nothing to do */
}

static void memreset(int controllers, const struct mem_controller *ctrl)
{
	/* Nothing to do */
}

static inline void activate_spd_rom(const struct mem_controller *ctrl)
{
	/* nothing to do */
}


static uint8_t spd_mt4lsdt464a[256] =
{
	0x80, 0x08, 0x04, 0x0C, 0x08, 0x01, 0x40, 0x00, 0x01, 0x70,
	0x54, 0x00, 0x80, 0x10, 0x00, 0x01, 0x8F, 0x04, 0x06, 0x01,
	0x01, 0x00, 0x0E, 0x75, 0x54, 0x00, 0x00, 0x0F, 0x0E, 0x0F,

	0x25, 0x08, 0x15, 0x08, 0x15, 0x08, 0x00, 0x12, 0x01, 0x4E,
	0x9C, 0xE4, 0xB7, 0x46, 0x2C, 0xFF, 0x01, 0x02, 0x03, 0x04,
	0x05, 0x06, 0x07, 0x08, 0x09, 0x01, 0x02, 0x03, 0x04, 0x05,
	0x06, 0x07, 0x08, 0x09, 0x00,
};

static uint8_t spd_micron_512MB_DDR333[256] =
{
	0x80, 0x08, 0x07, 0x0d, 0x0b, 0x02, 0x48, 0x00, 0x04, 0x60,
	0x70, 0x02, 0x82, 0x04, 0x04, 0x01, 0x0e, 0x04, 0x0c, 0x01,
	0x02, 0x26, 0xc0, 0x75, 0x70, 0x00, 0x00, 0x48, 0x30, 0x48,
	0x2a, 0x80, 0x80, 0x80, 0x45, 0x45, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x3c, 0x48, 0x30, 0x28, 0x50, 0x00, 0x01, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x10, 0x6f, 0x2c, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0x01, 0x33, 0x36, 0x56, 0x44, 0x44, 0x46, 0x31,
	0x32, 0x38, 0x37, 0x32, 0x47, 0x2d, 0x33, 0x33, 0x35, 0x43,
	0x33, 0x03, 0x00, 0x03, 0x23, 0x17, 0x07, 0x5a, 0xb2, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

static uint8_t spd_micron_256MB_DDR333[256] =
{
	0x80, 0x08, 0x07, 0x0d, 0x0b, 0x01, 0x48, 0x00, 0x04, 0x60,
	0x70, 0x02, 0x82, 0x04, 0x04, 0x01, 0x0e, 0x04, 0x0c, 0x01,
	0x02, 0x26, 0xc0, 0x75, 0x70, 0x00, 0x00, 0x48, 0x30, 0x48,
	0x2a, 0x80, 0x80, 0x80, 0x45, 0x45, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x3c, 0x48, 0x30, 0x23, 0x50, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x58, 0x2c, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0x01, 0x31, 0x38, 0x56, 0x44, 0x44, 0x46, 0x36,
	0x34, 0x37, 0x32, 0x47, 0x2d, 0x33, 0x33, 0x35, 0x43, 0x31,
	0x20, 0x01, 0x00, 0x03, 0x19, 0x17, 0x05, 0xb2, 0xf4, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
};

#define MAX_DIMMS 16
static uint8_t spd_data[MAX_DIMMS*256];

static unsigned spd_count, spd_fail_count;
static int spd_read_byte(unsigned device, unsigned address)
{
	int result;
	spd_count++;
	if ((device < DIMM0) || (device >= (DIMM0 + MAX_DIMMS))) {
		result = -1;
	}
	else {
		device -= DIMM0; /* 0x50 */

		if (address > 256) {
			result = -1;
		}
		else if (spd_data[(device << 8) | 2] != 7) {
			result = -1;
		}
		else {
			result = spd_data[(device << 8) | address];
		}
	}
#if 0
	printk(BIOS_DEBUG, "spd_read_byte(%08x, %08x) -> %08x\n",
		device, address, result);
#endif
	if (spd_count >= spd_fail_count) {
		result = -1;
	}
	return result;
}

/* no specific code here. this should go away completely */
static void coherent_ht_mainboard(unsigned cpus)
{
}

#include "raminit.c"
#include "../../../lib/generic_sdram.c"

#define FIRST_CPU  1
#define SECOND_CPU 1
#define TOTAL_CPUS (FIRST_CPU + SECOND_CPU)
static void raminit_main(void)
{
	/*
	 * GPIO28 of 8111 will control H0_MEMRESET_L
	 * GPIO29 of 8111 will control H1_MEMRESET_L
	 */
	static const struct mem_controller cpu[] = {
#if FIRST_CPU
		{
			.node_id = 0,
			.f0 = PCI_DEV(0, 0x18, 0),
			.f1 = PCI_DEV(0, 0x18, 1),
			.f2 = PCI_DEV(0, 0x18, 2),
			.f3 = PCI_DEV(0, 0x18, 3),
			.channel0 = { DIMM0+0, DIMM0+2, DIMM0+4, DIMM0+6 },
			.channel1 = { DIMM0+1, DIMM0+3, DIMM0+5, DIMM0+7 },
		},
#endif
#if SECOND_CPU
		{
			.node_id = 1,
			.f0 = PCI_DEV(0, 0x19, 0),
			.f1 = PCI_DEV(0, 0x19, 1),
			.f2 = PCI_DEV(0, 0x19, 2),
			.f3 = PCI_DEV(0, 0x19, 3),
			.channel0 = { DIMM0+8, DIMM0+10, DIMM0+12, DIMM0+14 },
			.channel1 = { DIMM0+9, DIMM0+11, DIMM0+13, DIMM0+15 },
		},
#endif
	};
	console_init();
	memreset_setup();
	sdram_initialize(ARRAY_SIZE(cpu), cpu);

}

static void reset_tests(void)
{
	/* Clear the results of any previous tests */
	memset(pci_register, 0, sizeof(pci_register));
	memset(spd_data, 0, sizeof(spd_data));
	spd_count = 0;
	spd_fail_count = UINT_MAX;

	pci_write_config32(PCI_DEV(0, 0x18, 3), NORTHBRIDGE_CAP,
		NBCAP_128Bit |
		NBCAP_MP|  NBCAP_BIG_MP |
		/* NBCAP_ECC | NBCAP_CHIPKILL_ECC | */
		(NBCAP_MEMCLK_200MHZ << NBCAP_MEMCLK_SHIFT) |
		NBCAP_MEMCTRL);

	pci_write_config32(PCI_DEV(0, 0x19, 3), NORTHBRIDGE_CAP,
		NBCAP_128Bit |
		NBCAP_MP|  NBCAP_BIG_MP |
		/* NBCAP_ECC | NBCAP_CHIPKILL_ECC | */
		(NBCAP_MEMCLK_200MHZ << NBCAP_MEMCLK_SHIFT) |
		NBCAP_MEMCTRL);

#if 0
	pci_read_config32(PCI_DEV(0, 0x18, 3), NORTHBRIDGE_CAP);
#endif
}

static void test1(void)
{
	reset_tests();

	memcpy(&spd_data[0*256], spd_micron_512MB_DDR333, 256);
	memcpy(&spd_data[1*256], spd_micron_512MB_DDR333, 256);
#if 0
	memcpy(&spd_data[2*256], spd_micron_512MB_DDR333, 256);
	memcpy(&spd_data[3*256], spd_micron_512MB_DDR333, 256);

	memcpy(&spd_data[8*256], spd_micron_512MB_DDR333, 256);
	memcpy(&spd_data[9*256], spd_micron_512MB_DDR333, 256);
	memcpy(&spd_data[10*256], spd_micron_512MB_DDR333, 256);
	memcpy(&spd_data[11*256], spd_micron_512MB_DDR333, 256);
#endif

	raminit_main();

#if 0
	printk(BIOS_DEBUG, "spd_count: %d\n", spd_count);
#endif

}


static void do_test2(int i)
{
	jmp_buf tmp_buf;
	memcpy(&tmp_buf, &end_buf, sizeof(end_buf));
	if (setjmp(end_buf) != 0) {
		goto done;
	}
	reset_tests();
	spd_fail_count = i;

	printk(BIOS_DEBUG, "\nSPD will fail after: %d accesses.\n", %d);

	memcpy(&spd_data[0*256], spd_micron_512MB_DDR333, 256);
	memcpy(&spd_data[1*256], spd_micron_512MB_DDR333, 256);

	raminit_main();

done:
	memcpy(&end_buf, &tmp_buf, sizeof(end_buf));
}

static void test2(void)
{
	int i;
	for (i = 0; i < 0x48; i++) {
		do_test2(i);
	}

}

int main(int argc, char **argv)
{
	if (setjmp(end_buf) != 0) {
		return -1;
	}
	test1();
	test2();
	return 0;
}
