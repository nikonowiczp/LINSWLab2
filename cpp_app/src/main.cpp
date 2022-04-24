#include <gpiod.h>
#include <error.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <ctime>
struct gpiod_chip *chip;
struct gpiod_line_bulk buttonLines;

struct gpiod_line_bulk events;

#ifndef	CONSUMER
#define	CONSUMER	"Consumer"
#endif

bool blinkSingleLed(gpiod_line *line){
	int ret = gpiod_line_set_value(line, 1);
	if (ret < 0) {
		perror("Set line output failed\n");
		return false;
	}
	struct timespec waitTime;
	waitTime.tv_sec = 0;
	waitTime.tv_nsec = 50* 1000000;
	nanosleep(&waitTime, NULL);

	ret = gpiod_line_set_value(line, 0);
	if (ret < 0) {
		perror("Set line output failed\n");
		return false;
	}
	return true;
}
bool setSingleLed(gpiod_line *line, int value){
	int ret = gpiod_line_set_value(line,value);
	if (ret < 0) {
		perror("Set line output failed\n");
		return false;
	}

	return true;
}

int main(int argc, char **argv)
{
	char *chipname = "gpiochip0";
	unsigned int led_01 = 23, led_02 = 27;
	unsigned int but_01 = 17, but_02 = 18;
	unsigned int end_button = 25;

	unsigned int offsets[3];
	
	struct gpiod_line *ledLine01;
	struct gpiod_line *ledLine02;

	int values[3];

	std::time_t started_counting = std::time(0);
	std::time_t current_time = std::time(0);

	bool isCurrentlyClicked = false;

	struct timespec begin_count;
	struct timespec last_count;
	struct timespec timeout;
	bool isStarted = false;

	int err,ret;

	values[0]=values[1]= values[2] = -1;
	offsets[0] = but_01;
	offsets[1] = but_02;
	offsets[2] = end_button;
	chip = gpiod_chip_open("/dev/gpiochip0");
  	if(!chip)
  	{
  	  perror("gpiod_chip_open");
  	  goto cleanup;
  	}

	ledLine01 = gpiod_chip_get_line(chip, led_01);
	ledLine02 = gpiod_chip_get_line(chip, led_02);
	if (!ledLine01 || !ledLine02 ) {
		perror("Get line failed\n");
		goto cleanup;
	}

	ret = gpiod_line_request_output(ledLine01, CONSUMER, 0);
	if (ret < 0) {
		perror("Request line as output failed\n");
		return false;
	}
	ret = gpiod_line_request_output(ledLine02, CONSUMER, 0);
	if (ret < 0) {
		perror("Request line as output failed\n");
		return false;
	}
	err = gpiod_chip_get_lines(chip, offsets, 3, &buttonLines);
  	if(err)
  	{
  	  perror("gpiod_chip_get_lines");
  	  goto cleanup;
  	}

	err = gpiod_line_request_bulk_falling_edge_events(&buttonLines, "rising edge example");
  	if(err)
  	{
  	  perror("gpiod_line_request_bulk_rising_edge_events");
  	  goto cleanup;
  	}

	timeout.tv_sec = 0;
	timeout.tv_nsec = 60 * 1000000;
	
	printf("Stopper started\n");

	
	while(true){
		//printf("Listening for signals\n");
		err = gpiod_line_event_wait_bulk(&buttonLines, NULL, &events);
  		if(err == -1)
  		{
  		  perror("gpiod_line_event_wait_bulk");
  		  goto cleanup;
  		}
  		else if(err == 0)
  		{
  		  	fprintf(stderr, "wait timed out\n");
			printf("Finishing stopper\n");
  		  	continue;
  		}
		nanosleep(&timeout,NULL);

		err = gpiod_line_get_value_bulk(&events, values);
  		if(err)
  		{
  		  perror("gpiod_line_get_value_bulk");
  		  goto cleanup;
  		}
		if(values[0] == 0&& !isCurrentlyClicked){
			isCurrentlyClicked = true;
			if(!isStarted){
				printf("Beginning time counting\n");
				//started_counting = std::time(nullptr);
				clock_gettime(CLOCK_REALTIME, &begin_count);
				if(!setSingleLed(ledLine01,1)) goto cleanup;
				isStarted = true;
			}else{
				//current_time = std::time(0);
				clock_gettime(CLOCK_REALTIME, &last_count);
				int resultsec = last_count.tv_sec - begin_count.tv_sec;
				int resultminisec = (last_count.tv_nsec - begin_count.tv_nsec)/1000000;
				if(resultminisec<0){
					resultsec -=1;
					resultminisec = 1000 + resultminisec;
				}
				printf("Time recorded: %d seconds %d miliseconds\n", resultsec,resultminisec);
				if(!blinkSingleLed(ledLine02)) goto cleanup;

			}
			
		}
		if(values[0]!= 0){
			isCurrentlyClicked = false;
		}

		if(values[1] == 0 && isStarted == true){
			if(!setSingleLed(ledLine01,0)) goto cleanup;
			isStarted= false;

			clock_gettime(CLOCK_REALTIME, &last_count);
			int resultsec = last_count.tv_sec - begin_count.tv_sec;
			int resultminisec = (last_count.tv_nsec - begin_count.tv_nsec)/1000000;
			if(resultminisec<0){
				resultsec -=1;
				resultminisec = 1000 + resultminisec;
			}
			printf("Time counter stopped. Time recorded: %d seconds %d miliseconds\n", resultsec,resultminisec);
			if(!blinkSingleLed(ledLine02)) goto cleanup;

		}
		if(values[2] == 0){
			if(!setSingleLed(ledLine01,0)) goto cleanup;
			printf("Ending program\n");

			
			goto cleanup;
		}
		
  		fflush(stdout);


	}

cleanup:
  gpiod_line_release_bulk(&buttonLines);
  gpiod_line_release(ledLine01);
  gpiod_line_release(ledLine02);
  gpiod_chip_close(chip);

end:
	return 0;
}