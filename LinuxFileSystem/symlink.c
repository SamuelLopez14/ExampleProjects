void mysymlink(char * pathname, char* pathname2)
{
  if(!strcmp(pathname,"")||!strcmp(pathname2,""))
  {
    printf("Pathname given is empty\n");
    return;
  }
  char newdir[128],newname[128],*newd,*newf;
  printf("symlink activated\n");
  printf("%s %s\n",pathname,pathname2);
  int oino = getino(pathname);
  if(oino ==0)
  {
    printf("%s doesn not exist\n",pathname);
    return;
  }

  int ino = getino(pathname2);
  if(ino != 0)
  {
    printf("%s already exists\n",pathname2);
    return;
  }

  strcpy(newdir,pathname2);
  newd = dirname(newdir);
  strcpy(newname,pathname2);
  newf = basename(newname);

  printf("new file dir: %s new file name %s\n",newd,newf);
  int pino = getino(newd);
  if(!pino)
  {
    printf("%s does not exist\n",newd);
    return;
  }
  MINODE *pmip= iget(dev,pino);
  if(!S_ISDIR(pmip->INODE.i_mode))
  {
    printf("%s is not a dir\n",newd);
    iput(pmip);
    return;
  }
  ino = ialloc(dev);
  MINODE *newNode = iget(dev,ino);

  INODE *ip = &newNode->INODE;
ip->i_mode = 0xA1FF; 
ip->i_uid = running->uid; // owner uid
ip->i_gid = running->gid; // group Id
ip->i_size = strlen(pathname); // size in bytes
ip->i_links_count = 1; 
ip->i_atime = ip->i_ctime = ip->i_mtime = time(0L);
ip->i_blocks = 0;
strcpy((char *)&ip->i_block[0],pathname);
enter_name(pmip,ino,newf);
pmip->dirty =1;
newNode->dirty =1;
iput(pmip);
iput(newNode);


}

int readlink(char *file, char buffer[])
{
  if(!strcmp(file,""))
  {
    printf("No file given\n");
    return 0;
  }
  int ino = getino(file);
  if(!ino)
  {
    printf("%s doesn not exist\n",file);
    return 0;
  }
  MINODE *mip = iget(dev,ino);
  if(!S_ISLNK(mip->INODE.i_mode))
  {
    printf("%s is not link type\n",file);
    iput(mip);
    return 0;
  }
  int len = mip->INODE.i_size;
  strcpy(buffer,(char *)&mip->INODE.i_block[0]);
  return len;
  iput(mip);
}