/*
 * fat.c
 *
 *  Created on: Jan 8, 2018
 *      Author: clittle
 */


#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "fat.h"

int read_rootdir(FILE *f, struct dirent *dir, uint16_t max)
{
	unsigned char	tmp[512];
	struct dirent	dir_entry;
	int				i=0;
	int				sec=19;
	int				c=0;

	read_sector(tmp, f,sec,1);
	while(c<max){
		while(i<SECTOR_SIZE){

			nulltmp((unsigned char*)dir_entry.fn);
			nulltmp((unsigned char*)dir_entry.ext);
			memcpy(dir_entry.fn, tmp+i,8);
			memcpy(dir_entry.ext,tmp+0x08+i,3);
			dir_entry.attr=tmp[0x0B+i];

			if(dir_entry.fn[0]!=0) { /* Process directory entries */
				if(i==SECTOR_SIZE-32) {
					if(dir_entry.attr==0x0F) {
						process_longattr();
					} else if (dir_entry.fn[0]==0xE5){
						;
					} else {
						printf("%s.%s\n",dir_entry.fn, dir_entry.ext);
					}
					sec++;
					i=0;
					read_sector(tmp, f, sec,1);
				} else {
					if(dir_entry.attr==0x0F) {
						process_longattr();
					} else if (dir_entry.fn[0] == 0xE5) {
						;
					} else {
						printf("%s.%s\n",dir_entry.fn, dir_entry.ext);
					}
					i+=32;
				}
			} else {
				i=SECTOR_SIZE;
			}
		}
		c++;
	}
	return 1;
}

int fat12_fill_boot_sector(unsigned char *data_block, struct boot_sector *boot_s)
{
	unsigned char 	tmp[10];

	strncpy(boot_s->oem_name,(char *)data_block + OEM_NAME,8);

	nulltmp(tmp);
	strncpy((char *)tmp,(char *)data_block + LOGICAL_SECTORS_PER_CLUSTER,1);
	boot_s->param_block.logical_sectors_per_cluster=(long)tmp[0];

	nulltmp(tmp);
	strncpy((char *)tmp, (char *)data_block + TOTAL_SEC,2);
	boot_s->param_block.total_logical_sectors=l2b_16bit(tmp);

	nulltmp(tmp);
	strncpy((char *)tmp,(char *)data_block + NO_ROOT_DIR_ENT,2);
	boot_s->param_block.max_root_entries=l2b_16bit(tmp);

	  return 0;
}

int l2b_16bit(unsigned char *i)
{
        return (unsigned int)i[0] | (unsigned int)i[1] << 8;
}

int find_loc(int sector, int blocksize)
{
    long    location=0;

    return location;
}

int nulltmp(unsigned char *i)
{
	int count;
	int c;
	count=sizeof(i);

	for(c=0;c<count;c++)
		i[c]=0;

    return 0;
}

int loadfat(unsigned char *fat, unsigned int *t, int count)
{

	uint16_t	tmp1;
	uint16_t	tmp2;
	unsigned int	i=0;
	uint16_t	b0;
	uint16_t	b1;
	uint16_t	b2;
/* Okay. Note to self. we have 9 sectores of FAT.
 * 9 sectors. Get them all in correctly.
 *
 *
 */
	for(int c=0;c<count;c+=2) {
		tmp1=0;
		tmp2=0;
		b0=fat[i];
		b1=fat[i+1];
		b2=fat[i+2];
/* 12bit FAT is encoded in 3 bytes.
 * Entry 1: 	High order 4 bits is low 4 bits of b1.
 * 				Low order 8 bits is b0
 * Entry 2:		High order 8 bits is b2
 * 				Low order 4 bits is high b1
 */
		tmp1=(b1 & 0x0f) << 8;
		t[c]=tmp1 | b0;
		tmp2=(b1 & 0xf0) >> 4;
		t[c+1]=(b2 << 4) | tmp2;

#if DEBUG
		printf("Entry: %04d |%03x | %02x %02x %02x\n",c,t[c],b0,b1,b2);
		printf("Entry: %04d |%03x |                | i = %x\n\n",c+1,t[c+1],i);
#endif

		i+=3;
	}

	printf("\n\n");
	return 0;
}

int read_sector(unsigned char *s, FILE *f, long int no, int count)
{
	int		error;

	fseek(f,no * SECTOR_SIZE,SEEK_SET);

	if (!feof(f)) {
		fread(s, SECTOR_SIZE * count, 1, f);
	}
	error=ferror(f);

	return error;
}

int print_sector(unsigned char *s)
{

	int c=0;
    for(int i=0;i<SECTOR_SIZE;i++) {
		printf("%02x ", s[i]);
		c++;
		if(((c%15) == 0) && c!=0)
			printf("\n");
	}

    printf("\n");

	return 0;
}

int process_longattr()
{
	return 0;
}
