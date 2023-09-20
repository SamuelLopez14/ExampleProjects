/*********** util.c file ****************/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ext2fs/ext2_fs.h>
#include <string.h>
#include <libgen.h>
#include <sys/stat.h>
#include <time.h>

//#include "type.h"

/**** globals defined in main.c file ****/
extern MINODE minode[NMINODE];
extern MINODE *root;
extern PROC   proc[NPROC], *running;

extern char gpath[128];
extern char *name[64];
extern int n;

extern int fd, dev;
extern int nblocks, ninodes, bmap, imap, iblk;

extern char line[128], cmd[32], pathname[128];

int get_block(int dev, int blk, char *buf)
{
   lseek(dev, (long)blk*BLKSIZE, 0);
   read(dev, buf, BLKSIZE);
}   

MOUNT *getmptr(int dev2)
{
   printf("Starting getmtptr\n");
    for (int i =0;i<8;i++)
    {
        if (mountTable[i].dev==dev2)
        {
           printf("Entry found\n");
            return &mountTable[i];
        }
    }
    printf("entry not found\n");
    return NULL;
}

int put_block(int dev, int blk, char *buf)
{
   lseek(dev, (long)blk*BLKSIZE, 0);
   write(dev, buf, BLKSIZE);
}   

int tokenize(char *pathname)
{
  int i;
  char *s;
  printf("tokenize %s\n", pathname);

  strcpy(gpath, pathname);   // tokens are in global gpath[ ]
  n = 0;

  s = strtok(gpath, "/");
  while(s){
    name[n] = s;
    n++;
    s = strtok(0, "/");
  }
  name[n] = 0;
  
  for (i= 0; i<n; i++)
    printf("%s  ", name[i]);
  printf("\n");
}

// return minode pointer to loaded INODE
MINODE *iget(int dev, int ino)
{
  int i;
  MINODE *mip;
  char buf[BLKSIZE];
  int blk, offset;
  INODE *ip;

  for (i=0; i<NMINODE; i++){
    mip = &minode[i];
    if (mip->refCount && mip->dev == dev && mip->ino == ino){
       mip->refCount++;
       //printf("found [%d %d] as minode[%d] in core\n", dev, ino, i);
       return mip;
    }
  }
    
  for (i=0; i<NMINODE; i++){
    mip = &minode[i];
    if (mip->refCount == 0){
       //printf("allocating NEW minode[%d] for [%d %d]\n", i, dev, ino);
       mip->refCount = 1;
       mip->dev = dev;
       mip->ino = ino;

       // get INODE of ino to buf    
       blk    = (ino-1)/8 + iblk;
       offset = (ino-1) % 8;

       //printf("iget: ino=%d blk=%d offset=%d\n", ino, blk, offset);

       get_block(dev, blk, buf);
       ip = (INODE *)buf + offset;
       // copy INODE to mp->INODE
       mip->INODE = *ip;
       return mip;
    }
  }   
  printf("PANIC: no more free minodes\n");
  return 0;
}

void iput(MINODE *mip)
{
 int i, block, offset;
 char buf[BLKSIZE];
 INODE *ip;

 if (mip==0) 
     return;

 mip->refCount--;
 
 if (mip->refCount > 0) return;
 if (!mip->dirty)       return;
 
 /* write INODE back to disk */
 /**************** NOTE ******************************
  For mountroot, we never MODIFY any loaded INODE
                 so no need to write it back
  FOR LATER WROK: MUST write INODE back to disk if refCount==0 && DIRTY

  Write YOUR code here to write INODE back to disk
 *****************************************************/

 block = ((mip->ino - 1)/8)+iblk;
 offset = (mip->ino - 1)%8;

 get_block(mip->dev,block,buf);
 ip = (INODE *)buf+offset;
 *ip = mip->INODE;
 put_block(mip->dev,block,buf);
} 

int search(MINODE *mip, char *name)
{
   int i; 
   char *cp, c, sbuf[BLKSIZE], temp[256];
   DIR *dp;
   INODE *ip;

   printf("search for %s in MINODE = [%d, %d]\n", name,mip->dev,mip->ino);
   ip = &(mip->INODE);

   /*** search for name in mip's data blocks: ASSUME i_block[0] ONLY ***/

   get_block(dev, ip->i_block[0], sbuf);
   printf("block[0]: %d\n",ip->i_block[0]);
   dp = (DIR *)sbuf;
   cp = sbuf;
   printf("  ino   rlen  nlen  name\n");

   while (cp < sbuf + BLKSIZE){
     strncpy(temp, dp->name, dp->name_len);
     temp[dp->name_len] = 0;
     printf("searching for %s\n%4d  %4d  %4d    %s\n",name, 
           dp->inode, dp->rec_len, dp->name_len, temp);
           //getchar();
     if (strcmp(temp, name)==0){
        printf("found %s : ino = %d\n", temp, dp->inode);
        return dp->inode;
     }
     cp += dp->rec_len;
     dp = (DIR *)cp;
   }
   return 0;
}

