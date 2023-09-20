int read_file(int fdnum, int nbytes)
{
    if (running->fd[fdnum]==0)
    {
        printf("fd %d is not open\n",fdnum);
        return -1;
    }
    if (nbytes<0)
    {
        printf("nbytes can't be negative: %d\n",nbytes);
        return -1;
    }
    if (running->fd[fdnum]->mode==1||running->fd[fdnum]->mode==3)
    {
        printf("fd:%d is not open for read\n",fdnum);
        return -1;
    }
    int actbytes = 0;
    if (nbytes>1024)
    {
        printf("read is only programmed to read at max 1024 bytes\n");
        actbytes = 1024;
    }
    else{
        actbytes = nbytes;
    }
    char buf[1024];
    for(int i =0;i<1024;i++)
    {
        buf[i]=0;
    }
    
    int readbytes = myread(fdnum,buf,actbytes);
    if(readbytes)
    {
    printf("%s\n",buf);
    }
    return readbytes;

}
int minOfThree(int a,int b,int c)
{
    int x = a;
    if (b<x)
    {
        x = b;
    }
    if (c<x)
    {
        x = c;
    }
    return x;
}

int myread(int fdnum,char *buf,int nbytes)
{
    int count = 0,minValue;
    int avail = running->fd[fdnum]->minodePtr->INODE.i_size - running->fd[fdnum]->offset,remain;
    char *cq = buf,readbuff[BLKSIZE],*cp;
    int lbk;
    int startbyte;
    int blk;
    int buf12[256],buf13[256],dbuf[256];

    while(nbytes&&avail)
    {
        lbk = running->fd[fdnum]->offset/BLKSIZE;
        startbyte = running->fd[fdnum]->offset%BLKSIZE;

        if(lbk<12)
        {
            blk = running->fd[fdnum]->minodePtr->INODE.i_block[lbk];
        }
        else if(lbk>=12 && lbk <256+12)
        {
            get_block(running->fd[fdnum]->minodePtr->dev,running->fd[fdnum]->minodePtr->INODE.i_block[12],(char *)buf12);
            blk = buf12[lbk-12];
            printf("lbk = %d,IDblk = %d, blk = %d\n",lbk,running->fd[fdnum]->minodePtr->INODE.i_block[12],blk);
        }
        else{
            int DIDBlk = lbk -(12+256);
            int dibn = DIDBlk/256;
            int dibo = DIDBlk%256;
            get_block(running->fd[fdnum]->minodePtr->dev,running->fd[fdnum]->minodePtr->INODE.i_block[13],(char *)buf12);
            get_block(running->fd[fdnum]->minodePtr->dev,buf12[dibn],(char *)buf13);
            blk = buf13[dibo];

        }

        get_block(running->fd[fdnum]->minodePtr->dev,blk,readbuff);
        cp = readbuff+startbyte;
        remain = BLKSIZE - startbyte;
        minValue = minOfThree(nbytes,remain,avail);

         while (remain > 0){
            //*cq++ = *cp++;             // copy byte from readbuf[] into buf[]
            memcpy(cq,cp,minValue);
             running->fd[fdnum]->offset+=minValue;           // advance offset 
             count+=minValue;                  // inc count as number of bytes read
             avail-=minValue; nbytes-=minValue;  remain-=minValue;
             if (nbytes <= 0 || avail <= 0) 
                 break;
       }


    }

    printf("Read %d bytes from fd: %d\n",count,fdnum);
    return count;

}

void mycat(char * pathname)
{
    printf("pathname: %s\n",pathname);
    int fd = open_file(pathname,0);
    printf("%d\n",fd);
    //return;
    if(fd==-1)
    {
        printf("CAT failed\n");
        return;
    }

    int n,count=0;
    char buf[1024],dummy;
   while(n = myread(fd,buf,1024))
   {
       count+=n;
       dummy = buf[1024];
       buf[1024]=0;
       printf("%s\n",buf);
       buf[1024] = dummy;
       for(int i =0;i<1024;i++)
       {
           buf[i]=0;
       }
   }
   printf("\nRead %d bytes total\n",count);
   close_file(fd);
}