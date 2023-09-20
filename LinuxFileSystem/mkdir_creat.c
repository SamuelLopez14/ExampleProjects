void mymkdir(char *pathname)
{
  //printf("mkdir has been activated\n");
  char temp[128],temp2[128], *dname, *bname;
  if(!strcmp(pathname,""))
  {
    printf("Pathname given is empty\n");
    return;
  }
  strcpy(temp,pathname);
  dname = dirname(temp);
  strcpy(temp2,pathname);

  bname = basename(temp2);
  printf("dirname is %s basename is %s\n",dname,bname);
  int pino = getino(dname);
  if(pino ==0)
  {
    printf("mkdir failed, returning to menu\n");
  
    return;
  }

  MINODE *pmip = iget(dev,pino);
  if(!S_ISDIR(pmip->INODE.i_mode))
  {
    printf("This is not a directory, returning to menu\n");
        iput(pmip);

    return;
  }

  if(search(pmip,bname))
  {
    printf("%s already exists\n",bname);
        iput(pmip);

    return;
  }

  printf("mkdir would continue\n");
  kmkdir(pmip,bname);

}

void kmkdir(MINODE *pmip,char *basename)
{
int newIno = ialloc(dev);
printf("New ino is %d\n",newIno);
int newBlock = balloc(dev);
printf("dev is %d\n",dev);
MINODE *mip = iget(dev,newIno);

 
INODE *ip = &mip->INODE;
ip->i_mode = 0x41ED; // 040755: DIR type and permissions
ip->i_uid = running->uid; // owner uid
ip->i_gid = running->gid; // group Id
ip->i_size = BLKSIZE; // size in bytes
ip->i_links_count = 2; // links count=2 because of . and ..
ip->i_atime = ip->i_ctime = ip->i_mtime = time(0L);
ip->i_blocks = 2; // LINUX: Blocks count in 512-byte chunks
ip->i_block[0] = newBlock; // new DIR has one data block
printf("Block is %d\n",ip->i_block[0]);
for(int i =1;i<15;i++)
{
  ip->i_block[i]=0;
}

mip->dirty = 1; // mark minode dirty
iput(mip); // write INODE to disk

char buf[BLKSIZE];
bzero(buf, BLKSIZE); // optional: clear buf[ ] to 0
DIR *dp = (DIR *)buf;
// make . entry
dp->inode = newIno;
dp->rec_len = 12;
dp->name_len = 1;
dp->name[0] = '.';
// make .. entry: pino=parent DIR ino, blk=allocated block
dp = (char *)dp + 12;
dp->inode = pmip->ino;
dp->rec_len = BLKSIZE-12; // rec_len spans block
dp->name_len = 2;
dp->name[0] = dp->name[1] = '.';
put_block(dev, newBlock, buf); // write to blk on diks
ls_dir(mip);
enter_name(pmip, newIno,basename);
pmip->INODE.i_links_count++;
pmip->dirty = 1;
iput(pmip);





}
int getideallength(int a)
{
  
  return 4*((8+a+3)/4);
}

int enter_name(MINODE *pip, int ino, char *name)
{
  char buf[BLKSIZE];
  get_block(dev,pip->INODE.i_block[0],buf);
  DIR *dp = (DIR *)buf;
  char *cp = buf;
  while (cp + dp->rec_len < buf + BLKSIZE){
  cp += dp->rec_len;
  dp = (DIR *)cp;
  }

  int n_len = strlen(name);
  int need_length = getideallength(n_len);
  int ideal_length = getideallength(dp->name_len); 
  int remain = dp->rec_len - ideal_length;
  printf("n_len: %d need: %d ideal %d remain: %d\n",n_len,need_length,ideal_length,remain);
  if(remain>=need_length)
  {
    dp->rec_len = ideal_length;
    cp+=dp->rec_len;
    dp = (DIR *)cp;
    dp->rec_len = remain;
    dp->inode = ino;
    dp->name_len = n_len;
    strncpy(dp->name,name,n_len);
    
    put_block(dev,pip->INODE.i_block[0],buf);
    return 1;
  }
  printf("New Block Needed\n");


}

void mycreat(char *pathname)
{
  //printf("mkdir has been activated\n");
  char temp[128],temp2[128], *dname, *bname;
  if(!strcmp(pathname,""))
  {
    printf("Pathname given is empty\n");
    return;
  }
  strcpy(temp,pathname);
  dname = dirname(temp);
  strcpy(temp2,pathname);

  bname = basename(temp2);
  printf("dirname is %s basename is %s\n",dname,bname);
  int pino = getino(dname);
  if(pino ==0)
  {
    printf("creat failed, returning to menu\n");
        

    return;
  }

  MINODE *pmip = iget(dev,pino);
  if(!S_ISDIR(pmip->INODE.i_mode))
  {
    printf("This is not a directory, returning to menu\n");
        iput(pmip);

    return;
  }

  if(search(pmip,bname))
  {
    printf("%s already exists\n",bname);
        iput(pmip);

    return;
  }

  printf("creat will continue\n");
  kcreat(pmip,bname);

}


void kcreat(MINODE *pmip,char *basename)
{
int newIno = ialloc(dev);
printf("New Ino is %d\n",newIno);
MINODE *mip = iget(dev,newIno);

 
INODE *ip = &mip->INODE;
ip->i_mode = 0x81A4; 
ip->i_uid = running->uid; // owner uid
ip->i_gid = running->gid; // group Id
ip->i_size = 0; // size in bytes
ip->i_links_count = 1; 
ip->i_atime = ip->i_ctime = ip->i_mtime = time(0L);
ip->i_blocks = 0; // LINUX: Blocks count in 512-byte chunks
//ip->i_block[0] = newBlock; // new DIR has one data block
//printf("Block is %d\n",ip->i_block[0]);
for(int i =0;i<15;i++)
{
  ip->i_block[i]=0;
}

mip->dirty = 1; // mark minode dirty
iput(mip); // write INODE to disk


ls_file(mip,basename);
enter_name(pmip, newIno,basename);
//pmip->INODE.i_links_count++;
pmip->dirty = 1;
iput(pmip);
}