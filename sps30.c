/*
 * sps30.c
 *
 *  Created on: Nov 11, 2020
 *      Author: Aniket Paluskar
 */

#include "sps30.h"

/**********************Static Functions**********************/

static float sensirion_bytes_to_float(const uint8_t* bytes);
static uint32_t sensirion_bytes_to_uint32_t(const uint8_t* bytes);
static bool SPS30_reset(I2C_Handle i2cHandle);
static bool isDataReady();
static bool SPS30_startMeasurement();

/**********************Variables Declaration**********************/
uint8_t data[10][6];
I2C_Transaction I2Ctransaction;
I2C_Handle i2cHandle;
uint16_t writeBuffer[MSGSIZE];

/********************************************************************
 * @fn              uint32_t sensirion_bytes_to_uint32_t()
 *
 * @brief           converts bytes into correct data
 *
 * @param           uint8_t -> Array of data
 *
 * @return          Float
 *
 */
uint32_t sensirion_bytes_to_uint32_t(const uint8_t* bytes) {
    return (uint32_t)bytes[0] << 24 | (uint32_t)bytes[1] << 16 |
           (uint32_t)bytes[3] << 8 | (uint32_t)bytes[4];
}

/********************************************************************
 * @fn              uint32_t sensirion_bytes_to_float()
 *
 * @brief           converts byte into float
 *
 * @param           uint8_t -> Array of data
 *
 * @return          Float
 *
 */
float sensirion_bytes_to_float(const uint8_t* bytes) {
    union {
        uint32_t u32_value;
        float float32;
    } tmp;

    tmp.u32_value = sensirion_bytes_to_uint32_t(bytes);
    return tmp.float32;
}
/*************************************************************************
 * @fn          SPS30_init()
 *
 * @brief       initializes the Sensor and parameters
 *
 * @param       I2c_Handle
 *
 * @return      None
 */

void SPS30_init(I2C_Handle i2c){
    i2cHandle = i2c;
    I2Ctransaction.slaveAddress = SPS_I2C_SLAVE_ADDRESS;
    I2Ctransaction.readBuf = (void *) data;
    I2Ctransaction.writeBuf = (void *) writeBuffer;

    // Reseting Sensor
    if(SPS30_reset(i2cHandle))
    {
        printf("Sensor is ready to measure");
    }
    else{
        printf("Issues in starting sensor");
    }
}

/*************************************************************************
 * @fn          SPS30_reset()
 *
 * @brief       Resets the Sensor
 *
 * @param       I2c_Handle
 *
 * @return      Bool
 */

bool SPS30_reset(I2C_Handle i2cHandle)
{
    writeBuffer[0] = SPS_I2C_SLAVE_ADDRESS;
    writeBuffer[1] = SPS_CMD_RESET;
    bool result = I2C_transfer(i2cHandle, &I2Ctransaction);

    return result;
}

bool SPS30_read_measurements(struct sps30_measurement* measurement)
{
    // Starting the measurement
    if(SPS30_startMeasurement())
    {
        printf("Started taking Measurement");
    }
    else{
        printf("Cannot start measuring");
    }

    //Checking if new data is available or not
    if(isDataReady())
    {
        writeBuffer[0] = SPS_I2C_SLAVE_ADDRESS;
        writeBuffer[1] = SPS_CMD_READ_MEASUREMENT;
        bool result = I2C_transfer(i2cHandle, &I2Ctransaction);

        if(result)
        {
                //Assigning values

                measurement->mc_1p0 = sensirion_bytes_to_float(data[0]);
                measurement->mc_2p5 = sensirion_bytes_to_float(data[1]);
                measurement->mc_4p0 = sensirion_bytes_to_float(data[2]);
                measurement->mc_10p0 = sensirion_bytes_to_float(data[3]);
                measurement->nc_0p5 = sensirion_bytes_to_float(data[4]);
                measurement->nc_1p0 = sensirion_bytes_to_float(data[5]);
                measurement->nc_2p5 = sensirion_bytes_to_float(data[6]);
                measurement->nc_4p0 = sensirion_bytes_to_float(data[7]);
                measurement->nc_10p0 = sensirion_bytes_to_float(data[8]);
                measurement->typical_particle_size = sensirion_bytes_to_float(data[9]);

                return result;
        }
        else
        {
            return result;
        }
    }
    else{
        return false;
    }
}


/**************************************************************************
 * @fn              SPS30_startMeasurement()
 *
 * @brief           Starts measuring
 *
 * @param           None
 */
bool SPS30_startMeasurement()
{
    writeBuffer[0] = SPS_I2C_SLAVE_ADDRESS;
    writeBuffer[1] = SPS_CMD_START_MEASUREMENT;
    writeBuffer[2] = SPS_CMD_START_MEASUREMENT_ARG;
    bool result = I2C_transfer(i2cHandle, &I2Ctransaction);

    return result;
}

/*****************************************************************************
 * @fn          isDataReady()
 *
 * @brief       Checks data flag if new measurements are available or not
 *
 * @param       None
 *
 * @return      Boolean
 *
 */
bool isDataReady()
{
    writeBuffer[0] = SPS_I2C_SLAVE_ADDRESS;
    writeBuffer[1] = SPS_CMD_GET_DATA_READY;
    bool result = I2C_transfer(i2cHandle, &I2Ctransaction);

    if(data[0][0] == SPS_NEW_MEASUREMENT_DATA_READY)
    {
        return true;
    }
    else{
        return false;
    }
}
