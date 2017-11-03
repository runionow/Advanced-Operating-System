#include <xinu.h>
#include <kernel.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>


#ifdef FS
#include <fs.h>

static struct fsystem fsd;
int dev0_numblocks;
int dev0_blocksize;
char *dev0_blocks;

extern int dev0;

char block_cache[512];

#define SB_BLK 0
#define BM_BLK 1
#define RT_BLK 2

#define NUM_FD 16
struct filetable oft[NUM_FD];
int next_open_fd = 0;
int node_count = 1;

#define INODES_PER_BLOCK (fsd.blocksz / sizeof(struct inode))
#define NUM_INODE_BLOCKS (( (fsd.ninodes % INODES_PER_BLOCK) == 0) ? fsd.ninodes / INODES_PER_BLOCK : (fsd.ninodes / INODES_PER_BLOCK) + 1)
#define FIRST_INODE_BLOCK 2

int fs_fileblock_to_diskblock(int dev, int fd, int fileblock);

int get_next_block() {
	int i;
	int totalBlocks = fsd.nblocks;
	for (i = 0; i<totalBlocks; i++) {
		if (fs_getmaskbit(i) == 0) {
			return i;
		}
	}
	return -1;
}

/* YOUR CODE GOES HERE */
int fs_fileblock_to_diskblock(int dev, int fd, int fileblock) {
	int diskblock;

	if (fileblock >= INODEBLOCKS - 2) {
		printf("No indirect block support\n");
		return SYSERR;
	}

	diskblock = oft[fd].in.blocks[fileblock]; //get the logical block address

	return diskblock;
}

/* read in an inode and fill in the pointer */
int
fs_get_inode_by_num(int dev, int inode_number, struct inode *in) {
	int bl, inn;
	int inode_off;

	if (dev != 0) {
		printf("Unsupported device\n");
		return SYSERR;
	}
	if (inode_number > fsd.ninodes) {
		printf("fs_get_inode_by_num: inode %d out of range\n", inode_number);
		return SYSERR;
	}

	bl = inode_number / INODES_PER_BLOCK;
	inn = inode_number % INODES_PER_BLOCK;
	bl += FIRST_INODE_BLOCK;

	inode_off = inn * sizeof(struct inode);

	/*
	printf("in_no: %d = %d/%d\n", inode_number, bl, inn);
	printf("inn*sizeof(struct inode): %d\n", inode_off);
	*/

	bs_bread(dev0, bl, 0, &block_cache[0], fsd.blocksz);
	memcpy(in, &block_cache[inode_off], sizeof(struct inode));

	return OK;

}

int
fs_put_inode_by_num(int dev, int inode_number, struct inode *in) {
	int bl, inn;

	if (dev != 0) {
		printf("Unsupported device\n");
		return SYSERR;
	}
	if (inode_number > fsd.ninodes) {
		printf("fs_put_inode_by_num: inode %d out of range\n", inode_number);
		return SYSERR;
	}

	bl = inode_number / INODES_PER_BLOCK;
	inn = inode_number % INODES_PER_BLOCK;
	bl += FIRST_INODE_BLOCK;

	/*
	printf("in_no: %d = %d/%d\n", inode_number, bl, inn);
	*/

	bs_bread(dev0, bl, 0, block_cache, fsd.blocksz);
	memcpy(&block_cache[(inn * sizeof(struct inode))], in, sizeof(struct inode));
	bs_bwrite(dev0, bl, 0, block_cache, fsd.blocksz);

	return OK;
}

int fs_mkfs(int dev, int num_inodes) {
	int i;

	if (dev == 0) {
		fsd.nblocks = dev0_numblocks;
		fsd.blocksz = dev0_blocksize;
	}
	else {
		printf("Unsupported device\n");
		return SYSERR;
	}

	if (num_inodes < 1) {
		fsd.ninodes = DEFAULT_NUM_INODES;
	}
	else {
		fsd.ninodes = num_inodes;
	}

	i = fsd.nblocks;
	while ((i % 8) != 0) { i++; }
	fsd.freemaskbytes = i / 8;

	if ((fsd.freemask = getmem(fsd.freemaskbytes)) == (void *)SYSERR) {
		printf("fs_mkfs memget failed.\n");
		return SYSERR;
	}

	/* zero the free mask */
	for (i = 0; i<fsd.freemaskbytes; i++) {
		fsd.freemask[i] = '\0';
	}

	fsd.inodes_used = 0;

	/* write the fsystem block to SB_BLK, mark block used */
	fs_setmaskbit(SB_BLK);
	bs_bwrite(dev0, SB_BLK, 0, &fsd, sizeof(struct fsystem));

	/* write the free block bitmask in BM_BLK, mark block used */
	fs_setmaskbit(BM_BLK);
	bs_bwrite(dev0, BM_BLK, 0, fsd.freemask, fsd.freemaskbytes);

	return 1;
}

