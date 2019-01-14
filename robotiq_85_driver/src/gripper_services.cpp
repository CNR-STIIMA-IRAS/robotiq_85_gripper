#include <ros/ros.h>
#include <std_srvs/SetBool.h>
#include <robotiq_85_msgs/GripperCmd.h>
#include <robotiq_85_msgs/GripperStat.h>
// 

ros::Publisher pub;
double speed=0;
double force=0;

bool serverCallback(std_srvs::SetBool::Request& req, std_srvs::SetBool::Response& res)
{
  robotiq_85_msgs::GripperCmd msg;
  msg.speed=speed;
  msg.force=force;
  if (req.data)
    msg.position=0;
  else 
    msg.position=0.085;
  
  pub.publish(msg);
  res.success=true;
  return true;
};

int main(int argc, char **argv)
{
  ros::init(argc, argv, "robotiq_85_server_node");
  ros::NodeHandle nh;
  
  if (!nh.getParam("robotiq_85/speed",speed))
  {
    ROS_WARN("robotiq_85/speed is not set, using 0.1");
    speed=0.1;
    nh.setParam("robotiq_85/speed",speed);
  }
  else if (speed>0.1)
  {
    ROS_WARN("robotiq_85/speed has to be less than 0.1");
    speed=0.1;
  }
  else if (speed<0.013)
  {
    ROS_WARN("robotiq_85/speed has to be greater than 0.013");
    speed=0.013;
  }
    
  if (!nh.getParam("robotiq_85/force",force))
  {
    ROS_WARN("robotiq_85/force is not set, using 10");
    force=10;
    nh.setParam("robotiq_85/force",force);
  }
  else if (force>220)
  {
    ROS_WARN("robotiq_85/force has to be less than 220");
    force=10;
  }
  else if (force<5)
  {
    ROS_WARN("robotiq_85/force has to be greater than 5");
    force=5;
  }
  
    
  pub=nh.advertise<robotiq_85_msgs::GripperCmd>("/gripper/cmd",1);
  ROS_INFO("  ---- Robotiq 85mm 2finger server ----\n call '/gripper/grasp' to close (true)/open(false) the hand\nupdate parameters robotiq_85/force (range [5,220]) and robotiq_85/speed (range [0.013-0.1]) to change speed and force");
  ros::ServiceServer gripper_server=nh.advertiseService("/gripper/grasp",serverCallback);

  ros::AsyncSpinner sp(4);
  sp.start();
  
  ros::Rate lp(1);
  while (ros::ok())
  {
    double old_speed=speed;
    double old_force=force;
    if (!nh.getParam("robotiq_85/speed",speed))
    {
      ROS_WARN("robotiq_85/speed is not set, using 0.1");
      speed=0.1;
    }
    else if (speed>0.1)
    {
      ROS_WARN("robotiq_85/speed has to be less than 0.1");
      speed=0.1;
    }
    else if (speed<0.013)
    {
      ROS_WARN("robotiq_85/speed has to be greater than 0.013");
      speed=0.013;
    }
    
    if (!nh.getParam("robotiq_85/force",force))
    {
      ROS_WARN("robotiq_85/force is not set, using 10");
      force=10;
    }
    else if (force>220)
    {
      ROS_WARN("robotiq_85/force has to be less than 220");
      force=10;
    }
    else if (force<5)
    {
      ROS_WARN("robotiq_85/force has to be greater than 5");
      force=5;
    }
    
    lp.sleep();
    if ((old_force!=force) && (old_speed!=speed))
    {
      old_force=force;
      old_speed=speed;
      ROS_INFO("robotiq_85 new values: speed=%.3f force=%.3f",speed,force);
    }
    
  }
  return 0;
}
