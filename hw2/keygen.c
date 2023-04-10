#include <stdlib.h>
#include <openssl/md5.h>
#include <cpuid.h>
#include <stdio.h>
typedef unsigned long ulong;
typedef unsigned char byte;
static MD5_CTX md5handler = {0};
static byte md5digest[256] = {0};
static char md5decode[256] = {0};
static char PSN[256] = {0};


void calc_md5(void *param_1, size_t param_2)
{
  MD5_Init((MD5_CTX *)&md5handler);
  MD5_Update((MD5_CTX *)&md5handler,param_1,param_2);
  MD5_Final(md5digest,(MD5_CTX *)&md5handler);
}

int main(long long param1, long long param_2) {
    int iVar1;
    long int in_R9;
    long in_FS_OFFSET;
    unsigned int local_34;
    int local_30;
    int local_2c;
    unsigned int local_28;
    int local_24;
    int local_20;
    int local_1c;
    unsigned int local_18;
    unsigned int local_14;

    local_1c = 1;
    local_34 = 3;
    local_30 = 0;
    local_2c = 0;
    local_28 = 0;
    local_18 = 0;
    local_14 = 0;
    local_24 = 0;
    __get_cpuid(1,&local_34,&local_30,&local_2c,&local_28);
    local_18 = local_34 << 0x18 | local_34 >> 0x18 | (local_34 & 0xff00) << 8 | local_34 >> 8 & 0xff00
    ;
    local_14 = local_28 << 0x18 | local_28 >> 0x18 | (local_28 & 0xff00) << 8 | local_28 >> 8 & 0xff00
    ;
    snprintf(PSN,0x11,"%08X%08X",(ulong)local_18,(ulong)local_14);
    calc_md5(PSN,0x10);
    for (local_20 = 0; local_20 < 0x10; local_20 = local_20 + 1) {
        sprintf(md5decode + local_20 * 2,"%02x",md5digest[0xf - local_20]);
    }

    printf("KEYGEN OUTPUT: %32s\n", md5decode);
    return 0;
}