#include <iostream>
#include <vector>
using namespace std;


typedef struct tagPOINT
{
     int x;
     int y;
}POINT, *PPOINT;


bool isinner(POINT _CurrentPos, vector<POINT> _vVertex)
{
     int iSize = _vVertex.size( );
     int iCount = 0;
     int flag = 0;
     for(int i = 0; i < iSize; ++i)
     {
          flag = 1;
          int j = ( i + 1 ) % iSize;
          //cout << _vVertex[ i ].x  <<endl;
          //cout << _vVertex[ i ].y  <<endl;
          bool is_true = _CurrentPos.y > _vVertex[ i ].y  != _CurrentPos.y > _vVertex[ j ].y;
          if((_CurrentPos.y == _vVertex[i].y))
          {
               cout << "ok" << endl;
               ++iCount;
               flag = 0;
          }
           if( is_true )
          {
                 int S[3] = {0,};
                 S[0] = _vVertex[ j ].x - _vVertex[ i ].x;
                 S[1] = _vVertex[ j ].y - _vVertex[ i ].y;
                 S[2] = _CurrentPos.y - _vVertex[ i ].y;


               float itsPosX = S[0] * S[2] / S[1] + _vVertex[ i ].x;
                 if( (_CurrentPos.x < itsPosX)&& flag  )
                      ++iCount;
          }    
     }
     return (iCount % 2) != 0;
} 

int main( )
{    
     const int POS_MAX = 5;
     vector<POINT> vV;
     POINT pt[POS_MAX] = { {3,5}, {5, 10}, {6, 5}, {8, 1} ,{8,10}};

     for (int i = 0; i < POS_MAX; ++i)
          vV.push_back(pt[i]);

     POINT ptPos = { 5,5 };

     if(isinner(ptPos, vV))
          cout << "내부" << endl;
     else
          cout << "외부" << endl;

}