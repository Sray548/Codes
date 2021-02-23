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

#include "DynArray.h"
#include "WLogger.h"

#define FILE_LOCATION "daemonlist"
static WLogger *myLog = wlogGetInit("DaemonService", WLOGLEVEL_DEBUG);

struct ExecArgs
{
	pid_t id;
	char linebuf[256];
	char *args[8];
};

int parserArgs(struct ExecArgs *cmd)
{
	const char *delim = " ";
	char *buf = cmd->linebuf;
	int i = 1;
	strtok(buf, delim);
	cmd->id = -1;
	cmd->args[0] = strrchr(buf, '/') + 1;
	while ((cmd->args[i++] = strtok(NULL, delim)))
	{
		printf("%s\n", cmd->args[i - 1]);
		WLOGD(myLog, "%d %s", i - 1, cmd->args[i - 1]);
	}
	return 1;
}

int main(int argc, char **argv)
{
	TDynArray<struct ExecArgs> cmdlist;
	FILE *in = fopen(FILE_LOCATION, "r");

	WLOGD(myLog, "%s", FILE_LOCATION);

	while (true)
	{
		struct ExecArgs *cmd = new (struct ExecArgs);

		char *p = fgets(cmd->linebuf, 256, in);
		if (p)
		{
			if (p[0] == '#')
			{
				//skip #line comment
				continue;
			}
			p[strlen(p) - 1] = '\0';
			if (parserArgs(cmd))
			{
				cmdlist.push_back(*cmd);
			}
		}
		else
		{
			break;
		}
	}

	fclose(in);

	for (int i = 0; i < cmdlist.size(); i++)
	{
		int ret = fork();
		if (ret == 0)
		{
			execvp(cmdlist[i].linebuf, cmdlist[i].args);
			exit(errno);
		}
		else if (ret < 0)
		{
		}
		else
		{
			cmdlist[i].id = ret;
		}
	}

	while (1)
	{
		pid_t id = waitpid(-1, NULL, 0);
		if (id == -1)
		{
			continue;
		}

		int i;
		bool foundApp = false;

		for (i = 0; i < cmdlist.size(); i++)
		{
			if (id == cmdlist[i].id)
			{
				foundApp = true;
				break;
			}
		}

		if (foundApp)
		{
			int ret = fork();
			if (ret == 0)
			{
				execvp(cmdlist[i].linebuf, cmdlist[i].args);
				exit(errno);
			}
			else if (ret < 0)
			{
				/* code */
			}
			else
			{
				cmdlist[i].id = ret;
			}
		}
		else
		{
		}
	}
}
