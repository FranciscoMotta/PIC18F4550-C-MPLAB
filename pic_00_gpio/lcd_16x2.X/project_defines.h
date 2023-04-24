/* 
 * File:   project_defines.h
 * Author: jmott
 *
 * Created on April 23, 2023, 10:40 PM
 */

#ifndef PROJECT_DEFINES_H
#define	PROJECT_DEFINES_H

#include <xc.h>

/*
 * Led del sistema
 */

#define Led_System_Gpio     0
#define Led_System_Tris     TRISD
#define Led_System_Lat      LATD
#define Led_System_Mask     (1 << Led_System_Gpio)

#endif	/* PROJECT_DEFINES_H */

