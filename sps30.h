/*
 * sps30.h
 *
 *  Created on: Nov 11, 2020
 *      Author: Aniket Paluskar
 */

#ifndef APPLICATION_SPS30_SPS30_H_
#define APPLICATION_SPS30_SPS30_H_
//Invoking I2C Driver
#include <ti/drivers/I2C.h>

#include "stdio.h"
// Macros
#define SPS_I2C_SLAVE_ADDRESS 0x69
#define SPS_CMD_START_MEASUREMENT 0x0010
#define SPS_CMD_START_MEASUREMENT_ARG 0x0300
#define SPS_CMD_STOP_MEASUREMENT 0x0104
#define SPS_CMD_READ_MEASUREMENT 0x0300
#define SPS_CMD_START_STOP_DELAY_USEC 20000
#define SPS_CMD_GET_DATA_READY 0x0202
#define SPS_CMD_AUTOCLEAN_INTERVAL 0x8004
#define SPS_CMD_GET_FIRMWARE_VERSION 0xd100
#define SPS_CMD_GET_SERIAL 0xd033
#define SPS_CMD_RESET 0xd304
#define SPS_CMD_SLEEP 0x1001
#define SPS_CMD_READ_DEVICE_STATUS_REG 0xd206
#define SPS_CMD_START_MANUAL_FAN_CLEANING 0x5607
#define SPS_CMD_WAKE_UP 0x1103
#define SPS_CMD_DELAY_USEC 5000
#define SPS_CMD_DELAY_WRITE_FLASH_USEC 20000
#define MSGSIZE 3
#define SPS_NEW_MEASUREMENT_DATA_READY 0x01


// Structure definition
struct sps30_measurement {
    float mc_1p0;
    float mc_2p5;
    float mc_4p0;
    float mc_10p0;
    float nc_0p5;
    float nc_1p0;
    float nc_2p5;
    float nc_4p0;
    float nc_10p0;
    float typical_particle_size;
};


//Function Definition

void SPS30_init(I2C_Handle i2c);
bool SPS30_read_measurements(struct sps30_measurement* measurement);


#endif /* APPLICATION_SPS30_SPS30_H_ */
