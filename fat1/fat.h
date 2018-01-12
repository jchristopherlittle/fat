/*
 * fat.h
 *
 *  Created on: Jan 8, 2018
 *      Author: clittle
 */

#ifndef FAT_H_
#define FAT_H_

#define SECTOR_SIZE			0x200
#define SECTOR_SIZE_DEC			512
#define FAT_SIZE		SECTOR_SIZE * 9
/* Locations */
#define BYTES_PER_LOGICAL_SECTOR	0x0b
#define LOGICAL_SECTORS_PER_CLUSTER	0x0d
#define RESERVED_LOGICAL_SECTORS	0x0e
#define NUMBER_OF_FATS			0x10
#define E_BOOT_SIG			0x26
#define VOLSER				0x27
#define LABEL				0x2b
#define OEM_NAME			0x03
#define PARAM_BLOCK			0x0b
#define BOOT_SIG			0x1f3
#define ROOT_DIR_SEC			0x13
#define TOTAL_SEC           		0x013
#define NO_ROOT_DIR_ENT			0x011

struct dirent {
	char		fn[9];
	char		ext[4];
	uint8_t		attr;
	uint8_t		res[2];
	uint8_t		ctime[2];
	uint8_t		cdate[2];
	uint8_t		adate[2];
	uint8_t		ignore[2];
	uint8_t		mtime[2];
	uint8_t		mdate[2];
	uint8_t		size[4];
	struct dirent	*prev;
	struct dirent	*next;
};

struct ebpb {
    uint8_t     bytes_per_logical_sector[2];    /* Offset 0x00b */
    long	logical_sectors_per_cluster;    /* Offset 0x00d */
    uint8_t     reserved_logical_sectors[2];    /* Offset 0x00e */
    uint8_t     no_fats;                        /* Offset 0x010 */
    uint16_t     max_root_entries;            /* Offset 0x011 */
    uint16_t     total_logical_sectors;       /* Offset 0x013 */
    uint8_t     media_descriptor;               /* Offset 0x015 */
    uint8_t     logical_sectors_per_fat[2];     /* Offset 0x016 */
    uint8_t     sectors_per_track[2];           /* Offset 0x018 */
    uint8_t     no_of_heads[2];                 /* Offset 0x01a */
    uint8_t     hidden_sectors[4];              /* Offset 0x01c */
    uint8_t     no_of_logical_sectors[4];       /* Offset 0x020 */
    uint8_t     drive_no;                       /* Offset 0x024 */
    uint8_t     res1;                           /* Offset 0x025 */
    uint8_t     e_bootsig;                      /* Offset 0x026 */
    uint8_t     volser[4];                      /* Offset 0x027 */
    uint8_t     label[11];                      /* Offset 0x02b */
    uint8_t     fstype[8];                      /* Offset 0x036 */
    uint8_t     pad[447];
};

struct boot_sector {
    uint8_t     header[3];      /* Offset 0x000 */
    char    	oem_name[8];    /* Offset 0x003 */
    struct ebpb	param_block;    /* Offset 0x00b */
    uint8_t     drive_num[1];   /* Offset 0x1fd */
    uint8_t     bootsig[2];     /* Offset 0x1f3 */
} ;

//int		cwd_sector=19;

int fat12_fill_boot_sector(unsigned char *, struct boot_sector *);
int read_fat_phys_sector(FILE *, unsigned char *);
int read_fat_log_sector(FILE *, unsigned char *);
int nulltmp(unsigned char *);
int find_loc(int, int);
int l2b_16bit(unsigned char *);
int read_dir(struct dirent *);
int loadfat(unsigned char *, unsigned int *, int);
int read_sector(unsigned char *, FILE *, long int, int);
int print_sector(unsigned char *);
int read_rootdir(FILE *,struct dirent *, uint16_t);
int process_longattr();

#endif /* FAT_H_ */
