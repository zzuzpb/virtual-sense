

#define EEPROM_ADDRESS	0x51
#define WRITE_ENABLE()	P2OUT &= ~BIT5;
#define WRITE_PROTECT() P2OUT |= BIT5;

#define     PAGE_SIZE   128

void write_byte_24AA512(uint8_t dev_address, uint16_t address, uint8_t data);
uint8_t read_byte_24AA512(uint8_t dev_address, uint16_t address);
uint8_t is_idle_24AA512(uint8_t dev_address);
void write_sequential_24AA512(uint8_t dev_address, uint16_t address, uint16_t size, unsigned char * data);
void read_sequential_24AA512(uint8_t dev_address, uint16_t address, uint16_t size, unsigned char * data);
