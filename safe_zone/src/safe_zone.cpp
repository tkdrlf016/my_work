#include <iostream>
#include <vector>
#include <ros/ros.h>
#include <std_msgs/Int32.h>
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

int main(int argc, char **argv)
{    
     ros::init(argc, argv, "safe_zone");
     ros::NodeHandle nh; 
     std_msgs::Int32 safe_zone_msg;
     ros::Publisher ros_tutorial_pub = nh.advertise<std_msgs::Int32>("safe_zone_msg", 100);
     ros::Rate loop_rate(10);
     const int POS_MAX = 5;
     vector<POINT> vV;
     POINT pt[POS_MAX] = { {3,5}, {5, 10}, {6, 5}, {8, 1} ,{8,10}};

     for (int i = 0; i < POS_MAX; ++i)
          vV.push_back(pt[i]);

     POINT ptPos = { 5,5 };
     while(ros::ok())
     {
          if(isinner(ptPos, vV)){
               safe_zone_msg.data = 1;
               ros_tutorial_pub.publish(safe_zone_msg);
               cout << "내부" << endl;
          }
          else{
               safe_zone_msg.data = 0;
               ros_tutorial_pub.publish(safe_zone_msg);
               cout << "외부" << endl;
          }
          loop_rate.sleep(); 
     }
}