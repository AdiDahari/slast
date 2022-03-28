#include <utmp.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define ARGS 2

int main(int argc, char *argv[]){
    if(ARGS != argc){
        perror("Invalid Number of Argumnets!");
        exit(1);
    }

    struct utmp line;
    int size = sizeof(line);
    int count = 0;
    int fd;
    if((fd = open(WTMP_FILE, O_RDONLY)) == -1){
        perror("Failed to open wtmp file");
        exit(1);
    }
    while(read(fd, &line, size) == size){
        if(line.ut_type != DEAD_PROCESS && line.ut_type != RUN_LVL) ++count; 
    }

    lseek(fd, 0, SEEK_SET);
    struct utmp lines[count];
    int inv = 1;
    while(read(fd, &line, size) == size){
        if(line.ut_type != DEAD_PROCESS && line.ut_type != RUN_LVL){
            lines[count - inv] = line;
            ++inv;
        }
    }

    for(int i = 0; i < count && i < atoi(argv[1]); ++i){
        printf("%s\t", lines[i].ut_user);
        if(lines[i].ut_type == BOOT_TIME) {
            printf("\tsystem boot\t");
            printf("%s\t", lines[i].ut_host);
        }
        else{
            printf("%s\t\t", lines[i].ut_line);
            printf("%s\t\t\t", lines[i].ut_host);
        }
        long time = lines[i].ut_tv.tv_sec;
        
        printf("%.20s\n", ctime(&time));

    }
    close(fd);
    return 0;

}