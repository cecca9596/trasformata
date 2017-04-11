#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "std_msgs/String.h"
#include <tf/transform_listener.h>
#include <time.h>
ros::Publisher share;
tf::TransformListener* listener;
void call_back(const sensor_msgs::LaserScan::ConstPtr& msg){
	ros::Time ora=ros::Time::now();
	tf::StampedTransform trasformata;
	
	if(listener->canTransform("/base_laser_link","/odom",ora,NULL)){
		listener->lookupTransform("/base_laser_link","/odom",ora,trasformata);
		tf::Vector3 vettore= trasformata.getOrigin();
		float x=vettore.getX();
		float y=vettore.getY();
		tf::Quaternion rotazione=trasformata.getRotation();
		float angolo=rotazione.getAngle();
		std_msgs::String messaggio;
		std::stringstream ss;
		ss <<"coordinate: x :" << x;
		ss << "y : "<< y;
		ss << "angolo : "<< angolo;
		messaggio.data=ss.str();
		share.publish(messaggio);
	}
}

int main(int argc,char** argv){
	ros::init(argc,argv,"trasformata");
	tf::TransformListener list1;
	listener=&list1;
	ros::NodeHandle l;
	ros::Subscriber sub=l.subscribe("base_scan",1000,call_back);
	share = l.advertise<std_msgs::String>("transform", 1000);
	ros::spin();
	
	return 0;
}
