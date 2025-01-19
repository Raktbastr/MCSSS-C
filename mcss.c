#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("Minecraft Server Startup Script\n");
    printf("By: Raktbastr\n");
    printf("-------------------------------\n");

    int osType;

    #ifdef _WIN32 
        printf("Windows Operating System\n");
        printf("Server directory will be created in the 'C:\\MinecaftServer' folder\n\n");
        osType = 1;
        system("mkdir \\MinecraftServer");
    #elif __APPLE__ 
        printf("MacOS Operating System\n");
        osType = 2;
        printf("Server directory will be created in your 'Documents' folder\n\n");
        system("mkdir ~/Documents/MinecraftServer");
    #elif __linux__ 
        printf("Linux-based Operating System\n");
        osType = 3;
        printf("Server directory will be created in your 'Documents' folder\n\n");
        system("mkdir ~/Documents/MinecraftServer");
    #endif 

    int serverType;

    printf("Which server base?\n");
    printf("1) Vanilla (no plugins, base game)\n");
    printf("2) Paper (plugins, slightly modifiable)\n");
    switch1:
    printf("Which type: \n");
    scanf("%d", &serverType);

    switch (serverType) {
    case 1:
        printf("Downloading 1.21.4 server jar.\n");
        system("curl -O server.jar https://piston-data.mojang.com/v1/objects/4707d00eb834b446575d89a61a11b5d548d8c001/server.jar");
        break;

    case 2:
        printf("Downloading 1.21.4 server jar.\n");
        system("curl -O server.jar https://api.papermc.io/v2/projects/paper/versions/1.21.4/builds/4/downloads/paper-1.21.4-4.jar");
        system("curl -O https://github.com/raktbastr/MCSS/");
        break;

    default:
        printf("Please input a vaild number.\n");
        goto switch1;
    }



    return 0;
}