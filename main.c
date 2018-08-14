/**
 * Copyright (C) Robert Bosch. All Rights Reserved. Confidential.
 *
 * Disclaimer
 *
 * Common: Bosch Sensortec products are developed for the consumer goods
 * industry. They may only be used within the parameters of the respective valid
 * product data sheet.    Bosch Sensortec products are provided with the express
 * understanding that there is no warranty of fitness for a particular purpose.
 * They are not fit for use in life-sustaining, safety or security sensitive
 * systems or any system or device that may lead to bodily harm or property
 * damage if the system or device malfunctions. In addition, Bosch Sensortec
 * products are not fit for use in products which interact with motor vehicle
 * systems.  The resale and/or use of products are at the purchaser's own risk
 * and his own responsibility. The examination of fitness for the intended use
 * is the sole responsibility of the Purchaser.
 *
 * The purchaser shall indemnify Bosch Sensortec from all third party claims,
 * including any claims for incidental, or consequential damages, arising from
 * any product use not covered by the parameters of the respective valid product
 * data sheet or not approved by Bosch Sensortec and reimburse Bosch Sensortec
 * for all costs in connection with such claims.
 *
 * The purchaser must monitor the market for the purchased products,
 * particularly with regard to product safety and inform Bosch Sensortec without
 * delay of all security relevant incidents.
 *
 * Engineering Samples are marked with an asterisk (*) or (e). Samples may vary
 * from the valid technical specifications of the product series. They are
 * therefore not intended or fit for resale to third parties or for use in end
 * products. Their sole purpose is internal client testing. The testing of an
 * engineering sample may in no way replace the testing of a product series.
 * Bosch Sensortec assumes no liability for the use of engineering samples. By
 * accepting the engineering samples, the Purchaser agrees to indemnify Bosch
 * Sensortec from all claims arising from the use of engineering samples.
 *
 * Special: This software module (hereinafter called "Software") and any
 * information on application-sheets (hereinafter called "Information") is
 * provided free of charge for the sole purpose to support your application
 * work. The Software and Information is subject to the following terms and
 * conditions:
 *
 * The Software is specifically designed for the exclusive use for Bosch
 * Sensortec products by personnel who have special experience and training. Do
 * not use this Software if you do not have the proper experience or training.
 *
 * This Software package is provided `` as is `` and without any expressed or
 * implied warranties, including without limitation, the implied warranties of
 * merchantability and fitness for a particular purpose.
 *
 * Bosch Sensortec and their representatives and agents deny any liability for
 * the functional impairment of this Software in terms of fitness, performance
 * and safety. Bosch Sensortec and their representatives and agents shall not be
 * liable for any direct or indirect damages or injury, except as otherwise
 * stipulated in mandatory applicable law.
 *
 * The Information provided is believed to be accurate and reliable. Bosch
 * Sensortec assumes no responsibility for the consequences of use of such
 * Information nor for any infringement of patents or other rights of third
 * parties which may result from its use.
 *
 * @file        algo_adapter.c
 *
 * @brief  this file defined the mcu component configuration
 *
 *
 */
 
/*********************************************************************/
/* header files */
#include <bmp3.h>
#include "stdio.h"
#include "SAM4S.h"
#include "board.h"
#include "twi.h"
#include "asf.h"
#include <tgmath.h> 
#include "hal_interface.h"

/*********************************************************************/
/* macro definitions */
#define BHY_INT_PIN			PIO_PC7_IDX
#define BHY_SAGPIO7_PIN		PIO_PC2_IDX
#define ON					(1)
#define OFF					(0)
// Debug Line Pin
#define TEST_PIN			PIO_PC1_IDX


/*********************************************************************/
/* functions */
int8_t I2C_READ(uint8_t device_addr, uint8_t register_addr, uint8_t *register_data, uint16_t length);
int8_t I2C_WRITE(uint8_t device_addr, uint8_t register_addr, uint8_t *register_data, uint16_t length);
extern void delaym_local(uint32_t);

