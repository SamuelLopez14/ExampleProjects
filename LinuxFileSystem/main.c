/****************************************************************************
*                   KCW: mount root file system                             *
*****************************************************************************/
//Worked in collaboration with partner: Juan Gil-Camacho 011618194
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ext2fs/ext2_fs.h>
#include <string.h>
#include <libgen.h>
#include <sys/stat.h>
#include <time.h>

#include "type.h"

extern MINODE *iget();

MINODE minode[NMINODE];
MINODE *root;
PROC   proc[NPROC], *running;
MOUNT mountTable[8];

char gpath[128]; // global for tokenized components
char *name[64];  // assume at most 64 components in pathname
int   n;         // number of component strings

int fd, dev;
int nblocks, ninodes, bmap, imap, iblk;
char line[128], cmd[32], pathname[128],pathname2[128];
#include "util.c"
#include "cd_ls_pwd.c"

#include "alloc.c"
#include "mkdir_creat.c"
#include "rmdir.c"
#include "link_unlink.c"
#include "symlink.c"
#include "open_close_lseek.c"
#include "Read_cat.c"
#include "write_cp.c"
#include "mount_umount.c"
int init()
{
  int i, j;
  MINODE *mip;
  PROC   *p;

  printf("init()\n");

  for (i=0; i<NMINODE; i++){
    mip = &minode[i];
    mip->dev = mip->ino = 0;
    mip->refCount = 0;
    mip->mounted = 0;
    mip->mptr = 0;

  }
  for (i=0; i<NPROC; i++){
    p = &proc[i];
    p->pid = i;
    p->uid = p->gid = 0;
    p->cwd = 0;
    for (j=0;j<64;j++)
    {
      p->fd[j] = NULL;
    }
  }

  for (i = 0;i<64;i++ )
  {
    oft[i].refCount =0;
  }

  for (int i = 0;i<8;i++)
  {
    mountTable[i].dev = 0;
  }
}

// load root INODE and set root pointer to it
int mount_root()
{  
  printf("mount_root()\n");
  root = iget(dev, 2);
}

