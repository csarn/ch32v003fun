#ifndef _MINICHLINK_H
#define _MINICHLINK_H

#include <stdint.h>

struct MiniChlinkFunctions
{
	// All functions return 0 if OK, negative number if fault, positive number as status code.

	// Low-level functions, if they exist.
	int (*WriteReg32)( void * dev, uint8_t reg_7_bit, uint32_t command );
	int (*ReadReg32)( void * dev, uint8_t reg_7_bit, uint32_t * commandresp );
	int (*FlushLLCommands)( void * dev );
	int (*DelayUS)( void * dev, int microseconds );

	// Higher-level functions can be generated automatically.
	int (*SetupInterface)( void * dev );
	int (*Control3v3)( void * dev, int bOn );
	int (*Control5v)( void * dev, int bOn );
	int (*Unbrick)( void * dev ); // Turns on chip, erases everything, powers off.

	int (*Exit)( void * dev );

	int (*HaltMode)( void * dev, int one_if_halt_zero_if_resume );
	int (*ConfigureNRSTAsGPIO)( void * dev, int one_if_yes_gpio );

	// WARNING: Reading/writing must be at 32-bit boundaries for 32-bit sizes.
	// WARNING: Writing binary blobs may write groups of 64-bytes.
	int (*WriteBinaryBlob)( void * dev, uint32_t address_to_write, uint32_t blob_size, uint8_t * blob );
	int (*ReadBinaryBlob)( void * dev, uint32_t address_to_read_from, uint32_t read_size, uint8_t * blob );
};


#define DMDATA0        0x04
#define DMDATA1        0x05
#define DMCONTROL      0x10
#define DMSTATUS       0x11
#define DMHARTINFO     0x12
#define DMABSTRACTCS   0x16
#define DMCOMMAND      0x17
#define DMABSTRACTAUTO 0x18
#define DMPROGBUF0     0x20
#define DMPROGBUF1     0x21
#define DMPROGBUF2     0x22
#define DMPROGBUF3     0x23
#define DMPROGBUF4     0x24
#define DMPROGBUF5     0x25
#define DMPROGBUF6     0x26
#define DMPROGBUF7     0x27

#define CPBR       0x7C
#define CFGR       0x7D
#define SHDWCFGR   0x7E

extern struct MiniChlinkFunctions MCF;

// Returns 'dev' on success, else 0.
void * TryInit_WCHLinkE();
void * TryInit_ESP32S2CHFUN();

// Returns 0 if ok, populated, 1 if not populated.
int SetupAutomaticHighLevelFunctions();

#endif

