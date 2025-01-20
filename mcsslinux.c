#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MAX_URL_LENGTH 2048

// Downloads plugins on Macos/Linux
void downloadFile(const char *url) {
    const char *username = getenv("SUDO_USER");
    if (username == NULL) {
        username = getenv("USER");
    }
    struct passwd *pw = getpwnam(username);
    if (pw == NULL) {
        perror("Failed to get user information");
        exit(EXIT_FAILURE);
    }
    const char *homeDir = pw->pw_dir;
    char command[MAX_URL_LENGTH + 100]; 
    snprintf(command, sizeof(command), "cd %s/Documents/MinecraftServer/plugins/ && curl -O '%s'", homeDir, url);
    system(command);
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
    sleep(seconds); 
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
        
    #ifdef __linux__ 
        printf("Linux-based Operating System\n\n");
        waitForSeconds(2);
        printf("Server directory will be created in your 'Documents' folder\n\n");
        waitForSeconds(2);
        const char *username = getenv("SUDO_USER");
        if (username == NULL) {
            username = getenv("USER");
        }
        struct passwd *pw = getpwnam(username);
        if (pw == NULL) {
            perror("Failed to get user information");
            exit(EXIT_FAILURE);
        }
        const char *homeDir = pw->pw_dir;
        char command[256];
        snprintf(command, sizeof(command), "mkdir -p %s/Documents/MinecraftServer/plugins", homeDir);
        system(command);
        printf("Would you like to install the necessary software? (1 for yes, 0 for no) ");
        scanf("%d", &installConsent);
        // Install necessary software with package manager, exits if you are not running with sudo
        if (installConsent == 1) {
            printf("Installing necessary software...\n");
            if (geteuid() != 0) {
                printf("To download necessary software please run with sudo.\n");
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
    // Downloads server files, plugins if wanted, and create server.properties file.
    int gamemode;
    int serverType;
    waitForSeconds(2);
    printf("\nWhich server base?\n");
    printf("1) Vanilla (no plugins, base game)\n");
    printf("2) Paper (plugins, slightly modifiable)\n");
    gotoServerType:
    printf("Which type: ");
    scanf("%d", &serverType);
    switch (serverType) {
    // Vanilla server setup
    case 1:
        printf("Downloading 1.21.4 server jar.\n");
        waitForSeconds(2);
        snprintf(command, sizeof(command), "curl -o %s/Documents/MinecraftServer/server.jar 'https://piston-data.mojang.com/v1/objects/4707d00eb834b446575d89a61a11b5d548d8c001/server.jar'", homeDir);
        system(command);
        snprintf(command, sizeof(command), "curl -o %s/Documents/MinecraftServer/start.bat 'https://raw.githubusercontent.com/Raktbastr/MCSSS-C/refs/heads/main/start.bat'", homeDir);
        system(command);
        snprintf(command, sizeof(command), "curl -o %s/Documents/MinecraftServer/start.sh 'https://raw.githubusercontent.com/Raktbastr/MCSSS-C/refs/heads/main/start.sh'", homeDir);
        system(command);
        snprintf(command, sizeof(command), "curl -o %s/Documents/MinecraftServer/start-nogui.bat 'https://raw.githubusercontent.com/Raktbastr/MCSSS-C/refs/heads/main/start-nogui.bat'", homeDir);
        system(command);
        snprintf(command, sizeof(command), "curl -o %s/Documents/MinecraftServer/start-nogui.sh 'https://raw.githubusercontent.com/Raktbastr/MCSSS-C/refs/heads/main/start-nogui.sh'", homeDir);
        system(command);
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
            snprintf(command, sizeof(command), "curl -o %s/Documents/MinecraftServer/server.properties 'https://raw.githubusercontent.com/Raktbastr/MCSSS-C/refs/heads/main/survivalserver.properties'", homeDir);
            system(command);
        } else if (gamemode == 2) {
            waitForSeconds(2);
            snprintf(command, sizeof(command), "curl -o %s/Documents/MinecraftServer/server.properties 'https://raw.githubusercontent.com/Raktbastr/MCSSS-C/refs/heads/main/creativeserver.properties'", homeDir);
            system(command);
        } else {
            printf("Please input a valid number.\n");
            goto switch1;
        }
    break;
    // Paper server setup
    case 2:
        printf("Downloading 1.21.4 Paper server jar.\n");
        waitForSeconds(2);
        snprintf(command, sizeof(command), "curl -o %s/Documents/MinecraftServer/server.jar 'https://api.papermc.io/v2/projects/paper/versions/1.21.4/builds/4/downloads/paper-1.21.4-4.jar'", homeDir);
        system(command);
        snprintf(command, sizeof(command), "curl -o %s/Documents/MinecraftServer/start.bat 'https://raw.githubusercontent.com/Raktbastr/MCSSS-C/refs/heads/main/start.bat'", homeDir);
        system(command);
        snprintf(command, sizeof(command), "curl -o %s/Documents/MinecraftServer/start.sh 'https://raw.githubusercontent.com/Raktbastr/MCSSS-C/refs/heads/main/start.sh'", homeDir);
        system(command);
        snprintf(command, sizeof(command), "curl -o %s/Documents/MinecraftServer/start-nogui.bat 'https://raw.githubusercontent.com/Raktbastr/MCSSS-C/refs/heads/main/start-nogui.bat'", homeDir);
        system(command);
        snprintf(command, sizeof(command), "curl -o %s/Documents/MinecraftServer/start-nogui.sh 'https://raw.githubusercontent.com/Raktbastr/MCSSS-C/refs/heads/main/start-nogui.sh'", homeDir);
        system(command);
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
            snprintf(command, sizeof(command), "curl -o %s/Documents/MinecraftServer/server.properties 'https://raw.githubusercontent.com/Raktbastr/MCSSS-C/refs/heads/main/papersurvivalserver.properties'", homeDir);
            system(command);
        } else if (gamemode == 2) {
            waitForSeconds(2);
            snprintf(command, sizeof(command), "curl -o %s/Documents/MinecraftServer/server.properties 'https://raw.githubusercontent.com/Raktbastr/MCSSS-C/refs/heads/main/papercreativeserver.properties'", homeDir);
            system(command);
        } else {
            printf("Please input a valid number.\n");
            goto switch2;
        }
        // Downloads plugins if wanted
        int pluginAmt;
        waitForSeconds(2);
        pluginGoto:
        printf("How many plugins would you like to add? ");
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
            downloadFile(url);
        }
        break;
    default:
        printf("Please input a vaild number.\n");
        goto gotoServerType;
    }

    // Finish server setup!
    waitForSeconds(2);
    printf("Making and accepting EULA file\n");
    snprintf(command, sizeof(command), "echo 'eula=true' > %s/Documents/MinecraftServer/eula.txt", homeDir);
    system(command);
    // Get the username of the user running the script
    const char *username2 = getenv("SUDO_USER");
    if (username2 == NULL) {
        username2 = getenv("USER");
    }
    if (username2 != NULL) {
        // Get the user ID and group ID of the user
        struct passwd *pw = getpwnam(username2);
        if (pw != NULL) {
            uid_t uid = pw->pw_uid;
            gid_t gid = pw->pw_gid;
            // Change ownership recursively
            snprintf(command, sizeof(command), "%s/Documents/MinecraftServer", homeDir);
            if (chown(command, uid, gid) == -1) {
                perror("Failed to change ownership of the server directory. Please run 'sudo chown -R <username> ~/Documents/MinecraftServer'");
            }
        } else {
            perror("Failed to change ownership of the server directory. Please run 'sudo chown -R <username> ~/Documents/MinecraftServer");
        }
    } else {
        perror("Failed to change ownership of the server directory. Please run 'sudo chown -R <username> ~/Documents/MinecraftServer");
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