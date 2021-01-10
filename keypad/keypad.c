#include <sys/time.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>
#include <time.h>
#include <termios.h>
#include "keypad.h"
/**************************************************************/
/**************************************************************/
/**************************************************************/

void keypadInit(char *dev)
{

  if ((inputFile = open(dev, O_RDONLY)) < 0) {
    perror("can't open input device");
    close(inputFile);
    return;
  }
  if (ioctl(inputFile, EVIOCGVERSION, &version)) {
    printf("can't get device version\n");
    //close(inputFile);
    //return;
  }
  // gerek yok versiona //LoL
  printf("Input driver version is %d.%d.%d\n", version >> 16, (version >> 8) & 0xff, version & 0xff);
}

uint8_t keypadInputProcess(char* device)
{
  //while (1)
  //{
    readInputEvent = read(inputFile, &info.ev, sizeof(struct input_event));

    if (readInputEvent < (int) sizeof(struct input_event))
    {
      printf("read file error\n");
      close(inputFile);
      return FALSE;
    }
    else
    {
      if (info.ev.type == EV_KEY)
      {
        
        if ( keyPressedName( info.ev ) ) {
          info.keyTime = getPressedTime(info.pressedTime, info.releseTime);
          if(  info.keyTime >= 0 )
          {
            info.keyCode = info.ev.code;
            return TRUE;
          }
        }
      }
    }
    return FALSE;
}

int keyPressedName( struct input_event event )
{
  int returnVal = FALSE;
  if (event.value == 1)
  {
    info.pressedTime = *localtime(&event.time.tv_sec);
  }
  else if (event.value == 0)
  {
    info.releseTime = *localtime(&info.ev.time.tv_sec);
    returnVal = TRUE;
  }
  return returnVal;
}

int getPressedTime( struct tm time1, struct tm time2 )
{
  return ( time2.tm_sec - time1.tm_sec);
}

char *fullDateInfo(struct tm time)
{
  strftime(info.pressedBuffer, sizeof(info.pressedBuffer), "%a %Y-%m-%d %H:%M:%S", &time);
  return info.pressedBuffer;
}

static struct termios tty_state_current;
// tty console ECHO ON
int echo_on() {
  struct termios tty_state;
 
  if(tcgetattr(0, &tty_state) < 0)
    return -1;
  tty_state.c_lflag |= ECHO;
  return tcsetattr(0, TCSANOW, &tty_state);
}
 
// tty console ECHO OFF
int echo_off() {
  struct termios tty_state;
 
  if(tcgetattr(0, &tty_state) < 0)
    return -1;
  tty_state.c_lflag &= ~ECHO;
  return tcsetattr(0, TCSANOW, &tty_state);
}
 
// Restore tty console settings to its previous original state
void restore_terminal_settings(void) {
  tcsetattr(0, TCSANOW, &tty_state_current);
}
 
// Disable waiting for ENTER and accept only one keypress as input
void disable_waiting_for_enter(void) {
  struct termios tty_state;
 
  tcgetattr(0, &tty_state_current);  // Get current tty console settings
  tty_state = tty_state_current;
  tty_state.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(0, TCSANOW, &tty_state);
  atexit(restore_terminal_settings); // Very handy function to restore settings
}



int main(int argc, char *argv[] )
{
    char *inDeviceame = NULL;

	if ( argc == 2 )
	{
		inDeviceame = argv[1];
		printf("input device name = %s\n",inDeviceame);
	}
	else
	{
		printf("Incorrect arguments, you must specify a file name\n");
		return(1);
	}
  disable_waiting_for_enter();
  echo_off();
  keypadInit(inDeviceame);
  while(1)
  {
    if( keypadInputProcess(inDeviceame))
    {
      printf("info.keyCode = %s\n", keys[info.keyCode]);
      printf("info.keyTime = %d \n", info.keyTime);
    }
  }
}