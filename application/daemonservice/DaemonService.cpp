/******************************************************************************

 Copyright (c) 2020 VIA Technologies, Inc. All Rights Reserved.

 This PROPRIETARY SOFTWARE is the property of VIA Technologies, Inc.
 and may contain trade secrets and/or other confidential information of
 VIA Technologies, Inc. This file shall not be disclosed to any third
 party, in whole or in part, without prior written consent of VIA.

 THIS PROPRIETARY SOFTWARE AND ANY RELATED DOCUMENTATION ARE PROVIDED AS IS,
 WITH ALL FAULTS, AND WITHOUT WARRANTY OF ANY KIND EITHER EXPRESS OR IMPLIED,
 AND VIA TECHNOLOGIES, INC. DISCLAIMS ALL EXPRESS OR IMPLIED
 WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIET
 ENJOYMENT OR NON-INFRINGEMENT.

******************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>



#define FILE_LOCATION "daemonlist"

struct ExecArgs
{
	pid_t id;
	char linebuf[256];
	char *args[8];
};

int main(int argc, char **argv)
{
	FILE *in = fopen(FILE_LOCATION, "r");
	char linebuf[256];
	char *args[8];
	pid_t id;

	while (fgets(linebuf, 256, in) != NULL)
	{
		// printf(linebuf);
		int ret = fork();
		printf("fork id = %d\n", ret);

		const char *delim = " ";

		int i = 1;
		args[0] = strtok(linebuf, delim);

		while (args[i++] = strtok(NULL, delim))
		{
			printf("%s\n", args[i - 1]);
		}

		if (ret == 0)
		{
			execvp(linebuf, args);
			exit(errno);
		}
		if (ret > 0)
		{
			id = ret;
		}
	}

	while (1)
	{
		pid_t pid = waitpid(-1, NULL, 0);
		printf("%d\n", id);
		if (pid == -1)
		{
			continue;
		}

		int i;
		bool foundApp = false;

		if (id == pid)
		{
			int ret = fork();
			if (ret == 0)
			{
				execvp(linebuf, args);
				exit(errno);
			}

			if (ret > 0)
			{
				id = ret;
			}
		}
	}
}
