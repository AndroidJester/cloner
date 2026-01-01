#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_SIZE 1024

#define WHITE "\033[37m"
#define BLUE "\033[34m"
#define ORANGE "\033[32m"
#define RED "\033[31m"
#define RESET "\033[0m"

#define INFORMATION(x) printf(RESET x "\n")
#define INFO(x, ...) printf(BLUE  "[+] " x RESET "\n", __VA_ARGS__)
#define WARN(x, ...) printf(ORANGE "[-] " x RESET "\n", __VA_ARGS__)
#define ERROR(x) fprintf(stderr, RED "[ERROR] " x RESET)

typedef enum { WORK, CYBER } WorkType;

typedef enum {
  HTTP,
  SSH,
  SSH_GIT,
} ConnectionType;

void print_help() {
  INFORMATION("Welcome to Cloner");
  INFORMATION("ARGS: cloner [OPTIONS] URL [DESTINATION]");

  INFORMATION("\n[OPTIONS]");
  INFORMATION("--work  | -w            Work SSH");
  INFORMATION("--cyber | -c           Cyber SSH(default)");
  INFORMATION("--help  | -h             Display this help message");

  INFORMATION("\n[DESTINATION]");
  INFORMATION("Default Directory: ~/.cloner");
}

void split_url(char *url, WorkType workType, char *destination) {
  ConnectionType type;

  char *protocol;
  char *user;
  char *site;
  char *repo_user;
  char *repository;
  size_t url_length = strlen(url);
  char *new_url = malloc(sizeof(char) * url_length);

  // Check ConnectionType
  if (strstr(url, "http")) {
    type = HTTP;

    // Split
    protocol = strtok(url, "//");
    site = strtok(NULL, "/");
    repo_user = strtok(NULL, "/");
    repository = strtok(NULL, "/");

  } else if (strstr(url, "git@")) {
    type = SSH_GIT;

    user = strtok(url, "@");
    site = strtok(NULL, ":");
    repo_user = strtok(NULL, "/");
    repository = strtok(NULL, "\0");

  } else if (strstr(url, "ssh://")) {
    ERROR("NOT YET IMPLEMENTED");
    exit(1);
    /* type = SSH; */
    /* protocol = strtok(url, ":"); */
    /* /\* strtok(NULL, "/"); *\/ */
    /* user = strtok(NULL, "@"); */
    /* site = strtok(NULL, ":"); */
    /* repo_user = strtok(NULL, "/"); */
    /* repository = strtok(NULL, "\0"); */

  } else {
    ERROR("FAILED TO IDENTIFY STRING");
    exit(1);
  }

  strncat(new_url, "git", 3);
  strncat(new_url, "@", 1);
  // Determine Stuff
  if (workType == CYBER) {
    site = "cybrgit";
  } else {
    site = "softgit";
  }

  // Build URL
  strncat(new_url, site, strlen(site));
  strncat(new_url, ":", 1);
  strncat(new_url, repo_user, strlen(repo_user));
  strncat(new_url, "/", 1);
  strncat(new_url, repository, strlen(repository));
  strncat(new_url, ".git", strlen(repository));
  INFO("NEW_URL: %s", new_url);

  size_t dest_string_len = strnlen(destination, MAX_SIZE);
  size_t repo_string_len = strnlen(repository, MAX_SIZE);

  // INFO("DEST_STRING_LEN: %zu", dest_string_len);
  // INFO("REPO_STRING_LEN: %zu", repo_string_len);
  // INFO("STRING_LEN: %zu", repo_string_len + dest_string_len);

  char *dest_loc = malloc(sizeof(char) * (dest_string_len + repo_string_len));
  // dest_loc = strcat(destination, repository); //, ));
  sprintf(dest_loc, "%s/%s", destination, repository);
  INFO("DEST_LOC: %s\n", dest_loc);

  // printf("RESET\n");
  // printf("RESET\n");



  execl("/bin/git", "git", "clone", "--progress", new_url, dest_loc, NULL);
}

void parse_arguments(int argc, char **argv) {
  WorkType type;
  char *URL;
  char *home_env = getenv("HOME");
  char *destination = malloc(MAX_SIZE);
  sprintf(destination, "%s/cloner", home_env);

  // INFO("ARG_COUNT: %d", argc);
  // INFO("DESTINATION: %s", destination);
  /* INFO("ARG1: %s", argv[0]); */
  /* INFO("ARG2: %s", argv[1]); */
  /* INFO("ARG3: %s", argv[2]); */

  if (argc == 1) {
    print_help();
  }

  if (argc == 2) {
  if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
    print_help();
    exit(0);
  }
    type = WORK;
    URL = argv[1];
    split_url(URL, type, destination);
  }

  if (argc >= 3) {

  if (argc == 3) {
    URL = argv[1];
    destination = argv[2];
  } else {
    URL = argv[2];
    if (argv[3] != NULL)
      destination = argv[3];
  }


    if (strncmp(argv[1], "--work", strlen(argv[1])) == 0 ||
        strncmp(argv[1], "-w", strlen(argv[1])) == 0) {
      type = WORK;
      INFO("TYPE: WORK\n", NULL);
    } else if (strncmp(argv[1], "--cyber", strlen(argv[1])) == 0 ||
               strncmp(argv[1], "-c", strlen(argv[1])) == 0) {
      type = CYBER;
      INFO("TYPE: CYBER\n", NULL);
    }

    split_url(URL, type, destination);
  }
}
int main(int argc, char **argv) {

  parse_arguments(argc, argv);

  return 0;
}
