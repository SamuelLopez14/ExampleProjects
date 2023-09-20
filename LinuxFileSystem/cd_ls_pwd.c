/************* cd_ls_pwd.c file **************/
int cd(char *PN)
{
  //printf("cd: under construction READ textbook!!!!\n");
  // READ Chapter 11.7.3 HOW TO chdir
  if(!strcmp(PN,""))
    {
      iput(running->cwd);
      running->cwd=iget(fd,root->ino);
      return 1;
      
      
    }
  int ino = getino(PN);
  if(ino==0)
    {
      printf("Pathname does not exist, CD failed\n");
      return 0;
    }
  MINODE *min = iget(dev,ino);
 
  if(S_ISDIR(min->INODE.i_mode)){
    iput(running->cwd);
    running->cwd = min;
    return 1;
  }else {
    printf("Not A Directory :(\n");
    iput (min);
    return 0;
  }
  
}

int ls_file(MINODE *mip, char *name)
{
 char *t1 = "xwrxwrxwr-------";
 char *t2 = "----------------";
 char ftime[256];
  // READ Chapter 11.7.3 HOW TO ls

 // printf("%d  %d\n",mip->dev,mip->ino);
 //printf("%x\n",mip->INODE.i_mode);
 //printf("%d\n",S_ISREG(mip->INODE.i_mode));

 if (S_ISREG(mip->INODE.i_mode)) // if (S_ISREG())
printf("%c",'-');
 if (S_ISDIR(mip->INODE.i_mode)) // if (S_ISDIR())
printf("%c",'d');
 if (S_ISLNK(mip->INODE.i_mode)) // if (S_ISLNK())
printf("%c",'l');
for (int i=8; i >= 0; i--){
if (mip->INODE.i_mode & (1 << i)) // print r|w|x
printf("%c", t1[i]);
else
printf("%c", t2[i]); // or print -
}
printf("%4d ",mip->INODE.i_links_count); // link count
printf("%4d ",mip->INODE.i_gid); // gid
printf("%4d ",mip->INODE.i_uid); // uid
printf("%8d ",mip->INODE.i_size); // file size
// print time
 const time_t tim = mip->INODE.i_ctime;
 strcpy(ftime,ctime(&tim)); // print time in calendar form
ftime[strlen(ftime)-1] = 0; // kill \n at end
printf("%s ",&ftime[4]);
// print name
 printf("%s", name); // print file basename
// print -> linkname if symbolic file
if ((mip->INODE.i_mode & 0xF000)== 0xA000){
// use readlink() to read linkname
printf(" -> %s", (char *)&mip->INODE.i_block[0]); // print linked name
}
 printf("\t[%d %d]",mip->dev,mip->ino);
printf("\n");
}

int ls_dir(MINODE *mip)
{
  //printf("ls_dir: list CWD's file names; YOU FINISH IT as ls -l\n");

  char buf[BLKSIZE], temp[256];
  DIR *dp;
  char *cp;
  MINODE *Min;

  get_block(dev, mip->INODE.i_block[0], buf);
  dp = (DIR *)buf;
  cp = buf;
  
  while (cp < buf + BLKSIZE){
     strncpy(temp, dp->name, dp->name_len);
     temp[dp->name_len] = 0;
     //printf("Getting inode with fd = %d, inum = %d\n",fd,dp->inode);
     Min = iget(dev,dp->inode);
     ls_file(Min,temp);
     iput(Min);

     cp += dp->rec_len;
     dp = (DIR *)cp;
  }
  printf("\n");
}

int ls(char *PN)
{
  MINODE *min;
  int ino;
  ino = getino(PN);
  printf("ino is %d\n",ino);
  min = iget(dev,ino);
  if(S_ISDIR(min->INODE.i_mode))
    {
      ls_dir(min);
    }else{
    ls_file(min,basename(PN));
  }
  iput(min);
}

void pwd(MINODE *wd)
{
  //printf("pwd: READ HOW TO pwd in textbook!!!!\n");
  if (wd == root){
    printf("/\n");
    return;
  }else {
    rpwd(wd);
    printf("\n");
  }
}

void rpwd(MINODE *wd){
int workingdev = wd->dev;
if (wd->ino==2&&wd!=root)
    {
      MINODE *temp = wd;
      MOUNT *tm = getmptr(wd->dev);
      wd = tm->mounted_inode;
      wd->refCount++;
      iput(temp);
      UpdateGlobals(wd->dev);
      
    }
  if(wd == root){
    return;
  }
  else{
    

    int j = findino(wd,wd->ino);
    MINODE *par = iget(dev,j);
    char nameholder[100];
    findmyname(par,wd->ino,nameholder);
    printf("myname is %s\n",nameholder);
    rpwd(par);
    //UpdateGlobals(workingdev);
    iput(par);
    printf("/%s",nameholder);
  }




}









void mystat(char *pathname)
{
  char tempname[64], ftime[256];
  int ino = getino(pathname);
  MINODE *mip = iget(running->cwd->dev, ino);
  strcpy(tempname, basename(pathname));
  ip = &mip->INODE;
  printf("\n");
  printf("File: %s\n", tempname);
  printf("Size: %8d\t Blocks:%8d\t IO Block: %8d\t", ip->i_size, ip->i_blocks, BLKSIZE);

  if(S_ISDIR(ip->i_mode))
  {
    printf("Directory\n");
  }

  else if(S_ISREG(ip->i_mode))
  {
    printf("Regular File\n");
  }

  else 
  {
    printf("It not either\n");
  }
  struct stat *thebuf;
  thebuf = malloc(sizeof(struct stat));
  mip->INODE.i_uid = thebuf->st_uid;
  mip->INODE.i_gid = thebuf->st_gid;
  //mip->INODE.i_uid = thebuf->st_uid;
  char temps[256];
  int acc = thebuf->st_dev;

  printf("Device: %d\t Inode: %d\t Links: %4d\n",acc, ino, ip->i_links_count);
  printf("Access: %d\t Uid: (%4d/ )\t Gid: (%4d/ )\n",thebuf->st_mode, mip->INODE.i_uid, mip->INODE.i_gid);

  // printf("Access: %s\n", thebuf->st_atime);
  // printf("Modify: %s\n", thebuf->st_mtime);
  // printf("Change: %s\n", thebuf->st_ctime);

  const time_t thetime = mip->INODE.i_ctime;
  strcpy(ftime, ctime(&thetime));
  ftime[strlen(ftime)-1] = 0;
  printf("Access: %s/n", &ftime[4]);
  const time_t anothertime = mip->INODE.i_mtime;
  memset(ftime, 0, 256);
  strcpy(ftime, ctime(&anothertime));
  ftime[strlen(ftime)-1] = 0;
  printf("Modify: %s\n", &ftime[4]);
  const time_t lasttime = mip->INODE.i_atime;
  memset(ftime, 0, 256);
  strcpy(ftime, ctime(&lasttime));
  ftime[strlen(ftime)-1] = 0;
  printf("Change: %s\n", &ftime[4]);
  printf("Birth: - \n");

  free(thebuf);
  mip->dirty = 1;
  iput(mip);

}










