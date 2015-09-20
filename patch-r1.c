/*
 * Copyright 2008-2013 Various Authors
 * Copyright 2004-2005 Timo Hirvonen
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */
 /*
	@cmus-youtube . patch-r1
	@author : hamza naper
 */
#ifdef linux
	#include <unistd.h>
 	#include <sys/stat.h>
#endif
static void ydl_createConf(char path[1025], char * username){
	FILE * conf = NULL;
	char pwd[1025];
	struct stat st = {0};

	conf = fopen(path,"a+");
	if(conf){
		/* write to file */
		fprintf(conf,"#cmus-ydl configuration file \n");
		fprintf(conf,"ydl-dir=/home/%s/.cmus-youtube/\n",username);
		fprintf(conf,"ydl-extention=mp3\n");
		fprintf(conf,"ydl-name=song\n");

		/* create a directory */
		snprintf(pwd,sizeof(pwd),"/home/%s/.cmus-youtube",username);
		if (stat(pwd, &st) == -1) {
    		mkdir(pwd, 0700);
		}	

		fclose(conf);
	}

	return ;
}

static void ydl_conf(const char * element){
	/* check if config , is available
		if not available , we will create one
	 */
	FILE * conf = NULL;
	char path[1025];
	char * username = NULL;	

	username = (char * )malloc(sizeof(char));
	username = getlogin();
	snprintf(path,sizeof(path),"/home/%s/.cmus-ydl.conf",username);

	conf = fopen(path,"r");
	if(!conf){
		/* create file */
		ydl_createConf(path, username);
	}else{
		/* read elem from config */
		fclose(conf);
	}

	if(username){
		//free(username);
		//username = NULL;
	}

	return ;		
}

static void ydl_getStr_fromF(const char * file, char string[1025]){
	FILE * fp = NULL;
	char result[1025];

	fp = fopen(file,"r");
	if(!fp){
		error_msg("error while checking a tmp file");
	}else{
		fscanf(fp,"%s\n",result);
		strcpy(string, result);
		fclose(fp);
	}

	return ;
}

static void cmd_youtube(char *arg){
	/*     
		This function allows to download a music from youtube and play it
		downloading .mp4 from youtube
		Using Avconv CBR to convert .mp4 to .mp3
	*/
#ifdef _YOUTUBE_DL
#ifdef linux
	char version[1025];
	char cmd[1025];
	char link[1025];

	/* check config */
	ydl_conf(NULL);
	system("youtube-dl -v > /tmp/cmus-ydl.check");
	/*  clear system error output , and refresh  */
	clearok(curscr, TRUE);
	refresh();

	ydl_getStr_fromF("/tmp/cmus-ydl.check",version);
	/* we will read file and get the version of youtube-dl
			if youtube-dl is not installed . we show an error msg to the user 
			so as to install youtube-dl
	*/
	if(strcmp(version,"") == 0){
		/* youtube-dl not installed */
		error_msg("youtube-dl not installed, install it using apt-get install youtube-dl");
	}else{
		snprintf(cmd,sizeof(cmd),"youtube-dl -g %s > /tmp/cmus-ydl.link", arg);
		system(cmd);
		clearok(curscr, TRUE);
		refresh();
		/*  Downlaod and convert the .mp4 file  */
		ydl_getStr_fromF("/tmp/cmus-ydl.link",link);
		error_msg("link %s => %s\n",arg,link);
		if(strcmp(link,"") == 0){
			error_msg("can't open '%s'",arg);
		}else{
			snprintf(cmd,sizeof(cmd),"wget \"%s\" -O video.mp4",link);
			info_msg("Downloading..please wait");
			system(cmd);
			clearok(curscr, TRUE);
			refresh();
			info_msg("File downloaded! converting ..");
			/* check if avconv is installed */
			system("avconv > /tmp/cmus-ydl.check");
			ydl_getStr_fromF("/tmp/cmus-ydl.check",version);
			if(strcmp(version,"") == 0){
				clearok(curscr, TRUE);
				refresh();
				error_msg("avconv not installed, install it using apt-get install avconv");
			}else{
			/* convert file */
				snprintf(cmd,sizeof(cmd),"yes | avconv -i video.mp4 -vn -acodec libmp3lame -ac 2 -ab 160k -ar 48000  song.mp3");
				system(cmd);
				clearok(curscr, TRUE);
				refresh();
				info_msg("converted ! song.mp3");
			}
		}
	}

#else 
	error_msg("no Windows version !");
#endif 
#else
	error_msg("youtube-dl not supported !");
#endif
	return ;
}
