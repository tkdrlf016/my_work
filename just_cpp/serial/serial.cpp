#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>

#include <string.h>

#include <fcntl.h>

#include <termios.h>

#include <sys/ioctl.h>

//------------------------------------------------------------------------

// 설 명 : 어플리케이션 처리 

// 매 계 : 명령행 매계변수

// 반 환 : 없음

// 주 의 : 

//------------------------------------------------------------------------

int main( int argc, char **argv )

{

    int     handle;

    struct  termios  oldtio,newtio;

    

    char    TitleMessage[30] = "Welcome Serial Port\r\n";

    char    Buff[256];

    int     RxCount;

    int     loop;

    int     ending;

    

            

    // 화일을 연다.

    handle = open( "/dev/ttyS10", O_RDWR | O_NOCTTY );

    if( handle < 0 ) 

    {

        //화일 열기 실패

        printf( "Serial Open Fail [/dev/ttyS3]\r\n "  );

        exit(0);

    }

    

    tcgetattr( handle, &oldtio );  // 현재 설정을 oldtio에 저장

    memset( &newtio, 0, sizeof(newtio) );

    newtio.c_cflag = B9600 | CS8 | CLOCAL | CREAD ; 

    newtio.c_iflag = IGNPAR;

    newtio.c_oflag = 0;

    //set input mode (non-canonical, no echo,.....)

    newtio.c_lflag = 0;

    

    newtio.c_cc[VTIME] = 30;    // time-out 값으로 사용된다. time-out 값은 TIME*0.1초 이다.

    newtio.c_cc[VMIN]  = 0;     // MIN은 read가 리턴되기 위한 최소한의 문자 개수

    

    tcflush( handle, TCIFLUSH );

    tcsetattr( handle, TCSANOW, &newtio );

    

    // 타이틀 메세지를 표출한다. 

    write( handle, TitleMessage, strlen( TitleMessage ) );

    

    // 1 문자씩 받아서 되돌린다. 

    ending = 0;

    while(!ending)

    {

        // 한 문자를 수신한다.

        RxCount = read( handle, Buff, 1 );

        if( RxCount == 0 ) 

        {

                printf( "Receive Time Over\n" );

                continue;

        }        

        if( RxCount < 0 ) 

        {

                printf( "Read Error\n" );

                break;

        }

        for( loop = 0; loop < RxCount; loop++ )

        {

            printf( "Read Data [%c]\n", Buff[loop] );

            // ESC 키 문자가 들어오면 중지한다. 

            if( Buff[loop] == 0x1b ) ending = 1;

        }   

        

        // 수신된 문자를 되돌린다.

        write( handle, Buff, RxCount );

        

    }

    

    tcsetattr( handle, TCSANOW, &oldtio ); // 이전 상태로 되돌린다. 

    

    close( handle );   // 화일을 닫는다.

    

    return 0;    

}

