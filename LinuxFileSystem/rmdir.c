void rm_child(MINODE *pmip, char *name)
{
  int blockNumber = pmip->INODE.i_block[0];
  char buf[BLKSIZE], *cp,temp[256];
  
  DIR *dp,*dpPrev;

  get_block(dev, blockNumber, buf);
   dp = (DIR *)buf;
   cp = buf;
   cp += dp->rec_len;
   dpPrev = dp;
   dp = (DIR *)cp;
   printf("  ino   rlen  nlen  name\n");

   while (cp < buf + BLKSIZE){
     strncpy(temp, dp->name, dp->name_len);
     temp[dp->name_len] = 0;
     printf("%4d  %4d  %4d    %s\n", 
           dp->inode, dp->rec_len, dp->name_len, temp);

           if(!strcmp(temp,name))
           {
             printf("located dir %s\n",temp);
             if(cp+dp->rec_len>=buf + BLKSIZE)
             {
               printf("Removing dir at the end\n");
               dpPrev->rec_len+=dp->rec_len;
               put_block(dev, blockNumber, buf);
               return;
             }
             else{
               printf("removing dir from the middle\n");
               cp+=dp->rec_len;
               printf("dp->reclen before %d\n",dp->rec_len);
               int remainder = dp->rec_len;
               printf("copying %d bytes\n",&buf[BLKSIZE]-cp);
               memcpy((void *)dp,(void *)cp,(&buf[BLKSIZE]-cp));
               //put_block(dev, blockNumber, buf);
               //search(pmip,"notthere");
               printf("dp->reclen adter %d remainder: %d\n",dp->rec_len,remainder);
               cp = (char *)dp;
               while(cp+remainder+dp->rec_len<(buf + BLKSIZE))
               {
                 printf("in the loop\n");
                 printf("cp is %ld dp is : %d ino is %d\n",cp,dp->rec_len,dp->inode);
                 //getchar();
                 cp+=dp->rec_len;
                 dp=(DIR *)cp;
               }
               dp->rec_len+=remainder;
               put_block(dev, blockNumber, buf);
               return;
             }
           }


     cp += dp->rec_len;
     dpPrev = dp;
     dp = (DIR *)cp;
   }

   printf("rmdir failed\n");

}

void myrmdir(char *pathname)
{
  printf("rmdir activated beepboop\n");
  if(!strcmp(pathname,""))
  {
    printf("Pathname given is empty\n");
    return;
  }
  int ino = getino(pathname);
  if(ino ==0)
  {
    printf("%s does not exist\n",pathname);
    return;
  }

  MINODE *mip = iget(dev,ino);
  if(mip == running->cwd)
  {
    printf("Cannot remove CWD\n");
    iput(mip);
    return;
  }
  if(!S_ISDIR(mip->INODE.i_mode))
  {
    printf("This is not a directory, returning to menu\n");
    iput(mip);
    return;
  }

  if(IsEmptyDir(mip)>2)
  {
    printf("The Dir is not empty, returning to menu\n");
    iput(mip);
    return;
  }
  printf("rmdir would continue\n");
  int pino = findino(mip,mip->ino);
  MINODE *pmip = iget(dev,pino);
  char myName[128];
  findmyname(pmip,mip->ino,myName);
  if(!strcmp(myName,".")||!strcmp(myName,".."))
  {
    printf("Can't remove Dir w/ name %s\n",myName);
  }
  printf("my name is %s\n",myName);
  rm_child(pmip,myName);
  printf("before :%d\n",pmip->INODE.i_links_count);
  pmip->INODE.i_links_count--;
  printf("after :%d\n",pmip->INODE.i_links_count);
  pmip->dirty =1;
  iput(pmip);
  bdalloc(mip->dev,mip->INODE.i_block[0]);
  idalloc(mip->dev,mip->ino);
  mip->dirty =1;
  iput(mip);
}

int IsEmptyDir(MINODE *mip)
{
  int blockNumber = mip->INODE.i_block[0],numDirs=0;
  char buf[BLKSIZE], *cp,temp[256];
  
  DIR *dp;

  get_block(dev, blockNumber, buf);
   dp = (DIR *)buf;
   cp = buf;
   printf("  ino   rlen  nlen  name\n");

   while (cp < buf + BLKSIZE){
     strncpy(temp, dp->name, dp->name_len);
     temp[dp->name_len] = 0;
     printf("%4d  %4d  %4d    %s\n", 
           dp->inode, dp->rec_len, dp->name_len, temp);

numDirs++;
     cp += dp->rec_len;
     dp = (DIR *)cp;
   }

   return numDirs;

}