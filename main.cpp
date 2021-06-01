//! @file
//! @brief	main executable



#include <unistd.h>
#include <inttypes.h>

#include <iostream>
#include <string>
#include <sstream>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <vector>
#include <cmath>
#include <fstream>

using std::cout;
using std::endl;
using std::string;

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }

    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();

    }
    return result;
}

double g =  9.80665;
double pi = 3.14159;
std::ifstream imuAccFile;
std::ifstream imuGyroFile;




long stepCount =0;
long periodNs = 10000000;
long timePassedNs =0;

double directionRad=0;
double targetDirRad=1;
double angularAccRadSec2 = 1.0;//given
double angularVelRadSec = 0.1;


void simStep(){
    stepCount++;
    timePassedNs+=periodNs;

directionRad+=(angularVelRadSec*periodNs/1000000000.0);
        //  double deltaYaw = motorControl->odometryFromControl->pose.calcYawToPoint(targetPos);
        double deltaYaw = targetDirRad-directionRad;
        deltaYaw = std::remainder(deltaYaw,2*M_PI); // normalize to -pi;pi
        //determine the sign of angular acceleration
        double angAccToZero = angularVelRadSec*angularVelRadSec/(2*std::abs(deltaYaw));
        double angAccSign = 1;
        if(angAccToZero > angularVelRadSec){// negative angular acceleration to increase turning radius
            angAccSign = -1;
        }
        double   angVelDelta = periodNs/1000000000.0*angularAccRadSec2*angAccSign;
       // std::cout<<"delta Yaw "<<deltaYaw<<" angVelDelta "<< angVelDelta<<"\n";

        if(std::abs(angularVelRadSec)<0.000000000000001)angularVelRadSec = 0.000000000000001; // avoid possible div/zero
       angularVelRadSec += angVelDelta;


    }

    int main(int argc, char *argv[])
    {


      while(stepCount<200){
      simStep();


        std::cout<<"t_ns "<<timePassedNs<<" "<<angularVelRadSec<<" \n";

}


    }