int main(int argc, char *argv[ ])
{
  char disk[128];
  strcpy(disk,argv[1]);

  int ino;
  char buf[BLKSIZE];

  printf("checking EXT2 FS ....");
  if ((fd = open(disk, O_RDWR)) < 0){
    printf("open %s failed\n", disk);
    exit(1);
  }

  dev = fd;    // global dev same as this fd   

  /********** read super block  ****************/
  get_block(dev, 1, buf);
  sp = (SUPER *)buf;

  /* verify it's an ext2 file system ***********/
  if (sp->s_magic != 0xEF53){
      printf("magic = %x is not an ext2 filesystem\n", sp->s_magic);
      exit(1);
  }     
  printf("EXT2 FS OK\n");
  ninodes = sp->s_inodes_count;
  nblocks = sp->s_blocks_count;

  get_block(dev, 2, buf); 
  gp = (GD *)buf;

  bmap = gp->bg_block_bitmap;
  imap = gp->bg_inode_bitmap;
  iblk = gp->bg_inode_table;


  printf("bmp=%d imap=%d inode_start = %d\n", bmap, imap, iblk);

  init();  
  mount_root();
    mountTable[0].mounted_inode = root;
  mountTable[0].bmap = bmap;
  mountTable[0].dev = dev;
  mountTable[0].iblk = iblk;
  mountTable[0].imap = imap;
  mountTable[0].nblocks = nblocks;
  mountTable[0].ninodes = ninodes;
  strcpy(mountTable[0].name,disk);
  strcpy(mountTable[0].mount_name,"/");
  printf("root refCount = %d\n", root->refCount);

  printf("creating P0 as running process\n");
  running = &proc[0];
  running->cwd = iget(dev, 2);
  printf("root refCount = %d\n", root->refCount);

  // WRTIE code here to create P1 as a USER process
  
  while(1){
    printf("input command : [ls|cd|pwd|mkdir|creat|rmdir|symlink|link|unlink|stat|open|close|lseek|read|cat|write|cp|mount|umount|quit] ");
    fgets(line, 128, stdin);
    line[strlen(line)-1] = 0;

    if (line[0]==0)
       continue;
    pathname[0] = 0;

    sscanf(line, "%s %s", cmd, pathname);
    printf("cmd=%s pathname=%s\n", cmd, pathname);
  
    if (strcmp(cmd, "ls")==0)
       ls(pathname);
    else if (strcmp(cmd, "cd")==0)
       cd(pathname);
    else if (strcmp(cmd, "pwd")==0){
      pwd(running->cwd);
    }
    else if (strcmp(cmd, "quit")==0){
       quit();
    }
    else if(strcmp(cmd,"mkdir")==0){
      mymkdir(pathname);
    }
    else if(strcmp(cmd,"creat")==0)
    {
      mycreat(pathname);
    }
    else if(strcmp(cmd,"rmdir")==0)
    {
      myrmdir(pathname);
    }
    else if(strcmp(cmd,"link")==0)
    {
      sscanf(line, "%s %s %s", cmd, pathname,pathname2);
      mylink(pathname,pathname2);
    }
    else if(strcmp(cmd,"unlink")==0)
    {
      myunlink(pathname);
    }
    else if(strcmp(cmd,"symlink")==0)
    {
      sscanf(line, "%s %s %s", cmd, pathname,pathname2);
      mysymlink(pathname,pathname2);
    }
    else if(!strcmp(cmd,"readlink"))
    {
      char buffer[128];
      int l = readlink(pathname,buffer);
      if(l)
      {
        printf("size: %d linked file: %s\n",l,buffer);
      }
    }
    else if(strcmp(cmd, "stat")==0)
    {
      mystat(pathname);
    }
    else if(strcmp(cmd,"pfd")==0)
    {
      pfd();
    }
    else if (strcmp(cmd,"open")==0)
    {
      int mode;
      sscanf(line, "%s %s %d",cmd,pathname,&mode);
      open_file(pathname,mode);
    }
    else if (strcmp(cmd, "close")==0)
    {
      int fd;
      sscanf(line,"%s %d",cmd,&fd);
      close_file(fd);
    }
    else if(strcmp(cmd,"lseek")==0)
    {
      int fd,byte;
      sscanf(line,"%s %d %d",cmd,&fd,&byte);
      mylseek(fd,byte);
    }
    else if(strcmp(cmd,"read")==0)
    {
      int rfd, rnb;
      sscanf(line,"%s %d %d",cmd,&rfd,&rnb);
      read_file(rfd,rnb);
    }
    else if(strcmp(cmd,"cat")==0)
    {
      mycat(pathname);
    }
    else if(strcmp(cmd,"write")==0)
    {
      int wfd;
      sscanf(line,"%s %d %s",cmd,&wfd,pathname);
      write_file(wfd,&line[8]);
    }
    else if(strcmp(cmd,"cp")==0)
    {
      sscanf(line,"%s %s %s",cmd,pathname,pathname2);
      mycp(pathname,pathname2);
    }
    else if(strcmp(cmd,"mount")==0)
    {
      if (!strcmp(pathname,""))
      {
        for (int i =0;i<8;i++)
        {
          printf("Index:%d\ndev:%d\niblk: %d\nimap: %d\n bmap: %d\nname: %s\n",i,mountTable[i].dev,mountTable[i].iblk,mountTable[i].imap,mountTable[i].bmap,mountTable[i].name);
        }
      }
      else
      {
        sscanf(line,"%s %s %s",cmd,pathname,pathname2);
        mount(pathname,pathname2);
      }
    }
    else if(strcmp(cmd,"umount")==0)
    {
      umount(pathname);
    }
  }
}

int quit()
{
  iput(running->cwd);
  for(int i =0;i<64;i++)
  {
    if(running->fd[i])
    {
      close_file(i);
    }
  }
  int i;
  MINODE *mip;
  for (i=0; i<NMINODE; i++){
    mip = &minode[i];
    printf("inode: %d ref: %d\n",mip->ino,mip->refCount);
    if (mip->refCount > 0)
      iput(mip);
  }
  exit(0);
}
