#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#ifdef _WIN32
#include <windows.h> 
#endif
#if defined(__APPLE__) || defined(__linux__)
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif

#define MAX_URL_LENGTH 2048

// Downloads plugins on Macos/Linux
void downloadFileUnix(const char *url) {
    char command[MAX_URL_LENGTH + 50]; 
    snprintf(command, sizeof(command), "cd ~/Documents/MinecraftServer/plugins/ && curl -O '%s'", url);
    system(command);
}
// Down`loads plugins on Windows
void downloadFileWindows(const char *url) {
    char command[MAX_URL_LENGTH + 50]; 
    snprintf(command, sizeof(command), "cd C:\\MinecraftServer\\plugins && curl -O '%s'", url);
    system(command);
}
// Creates the EULA file on Windows
void createEulaFileWindows() {
    FILE *file = fopen("C:\\MinecraftServer\\eula.txt", "a");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    fprintf(file, "eula=true\n");
    fclose(file);
}
// Detects the package manager on Linux
const char* detectPackageManager() {
    if (access("/usr/bin/apt", F_OK) == 0) {
        return "apt";
    } else if (access("/usr/bin/yum", F_OK) == 0) {
        return "yum";
    } else if (access("/usr/bin/dnf", F_OK) == 0) {
        return "dnf";
    } else if (access("/usr/bin/pacman", F_OK) == 0) {
        return "pacman";
    } else {
        return NULL;
    }
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
    #ifdef _WIN32
    Sleep(seconds * 1000); 
    #else
    sleep(seconds); 
    #endif
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
            printf("If you are running this server under a MacOS or Linux machine it is reccomended to install tmux or screen.\n");
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

    int osType;
    int installConsent;

    // Check the operating system, create server directoy, and install necessary software if possible
    #ifdef _WIN32 
        printf("Windows Operating System\n\n");
        waitForSeconds(2);
        printf("Server directory will be created in the 'C:\\MinecaftServer' folder\n\n");
        waitForSeconds(2);
        osType = 1;
        system("mkdir C:\\MinecraftServer\\plugins");
        printf("Would you like to install the necessary software? (1 for yes, 0 for no) ");
        scanf("%d", &installConsent);
        if (installConsent == 1) {
            printf("Installing necessary software...\n");
            system("winget install curl -h -y && winget install -e --id Oracle.JDK.17 -h -y");
        }
        
    #elif __APPLE__ 
        printf("MacOS Operating System\n\n");
        waitForSeconds(2);
        osType = 2;
        printf("Server directory will be created in your 'Documents' folder\n\n");
        waitForSeconds(2);
        system("mkdir -p ~/Documents/MinecraftServer/plugins");
        printf("Would you like to install the necessary software? (1 for yes, 0 for no) ");
        scanf("%d", &installConsent);
        if (installConsent == 1) {
            printf("Installing necessary software...\n");
            system("brew install curl openjdk@17");
        }
        
    #elif __linux__ 
        printf("Linux-based Operating System\n\n");
        waitForSeconds(2);
        osType = 3;
        printf("Server directory will be created in your 'Documents' folder\n\n");
        waitForSeconds(2);
        system("mkdir -p ~/Documents/MinecraftServer/plugins");
        printf("Would you like to install the necessary software? (1 for yes, 0 for no) ");
        scanf("%d", &installConsent);
        // Install necessary software with package manager, exits if you are not running with sudo
        if (installConsent == 1) {
            printf("Installing necessary software...\n");
            if (geteuid() != 0) {
                printf("To download necessarry software please run with sudo.\n");
                exit(EXIT_FAILURE);
            }
            const char* packageManager = detectPackageManager();
            if (packageManager == NULL) {
                printf("No supported package manager found. Please install curl and openjdk manually.\n");
                exit(EXIT_FAILURE);
            }

            char installCommand[100];
            if (strcmp(packageManager, "apt") == 0) {
                snprintf(installCommand, sizeof(installCommand), "sudo apt update && sudo apt install -y curl openjdk-17-jdk");
            } else if (strcmp(packageManager, "dnf") == 0) {
                snprintf(installCommand, sizeof(installCommand), "sudo dnf install -y curl java-17-openjdk");
            } else if (strcmp(packageManager, "pacman") == 0) {
                snprintf(installCommand, sizeof(installCommand), "sudo pacman -Sy --noconfirm curl jdk17-openjdk");
            }

            system(installCommand);
        }

    #else
        printf("Unsupported Operating System\n");
        exit(EXIT_FAILURE);

    #endif
    // Downloads server files, plugins if wanted, and create server.properties file on MacOS/Linux
    if (osType == 2 || osType == 3) {
        int gamemode;
        int serverType;

        waitForSeconds(2);
        printf("\nWhich server base?\n");
        printf("1) Vanilla (no plugins, base game)\n");
        printf("2) Paper (plugins, slightly modifiable)\n");
        gotoServerTypeUnix:
        printf("Which type: ");
        scanf("%d", &serverType);

        switch (serverType) {
        // Vanilla server setup
        case 1:
            printf("Downloading 1.21.4 server jar.\n");
            waitForSeconds(2);
            system("curl -o ~/Documents/MinecraftServer/server.jar 'https://piston-data.mojang.com/v1/objects/4707d00eb834b446575d89a61a11b5d548d8c001/server.jar'");
            system("curl -o ~/Documents/MinecraftServer/start.bat 'https://raw.githubusercontent.com/Raktbastr/MCSSS-C/refs/heads/main/start.bat'");
            system("curl -o ~/Documents/MinecraftServer/start.sh 'https://raw.githubusercontent.com/Raktbastr/MCSSS-C/refs/heads/main/start.sh'");
            system("curl -o ~/Documents/MinecraftServer/start-nogui.bat 'https://raw.githubusercontent.com/Raktbastr/MCSSS-C/refs/heads/main/start-nogui.bat'");
            system("curl -o ~/Documents/MinecraftServer/start-nogui.sh 'https://raw.githubusercontent.com/Raktbastr/MCSSS-C/refs/heads/main/start-nogui.sh'");

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
                system("curl -o ~/Documents/MinecraftServer/server.properties 'https://raw.githubusercontent.com/Raktbastr/MCSSS-C/refs/heads/main/survivalserver.properties'");
            } else if (gamemode == 2) {
                waitForSeconds(2);
                system("curl -o ~/Documents/MinecraftServer/server.properties 'https://raw.githubusercontent.com/Raktbastr/MCSSS-C/refs/heads/main/creativeserver.properties'");
            } else {
                printf("Please input a valid number.\n");
                goto switch2;
            }
        break;
        // Paper server setup
        case 2:
            printf("Downloading 1.21.4 Paper server jar.\n");
            waitForSeconds(2);
            system("curl -o ~/Documents/MinecraftServer/server.jar 'https://api.papermc.io/v2/projects/paper/versions/1.21.4/builds/4/downloads/paper-1.21.4-4.jar'");
            system("curl -o ~/Documents/MinecraftServer/start.bat 'https://raw.githubusercontent.com/Raktbastr/MCSSS-C/refs/heads/main/start.bat'");
            system("curl -o ~/Documents/MinecraftServer/start.sh 'https://raw.githubusercontent.com/Raktbastr/MCSSS-C/refs/heads/main/start.sh'");
            system("curl -o ~/Documents/MinecraftServer/start-nogui.bat 'https://raw.githubusercontent.com/Raktbastr/MCSSS-C/refs/heads/main/start-nogui.bat'");
            system("curl -o ~/Documents/MinecraftServer/start-nogui.sh 'https://raw.githubusercontent.com/Raktbastr/MCSSS-C/refs/heads/main/start-nogui.sh'");

            waitForSeconds(2);
            printf("Which server gamemode?\n");
            printf("1) Surival\n");
            printf("2) Creative\n");
            switch3:
            printf("Which type: ");
            scanf("%d", &gamemode);

            // Downloads the server.properties file based on the gamemode
            if (gamemode == 1) {
                waitForSeconds(2);
                system("curl -o ~/Documents/MinecraftServer/server.properties 'https://raw.githubusercontent.com/Raktbastr/MCSSS-C/refs/heads/main/papersurvivalserver.properties'");
            } else if (gamemode == 2) {
                waitForSeconds(2);
                system("curl -o ~/Documents/MinecraftServer/server.properties 'https://raw.githubusercontent.com/Raktbastr/MCSSS-C/refs/heads/main/papercreativeserver.properties'");
            } else {
                printf("Please input a valid number.\n");
                goto switch3;
            }
            // Downloads plugins if wanted
            int pluginAmt;
            char input[10];
            waitForSeconds(2);
            pluginGoto1:
            printf("How many plugins would you like to add? ");
            fgets(input, sizeof(input), stdin);
            if (sscanf(input, "%d", &pluginAmt) != 1 || pluginAmt < 0) {
                printf("Please input a valid number.\n");
                goto pluginGoto1;
            }
            for (int i = 0; i < pluginAmt; i++ ) {
                char url[MAX_URL_LENGTH];

                printf("Enter the URL to download plugin \n");
                printf("Make sure the URL ends with a filename (e.g. 'https://example.com/plugin.jar'): ");
                fgets(url, sizeof(url), stdin);

                size_t len = strlen(url);
                if (len > 0 && url[len - 1] == '\n') {
                    url[len - 1] = '\0';
                }
                waitForSeconds(2);
                downloadFileUnix(url);
            }

            break;

        default:
            printf("Please input a vaild number.\n");
            goto gotoServerTypeUnix;
        }
    }
    // Downloads server files, plugins if wanted, and create server.properties file on Windows
    if (osType == 1) {
        int gamemode;
        int serverType;

        waitForSeconds(2);
        printf("Which server base?\n");
        printf("1) Vanilla (no plugins, base game)\n");
        printf("2) Paper (plugins, slightly modifiable)\n");
        gotoServerTypeWindows:
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
            switch4:
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
                goto switch4;
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
            switch5:
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
                goto switch5;
            }

            // Downloads plugins if wanted
            int pluginAmt;
            char input[10];
            waitForSeconds(2);
            pluginGoto2:
            printf("How many plugins would you like to add? \n");
            fgets(input, sizeof(input), stdin);
            if (sscanf(input, "%d", &pluginAmt) != 1 || pluginAmt < 0) {
                printf("Please input a valid number.\n");
                goto pluginGoto2;
            }
            for (int i = 0; i < pluginAmt; i++ ) {
                char url[MAX_URL_LENGTH];

                printf("Enter the URL to download plugin \n");
                printf("Make sure the URL ends with a filename (e.g. 'https://example.com/plugin.jar'): ");
                fgets(url, sizeof(url), stdin);

                size_t len = strlen(url);
                if (len > 0 && url[len - 1] == '\n') {
                    url[len - 1] = '\0';
                }
                waitForSeconds(2);
                downloadFileWindows(url);
            }

            break;

        default:
            printf("Please input a vaild number.\n");
            goto gotoServerTypeWindows;
        }
    }

    // Finish server setup!
    waitForSeconds(2);
    printf("Making and accpeting EULA file\n");
    if (osType == 1) {
        createEulaFileWindows();
    } else {
        system("echo 'eula=true' > ~/Documents/MinecraftServer/eula.txt");
        // Get the username of the user running the script
        const char *username = getenv("SUDO_USER");
        if (username == NULL) {
            username = getenv("USER");
        }

        if (username != NULL) {
            // Get the user ID and group ID of the user
            struct passwd *pw = getpwnam(username);
            if (pw != NULL) {
                uid_t uid = pw->pw_uid;
                gid_t gid = pw->pw_gid;

                // Change ownership recursively
                if (chown("/Users/raktbastr/Documents/MinecraftServer", uid, gid) == -1) {
                    perror("Failed to change ownership of the server directory. Please run 'sudo chown -R <username> ~/Documents/MinecraftServer'");
                }
            } else {
                perror("Failed to change ownership of the server directory. Please run 'sudo chown -R <username> ~/Documents/MinecraftServer");
            }
    } else {
        perror("Failed to change ownership of the server directory. Please run 'sudo chown -R <username> ~/Documents/MinecraftServer");
    }
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