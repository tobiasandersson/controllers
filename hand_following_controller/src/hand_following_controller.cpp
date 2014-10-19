#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "geometry_msgs/Vector3.h"
#include "std_msgs/Float64.h"

double distance;

/**
  * Class that will handle the distance read by the camera
 
class Distance {
public:
	static double value;
};

void Distance::setValue(double newValue)
{
    double Distance::value = newValue;
}

double Distance::getvalue()
{
	double Distance::value;
    return value;
}*/




/**
 * Function that will be executed when the value from the depth
 * image of the camera has been read
 */
void cameraCallback(const std_msgs::Float64 value)
{
	distance = value.data;
}


int main(int argc, char **argv)
{
  /*
   * Init the node
   */
  ros::init(argc, argv, "line_cartesian_controller");

  ros::NodeHandle n;

  /*
   * Publish to the topic "/motor_controller/twist"
   */
  ros::Publisher pub = n.advertise<geometry_msgs::Twist>("/motor_controller/twist", 1000);
  /*
   * Subscribe to the topic "/object_distance"
   */
  ros::Subscriber sub = n.subscribe("/object_distance",1000,cameraCallback);

  ros::Rate loop_rate(10);

  /*
   * Message to send
   */
  geometry_msgs::Twist twist;
  distance = 0;

  twist.linear.x = 0;  //linear velocity
  twist.linear.y = 0;
  twist.linear.z = 0;
  twist.angular.x = 0;
  twist.angular.y = 0;
  twist.angular.z = 0;

  /*
   * A count of how many messages we have sent. This is used to create
   * a unique string for each message.
   */
  float count = 0;
  while (ros::ok())
  {
    if (distance > 0.3)
    {
        twist.linear.x = 0.1;
    } else
    {
        twist.linear.x = 0;
    }

    pub.publish(twist);
    ros::spinOnce();
    loop_rate.sleep();
    ++count;
  }

  return 0;
}