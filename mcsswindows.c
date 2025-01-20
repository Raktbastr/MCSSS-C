#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <windows.h>
#include <_stdio.h>

#define MAX_URL_LENGTH 2048

// Downloads plugins on Windows
void downloadFileWindows(const char *url) {
    char command[MAX_URL_LENGTH + 50]; 
    snprintf(command, sizeof(command), "cd C:\\MinecraftServer\\plugins && curl -O '%s'", url);
    system(command);
}
// Creates the EULA file on Windows
void createEulaFile() {
    FILE *file = fopen("C:\\MinecraftServer\\eula.txt", "a");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    fprintf(file, "eula=true\n");
    fclose(file);
}
// Prints the help message
void printHelp() {
    printf("Usage: mcss [options]\n");
    printf("Options:\n");
    printf("  -h, --help            Show this help message and exit\n");
    printf("  -v, --version         Show the version information and exit\n");
    printf("  -s, --suggestions     Show reccomendations and suggestions\n");
}
// Waits for a certain amount of seconds
void waitForSeconds(int seconds) {
    Sleep(seconds * 1000); 
}

int main(int argc, char *argv[]) {
    // Check for command line arguments
    if (argc > 1) {
        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
            printHelp();
            return 0;
        } else if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0) {
            printf("Minecraft Server Startup Script version 1.0\n");
            return 0;
        } else if (strcmp(argv[1], "-s") == 0 || strcmp(argv[1], "--suggestions") == 0) {
            printf("\nIf you are running this server under a MacOS or Linux machine it is reccomended to install tmux or screen.\n");
            printf("It will allow you to detach from the terminal without closing running apps.\n");
            printf("If you plan on remotely starting and or managing this server via SSH, tmux/screen is virtually a requirement.\n\n");
            printf("If you are going to make another server with this script, make sure to change the server port in the server.properties file and rename the server folder.\n\n");
            printf("The provided server.properties file is set to the defaults with the exception of your gamemode choice.\n");
            printf("I reccomend looking at the server.properties file and changing the settings to your liking.\n\n");
            return 0;
        } else {
            printf("Unknown option: %s\n", argv[1]);
            printHelp();
            return 1;
        }
    }
    printf("\n-------------------------------\n");
    printf("Minecraft Server Startup Script\n");
    printf("By: Raktbastr\n");
    printf("-------------------------------\n");

    int installConsent;

    // Check the operating system, create server directoy, and install necessary software if possible
    #ifdef _WIN32 
        printf("Windows Operating System\n\n");
        waitForSeconds(2);
        printf("Server directory will be created in the 'C:\\MinecaftServer' folder\n\n");
        waitForSeconds(2);
        system("mkdir C:\\MinecraftServer\\plugins");
        printf("Would you like to install the necessary software? (1 for yes, 0 for no) ");
        scanf("%d", &installConsent);
        if (installConsent == 1) {
            printf("Installing necessary software...\n");
            system("winget install curl -h -y && winget install -e --id Oracle.JDK.17 -h -y");
        } else if (installConsent == 0) {
            printf("Skipping software installation...\n");
        } else {
            printf("Please input a valid number.\n");
            exit(EXIT_FAILURE);
        }

    #else
        printf("Unsupported Operating System\n");
        exit(EXIT_FAILURE);

    #endif
        ifdef __linux__ 
    waitForSeconds(2);
    printf("Which server base?\n");
    printf("1) Vanilla (no plugins, base game)\n");
    printf("2) Paper (plugins, slightly modifiable)\n");
    gotoServerType:
    printf("Which type: ");
    scanf("%d", &serverType);
    switch (serverType) {
    // Vanilla server setup
    case 1:
        printf("Downloading 1.21.4 server jar and required files.\n");
        waitForSeconds(2);
        system("curl -o C:\\MinecraftServer\\server.jar 'https://piston-data.mojang.com/v1/objects/4707d00eb834b446575d89a61a11b5d548d8c001/server.jar'");
        system("curl -o C:\\MinecraftServer\\start.bat 'https://raw.githubusercontent.com/Raktbastr/MCSSS-C/refs/heads/main/start.bat'");
        system("curl -o C:\\MinecraftServer\\start.sh 'https://raw.githubusercontent.com/Raktbastr/MCSSS-C/refs/heads/main/start.sh'");
        system("curl -o C:\\MinecraftServer\\start-nogui.bat 'https://raw.githubusercontent.com/Raktbastr/MCSSS-C/refs/heads/main/start-nogui.bat'");
        system("curl -o C:\\MinecraftServer\\start-nogui.sh 'https://raw.githubusercontent.com/Raktbastr/MCSSS-C/refs/heads/main/start-nogui.sh'");
        
        waitForSeconds(2);
        printf("Which server gamemode?\n");
        printf("1) Surival\n");
        printf("2) Creative\n");
        switch1:
        printf("Which type: ");
        scanf("%d", &gamemode);
        // Downloads the server.properties file based on the gamemode
        if (gamemode == 1) {
            waitForSeconds(2);
            system("curl -o C:\\MinecraftServer\\server.properties 'https://raw.githubusercontent.com/Raktbastr/MCSSS-C/refs/heads/main/survivalserver.properties'");
        } else if (gamemode == 2) {
            waitForSeconds(2);
            system("curl -o C:\\MinecraftServer\\server.properties 'https://raw.githubusercontent.com/Raktbastr/MCSSS-C/refs/heads/main/creativeserver.properties'");
        } else {
            printf("Please input a valid number.\n");
            goto switch1;
        }
        break;
    // Paper server setup
    case 2:
        printf("Downloading 1.21.4 Paper server jar.\n");
        waitForSeconds(2);
        system("curl -o C:\\MinecraftServer\\server.jar 'https://api.papermc.io/v2/projects/paper/versions/1.21.4/builds/4/downloads/paper-1.21.4-4.jar'");
        system("curl -o C:\\MinecraftServer\\start.bat 'https://raw.githubusercontent.com/Raktbastr/MCSSS-C/refs/heads/main/start.bat'");
        system("curl -o C:\\MinecraftServer\\start.sh 'https://raw.githubusercontent.com/Raktbastr/MCSSS-C/refs/heads/main/start.sh'");
        system("curl -o C:\\MinecraftServer\\start-nogui.bat 'https://raw.githubusercontent.com/Raktbastr/MCSSS-C/refs/heads/main/start-nogui.bat'");
        system("curl -o C:\\MinecraftServer\\start-nogui.sh 'https://raw.githubusercontent.com/Raktbastr/MCSSS-C/refs/heads/main/start-nogui.sh'");
        
        waitForSeconds(2);
        printf("Which server gamemode?\n");
        printf("1) Surival\n");
        printf("2) Creative\n");
        switch2:
        printf("Which type: ");
        scanf("%d", &gamemode);
        // Downloads the server.properties file based on the gamemode
        if (gamemode == 1) {
            waitForSeconds(2);
            system("curl -o C:\\MinecraftServer\\server.properties 'https://raw.githubusercontent.com/Raktbastr/MCSSS-C/refs/heads/main/papersurvivalserver.properties'");
        } else if (gamemode == 2) {
            waitForSeconds(2);
            system("curl -o C:\\MinecraftServer\\server.properties 'https://raw.githubusercontent.com/Raktbastr/MCSSS-C/refs/heads/main/papercreativeserver.properties'");
        } else {
            printf("Please input a valid number.\n");
            goto switch2;
        }
        // Downloads plugins if wanted
        int pluginAmt;
        waitForSeconds(2);
        pluginGoto:
        printf("How many plugins would you like to add? \n");
        if (scanf("%d", &pluginAmt) != 1 || pluginAmt < 0) {
            printf("Please input a valid number.\n");
            while (getchar() != '\n'); // Clear the input buffer
            goto pluginGoto;
        }
        for (int i = 0; i < pluginAmt; i++ ) {
            char url[MAX_URL_LENGTH];
            printf("Enter the URL to download plugin \n");
            printf("Make sure the URL ends with a filename (e.g. 'https://example.com/plugin.jar'): ");
            scanf("%s", url);
            waitForSeconds(2);
            downloadFileWindows(url);
        }
        break;
    default:
        printf("Please input a vaild number.\n");
        goto gotoServerType;
    }

    // Finish server setup!
    waitForSeconds(2);
    printf("Making and accepting EULA file\n");
    if (osType == 1) {
        createEulaFile();
    }

    waitForSeconds(2);
    printf("Server setup complete.\n\n");
    printf("To start the server, run the start script in the server directory.\n");
    printf("For Windows, run 'start.bat' or 'start-nogui.bat' for no GUI.\n");
    printf("For MacOS/Linux, run 'start.sh' or 'start-nogui.sh' for no GUI.\n");
    printf("For extra info and suggestions, run 'mcss -s'\n\n");
    printf("Enjoy your server!\n");

    return 0;
}