int8_t set_normal_mode(struct bmp3_dev *dev);
int8_t get_sensor_data(struct bmp3_dev *dev);
int8_t configure_int_pin(struct bmp3_dev *dev);
int8_t configure_and_get_fifo_data(struct bmp3_dev *dev);


int main(void)
{
	struct bmp3_dev dev;
	int8_t rslt = BMP3_OK;
	
	dev.dev_id = BMP3_I2C_ADDR_PRIM;
	dev.intf = BMP3_I2C_INTF;
	dev.read = I2C_READ;
	dev.write = I2C_WRITE;
	dev.delay_ms = delaym_local;	

	/* system initialization */
    sysclk_init();
    board_init();
	configure_shuttle_vdds();
	// pin initialization
	ioport_set_pin_dir(BHY_INT_PIN, IOPORT_DIR_INPUT);
	ioport_set_pin_level(BHY_INT_PIN, 0);
	ioport_set_pin_dir(BHY_SAGPIO7_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(BHY_SAGPIO7_PIN, 0);
	
	ioport_set_pin_dir(TEST_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(TEST_PIN, 0);

	/*power on the shuttle board */
	delaym_local(500);
	set_shuttle_vdd(ON);
	set_shuttle_vddio(ON);
	delaym_local(500);

	rslt = bmp3_init(&dev);
	
	rslt = configure_and_get_fifo_data(&dev); 	
}


int8_t I2C_READ(uint8_t device_addr, uint8_t register_addr, uint8_t *register_data, uint16_t length)
{
	twi_packet_t packet_rx;
	
	packet_rx.chip        = device_addr;
	packet_rx.addr[0]     = register_addr;
	packet_rx.addr_length = 0x01;
	packet_rx.buffer      = register_data;
	packet_rx.length      = length;
	
	return (0 - twi_master_read(TWI0, &packet_rx) );
}

int8_t I2C_WRITE(uint8_t device_addr, uint8_t register_addr, uint8_t *register_data, uint16_t length)
{
	twi_packet_t packet_tx;
	
	packet_tx.chip        = device_addr;
	packet_tx.addr[0]     = register_addr;
	packet_tx.addr_length = 0x01;
	packet_tx.buffer      = register_data;
	packet_tx.length      = length;

	return (0 - twi_master_write(TWI0, &packet_tx) );
}

int8_t set_normal_mode(struct bmp3_dev *dev)
{
	int8_t rslt;
	/* Used to select the settings user needs to change */
	uint16_t settings_sel;

	/* Select the pressure and temperature sensor to be enabled */
	dev->settings.press_en = BMP3_ENABLE;
	dev->settings.temp_en = BMP3_ENABLE;
	/* Select the output data rate and oversampling settings for pressure and temperature */
	dev->settings.odr_filter.press_os = BMP3_NO_OVERSAMPLING;
	dev->settings.odr_filter.temp_os = BMP3_NO_OVERSAMPLING;
	dev->settings.odr_filter.odr = BMP3_ODR_200_HZ;
	/* Assign the settings which needs to be set in the sensor */
	settings_sel = BMP3_PRESS_EN_SEL | BMP3_TEMP_EN_SEL | BMP3_PRESS_OS_SEL | BMP3_TEMP_OS_SEL | BMP3_ODR_SEL;
	rslt = bmp3_set_sensor_settings(settings_sel, dev);

	/* Set the power mode to normal mode */
	dev->settings.op_mode = BMP3_NORMAL_MODE;
	rslt = bmp3_set_op_mode(dev);
	
	rslt = configure_int_pin(dev);

	return rslt;
}

int8_t get_sensor_data(struct bmp3_dev *dev)
{
	int8_t rslt;
	/* Variable used to select the sensor component */
	uint8_t sensor_comp;
	/* Variable used to store the compensated data */
	struct bmp3_data data;

	/* Sensor component selection */
	sensor_comp = BMP3_PRESS | BMP3_TEMP;
	/* Temperature and Pressure data are read and stored in the bmp3_data instance */
	rslt = bmp3_get_sensor_data(sensor_comp, &data, dev);

	/* Print the temperature and pressure data */
	printf("Temperature\t Pressure\t\n");
	printf("%0.2f\t\t %0.2f\t\t\n",data.temperature, data.pressure);

	return rslt;
}

int8_t configure_int_pin(struct bmp3_dev *dev)
{
	int8_t rslt;
	/* Used to select the settings user needs to change */
	uint16_t settings_sel;
	
	dev->settings.int_settings.drdy_en = BMP3_DISABLE;
	dev->settings.int_settings.latch = BMP3_INT_PIN_NON_LATCH;
	dev->settings.int_settings.level = BMP3_INT_PIN_ACTIVE_LOW;
	dev->settings.int_settings.output_mode = BMP3_INT_PIN_PUSH_PULL;
	
	 settings_sel = BMP3_OUTPUT_MODE_SEL | BMP3_LEVEL_SEL | BMP3_LATCH_SEL | BMP3_DRDY_EN_SEL;
	 
	 rslt = bmp3_set_sensor_settings(settings_sel, dev);
}

int8_t configure_and_get_fifo_data(struct bmp3_dev *dev)
{
	int8_t rslt;
	/* Loop Variable */
	uint8_t i;
	/* FIFO object to be assigned to device structure */
	struct bmp3_fifo fifo;
	/* Pressure and temperature array of structures with maximum frame size */
	struct bmp3_data sensor_data[73] = {0};
	/* Used to select the settings user needs to change */
	uint16_t settings_sel;
	/* try count for polling the watermark interrupt status */
	uint16_t try_count;
	
	fifo.settings.mode = BMP3_ENABLE;
	fifo.settings.press_en = BMP3_ENABLE;
	fifo.settings.temp_en = BMP3_ENABLE;
	
	fifo.settings.time_en = BMP3_DISABLE;
	fifo.settings.down_sampling = BMP3_FIFO_NO_SUBSAMPLING;	
	fifo.settings.stop_on_full_en = BMP3_DISABLE;
	fifo.settings.ffull_en = BMP3_DISABLE;
	
	/* FIFO watermark interrupt enable */
	fifo.settings.fwtm_en = BMP3_ENABLE;

	/* Link the fifo object to device structure */
	dev->fifo = &fifo;
	/* Select the settings required for fifo */
	settings_sel = BMP3_FIFO_MODE_SEL | BMP3_FIFO_TIME_EN_SEL | BMP3_FIFO_TEMP_EN_SEL |\
	BMP3_FIFO_PRESS_EN_SEL | BMP3_FIFO_DOWN_SAMPLING_SEL | BMP3_FIFO_FWTM_EN_SEL | BMP3_FIFO_FULL_EN_SEL | BMP3_FIFO_STOP_ON_FULL_EN_SEL;
	/* Set the selected settings in fifo */
	rslt = bmp3_set_fifo_settings(settings_sel, dev);

	/* Set the number of frames to be read so as to set the watermark length in the sensor */
	dev->fifo->data.req_frames = 1;
	rslt = bmp3_set_fifo_watermark(dev);

	/* Set the power mode to normal */
	rslt = set_normal_mode(dev);


while(1)
{	
	try_count = 0xffff;
	/* Poll till watermark level is reached in fifo */
	do {
		rslt = bmp3_get_status(dev);
		try_count--;
	} while ((dev->status.intr.fifo_wm == 0) && (try_count > 0));
	 
	if (try_count > 0) 
	{	
		// this part was used for debugging purposes 
		// set Debug Line high
		ioport_set_pin_level(TEST_PIN, 1);
		rslt = bmp3_get_fifo_data(dev);
		ioport_set_pin_level(TEST_PIN, 0);
		rslt = bmp3_extract_fifo_data(sensor_data, dev);
		ioport_set_pin_level(TEST_PIN, 1);
		// read status registers to clear interrupt
		//rslt = bmp3_get_status(dev);
		
		if (dev->status.intr.drdy == 1)
		{
			rslt = get_sensor_data(dev);
		}	
	}
}
	return rslt;
}