int getino(char *pathname)
{
  int i, ino, blk, offset;
  char buf[BLKSIZE];
  INODE *ip;
  MINODE *mip,*temp;

  printf("getino: pathname=%s\n", pathname);
  if (strcmp(pathname, "/")==0)
      return 2;
  
  // starting mip = root OR CWD
  if (pathname[0]=='/')
     mip = root;
  else
     mip = running->cwd;
 
   if (mip->mounted)
   {
      mip = iget(mip->mptr->dev,2);
   }
   UpdateGlobals(mip->dev);
  mip->refCount++;         // because we iput(mip) later
  ino = mip->ino;
  tokenize(pathname);

  for (i=0; i<n; i++){
      printf("===========================================\n");
      printf("getino: i=%d name[%d]=%s\n", i, i, name[i]);

      if (strcmp(name[i],"..")==0 && mip->ino==2 && mip!=root)
      {
         temp = mip;
         printf("mip dev is %d\n",mip->dev);
         MOUNT * tempmount = getmptr(mip->dev);
       
         mip = tempmount->mounted_inode;
         mip->refCount++;
         iput(temp);
         UpdateGlobals(mip->dev);

      }
 
      ino = search(mip, name[i]);

      if (ino==0){
         iput(mip);
         printf("name %s does not exist\n", name[i]);
         return 0;
      }
      iput(mip);
      mip = iget(dev, ino);
      if (mip->mounted)
      {
         temp = mip;
         iput(temp);
         UpdateGlobals(mip->mptr->dev);
         mip = iget(mip->mptr->dev,2);
         //iput(temp);
         UpdateGlobals(mip->dev);

         ino = 2;
      }
   }

   iput(mip);
   return ino;
}

void UpdateGlobals(int dev2)
{
   printf("Updating dev to %d\n",dev2);

   MOUNT *tempMount = getmptr(dev2);
   dev = tempMount->dev;
   iblk = tempMount->iblk;
   imap = tempMount->imap;
   bmap = tempMount->bmap;
   nblocks = tempMount->nblocks;
   ninodes = tempMount->ninodes;
   printf("dev updated\n");
   
}

// These 2 functions are needed for pwd()
int findmyname(MINODE *parent, u32 myino, char myname[ ]) 
{
  // WRITE YOUR code here
  // search parent's data block for myino; SAME as search() but by myino
  // copy its name STRING to myname[ ]
  int i; 
   char *cp, c, sbuf[BLKSIZE], temp[256];
   DIR *dp;
   INODE *ip;

  

   get_block(dev, parent->INODE.i_block[0], sbuf);
   dp = (DIR *)sbuf;
   cp = sbuf;
   

   while (cp < sbuf + BLKSIZE){
     strncpy(temp, dp->name, dp->name_len);
     temp[dp->name_len] = 0;
    
     if (dp->inode==myino){
       //printf("found %s : ino = %d\n", temp, dp->inode);
       strcpy(myname,temp);
       //printf("%s\n",myname);
       return 1;
     }
     cp += dp->rec_len;
     dp = (DIR *)cp;
   }
   return 0;

  
}

int findino(MINODE *mip, u32 *myino) // myino = i# of . return i# of ..
{
   int i; 
   char *cp, c, sbuf[BLKSIZE], temp[256];
   DIR *dp;
   INODE *ip;

  

   get_block(dev, mip->INODE.i_block[0], sbuf);
   dp = (DIR *)sbuf;
   cp = sbuf;
   

   while (cp < sbuf + BLKSIZE){
     strncpy(temp, dp->name, dp->name_len);
     temp[dp->name_len] = 0;
    
     if (strcmp(temp, "..")==0){
       //printf("found %s : ino = %d\n", temp, dp->inode);
        return dp->inode;
     }
     cp += dp->rec_len;
     dp = (DIR *)cp;
   }
   return 0;
}
