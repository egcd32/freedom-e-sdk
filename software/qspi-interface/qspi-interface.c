#include <stdio.h>
#include <metal/spi.h>

int main() {
	puts("test");
	/* Instruction Hex code according to Cypress datasheet */
	
	/* Read 4-byte address */
	const char FOUR_READ = 0x13;
	
	/* Read device ID */
	const char RDID = 0x9F;

	/* Iterate over a few device numbers to select the first available SPI device */
	int number_of_tries = 0;
	struct metal_spi *spi;
	while ((spi = metal_spi_get_device(number_of_tries)) == NULL && number_of_tries < 10) {
		number_of_tries++;
	}
	
	/* Report that no SPI device can be found. */
	if (spi == NULL) {
		puts("Failed to get spi device\n");
		return 1;
	}

	/* Initialize SPI device to 100,000 baud rate */
	metal_spi_init(spi, 100000);

	/* CPOL = 0, CPHA = 0, quad spi, MSb-first, CS active low */
	struct metal_spi_config config = {
		.protocol = METAL_SPI_QUAD,
		.polarity = 0,
		.phase = 0,
		.little_endian = 0,
		.cs_active_high = 0,
		.csid = 0,
	};

	/* Instruction for reading a 4-byte address */
	char instruction[1] = {RDID};		
	char receive_buffer[1] = {0};
	int transfer_success = metal_spi_transfer(spi, &config, 1, instruction, receive_buffer);
	if (transfer_success != 0) {
		puts("spi transfer failed.");
		return 1;
	}
	return 0;
}
