void mylink(char * pathname, char *pathname2)
{
  char oldfile[128],newfile[128], newdir[128],newname[128],*newd,*newf;
  if(!strcmp(pathname,"")||!strcmp(pathname2,""))
  {
    printf("Pathname given is empty\n");
    return;
  }
  strcpy(oldfile,pathname);
  strcpy(newfile,pathname2);
  printf("The old file is: %s  The new file is %s\n",oldfile,newfile);
  int oldIno = getino(oldfile);
  if(oldIno==0)
  {
    printf("%s does not exist\n",oldfile);
    return;
  }
  MINODE *oldMip = iget(dev,oldIno);
  if(S_ISDIR(oldMip->INODE.i_mode))
  {
    printf("Cannot link from a directory: %s\n",oldfile);
    iput(oldMip);
    return;
  }

  printf("link would continue\n");
  strcpy(newdir,pathname2);
  newd = dirname(newdir);
  strcpy(newfile,pathname2);
  newf = basename(newfile);
  printf("new file dir: %s new file name %s\n",newd,newf);

  int pino = getino(newd);
  if(pino == 0)
  {
    printf("%s does not exist\n",newd);
    iput(oldMip);
    return;
  }
  MINODE *pmip = iget(dev,pino);

  if(!S_ISDIR(pmip->INODE.i_mode))
  {
    printf("%s is not a dir\n",newd);
    iput(oldMip);
    iput(pmip);
    return;

  }

  if(search(pmip,newf))
  {
    printf("%s already exists in %s\n",newf,newd);
    iput(oldMip);
    iput(pmip);
    return;
  }

  printf("Adding Entry %s\n",newf);
  enter_name(pmip,oldIno,newf);
  oldMip->INODE.i_links_count++;
  oldMip->dirty =1;
  pmip->dirty =1;
  iput(oldMip);
  iput(pmip);
  
}

void  myunlink(char *pathname)
{

  char newdir[128],newname[128],*newd,*newf;
  printf("you activated unlink boi\n");
  if(!strcmp(pathname,""))
  {
    printf("Pathname given is empty\n");
    return;
  }

  int ino = getino(pathname);
  if(ino == 0)
  {
    printf("%s not found\n",pathname);
    return;
  }

  MINODE *mip = iget(dev,ino);
  if(S_ISDIR(mip->INODE.i_mode))
  {
    printf("Can't unlink dir: %s\n",pathname);
    iput(mip);
    return;
  }
  strcpy(newdir,pathname);
  newd = dirname(newdir);
  strcpy(newname,pathname);
  newf = basename(newname);
  printf("newd: %s newf: %s\n",newd,newf);
  int pino = getino(newd);
  MINODE *pmip = iget(dev,pino);
  rm_child(pmip,newf);
  pmip->dirty =1;
  iput(pmip);

  mip ->INODE.i_links_count--;
  if(mip->INODE.i_links_count>0)
  {
    mip->dirty = 1;
  }
  else if(mip->INODE.i_links_count==0)
  {
    if(!S_ISLNK(mip->INODE.i_mode))
    {
      char buf2[BLKSIZE],buf3[BLKSIZE];
      int *temp,*temp2;
      printf("****************  DISK BLOCKS  *******************\n");
   for (int i=0; i<15; i++){
      if (mip->INODE.i_block[i]) 
         printf("block[%d] = %d\n", i, mip->INODE.i_block[i]);
   }

   printf("================ Direct Blocks ===================\n");
   for(int i =0;i<12&&mip->INODE.i_block[i]!=0;i++)
     {
       printf("deallocating: %d\n",mip->INODE.i_block[i]);
       bdalloc(dev,mip->INODE.i_block[i]);

     }
   if (mip->INODE.i_block[12]){
      printf("===============  Indirect blocks   ===============\n");
      
      
      get_block(dev,mip->INODE.i_block[12],buf2);
      temp = (int *)buf2;
      for(int i =0;i<BLKSIZE/4&&*temp;i++)
	{
	  printf("removing:%d\n ",*temp);
    bdalloc(dev,*temp);
	  temp = temp+1;
	  
	}
      printf("Deallocating Block: %d\n",mip->INODE.i_block[12]);
      bdalloc(dev,mip->INODE.i_block[12]);
   }

   if (mip->INODE.i_block[13]){
     printf("===========  Double Indirect blocks   ============\n");
     
     
      get_block(dev,mip->INODE.i_block[13],buf2);
      temp = (int *)buf2;
      while(*temp&&temp<buf2+BLKSIZE)
	{
	  get_block(dev,*temp,buf3);
	  temp2=(int *)buf3;
	  for(int i =0;i<BLKSIZE/4&&*temp2;i++)
	{
	  printf("deallocating: %d ",*temp2);
    bdalloc(dev,*temp2);
	  temp2 = temp2+1;
	  
	}
	  printf("deallocating: %d\n",*temp);
    bdalloc(dev,*temp);
	  temp = temp+1;
	}
  bdalloc(dev,mip->INODE.i_block[13]);
   }

    }
    idalloc(dev,mip->ino);
    mip->dirty=1;
    
  }
  iput(mip);



}