void
fs_print_fsd(void) {

	printf("fsd.ninodes: %d\n", fsd.ninodes);
	printf("sizeof(struct inode): %d\n", sizeof(struct inode));
	printf("INODES_PER_BLOCK: %d\n", INODES_PER_BLOCK);
	printf("NUM_INODE_BLOCKS: %d\n", NUM_INODE_BLOCKS);
}

/* specify the block number to be set in the mask */
int fs_setmaskbit(int b) {
	int mbyte, mbit;
	mbyte = b / 8;
	mbit = b % 8;

	fsd.freemask[mbyte] |= (0x80 >> mbit);
	return OK;
}

/* specify the block number to be read in the mask */
int fs_getmaskbit(int b) {
	int mbyte, mbit;
	mbyte = b / 8;
	mbit = b % 8;

	return(((fsd.freemask[mbyte] << mbit) & 0x80) >> 7);
	return OK;

}

/* specify the block number to be unset in the mask */
int fs_clearmaskbit(int b) {
	int mbyte, mbit, invb;
	mbyte = b / 8;
	mbit = b % 8;

	invb = ~(0x80 >> mbit);
	invb &= 0xFF;

	fsd.freemask[mbyte] &= invb;
	return OK;
}

/* This is maybe a little overcomplicated since the lowest-numbered
block is indicated in the high-order bit.  Shift the byte by j
positions to make the match in bit7 (the 8th bit) and then shift
that value 7 times to the low-order bit to print.  Yes, it could be
the other way...  */
void fs_printfreemask(void) {
	int i, j;

	for (i = 0; i < fsd.freemaskbytes; i++) {
		for (j = 0; j < 8; j++) {
			printf("%d", ((fsd.freemask[i] << j) & 0x80) >> 7);
		}
		if ((i % 8) == 7) {
			printf("\n");
		}
	}
	printf("\n");
}

// FS_OPEN
int fs_open(char *filename, int flags) {
	int fileFound = 0;
	int fd = next_open_fd;
	int i;
	struct inode node;
	bs_bread(dev0, SB_BLK, 0, &fsd, sizeof(struct fsystem));
	// Getting the entries count from rrot Directory
	int entriesCount = fsd.root_dir.numentries;

	for (i = 0; i < entriesCount; i++) {
		// Checking for the matching file name.
		if (strcmp(fsd.root_dir.entry[i].name, filename) == 0) {
			// Setting File State to open.
			oft[fd].state = FSTATE_OPEN;
			// Moving File Pointer to 0 
			oft[fd].fileptr = 0;
			// Update Directory entry.
			oft[fd].de = &fsd.root_dir.entry[i];
			fs_get_inode_by_num(0, fsd.root_dir.entry[i].inode_num, &node);
			// Updating inode in filetable.
			oft[fd].in = node;
			// update flag at inode
			node.type = flags;
			fs_put_inode_by_num(0, fsd.root_dir.entry[i].inode_num, &node);
			//Updating the flag to 1 if file is found.
			fileFound = 1;
		}
	}
	//Throw Error if file not found.
	if (!fileFound) {
		printf("No File Exists witht the FileName.");
		return SYSERR;
	}
	//Incrementing the next_open_fd
	next_open_fd++;
	return fd;
}

// FS_CLOSE
int fs_close(int fd) {
	// Check whether if fd is valid or not
	if ((fd > next_open_fd) || (fd < 0)) {
		printf("File entry not found to close. Please check the File entry.\n");
		return SYSERR;
	}
	else {
		// Changing the state to FSTATE_CLOSED
		oft[fd].state = FSTATE_CLOSED;
		// Updating the pointer.
		oft[fd].fileptr = 0;
		next_open_fd--;
		return OK;
	}
}

// FS_CREATE
int fs_create(char *filename, int mode) {
	int fileFound = 0;
	int i = 1;
	struct filetable ft;
	struct inode in;

	// Get Root Directory
	struct directory dir = fsd.root_dir;
	int entriesCount = dir.numentries;

	// Check if the the file already exists or not.
	for (i = 0; i < entriesCount; i++) {
		if ((strcmp(filename, dir.entry[i].name) == 0)) {
			printf("File you are looking, already exists.\n");
			fileFound = 1;
			return SYSERR;
		}
	}

	if (mode == O_CREAT) {
		int fd = next_open_fd++;
		// Changing State
		ft.state = FSTATE_OPEN;
		
		// set initial File pointer.
		ft.fileptr = 0;
		ft.de = &(dir.entry[dir.numentries++]);
		
		// Updating the file name
		strcpy((ft.de)->name, filename);
		
		//Update Root Directory.
		memcpy(&(fsd.root_dir), &dir, sizeof(struct directory));

		in.id = node_count++;
		in.type = INODE_TYPE_FILE;

		// Put inode
		fs_put_inode_by_num(0, in.id, &in);
		int bl = in.id / INODES_PER_BLOCK;
		bl += FIRST_INODE_BLOCK;
		fs_setmaskbit(bl);
		
		// Updating file Table and file Directory.
		memcpy(&(ft.in), &in, sizeof(struct inode));
		memcpy(&(ft.de->inode_num), &(in.id), sizeof(int));
		memcpy(&(fsd.root_dir.entry[dir.numentries - 1].inode_num), &(in.id), sizeof(int));
		memcpy(oft + fd, &ft, sizeof(struct filetable));

		// Increment inode Counter.
		fsd.inodes_used++;
		return fd;
	}
	else {
		return SYSERR;
	}	
}

