#ifndef _REGISTERS_H
#define _REGISTERS_H

/************************************************
** Register Operations:
*************************************************/
#pragma pack(1)

#define MOVTO_AL(X) __asm__ __volatile__ ("movb %0, %%al;"    \
                                          :                   \
                                          :"g" (X)            \
                                          :"%al"              \
                                         );
                    
#define MOVFROM_AL(X) __asm__ __volatile__ ("movb %%al, %0;"  \
                                            :"=m" (X)         \
                                           );                

#define MOVTO_EAX(X) __asm__ __volatile__ ("movl %0, %%eax;"  \
                                            :                 \
                                           :"g" (X)           \
                                           :"%eax"            \
                                          );

#define MOVFROM_EAX(X) __asm__ __volatile__ ("movl %%eax, %0;"  \
                                             :"=m" (X)          \
                                            ); 


#define MOVTO_EBX(X) __asm__ __volatile__ ("movl %0, %%ebx;"  \
                                            :                 \
                                           :"g" (X)           \
                                           :"%ebx"            \
                                          );

#define MOVFROM_EBX(X) __asm__ __volatile__ ("movl %%ebx, %0;"  \
                                             :"=m" (X)          \
                                            ); 

#define MOVTO_ECX(X) __asm__ __volatile__ ("movl %0, %%ecx;"  \
                                            :                 \
                                           :"g" (X)           \
                                           :"%ecx"            \
                                          );

#define MOVFROM_ECX(X) __asm__ __volatile__ ("movl %%ecx, %0;"  \
                                             :"=m" (X)          \
                                            ); 

#define MOVTO_EDX(X) __asm__ __volatile__ ("movl %0, %%edx;"  \
                                            :                 \
                                           :"g" (X)           \
                                           :"%edx"            \
                                          );

#define MOVFROM_EDX(X) __asm__ __volatile__ ("movl %%edx, %0;"  \
                                             :"=m" (X)          \
                                            );                         

#endif					    