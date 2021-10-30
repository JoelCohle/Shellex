#include "headers.h"
#include "ls.h"

void print_stats(struct stat stats)
{
    struct passwd *pass = getpwuid(stats.st_uid);
    struct group *grp = getgrgid(stats.st_gid);

    char gettime[600];
    strcpy(gettime, ctime(&stats.st_mtime));
    gettime[strcspn(gettime, "\n")] = 0;

    char fileType;
    if (S_ISDIR(stats.st_mode))
        fileType = 'd';
    else if (S_ISCHR(stats.st_mode))
        fileType = 'c';
    else if (S_ISREG(stats.st_mode))
        fileType = '-';
    else if (S_ISLNK(stats.st_mode))
        fileType = 'l';
    else if (S_ISFIFO(stats.st_mode))
        fileType = 'f';
    printf("%c", fileType);
    printf((stats.st_mode & S_IRUSR) ? "r" : "-");
    printf((stats.st_mode & S_IWUSR) ? "w" : "-");
    printf((stats.st_mode & S_IXUSR) ? "x" : "-");
    printf((stats.st_mode & S_IRGRP) ? "r" : "-");
    printf((stats.st_mode & S_IWGRP) ? "w" : "-");
    printf((stats.st_mode & S_IXGRP) ? "x" : "-");
    printf((stats.st_mode & S_IROTH) ? "r" : "-");
    printf((stats.st_mode & S_IWOTH) ? "w" : "-");
    printf((stats.st_mode & S_IXOTH) ? "x  " : "-  ");
    printf("%ld  ", stats.st_nlink);
    printf("%s\t", pass->pw_name);
    printf("%s\t", grp->gr_name);
    printf("%ld\t", stats.st_size);
    printf("%s\t", gettime);
}

void ls(char **command, char *home_dir)
{
    int l_flag = 0, a_flag = 0;
    char directories[30][1024] = {'\0'};

    int k = 0;
    for(int i = 1; command[i] ; i++)
    {
        if (command[i][0] == '-' && strlen(command[i]) > 1)
        {
            if (command[i][1] == 'l' || command[i][2] == 'l')
                l_flag = 1;
            if (command[i][1] == 'a' || command[i][2] == 'a')
                a_flag = 1;
        }
        else
        {
            strcpy(directories[k], command[i]);
            k++;
        }
    }

    char *curr_dir = (char *)malloc(1024 * sizeof(char));
    if (curr_dir == NULL)
    {
        perror("malloc failed");
        exit(1);
    }
    getcwd(curr_dir, 1024);
    if (directories[0][0] == 0)
    {
        strcpy(directories[0], curr_dir);
    }

    for (int j = 0; directories[j][0] != '\0'; j++)
    {
        if (directories[j][0] == '~')
            strcpy(directories[j], home_dir);

        struct stat stats;
        lstat(directories[j], &stats);
        int isfile = 0;

        if ((access(directories[j], F_OK)) && !(S_ISDIR(stats.st_mode)))
        {
            printf("ls: cannot access '%s': No such file or directory\n", directories[j]);
            continue;
        }
        else
        {
            DIR *lsdir;
            struct dirent *sd;
            printf("%s:\n", directories[j]);
            lsdir = opendir(directories[j]);
            if (lsdir == NULL)
            {
                if ((access(directories[j], F_OK)) == 0)
                {
                    if (l_flag)
                        print_stats(stats);
                    printf("%s\n", directories[j]);
                    continue;
                }
                else
                {
                    perror("file access failed");
                    continue;
                }
            }

            struct dirent *temp_dir;
            DIR *tempreader = opendir(directories[j]);
            int size = 0;
            while ((temp_dir = readdir(tempreader)) != NULL)
            {
                char *file_path = (char *)malloc(1024 * sizeof(char));
                strcpy(file_path, directories[j]);
                strcat(file_path, "/");
                strcat(file_path, temp_dir->d_name);
                struct stat s;
                lstat(file_path, &s);
                size += s.st_blocks;
            }
            printf("total %d\n", size);

            if (a_flag && l_flag)
            {
                while ((sd = readdir(lsdir)) != NULL)
                {
                        print_stats(stats);
                        printf("%s\n", sd->d_name);
                }
            }
            else if (!a_flag && l_flag)
            {
                while ((sd = readdir(lsdir)) != NULL)
                {
                    if (sd->d_name[0] != '.')
                    {
                        print_stats(stats);
                        printf("%s\n", sd->d_name);
                    }
                }
            }
            else if (!a_flag && !l_flag)
            {
                while ((sd = readdir(lsdir)) != NULL)
                {
                    if (sd->d_name[0] != '.')
                        printf("%s\n", sd->d_name);
                }
            }
            else if (a_flag && !l_flag)
            {
                while ((sd = readdir(lsdir)) != NULL)
                    printf("%s\n", sd->d_name);
            }
            printf("\n");
            closedir(lsdir);
        }
    }
}