// FS_SEEK
int fs_seek(int fd, int offset) {
	//struct filetable ft = oft[fd];
	offset = oft[fd].fileptr + offset;
	if (offset<0) {
		printf("Exceeded The File Boundary ! Reseting the pointer.");
		offset = 0;
	}
	memcpy(&((oft + fd)->fileptr), &offset, sizeof(int));
	return fd;
}

// FS_READ
int fs_read(int fd, void *buf, int nbytes) {
	// Initialization Common Items.
	struct filetable ft = oft[fd];
	struct inode in = ft.in;
	//struct directory dir = fsd.root_dir;
	int orig_nbytes = nbytes;
	int inodeblk = (ft.fileptr / fsd.blocksz);
	int inodeoffset = (ft.fileptr % fsd.blocksz);

	// check if the File is closed or not.
	if (ft.state == FSTATE_CLOSED) {
		printf("File is Closed State");
		return SYSERR;
	}
	if (inodeblk<INODEBLOCKS) {
		int dst_blk = ft.in.blocks[inodeblk];
	while (nbytes>0) {
		// If the entire data is in same block.
		if (nbytes<(fsd.blocksz - inodeoffset)) {
			bs_bread(0, dst_blk, inodeoffset, buf, nbytes);
			ft.fileptr += nbytes;
			buf = buf + nbytes;
			nbytes = 0;
			return orig_nbytes;
		}
		else {
			if (inodeblk == INODEBLOCKS - 1) {
				printf("Requested bytes Exceeded the limits");
				return orig_nbytes - nbytes;
			}
			bs_bread(0, dst_blk, inodeoffset, buf, fsd.blocksz - inodeoffset);
			buf += (fsd.blocksz - inodeoffset);
			nbytes -= (fsd.blocksz - inodeoffset);
			ft.fileptr += (fsd.blocksz - inodeoffset);
			memcpy(oft + fd, &ft, sizeof(struct filetable));
			dst_blk = in.blocks[++inodeblk];
			inodeoffset = 0;
		}
	}
	}
	return orig_nbytes - nbytes;
}

// FS_WRITE
int fs_write(int fd, void *buf, int nbytes) {
	// Initialization Common Items.
	int i;
	struct filetable ft = oft[fd];
	struct inode in = ft.in;
	int orig_nbytes = nbytes;
	int inodeoffset = (ft.fileptr % fsd.blocksz);
	int inodeblk = (ft.fileptr / fsd.blocksz);
	
	// check if the File is closed or not.
	if (ft.state == FSTATE_CLOSED) {
		printf("File is Closed State");
		return SYSERR;
	}
	else if (inodeblk < INODEBLOCKS) {
		while (nbytes>0) {
			if (in.blocks[inodeblk] == 0) {
				i = get_next_block();
				memcpy(in.blocks + inodeblk, &i, sizeof(int));
				memcpy(&((oft + fd)->in), &(in), sizeof(struct inode));
				ft.in = in;
				fs_put_inode_by_num(0, in.id, &in);
				fs_setmaskbit(i);
			}
			else if (in.blocks[inodeblk]>0) {
				i = in.blocks[inodeblk];
			}
			if (nbytes<(fsd.blocksz - inodeoffset)) {
				bs_bwrite(0, i, inodeoffset, buf, nbytes);
				ft.fileptr += nbytes;
				nbytes = 0;
				memcpy(oft + fd, &ft, sizeof(struct filetable));
				return orig_nbytes;
			}
			else {
				if (inodeblk == INODEBLOCKS - 1) {
					printf("Requested bytes Exceeded the limits");
					return orig_nbytes - nbytes;
				}
				bs_bwrite(0, i, inodeoffset, buf, fsd.blocksz - inodeoffset);
				buf += (fsd.blocksz - inodeoffset);
				nbytes -= (fsd.blocksz - inodeoffset);
				ft.fileptr += (fsd.blocksz - inodeoffset);
				memcpy(oft + fd, &ft, sizeof(struct filetable));
				inodeblk++;
				inodeoffset = 0;
			}
		}
	}
	return orig_nbytes - nbytes;
}
#endif