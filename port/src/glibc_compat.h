/* Forced into every runtime object (port/Makefile: -include): the program asks only for what glibc 2.35 has, so the
 * one build runs on older systems too (Ubuntu 22.04, Debian 12; make dist-linux checks it). Since glibc 2.38,
 * _GNU_SOURCE makes strtol, sscanf and their kin the C23 versions (__isoc23_*, which also take 0b binary numbers);
 * these lines bind them back to the plain ones every glibc has. */
#ifndef GLIBC_COMPAT_H
#define GLIBC_COMPAT_H
/* (nothing is included here, so each file still picks its own feature macros; the lines are harmless where nothing
 * refers to those names) */
#if defined(__linux__) && defined(__i386__)
__asm__(".symver __isoc23_strtol, strtol@GLIBC_2.0");
__asm__(".symver __isoc23_strtoul, strtoul@GLIBC_2.0");
__asm__(".symver __isoc23_strtoll, strtoll@GLIBC_2.0");
__asm__(".symver __isoc23_strtoull, strtoull@GLIBC_2.0");
__asm__(".symver __isoc23_sscanf, sscanf@GLIBC_2.0");
__asm__(".symver __isoc23_vsscanf, vsscanf@GLIBC_2.0");
__asm__(".symver __isoc23_fscanf, fscanf@GLIBC_2.0");
__asm__(".symver __isoc23_scanf, scanf@GLIBC_2.0");
#endif
#endif
