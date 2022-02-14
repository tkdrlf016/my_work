#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <termios.h>
#include <fcntl.h>

int main( void)
{
   int fd;                                                     // (1) 시리얼 통신의 파일 디스크립터 
   struct termios newtio;
   
   fd = open( "/dev/ttyS10", O_RDWR | O_NOCTTY | O_NONBLOCK );  // (2) com1을 open?
   
   memset( &newtio, 0, sizeof(newtio) );
   newtio.c_cflag = B9600;                                   // (3) 통신 속도 115200 
   newtio.c_cflag |= CS8;                                      // (4) 데이터 비트가 8bit 
   newtio.c_cflag |= CLOCAL | CREAD;                           // (5) 쓰기는 기본, 읽기도 가능하게 
   newtio.c_iflag = 0;                                         // (6) 패리티 사용 안함 
   newtio.c_oflag = 0;
   newtio.c_lflag = 0;
   newtio.c_cc[VTIME] = 0; 
   newtio.c_cc[VMIN] = 1; 
   
   tcflush (fd, TCIFLUSH );
   tcsetattr(fd, TCSANOW, &newtio );
   
   write( fd, "forum.falinux.com", 17);                        // (7) 시리얼로 17자의 문자열을 전송 
   
   close( fd);                                                 // (8) 통신 포트 사용 중지 
   
   return 0; 
}