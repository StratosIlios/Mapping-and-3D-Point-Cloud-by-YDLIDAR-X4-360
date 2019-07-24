#include <ros/ros.h>
#include <laser_assembler/AssembleScans.h>
//#include <laser_assembler/AssembleScans2.h>
#include <tf/transform_listener.h>
#include <sensor_msgs/point_cloud_conversion.h>
#include <pcl_conversions/pcl_conversions.h>

int main (int argc, char **argv)
{
 /** Initialization of service caller node and create object n **/	
 ros::init(argc,argv,"pcl_3d");
 ros::NodeHandle n;
 /** Waiting for service to be advertised and availlable ***/
 ros::service::waitForService("assemble_scans"); 
  
 /** Calling the Service with the Handle way. The n is the usual ros::NodeHandle object. 
     We are calling its service-Client method. The laser_assembler::AssembleScans is the name 
     of the service object. The "assemble_scans" is a string naming the service that we want 
     to call. **/
 ros::ServiceClient client=n.serviceClient<laser_assembler::AssembleScans>("assemble_scans");
		
 /** Create the publisher and the topic for PCL **/	
 ros::Publisher point_cloud_publisher_;	
 point_cloud_publisher_=n.advertise<sensor_msgs::PointCloud>("/cloud", 100, false);

 /** Create the topic for PCL2 **/
 /*ros::Publisher point_cloud2_publisher_;
 point_cloud2_publisher_=n.advertise<sensor_msgs::PointCloud2>("/cloud2", 100, false);**/

 /** Rate allow to specify a frequency that we want to loop at. Here is 10Hz **/
 ros::Rate loop_rate(10);
 //sensor_msgs::PointCloud2 point_cloud2;
	
 while(ros::ok())
 {
  /** Make the call to get a point cloud **/
  laser_assembler::AssembleScans srv;	
  srv.request.begin=ros::Time(0,0);
  srv.request.end=ros::Time::now();
  if (client.call(srv))
  {
   printf("Got cloud with %u points\n", srv.response.cloud.points.size());
   if (srv.response.cloud.points.size()>0)
   {
    /** Publishes the response of the service for PCL **/				
    point_cloud_publisher_.publish(srv.response.cloud);
    //sensor_msgs::convertPointCloudToPointCloud2(srv.response.cloud, point_cloud2);				
    /** Publishes the response of the service for PCL2 **/
    //point_cloud2_publisher_.publish(point_cloud2);
   } 				
  }
  else
  {
   printf("Service call failed\n");
   loop_rate.sleep();
  }
 }
}
