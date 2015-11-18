#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

#define S_US_ID 0x4000   //set-user-ID bit
#define S_GR_ID 0x2000   //set-group-ID bit 
#define S_STX_BIT 0x1000 //sticky bit

#define S_USR_RD 0x400   //owner has read permission
#define S_USR_WR 0x200   //owner has write permission
#define S_USR_EX 0x100   //owner has execute permission

#define S_GR_RD 0x040   //group has read permission
#define S_GR_WR 0x020   //group has write permission
#define S_GR_EX 0x010   //group has execute permission
           
#define S_OTH_RD 0x004   //others have read permission
#define S_OTH_WR 0x002   //others have write permission
#define S_OTH_EX 0x001   //others have execute permission


int main(int argc, const char *argv[])
{
	struct stat stats;
	// init printable_mode
        unsigned int printable_mode = 0; 
	
	stat(argv[1], &stats);   //store file info to stat structure
	
	switch (stats.st_mode & S_IFMT) {
           case S_IFCHR:  write(STDOUT_FILENO, "\n Is character special file\n", 29); 
						  printable_mode |= S_IFCHR;
						  break;
           case S_IFDIR:  write(STDOUT_FILENO, "\n Is directory file\n", 21); 
						  printable_mode |= S_IFDIR;
						  break;						
		   case S_IFREG:  write(STDOUT_FILENO, "\n Is regular file\n", 19); 
						  printable_mode |= S_IFREG;
						  break;
           default:       write(STDOUT_FILENO, "\nunknown\n", 10);  break;
    }
	
    
		
        if( stats.st_mode & S_ISUID )
        printable_mode |= S_US_ID;

        if( stats.st_mode & S_ISGID )
        printable_mode |= S_GR_ID;

        if( stats.st_mode & S_ISVTX )
        printable_mode |= S_STX_BIT;

        if( stats.st_mode & S_IRUSR )
        printable_mode |= S_USR_RD;

        if( stats.st_mode & S_IWUSR )
        printable_mode |= S_USR_WR;

        if( stats.st_mode & S_IXUSR )
        printable_mode |= S_USR_EX;

        if( stats.st_mode & S_IRGRP )
        printable_mode |= S_GR_RD;

        if( stats.st_mode & S_IWGRP )
        printable_mode |= S_GR_WR;

        if( stats.st_mode & S_IXGRP )
        printable_mode |= S_GR_EX ;

        if( stats.st_mode & S_IROTH )
        printable_mode |= S_OTH_RD;

        if( stats.st_mode & S_IWOTH )
        printable_mode |= S_OTH_WR;

        if( stats.st_mode & S_IXOTH )
        printable_mode |= S_OTH_EX;
	
	printf("%04x\n", printable_mode);
}
