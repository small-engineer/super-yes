#include<unistd.h>
#include<string.h>
#include<stdlib.h>

int main(int ac,char**av){
 if(ac<2)return 1;

 char *s=av[1],*bp,*tp;
 size_t ln=strlen(s),i=0,j,sz=ln*8;
 int *tbl,*ip,fd[2];

 ip=malloc(ln*sizeof(int));
 tbl=ip?ip:(exit(1),(int*)0);
 for(j=0;j<ln;j++)tbl[j]=s[j]-'y';

 tp=malloc(sz);
 bp=tp?tp:(exit(1),(char*)0);

 pipe(fd);
 if(!fork())dup2(fd[1],1),execlp("yes","yes",0),_exit(1);
 close(fd[1]);

 for(;;){
  ssize_t r=read(fd[0],bp,sz);
  if(r<=0)break;
  for(j=0;j<r;j++)bp[j]=(bp[j]=='\n')?'\n':bp[j]+tbl[i],i=(i+1)%ln;
  write(1,bp,r);
 }

 free(tbl);
 free(bp);
}

