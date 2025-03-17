struct file {
  enum { FD_NONE, FD_PIPE, FD_INODE } type;
  int ref; // reference count
  char readable;
  char writable;
  struct pipe *pipe;
  struct inode *ip;
  uint off;
};

struct inode_operations {
	struct inode* (*ialloc)(uint, short);
	void (*iupdate)(struct inode *);
	void (*ilock)(struct inode *);
	void (*iunlock)(struct inode *);
	void (*iput)(struct inode *);
	int (*readi)(struct inode *, char *, uint, uint);
	int (*writei)(struct inode *, char *, uint, uint);
	int (*namecmp)(const char *, const char *);
	struct inode* (*dirlookup)(struct inode *, char *, uint *);
	int (*dirlink)(struct inode *, char *, uint);
	struct inode * (*namei)(char *);
};

// in-memory copy of an inode
struct inode {
  uint dev;           // Device number
  uint inum;          // Inode number
  int ref;            // Reference count
  struct sleeplock lock; // protects everything below here
  int valid;          // inode has been read from disk?
  struct inode_operations *iops;

  short type;         // copy of disk inode
  short major;
  short minor;
  short nlink;
  uint size;
  uint addrs[NDIRECT+1];
};

// table mapping major device number to
// device functions
struct devsw {
  int (*read)(struct inode*, char*, int);
  int (*write)(struct inode*, char*, int);
};

extern struct devsw devsw[];

#define CONSOLE 1
