//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <endian.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <dirent.h>

unsigned char poi[]={0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x35,0x31,0x32,0x2d,0x30,0x38,0x32,0x31,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x42,0x54,0x4a,0x00,0x7c,0x33,0x0e,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};

char *replace(const char *s, char ch, const char *repl) {
    int count = 0;
    const char *t;
    for(t=s; *t; t++)
        count += (*t == ch);

    size_t rlen = strlen(repl);
    char *res = malloc(strlen(s) + (rlen-1)*count + 1);
    char *ptr = res;
    for(t=s; *t; t++) {
        if(*t == ch) {
            memcpy(ptr, repl, rlen);
            ptr += rlen;
        } else {
            *ptr++ = *t;
        }
    }
    *ptr = 0;
    return res;
}
int kommapos=0;
unsigned int error=0;
int getCoord(int argc,char* argv[],char* koord,int i)
{
      int t;
      char x[100];
      if (strlen(argv[i])==2)
	    {
	      if (argc>(i+1)) 
	      {
	        strncpy(koord,argv[i+1],8);
	      }
	    } else {
	      strncpy(koord,&argv[i][2],8);
	    }
	    
	    
	    strcpy(koord,replace(koord,',',"."));
	    for (t=0;t<strlen(koord);t++)
	    {
	      if (koord[t]=='.') return 0; 
	    }
      error=error|1;
	    return 1;
}
int setFilename(int argc,char* argv[],char* name,int i)
{
     
      if (strlen(argv[i])==2)
	    {
	      if (argc>(i+1)) 
	      {
	        if (argv[i+1][0]!='-') strncpy(name,argv[i+1],255);
	        else error=error|4;
	      }
	    } else {
	      if (argv[i][2]!='-') strncpy(name,&argv[i][2],255);
	      else error=error|4;
	    }
}
uint32_t  convertString(char *Koord)
{
  char x[1000];
  int i=0,t,m=0,r=0;
  uint32_t value=0;

  for (t=0;t<strlen(Koord);t++)
	{
      m++;
      if (i==1) r++;
	    if (Koord[t]!='.')  value=value*10+(Koord[t]-'0');
      if (Koord[t]=='.')  {m=0;i=1;}
      if (r==5) break;
	}  
  if (i==0) {error=error|2; return -1;}
  for (i=m;i<5;i++) value=value*10;
  return value;
}
int main(int argc, char* argv[])
{

  FILE *f;
  int i,t,error=0;
  int8_t max=-1,symb=0;
  
  int32_t Nk=0,Sk=0,Ek=0,Wk=0;
  int32_t Ns=0,Ss=0,Es=0,Ws=0;

  char N[100];
  char S[100];
  
  char E[100];
  char W[100];
  char filename[255]="\0";
  char filename2[255]="\0";
  char gdate[255]="0000\0";
  char gtime[255]="0000\0";
  char symbol[255]="0\0";
  
  for (i=0;i<100;i++)
  {
    N[i]=0;
    S[i]=0;
    E[i]=0;
    W[i]=0;
  }
  //printf("argc: %d\n", argc);	
	for(i=0; i < argc; i++) 
	{
		//printf("argv[%d]: %s\n", i, argv[i]);
	  if (strncmp(argv[i],"-N",2)==0)
	  {
	    getCoord(argc,argv,N,i);
      Nk=convertString(N);
      Ns=1;
#ifdef debug 
	    printf("%d\n",Nk);
#endif
	  } 	
	  if (strncmp(argv[i],"-S",2)==0)
	  {
	    getCoord(argc,argv,S,i);
      Sk=convertString(S);
      Sk=-Sk;
      Ss=1;
#ifdef debug 
	    printf("%d\n",Sk);
#endif	    
	  }
	

	  if (strncmp(argv[i],"-E",2)==0)
	  {
	    getCoord(argc,argv,E,i);
      Ek=convertString(E);
      Es=1;
#ifdef debug       
	    printf("%d\n",Ek);
#endif
	  } 
	
	  if (strncmp(argv[i],"-W",2)==0)
	  {
	    getCoord(argc,argv,W,i);
      Wk=convertString(W);
      Wk=-Wk;
      Ws=1;
#ifdef debug       
	    printf("%d\n",Wk);
#endif
	  }
	  if (strncmp(argv[i],"-o",2)==0)
	  {
	    setFilename(argc,argv,filename,i);
	    
	    if (!strcmp(filename,"auto"))
	    {
	      int run=0;
	      DIR *dirHandle;
        struct dirent * dirEntry;

        dirHandle = opendir(".");
        if (dirHandle) 
        {
          while (0 != (dirEntry = readdir(dirHandle))) 
          {
             if (strlen(dirEntry->d_name)==3)
             {
#ifdef debug              
                printf("%d) %s\n", run, dirEntry->d_name);
#endif
                if (atoi(dirEntry->d_name)>max) max=atoi(dirEntry->d_name);
                run++;
             }
          }
          closedir(dirHandle);
setmax:
          if (max>125) error=error|8;
          else 
          {
            max++;
            strcpy(filename2,"");
            if (max<10) strcat(filename2,"00");
            else
            if (max<100) strcat(filename2,"0");
            
#ifdef debug  
            printf(" filename 2 = %s     max = %d\n\n",filename2,max);
#endif
            sprintf(filename,"%s%d",filename2,max);  
          }
        }
	    } else { //if filename is not auto
        if (strlen(filename)>3) error=error|4;
        for (t=0;t<strlen(filename);t++) 
        {
          if ((filename[t]<48) || (filename[t]>57)) error=error|4;
        }
        if (error==0) {max=atoi(filename)-1;goto setmax;}
      }
	  }
	
		if (strncmp(argv[i],"-d",2)==0)
	  {
	    setFilename(argc,argv,gdate,i);
      if (strlen(gdate)!=4) error=error|16;
      else
      for (t=0;t<strlen(gdate);t++) 
      {
        if ((gdate[t]<48) || (gdate[t]>57)) error=error|16;
      }
#ifdef debug 
	    printf("gdate = %s\n",gdate);
#endif	    
	  }
	  
	  if (strncmp(argv[i],"-t",2)==0)
	  {
	    setFilename(argc,argv,gtime,i);
      if (strlen(gtime)!=4) error=error|16;
      else
      for (t=0;t<strlen(gtime);t++) 
      {
        if ((gtime[t]<48) || (gtime[t]>57)) error=error|16;
      }
#ifdef debug 
	    printf("gtime = %s\n",gtime);
#endif	    
	  }
	  
	  if (strncmp(argv[i],"-s",2)==0)
	  {
	    setFilename(argc,argv,symbol,i);
      
      
      if (strcmp(symbol,"star")==0) symb=0; else
      if (strcmp(symbol,"house")==0) symb=1;else
      if (strcmp(symbol,"flag")==0) symb=2;else
      if (strcmp(symbol,"car")==0) symb=3;else
      if (strcmp(symbol,"eat")==0) symb=4;else
      if (strcmp(symbol,"bus")==0) symb=5;else
      if (strcmp(symbol,"gas")==0) symb=6;else
      if (strcmp(symbol,"skyscraper")==0) symb=7;else
      if (strcmp(symbol,"plane")==0) symb=8;else 
      if (strlen(symbol)==1) symb=atoi(symbol); else 
      error=error|32;
      if (symb > 8) error=error|32;
     
     
#ifdef debug 
	    printf("symb = %d\n",symb;
#endif	    
	  }
	  //read coordinates from POI file and print them
	  if (strncmp(argv[i],"-r",2)==0)
	  {
	    setFilename(argc,argv,filename,i);
      
      f=fopen(filename,"r");
      if (f==NULL) error=error | 64;
      else {
        int32_t N=0,NNN=0;
        int32_t E=0,EEE=0;
        int zzu;
        for (zzu=0;zzu<128;zzu++) poi[zzu]=0;
        fgets(poi, 128, f);
        fclose(f);
        N = le32toh(*((int32_t  *) &poi[76]));
        
        E = le32toh(*((int32_t *) &poi[80]));
        if (N==0) error=error|64;
        if (E==0) error=error|64;
        if (error&64) goto error_handler;
        NNN=-N;
        EEE=-E;
        printf("\n");
        if (N<0) printf("S%f   ",(double)NNN/100000);
        else printf("N%f ",(double)N/100000);
        if (E<0) printf("W%f ",(double)EEE/100000);
        else printf("E%f ",(double)E/100000);
        printf("\n");printf("\n");
#ifdef debug         
        printf("coordinates integer values adr 76: %f     adr 80: %f\n\n",(double)N/100000,(double)E/100000);
#endif
        goto error_handler;
        
      }
     
#ifdef debug 
	    printf("gtime = %s\n",gtime);
#endif	    
	  }
	
	}
  if (Ns==1)       * ((int32_t*) &poi[76]) = htole32(Nk);
  else if (Ss==1)  * ((int32_t*) &poi[76]) = htole32(Sk);
  if (Es==1)       * ((int32_t*) &poi[80]) = htole32(Ek);
  else if (Ws==1)  * ((int32_t*) &poi[80]) = htole32(Wk);
  if (filename[0]=='\0') error=error | 4;
  if ((max>=0) &&  (max<126)) * ((int8_t*) &poi[4]) = max;
  if (strlen(gdate)==4)
  {
    for (t=0;t<4;t++) 
    { 
      * ((char *) &poi[12+t]) = (char )gdate[t];
    }
  }
  if (strlen(gtime)==4)
  {
    for (t=0;t<4;t++) 
    {
      * ((char *) &poi[17+t]) = (char )gtime[t];
    }
  }
  
  * ((int8_t*) &poi[16])='-';
  * ((int8_t*) &poi[1])=symb;
  
#ifdef debug 
  printf("\n\n error : %d \n\n",error);
  printf("\n\n max : %d \n\n",max);
#endif
  if (error==0) 
  {
    printf("\nwriting filename %s \n \n",filename);
    f=fopen(filename, "w");
    for (i=0;i<128;i++)
    {
      fputc(poi[i],f);
    }
    fclose(f);
  }
error_handler:
  if (error&1)  printf("\n Error, Wrong Coordinate Format.\n\n"); 
  if (error&2)  printf("\n Error, Wrong Coordinate Format.\n\n"); 
  if (error&4)  printf("\n Wrong Filename or Filename missing, use -o to set the same.\n\n"); 
  if (error&8)  printf("\n Auto option cannot write file, because of number conflicts.\n\n"); 
  if (error&16) printf("\n Option for time or date -t -d format wrong.\n\n"); 
  if (error&32) printf("\n Option for POI symbol -s wrong. 0-8 is allowed only.\n\n"); 
  if (error&64) printf("\n Reading filename with -r caused an error. Maybe file or path does not exist or file is too short.\n\n"); 

  return 0;
}
