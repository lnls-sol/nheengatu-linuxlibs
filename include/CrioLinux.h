#ifndef CRIOLINUX_H
#define CRIOLINUX_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Macros */
#define CRIO_BI_ARRAY_BIT(x) (UINT64_C(1)<<(x))
#define CRIO_BI_ARRAY_USER_PUSH_BUTTON	CRIO_BI_ARRAY_BIT(0)
#define CRIO_BI_ARRAY_SCAN_CLOCK	CRIO_BI_ARRAY_BIT(1)
#define CRIO_BI_ARRAY_MOD2_DIO0		CRIO_BI_ARRAY_BIT(2)
#define CRIO_BI_ARRAY_MOD2_DIO1		CRIO_BI_ARRAY_BIT(3)
#define CRIO_BI_ARRAY_MOD2_DIO2		CRIO_BI_ARRAY_BIT(4)
#define CRIO_BI_ARRAY_MOD2_DIO3		CRIO_BI_ARRAY_BIT(5)
#define CRIO_BI_ARRAY_MOD3_DIO0		CRIO_BI_ARRAY_BIT(6)
#define CRIO_BI_ARRAY_MOD3_DIO1		CRIO_BI_ARRAY_BIT(7)
#define CRIO_BI_ARRAY_MOD3_DIO2		CRIO_BI_ARRAY_BIT(8)
#define CRIO_BI_ARRAY_MOD3_DIO3		CRIO_BI_ARRAY_BIT(9)
#define CRIO_BI_ARRAY_MOD3_DIO4		CRIO_BI_ARRAY_BIT(10)
#define CRIO_BI_ARRAY_MOD3_DIO5		CRIO_BI_ARRAY_BIT(11)
#define CRIO_BI_ARRAY_MOD3_DIO6		CRIO_BI_ARRAY_BIT(12)
#define CRIO_BI_ARRAY_MOD3_DIO7		CRIO_BI_ARRAY_BIT(13)
#define CRIO_BI_ARRAY_MOD4_DIO0		CRIO_BI_ARRAY_BIT(14)
#define CRIO_BI_ARRAY_MOD4_DIO1		CRIO_BI_ARRAY_BIT(15)
#define CRIO_BI_ARRAY_MOD4_DIO2		CRIO_BI_ARRAY_BIT(16)
#define CRIO_BI_ARRAY_MOD4_DIO3		CRIO_BI_ARRAY_BIT(17)
#define CRIO_BI_ARRAY_MOD4_DIO4		CRIO_BI_ARRAY_BIT(18)
#define CRIO_BI_ARRAY_MOD4_DIO5		CRIO_BI_ARRAY_BIT(19)
#define CRIO_BI_ARRAY_MOD4_DIO6		CRIO_BI_ARRAY_BIT(20)
#define CRIO_BI_ARRAY_MOD4_DIO7		CRIO_BI_ARRAY_BIT(21)
#define CRIO_BI_ARRAY_MOD5_DIO0		CRIO_BI_ARRAY_BIT(22)
#define CRIO_BI_ARRAY_MOD5_DIO1		CRIO_BI_ARRAY_BIT(23)
#define CRIO_BI_ARRAY_MOD5_DIO2		CRIO_BI_ARRAY_BIT(24)
#define CRIO_BI_ARRAY_MOD5_DIO3		CRIO_BI_ARRAY_BIT(25)
#define CRIO_BI_ARRAY_MOD5_DIO4		CRIO_BI_ARRAY_BIT(26)
#define CRIO_BI_ARRAY_MOD5_DIO5		CRIO_BI_ARRAY_BIT(27)
#define CRIO_BI_ARRAY_MOD5_DIO6		CRIO_BI_ARRAY_BIT(28)
#define CRIO_BI_ARRAY_MOD5_DIO7		CRIO_BI_ARRAY_BIT(29)
#define CRIO_BI_ARRAY_COUNT 30

/* Type definitions */
typedef	unsigned int CrioSession;

/* Structures */

/* Functions */
void CrioCleanup(CrioSession Session);
int CrioReadBIArray(CrioSession Session, uint64_t *Output);
int CrioGetBIArrayItemName(CrioSession Session, unsigned Item, const char **Name);
int CrioSetup(CrioSession *Session);

#ifdef __cplusplus
}
#endif

#endif /* CRIOLINUX_H */
