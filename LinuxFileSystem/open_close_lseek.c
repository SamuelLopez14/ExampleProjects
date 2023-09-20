int pfd()
{
    printf("fd  mode    offset  INODE\n");
    printf("---------------------------\n");
    for (int i =0;i<64;i++)
    {
        
        if(running->fd[i])
        {
            printf("%d  ",i);
            if(running->fd[i]->mode == 0)
            {
                printf("read    %d  [%d,%d]\n",running->fd[i]->offset,running->fd[i]->minodePtr->dev,running->fd[i]->minodePtr->ino);
            }
            if(running->fd[i]->mode == 1)
            {
                printf("write    %d  [%d,%d]\n",running->fd[i]->offset,running->fd[i]->minodePtr->dev,running->fd[i]->minodePtr->ino);
            }
            if(running->fd[i]->mode ==2)
            {
                printf("R/W    %d  [%d,%d]\n",running->fd[i]->offset,running->fd[i]->minodePtr->dev,running->fd[i]->minodePtr->ino);
            }
            if(running->fd[i]->mode == 3)
            {
                printf("Append    %d  [%d,%d]\n",running->fd[i]->offset,running->fd[i]->minodePtr->dev,running->fd[i]->minodePtr->ino);
            }
            //printf("mip ref: %d", running->fd[i]->minodePtr->refCount);
        }
    }
}

int open_file(char *pathname, int selected_mode)
{
    //int is_open = 0;
    int oftNum = 0;
    int fdNum = 0;
    printf("Pathname: %s Mode:%d\n",pathname,selected_mode);
    if (selected_mode<0 || selected_mode>3)
    {
        printf("%d is not a possible mode choice\n",selected_mode);
        return -1;
    }
    int ino = getino(pathname);
    if(ino ==0)
    {
        printf("%s does not exist\n",pathname);
        return -1;
    }
    MINODE *mip = iget(dev,ino);
    if (!S_ISREG(mip->INODE.i_mode))
    {
        printf("%s is not a regular file\n",pathname);
        iput(mip);
        return -1;
    }
    for(int i =0;i<64;i++)
    {

        if (oft[i].refCount>0 && oft[i].minodePtr->ino == ino)
        {
            if(oft[i].mode==0&&selected_mode==0)
            {
                printf("%s already open for read, opening for read again\n",pathname);
                //break;
            }
            else{
                printf("%s is open in uncompatible mode\n",pathname);
                iput(mip);
                return -1;
            }
        }

    }

    for (int i =0;i<64;i++)
    {
        if (oft[i].refCount == 0)
        {
            oftNum = i;
            break;
        }
    }

    oft[oftNum].mode = selected_mode;
    oft[oftNum].refCount = 1;
    oft[oftNum].minodePtr = mip;

    switch(selected_mode){
         case 0 : oft[oftNum].offset = 0;     // R: offset = 0
                  break;
         case 1 : truncate(mip);        // W: truncate file to 0 size
                  oft[oftNum].offset = 0;
                  break;
         case 2 : oft[oftNum].offset = 0;     // RW: do NOT truncate file
                  break;
         case 3 : oft[oftNum].offset =  mip->INODE.i_size;  // APPEND mode
                  break;
         default: printf("invalid mode\n");
                  return(-1);
      }

    for(int i =0;i<64;i++)
    {
        if (running->fd[i]==0)
        {
            fdNum = i;
            break;
        }
    }
    running->fd[fdNum] = &oft[oftNum];
    if (selected_mode==0)
    {
        mip->INODE.i_atime = time(0L);
    }
    else{
        mip->INODE.i_atime = mip->INODE.i_mtime = time(0L);
    }
    mip->dirty = 1;
    //printf("%d",fdNum);
    return fdNum;
}

int truncate(MINODE *mip)
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
	  printf("deallocating: %d\n ",*temp2);
    bdalloc(dev,*temp2);
	  temp2 = temp2+1;
	  
	}
	  printf("deallocating: %d\n",*temp);
    bdalloc(dev,*temp);
	  temp = temp+1;
	}
  bdalloc(dev,mip->INODE.i_block[13]);
   }
   mip->INODE.i_size = 0;

}

int close_file(int fd)
{
    if(fd<0||fd>=64)
    {
        printf("Invalid fd\n");
        return -1;
    }
    if (running->fd[fd]==0)
    {
        printf("fd: %d is not open\n",fd);
        return -1;
    }
    running->fd[fd]->refCount --;
    if(running->fd[fd]->refCount>0)
    {
        running->fd[fd] = 0;
        return 0;
    }
    iput(running->fd[fd]->minodePtr);
    running->fd[fd]=0;
}

int mylseek(int fd, int byte)
{
    printf("Fd: %d  Byte: %d\n",fd,byte);
    if (byte<0 || byte > running->fd[fd]->minodePtr->INODE.i_size)
    {
        printf("Invalid Byte Given\n");
        return -1;
    }
    int oldbyte = running->fd[fd]->offset;
    running->fd[fd]->offset = byte;
}