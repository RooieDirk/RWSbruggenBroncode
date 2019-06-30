//  https://github.com/cvlxm/Simple-C-Logger

#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <string>
#include <fstream>
#include <ctime>

#define SETTINGS_COUNT 3
#define LOG_FILE "RWS_bruggen.log"

int* settings;
int lns_written;

enum LOG_BASIC_VALUES
{
	LOG_FALSE,
	LOG_TRUE
};

enum LOG_PRMS
{
	LOGPRM_NONE = 0,
	LOGPRM_CUSTOM_PREFIX = 1
};

enum LOG_MODES
{
	MODE_COMBINED,
	MODE_CMD_ONLY,
	MODE_FILE_ONLY
};

enum LOG_TYPES
{
	TYPE_DEBUG,
	TYPE_INFO,
	TYPE_ERROR
};

enum LOG_SETTING_NAMES
{
	SETTING_DEFAULT_MODE,
	SETTING_DEFAULT_TYPE,
	SETTING_DEFAULT_PRMS
};

int logtofile(const char* message)
{
	FILE* lf = fopen(LOG_FILE, "a");
	fprintf(lf, "%s\n", message);
	fclose(lf);
	lns_written++;
	return 0;
}

bool hasprm(int &prms, int cnt)
{
	if (prms >= cnt)
	{
		prms -= cnt;
		return true;
	}

	return false;
}

int log(const char* _message, int mode, int type, int prms, std::string _prefix = "")
{

	bool s_customPrefix = hasprm(prms, LOGPRM_CUSTOM_PREFIX);
    char MY_TIME[10];
	std::string prefix, message;
    std::time_t now;
	if (s_customPrefix)
	{
		prefix = _prefix;
	}
	else
	{
		if (type == TYPE_DEBUG)
			prefix = "[Debug]";
		else if (type == TYPE_ERROR)
			prefix = "[Error]";
		else if (type == TYPE_INFO)
			prefix = "[Info]";
		else
			prefix = "[Unknown]";
	}
	 // current date/time based on current system
   now = time(0);
   
   // convert now to string form
   struct tm* dt = localtime(&now);
// using strftime to display time 
    strftime(MY_TIME, sizeof(MY_TIME), "%H:%M:%S", dt);
    message ="[";
    message += MY_TIME;
    message += "] ";
    message +=  prefix;
    message += ": ";
    message +=  _message;
    message += ": ";
	switch (mode)
	{
	case MODE_CMD_ONLY:
		std::cout << message << std::endl;
		return 0;
	case MODE_FILE_ONLY:
		return logtofile(message.c_str());
	case MODE_COMBINED:
		std::cout << message << std::endl;
		return logtofile(message.c_str());
	}

	return -1;
}

int logset(int setting, int value)
{
	if (setting >= SETTINGS_COUNT)
		return -1;

	settings[setting] = value;

	return 0;
}

int logget(int setting)
{
	if (setting >= SETTINGS_COUNT)
	{
		log("Invalid setting: " + setting, MODE_COMBINED, TYPE_ERROR, LOGPRM_NONE);
		return -1;
	}

	return settings[setting];
}

int log(const char* message, int mode)
{
	return log(message, mode, logget(SETTING_DEFAULT_TYPE), logget(SETTING_DEFAULT_PRMS));
}


int log(const char* message)
{
	return log(message, logget(SETTING_DEFAULT_MODE));
}

int clrlog()
{
	if (remove(LOG_FILE) != 0)
		return -1;
	if (log("NEW LOG STARTED.", MODE_FILE_ONLY, TYPE_INFO, LOGPRM_CUSTOM_PREFIX, "[Logger]") != 0)
		return -1;
	return 0;
}

int initlog()
{
	lns_written = 0;
	settings = new int[SETTINGS_COUNT]();

	return clrlog();
}

int loglen()
{
	return lns_written;
}

int endlog()
{
	delete [] settings;
	settings = 0;
	return 0;
}
#endif // LOG_H
