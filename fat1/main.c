/*
 * main.c
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

int		cwd_sector=19;


int main(int argc, char **argv)

{

    int                 opt;
    struct boot_sector	*bs;
    char                *fn;
    FILE				*fd;
    unsigned char		*sector;
    char				file_n[255]="/home/clittle/src/os/FLOPPY.old.img";
    unsigned char		*fat1;
	unsigned char		*fat2;
	struct dirent		*root_dir;
	unsigned int 		*table1;
	unsigned int 		*table2;

/*Get arguments */
    if(argc<2) {
        printf("Usage: testfat -f <diskimage>\n");
	fn=strdup(file_n);
    }

    while((opt = getopt(argc,argv, "f:h")) != -1 ) {
            switch(opt) {
                case 'h' :
                    printf("Usage: testfat -f <diskimage>\n");
		    exit (0);
                case 'f' :
                        fn=strdup(optarg);
                        printf("Filename: %s\n",fn);
                        break;
            }
    }


/* Intialize our memory */
    sector=(unsigned char *)malloc(SECTOR_SIZE);
    bs=(struct boot_sector*)malloc(sizeof(struct boot_sector));

	root_dir=(struct dirent *)malloc(sizeof(struct dirent));
		root_dir->prev = root_dir;
		root_dir->next = root_dir;

	fat1=(unsigned char *)malloc(FAT_SIZE);
	fat2=(unsigned char *)malloc(FAT_SIZE);
	for(int c=0;c<FAT_SIZE;c++) {
		fat1[c]=0xBE;
		fat2[c]=0xBE;
	}

	fd=fopen(fn,"rb");

/* First thing to do is read the boot sector and parse
 *out the interesting info */

	read_sector(sector, fd,0,1);
    fat12_fill_boot_sector(sector, bs);

/* Next read in the two FATs and compare them to make
 * certain there is no corruption
 */
	read_sector(fat1, fd, 1, 9);
	read_sector(fat2, fd, 10,9);

	for(int i=0;i<FAT_SIZE;i++) {
		if(fat1[i] != fat2[i]) {
			printf("uh oh shaggy\n");

		}
	}

/*
 * Next parse out the packed table
 * We're making the assumption that this is FAT12
 *
 */
	table1=(unsigned int *)malloc(sizeof(unsigned int) * bs->param_block.total_logical_sectors);
	table2=(unsigned int *)malloc(sizeof(unsigned int) * bs->param_block.total_logical_sectors);
	loadfat(fat1, table1, bs->param_block.total_logical_sectors);
	loadfat(fat2, table2, bs->param_block.total_logical_sectors);

	read_rootdir(fd,root_dir,bs->param_block.max_root_entries);

/* print the boot sector */

//	print_sector(sector);

    printf("OEM Name: %s\n",bs->oem_name);
    printf("Log sectors per clus: %ld\n",bs->param_block.logical_sectors_per_cluster);
    printf("Total secotrs: %d\n",bs->param_block.total_logical_sectors);
	printf("No root directory entries: %d\n",bs->param_block.max_root_entries);
    fclose(fd);

    return 0;
}


