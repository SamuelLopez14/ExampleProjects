

int mount(char *disk, char *dirname)
{
  MOUNT *emptyTable;  
    for(int i =0;i<8;i++)
    {
        if (strcmp(mountTable[i].name,disk)==0&&mountTable[i].dev!=0)
        {
            return -1;
        }
    }

    for (int i =0;i<8;i++)
    {
        if (mountTable[i].dev==0)
        {
            emptyTable = &mountTable[i];
            break;
        }
    }
    int ino = getino(dirname);
    if (ino ==0)
    {
        printf("failed to find mounting point %s\n",dirname);
        return -1;
    }
    MINODE *pmip = iget(dev,ino);
    if(!S_ISDIR(pmip->INODE.i_mode)||pmip==running->cwd)
    {
        iput(pmip);
        printf("Directory %s is CWD or not a directory\n",pathname);
        return -1;
    }

    int fd = open(disk,O_RDWR);
    if(fd==0)
    {
        printf("%s not found\n",disk);
        iput(pmip);
        return -1;
    }
    emptyTable->dev = fd;
    char buf[1024];
    get_block(fd, 1, buf);
    sp = (SUPER *)buf;

  /* verify it's an ext2 file system ***********/
  if (sp->s_magic != 0xEF53){
      printf("magic = %x is not an ext2 filesystem\n", sp->s_magic);
      iput(pmip);
      return -1;
    }

    emptyTable->ninodes = sp->s_inodes_count;
    emptyTable->nblocks = sp->s_blocks_count;

    get_block(fd, 2, buf); 
    gp = (GD *)buf;

    emptyTable->bmap = gp->bg_block_bitmap;
    emptyTable->imap = gp->bg_inode_bitmap;
    emptyTable->iblk = gp->bg_inode_table;
    emptyTable->mounted_inode = pmip;
    strcpy(emptyTable->name,disk);
    pmip->mptr = emptyTable;
    pmip->mounted = 1;
    pmip->dirty = 1;
    return 0;
}

int umount(char *filesys)
{
    MOUNT *workingMount=NULL;
    for(int i = 0;i<8;i++)
    {
        if(strcmp(filesys,mountTable[i].name)==0&&mountTable[i].dev!=0)
        {
            workingMount = &mountTable[i];
            break;
        }

    }

    if (workingMount==NULL)
    {
        printf("%s not found in mountTable\n",filesys);
        return -1;
    }

    for (int i =0;i<NMINODE;i++)
    {
        if (minode[i].dev==workingMount->dev && minode[i].refCount>0)
        {
            printf("File System: %s still in use\n",filesys);
            return -1;
        }
    }

    MINODE *mip = workingMount->mounted_inode;
    workingMount->dev=0;
    mip->mounted=0;
    mip->dirty=1;
    iput(mip);
    return 0;
}