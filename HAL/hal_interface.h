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
 * @file        ${hal_interface.h}
 *
 * @brief  
 *
 *
 */

#ifndef HAL_INTERFACE_H_
#define HAL_INTERFACE_H_


/*********************************************************************/
/* function prototype declarations */

/*!
 * @brief initialize shuttle vdd pins
 *
 *
 * @param[in] 
 * @param[out]
 * @param[in,out]
 *
 * @return
 */ 
void configure_shuttle_vdds(void);

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
int32_t set_shuttle_vdd(uint8_t ucState);



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
int32_t set_shuttle_vddio(uint8_t ucState);

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
void configure_buttons(void);

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
void configure_twi0(void);

/*!
 * @brief initialize the systick
 *
 *
 * @param[in] 
 * @param[out]
 * @param[in,out]
 *
 * @return
 */ 
void systick_init(void);

/*!
 * @brief: set a timer and input it's timer interval(customer could set the timer as software timer, RTT, TC...) 
 *
 * @param[in]: timer_index: timer id; count: timer inteval 
 * @param[out]: no
 * @param[in,out]: no
 *
 * @return: 0 when succeed set timer
 */ 
uint32_t set_timer_interval(uint32_t id, uint32_t count);

/*!
 * @brief get timer interval
 *
 *
 * @param[in] 
 * @param[out]
 * @param[in,out]
 *
 * @return 0 when time is up
 */ 
uint32_t get_timer_interval(uint32_t id);



#endif /* HAL_INTERFACE_H_ */



/** @}*/