#! /usr/bin/python

import rospy
from laser_assembler.srv import AssembleScans
from sensor_msgs.msg import PointCloud


rospy.init_node("pcl_3d")
#blocks until service is available
rospy.wait_for_service("assemble_scans")
#create a handle for calling the service. A callable proxy to a service
assemble_scans=rospy.ServiceProxy("assemble_scans",AssembleScans)
#Publisher and topic, (topic name, message type, limits the amount of queued messages)
point_cloud_publisher_=rospy.Publisher("/cloud", PointCloud, queue_size=1)


#Creates a Rate object rate, 10Hz
rate=rospy.Rate(10)

while(True):
	try:
		#Call and response of the service. Time interval(Time instance, get current time)
		response=assemble_scans(rospy.Time(0,0),rospy.get_rostime())
		point_cloud_publisher_.publish(response.cloud)
	except rospy.ServiceException, e:
		   print "Service is failed: %s" %e

	rate.sleep()
