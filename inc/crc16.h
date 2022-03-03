/*
 * crc16.h
 *
 *  Created on: 18/03/2014
 *      Author: Renato Coral Sampaio
 * 
 *  Modifien on: 16/02/2022
 *      Author: Bruno Carmo Nunes
 *      Changes: Convert to C++ files, and convert int to short type.
 */

#ifndef CRC16_H_
#define CRC16_H_

short CRC16(short crc, char data);
short calcula_CRC(unsigned char *commands, int size);

#endif /* CRC16_H_ */
