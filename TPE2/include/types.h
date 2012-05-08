/*
 * types.h
 *
 *  Created on: 06/05/2012
 *      Author: neon
 */

#ifndef _TYPES_H_
#define _TYPES_H_

#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned int size_t;
#endif

#ifndef _SSIZE_T
#define _SSIZE_T
typedef int ssize_t;
#endif

#ifndef _TIME_T
#define _TIME_T
typedef long time_t;
#endif

#ifndef _CLOCK_T
#define _CLOCK_T
typedef long clock_t;
#endif

#ifndef _SIGSET_T
#define _SIGSET_T
typedef unsigned long sigset_t;
#endif

typedef short			dev_t;
typedef char			gid_t;
typedef unsigned short  ino_t;
typedef unsigned short mode_t;
typedef char		  nlink_t;
typedef unsigned long	off_t;
typedef int				pid_t;
typedef short			uid_t;
typedef unsigned long  zone_t;
typedef unsigned long block_t;
typedef unsigned long 	bit_t;
typedef unsigned short bitchunk_t;

typedef unsigned char	u8_t;
typedef unsigned short u16_t;
typedef unsigned long  u32_t;

typedef char			i8_t;
typedef short		   i16_t;
typedef long		   i32_t;



#endif /* TYPES_H_ */
