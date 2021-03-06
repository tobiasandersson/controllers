#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include "kinematic_controllers/controller_base.h"
#include "kinematic_controllers/turn_controller.h"
#include "kinematic_controllers/turn_controller_theta.h"
#include "kinematic_controllers/forward_controller.h"
#include "kinematic_controllers/wall_following_controller.h"


//------------------------------------------------------------------------------
// Member

geometry_msgs::Twist _twist;

//------------------------------------------------------------------------------
// Entry point

int main(int argc, char **argv)
{
    ros::init(argc, argv, "controller_adapter");

    ros::NodeHandle nh;
    ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("/motor_controller/twist", 10);
    ros::Rate loop_rate(robot::prop::encoder_publish_frequency);

    ControllerBase* controllers[] = {
        new ForwardController(nh, robot::prop::encoder_publish_frequency),
        //new TurnController(nh, robot::prop::encoder_publish_frequency),
        new TurnControllerTheta(nh, robot::prop::encoder_publish_frequency),
        new WallFollowingController(nh, robot::prop::encoder_publish_frequency)
    };
    int nControllers = sizeof(controllers)/sizeof(ControllerBase*);


    while(ros::ok()) {

        _twist.linear.x = 0;
        _twist.linear.y = 0;
        _twist.linear.z = 0;
        _twist.angular.x = 0;
        _twist.angular.y = 0;
        _twist.angular.z = 0;

        for(int i = 0; i < nControllers; ++i) {

            const geometry_msgs::TwistConstPtr twist = controllers[i]->update();

            //combine the twists blindly

            _twist.angular.z += twist->angular.z;
            _twist.linear.x += twist->linear.x;
        }

        pub.publish(_twist);
        ros::spinOnce();
        loop_rate.sleep();

    }

    //delete all controllers
    for(int i = 0; i < nControllers; ++i) {
        delete controllers[i];
    }

    return 0;
}
