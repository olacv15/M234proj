// K. M. Knausgård / MAS234 2017
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include <linux/can.h>
#include <linux/can/raw.h>

#include <iostream>
#include <time.h>


// Can bus configuration.
const char *ifname = "can0";
int canSocketDescriptor;

// Task rate control.
const double periodicTaskRate = 100.0; // Run task at 100 Hz
const int64_t periodicTaskDtNs = static_cast<int64_t>((1.0/(periodicTaskRate*2)) * 1.0e9); //convert to nanoseconds
bool taskToggle = false;

// Functions
bool createCanSocket(int& socketDescriptor);
void sendCanMessage(int socketDescriptor, can_frame frame);
void reciveCanMessage(int socketDescriptor);
void calculateAcc();


// Global structs
struct rawACCL
{
    int16_t rawAcclZ;
};

struct ACCL
{
    double acclZ;
};


rawACCL r_mpu6050;
ACCL mpu6050;




void myPeriodicTask1()
{

    struct can_frame frame;
    frame.can_id  = 0x30;
    frame.can_dlc = 8; // data length code: 0 .. 8

    calculateAcc(); // calculate output from previous input

    memcpy(frame.data, &mpu6050, sizeof(mpu6050)); //copy from struct to data frame


   sendCanMessage(canSocketDescriptor,frame); //send can message

}


void myPeriodicTask2()
{

   reciveCanMessage(canSocketDescriptor);
}



int main()
{

  std::cout << "Periodic tick example for MAS234 using clock_nanosleep.." << std::endl;

  // Set up CAN.
  if (!createCanSocket(canSocketDescriptor)) // Passed by reference; canSocketDescriptor is the output variable!
  {
     std::cout << "Could not create CAN socket" << std::endl;
     return 0;
  }

  bool running = true;
  while(running)
  {
    // USE CLOCK_MONOTONIC, and NOT NOT NOT CLOCK_REALTIME.
    // "Realtime" sounds tempting but is non-monotonic and actually a
    // wall-time-realtime, not usable for real time tasks.
    // For hard real time systems, use a RTOS or at least linux with real time extensions.
    // Note: This example has NO error checking (not good).

    // 1. Get current time stamp.
    struct timespec nextTimerDeadline;
    clock_gettime(CLOCK_MONOTONIC, &nextTimerDeadline);


    // 1.1 Periode counter
    static uint64_t ii(0U);
    ++ii;
    std::cout << "Tick " << ii << ".." << std::endl;


    // 1.2 Run periodic task
    switch (taskToggle)
    {
    case false:
        myPeriodicTask2();
        taskToggle = true;
        break;
    case true:
        myPeriodicTask1();
        taskToggle = false;
        break;
    }


    // 2. Add number of nanoseconds the task needs to sleep to the initial timestamp.
    // Could add logic here to check if the task used too much time.
    const int64_t nextTvNsec = static_cast<int64_t>(nextTimerDeadline.tv_nsec) + periodicTaskDtNs;
    std::cout << "  " << nextTvNsec << ", " << nextTimerDeadline.tv_nsec << std::endl;


    // 2.1 Devides nextTvNsec into seconds and nanoseconds .........hva under 1sek
    if (nextTvNsec >= 1000000000L)
    {
        // The timespec struct has one part for nanoseconds and one for seconds,
        // nsec part must be less than one second.
        nextTimerDeadline.tv_sec += static_cast<long int>(nextTvNsec / 1000000000L);
        nextTimerDeadline.tv_nsec = static_cast<long int>(nextTvNsec % 1000000000L);
    }
    else
    {
        nextTimerDeadline.tv_nsec = static_cast<long int>(nextTvNsec);
    }



    // 2.2 Check if the task used too much time
    struct timespec compareTime; //timestap after task execution and before sleep
    clock_gettime(CLOCK_MONOTONIC, &compareTime);

    if ((compareTime.tv_sec > nextTimerDeadline.tv_sec) || ((compareTime.tv_sec == nextTimerDeadline.tv_sec) && (compareTime.tv_nsec >= nextTimerDeadline.tv_nsec)))
    {
        std::cout << "canbus faliure, current task used too much time" << std::endl;
        // could send canbus error message, not implemented
    }



    // 3. Sleep until next deadline.
    // See http://man7.org/linux/man-pages/man2/clock_nanosleep.2.html for documentation.

    // ---------------------------forklaring på dette og (+=) ???????
    std::cout << "deadline = sec: " << nextTimerDeadline.tv_sec << " nanosec: " << nextTimerDeadline.tv_nsec << std::endl;
    std::cout << "finished = sec: " << compareTime.tv_sec << " nanosec: " << compareTime.tv_nsec << std::endl;
   // clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &nextTimerDeadline, NULL);

  }


  return 0;
}


bool createCanSocket(int& socketDescriptor)
{

  struct ifreq ifr;

  if((socketDescriptor = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
          perror("Error while opening CAN socket.");
          return false;
  }

  strcpy(ifr.ifr_name, ifname);
  ioctl(socketDescriptor, SIOCGIFINDEX, &ifr);

  struct sockaddr_can addr;
  addr.can_family  = AF_CAN;
  addr.can_ifindex = ifr.ifr_ifindex;

  if(bind(socketDescriptor, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    perror("Error in socketcan bind.");
    return false;
  }

  return true;
}


void sendCanMessage(int socketDescriptor, struct can_frame frame)
{
  write(socketDescriptor, &frame, sizeof(struct can_frame));
  std::cout << "send can" << std::endl;
}


void reciveCanMessage(int socketDescriptor)
{
   struct can_frame frame;

   read(socketDescriptor, &frame, sizeof(struct can_frame));
   memcpy(&r_mpu6050, frame.data, sizeof(r_mpu6050));


   // prints out message in terminal
   std::cout << std::hex << unsigned (frame.can_id) << "  [" << unsigned (frame.can_dlc) << "]  ";

   for(int i = 0; i <= 7; ++i)
   {
       std::cout << std::hex << std::uppercase << unsigned (frame.data[i]) << " ";
   }
   std::cout << std::nouppercase << std::dec << "\n";
}


void calculateAcc()
{
    mpu6050.acclZ = (r_mpu6050.rawAcclZ * 9.81/16384); // convert input to m/s²

}



