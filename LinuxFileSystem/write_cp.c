int write_file(int fdnum, char *words)
{
    if (running->fd[fdnum]==0)
    {
        printf("fd %d is not open\n",fdnum);
        return -1;
    }
    if (strcmp(words,"")==0)
    {
        printf("Can't write an empty string");
        return -1;
    }
    if (running->fd[fdnum]->mode==0)
    {
        printf("fd:%d is not open for a mode that supports writing\n",fdnum);
        return -1;
    }
    
    char buf[1024];
    strncpy(buf,words,1024);
    printf("portion written is: %s FD: %d\n",buf,fdnum);
    int returno = mywrite(fdnum,buf,strlen(buf));
    return returno;
    


}

int mywrite(int fdnum, char buf[],int nbytes)
{
    int count = nbytes,minValue;
    int avail = running->fd[fdnum]->minodePtr->INODE.i_size - running->fd[fdnum]->offset,remain;
    char *cq = buf,readbuff[BLKSIZE],*cp;
    int lbk;
    int startbyte;
    int blk;
    int buf12[256],buf13[256],dbuf[256];

    while(nbytes>0)
    {
         lbk = running->fd[fdnum]->offset/BLKSIZE;
        startbyte = running->fd[fdnum]->offset%BLKSIZE;

        if(lbk<12)
        {
            if(running->fd[fdnum]->minodePtr->INODE.i_block[lbk]==0)
            {
                running->fd[fdnum]->minodePtr->INODE.i_block[lbk] = balloc(running->fd[fdnum]->minodePtr->dev);
                get_block(running->fd[fdnum]->minodePtr->dev,running->fd[fdnum]->minodePtr->INODE.i_block[lbk],readbuff);
                for(int i =0;i<BLKSIZE;i++)
                {
                    readbuff[i] = 0;
                }
                put_block(running->fd[fdnum]->minodePtr->dev,running->fd[fdnum]->minodePtr->INODE.i_block[lbk],readbuff);
            }
            blk = running->fd[fdnum]->minodePtr->INODE.i_block[lbk];
        }
        else if(lbk>=12 && lbk<256+12)
        {
            if(running->fd[fdnum]->minodePtr->INODE.i_block[12]==0)
            {
                running->fd[fdnum]->minodePtr->INODE.i_block[12] = balloc(running->fd[fdnum]->minodePtr->dev);
                get_block(running->fd[fdnum]->minodePtr->dev,running->fd[fdnum]->minodePtr->INODE.i_block[12],readbuff);
                for(int i =0;i<BLKSIZE;i++)
                {
                    readbuff[i] = 0;
                }
                put_block(running->fd[fdnum]->minodePtr->dev,running->fd[fdnum]->minodePtr->INODE.i_block[12],readbuff);
            }
            get_block(running->fd[fdnum]->minodePtr->dev,running->fd[fdnum]->minodePtr->INODE.i_block[12],(char *)buf12);
            if(buf12[lbk-12]==0)
            {
                buf12[lbk-12] = balloc(running->fd[fdnum]->minodePtr->dev);
                zero_blk(running->fd[fdnum]->minodePtr->dev,buf12[lbk-12]);
                

            }
            blk = buf12[lbk-12];
            put_block(running->fd[fdnum]->minodePtr->dev,running->fd[fdnum]->minodePtr->INODE.i_block[12],(char *)buf12);
            printf("lbk = %d,IDblk = %d, blk = %d\n",lbk,running->fd[fdnum]->minodePtr->INODE.i_block[12],blk);
        }
        else
        {
            if(running->fd[fdnum]->minodePtr->INODE.i_block[13]==0)
            {
                running->fd[fdnum]->minodePtr->INODE.i_block[13] = balloc(running->fd[fdnum]->minodePtr->dev);
                 zero_blk(running->fd[fdnum]->minodePtr->dev,running->fd[fdnum]->minodePtr->INODE.i_block[13]);
            }
            int DIDBlk = lbk -(12+256);
            int dibn = DIDBlk/256;
            int dibo = DIDBlk%256;
            get_block(running->fd[fdnum]->minodePtr->dev,running->fd[fdnum]->minodePtr->INODE.i_block[13],(char *)buf12);
            if(buf12[dibn]==0)
            {
                buf12[dibn] = balloc(running->fd[fdnum]->minodePtr->dev);
                zero_blk(running->fd[fdnum]->minodePtr->dev,buf12[dibn]);
            }
            get_block(running->fd[fdnum]->minodePtr->dev,buf12[dibn],(char *)buf13);
            if(buf13[dibo]==0)
            {
                buf13[dibo] = balloc(running->fd[fdnum]->minodePtr->dev);
                zero_blk(running->fd[fdnum]->minodePtr->dev,buf13[dibo]);   
            }
            blk = buf13[dibo];
            put_block(running->fd[fdnum]->minodePtr->dev,buf12[dibn],(char *)buf13);
            put_block(running->fd[fdnum]->minodePtr->dev,running->fd[fdnum]->minodePtr->INODE.i_block[13],(char *)buf12);



        }
     get_block(running->fd[fdnum]->minodePtr->dev, blk, readbuff);   // read disk block into wbuf[ ]  
     char *cp = readbuff + startbyte;      // cp points at startByte in wbuf[]
     remain = BLKSIZE - startbyte;     // number of BYTEs remain in this block
     minValue = minOfThree(remain,nbytes,nbytes);

     while (remain > 0){               // write as much as remain allows  
           //*cp++ = *cq++;              // cq points at buf[ ]
           memcpy(cp,cq,minValue);
           nbytes-=minValue; remain-=minValue;         // dec counts
           running->fd[fdnum]->offset+=minValue;             // advance offset
           if (running->fd[fdnum]->offset > running->fd[fdnum]->minodePtr->INODE.i_size)  // especially for RW|APPEND mode
               running->fd[fdnum]->minodePtr->INODE.i_size=running->fd[fdnum]->offset;    // inc file size (if offset > fileSize)
           if (nbytes <= 0) break;     // if already nbytes, break
     }
     put_block(running->fd[fdnum]->minodePtr->dev, blk, readbuff);

    }
     running->fd[fdnum]->minodePtr->dirty = 1;       // mark mip dirty for iput() 
  printf("wrote %d char into file descriptor fd=%d\n", count, fdnum);           
  return count;
}

void zero_blk(int dev, int block)
{
    char buf[BLKSIZE];
    get_block(dev,block,buf);
    bzero(buf,BLKSIZE);
    put_block(dev,block,buf);
}

void mycp(char *path1, char *path2)
{
    int fd1 = open_file(path1,0);
    if(fd1==-1)
    {
        printf("cp failed\n");
        return;
    }
    int fd2 = getino(path2);
    if(fd2==0)
    {
        printf("%s does not exist yet, creating file\n",path2);
        mycreat(path2);
    }
    fd2 = open_file(path2,1);
    if(fd2==-1)
    {
        printf("cp failed opening second file\n");
        return;
    }
    int n,count = 0;
    char buf[1024];
    printf("fd1: %d  fd2: %d\n",fd1,fd2);
    pfd();
    while(n = myread(fd1,buf,1024))
    {
        printf("n is %d\n",n);
        count +=n;
        mywrite(fd2,buf,n);
    }
    printf("Wrote %d bytes total\n",count);
    close_file(fd1);
    close_file(fd2);

}

