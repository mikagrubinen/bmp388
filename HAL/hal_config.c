/**
 * Copyright (C) Robert Bosch. All Rights Reserved. Confidential.
 * $license$
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
 * @file        ${hal_config.c}
 *
 * @brief  this file defined the mcu component configuration
 *
 *
 */


/*********************************************************************/
/* system header includes */
#include "asf.h"
#include "conf_board.h"
#include "conf_clock.h"

#define SHUTTLE_VDD_PIN PIO_PA0_IDX
#define SHUTTLE_VDDIO_PIN PIO_PA1_IDX

/*********************************************************************/
/* local macro definitions */
#define TWI_CLK     (400000)   /**< TWI Bus Clock 400kHz, i2c fast mode */ 
#define IRQ_PRIOR_PIO    (0) /**< IRQ priority for PIO (The lower the value, the greater the priority) */ 

/*********************************************************************/
/* functions */

/*!
 * @brief To power ON/OFF VDD
 *
 *
 * @param[in] ucState-1/0 depending on whether power is to be on/off
 * @param[out]
 * @param[in,out]
 *
 * @return S32-returns success message
 */ 
int32_t set_shuttle_vdd(uint8_t ucState)
{
	ioport_set_pin_level(SHUTTLE_VDD_PIN, ucState != 0);
	return 0;//OK;
}


/*!
 * @brief To power ON/OFF VDDIO
 *
 *
 * @param[in] ucState-1/0 depending on whether power is to be on/off
 * @param[out]
 * @param[in,out]
 *
 * @return S32-returns success message
 */ 
int32_t set_shuttle_vddio(uint8_t ucState)
{
	ioport_set_pin_level(SHUTTLE_VDDIO_PIN, ucState != 0);
	return 0;//OK;	 
}


/*!
 * @brief handle the button event
 *
 *
 * @param[in] 
 * @param[out]
 * @param[in,out]
 *
 * @return
 */ 
void process_button_evt(uint8_t uc_button)
{
	switch(uc_button)
	{
		case 0:
		{
		    ioport_toggle_pin_level(LED0_GPIO);
		}
        break;
		case 1:
		{
		    ioport_toggle_pin_level(LED1_GPIO);
		}	
		break;
	}
}


/*!
 * @brief Handler for Button 1 rising edge interrupt.
 *
 *
 * @param[in] 
 * @param[out]
 * @param[in,out]
 *
 * @return
 */ 
static void button1_handler(uint32_t id, uint32_t mask)
{
	if (PIN_PUSHBUTTON_1_ID == id && PIN_PUSHBUTTON_1_MASK == mask) 
	{
		process_button_evt(0);
	}
}


/*!
 * @brief Handler for Button 2 rising edge interrupt.
 *
 *
 * @param[in] 
 * @param[out]
 * @param[in,out]
 *
 * @return
 */  
static void button2_handler(uint32_t id, uint32_t mask)
{
	if (PIN_PUSHBUTTON_2_ID == id && PIN_PUSHBUTTON_2_MASK == mask) 
	{
		process_button_evt(1);
	}
}


 /*!
 * @brief Configure the Pushbuttons. Configure the PIO as inputs and generate corresponding interrupt when
 *        pressed or released.
 *
 * @param[in] 
 * @param[out]
 * @param[in,out]
 *
 * @return
 */ 
void configure_buttons(void)
{
// [main_button1_configure]
	/* Configure Pushbutton 1 */
	pmc_enable_periph_clk(PIN_PUSHBUTTON_1_ID);
	pio_set_debounce_filter(PIN_PUSHBUTTON_1_PIO, PIN_PUSHBUTTON_1_MASK, 10);
	/* Interrupt on rising edge  */
	pio_handler_set(PIN_PUSHBUTTON_1_PIO, PIN_PUSHBUTTON_1_ID,
			PIN_PUSHBUTTON_1_MASK, PIN_PUSHBUTTON_1_ATTR, button1_handler);
	NVIC_EnableIRQ((IRQn_Type) PIN_PUSHBUTTON_1_ID);
	pio_handler_set_priority(PIN_PUSHBUTTON_1_PIO,
			(IRQn_Type) PIN_PUSHBUTTON_1_ID, IRQ_PRIOR_PIO);
	pio_enable_interrupt(PIN_PUSHBUTTON_1_PIO, PIN_PUSHBUTTON_1_MASK);

// [main_button2_configure]
	/* Configure Pushbutton 2 */
	pmc_enable_periph_clk(PIN_PUSHBUTTON_2_ID);
	pio_set_debounce_filter(PIN_PUSHBUTTON_2_PIO, PIN_PUSHBUTTON_2_MASK, 10);
	/* Interrupt on falling edge */
	pio_handler_set(PIN_PUSHBUTTON_2_PIO, PIN_PUSHBUTTON_2_ID,
			PIN_PUSHBUTTON_2_MASK, PIN_PUSHBUTTON_2_ATTR, button2_handler);
	NVIC_EnableIRQ((IRQn_Type) PIN_PUSHBUTTON_2_ID);
	pio_handler_set_priority(PIN_PUSHBUTTON_2_PIO,
			(IRQn_Type) PIN_PUSHBUTTON_2_ID, IRQ_PRIOR_PIO);
	pio_enable_interrupt(PIN_PUSHBUTTON_2_PIO, PIN_PUSHBUTTON_2_MASK);
}



/*!
 * @brief configure the twi, then initialize i2c
 *
 *
 * @param[in] 
 * @param[out]
 * @param[in,out]
 *
 * @return
 */ 
void configure_twi0(void)
{
	twi_options_t opt;
	
	/* Enable the peripheral clock for TWI */
	pmc_enable_periph_clk(ID_TWI0);
	
	opt.master_clk = sysclk_get_cpu_hz();
	opt.speed      = TWI_CLK; //I2c fast mode
		
	if (twi_master_init(TWI0, &opt) != TWI_SUCCESS) 
	{
		while (1) 
		{
			/* Capture error */
		}
	}
}


/*!
 * @brief configure the shuttle vdd pins
 *
 *
 * @param[in] 
 * @param[out]
 * @param[in,out]
 *
 * @return
 */ 
void configure_shuttle_vdds(void)
{
	ioport_set_pin_dir(SHUTTLE_VDD_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(SHUTTLE_VDDIO_PIN, IOPORT_DIR_OUTPUT);
	set_shuttle_vdd(0);
	set_shuttle_vddio(0);
}

/** @}*/