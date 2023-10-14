#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "hashtable.h"

#define SIMPLE_DB_VERSION "1.0"
#define BUFFER_SIZE 256

typedef enum {
    STATEMENT_GET,
    STATEMENT_SET,
    STATEMENT_DELETE
} StatementType;

typedef enum { 
    PREPARE_SUCCESS, 
    PREPARE_UNRECOGNIZED_STATEMENT,
    PREPARE_SYNTAX_ERROR
} PrepareResult;
typedef enum {
  EXECUTE_SUCCESS,
  EXECUTE_KEY_NOT_FOUND
} ExecuteResult;
typedef struct {
    char command[BUFFER_SIZE];
} Command;

typedef struct {
    StatementType type;
    int32_t key;
    int32_t value;
} Statement;

static struct {
  HTable* db; 
} db_data;

void print_prompt() {
    printf("SimpleDB > ");
}

static uint64_t str_hash(const uint8_t *data, size_t len) {
    uint32_t h = 0x811C9DC5;
    for (size_t i = 0; i < len; i++) {
        h = (h + data[i]) * 0x01000193;
    }
    return h;
}

static bool entry_eq(HNode *lhs, HNode *rhs) {
  return lhs->hcode == rhs->hcode;
}

PrepareResult prepare_set(Command* cmd,Statement* statement){
  char* keyword = strtok(cmd->command, " ");
  char* key = strtok(NULL, " ");
  char* value = strtok(NULL, " ");

  if(keyword == NULL || key == NULL || value == NULL){
    return PREPARE_SYNTAX_ERROR;
  }  

  int32_t int_key = atoi(key);
  int32_t int_value = atoi(value);

  statement->key = int_key;
  statement->value = int_value;
  return PREPARE_SUCCESS;  
}

PrepareResult prepare_get(Command* cmd,Statement* statement){
  char* keyword = strtok(cmd->command, " ");
  char* key = strtok(NULL, " ");

  if(keyword == NULL || key == NULL){
    return PREPARE_SYNTAX_ERROR;
  }  

  int32_t int_key = atoi(key);

  statement->key = int_key;
  return PREPARE_SUCCESS;  
}

PrepareResult prepare_delete(Command* cmd,Statement* statement){
  char* keyword = strtok(cmd->command, " ");
  char* key = strtok(NULL, " ");

  if(keyword == NULL || key == NULL){
    return PREPARE_SYNTAX_ERROR;
  }  

  int32_t int_key = atoi(key);

  statement->key = int_key;
  return PREPARE_SUCCESS;  
}


PrepareResult prepare_statement(Command* cmd,Statement* statement){
    if (strncmp(cmd->command, "GET", 3) == 0) {
        statement->type = STATEMENT_GET;
        return prepare_get(cmd,statement);;
    }
    if (strncmp(cmd->command, "SET",3) == 0) {
        statement->type = STATEMENT_SET;
        return prepare_set(cmd,statement);
    }
    if (strncmp(cmd->command,"DELETE",6) == 0){
        statement->type = STATEMENT_DELETE;
        return prepare_delete(cmd,statement);
    }
    return PREPARE_UNRECOGNIZED_STATEMENT;
}

ExecuteResult execute_set(Statement* statement){
  HNode* node = (HNode *)malloc(sizeof(HNode));
  node->hcode = statement->key;
  node->value = statement->value;
  ht_insert(&db_data.db,node);
  return EXECUTE_SUCCESS;
}
ExecuteResult execute_get(Statement* Statement){
  HNode* node = (HNode *)malloc(sizeof(HNode));
  node->hcode = Statement->key;
  HNode** output_node = ht_lookup(&db_data.db,node,entry_eq); 
  if(output_node){
    printf("value: %d\n",(*output_node)->value);
    return EXECUTE_SUCCESS;
  }
  return EXECUTE_KEY_NOT_FOUND;
}
ExecuteResult execute_delete(Statement* statement){
  HNode* node = (HNode *)malloc(sizeof(HNode));
  node->hcode = statement->key; 
  HNode* output_node = ht_pop(&db_data.db,node,entry_eq);
  if(output_node){
    printf("%d Key Deleted Successfuly.\n",output_node->value);
    return EXECUTE_SUCCESS;
  }
  return EXECUTE_KEY_NOT_FOUND;
}

ExecuteResult execute_statement(Statement* statement){

    if (statement->type == STATEMENT_SET) {
            // printf("SET: %d : %d\n",statement->key,statement->value);
            return execute_set(statement);
    } else if (statement->type == STATEMENT_GET) {
            // printf("GET: %d\n",statement->key);
            return execute_get(statement);
    } else if (statement->type == STATEMENT_DELETE) {
            // printf("DELETE: %d\n", statement->key);
            return execute_delete(statement);
    } 
    return EXECUTE_SUCCESS;
}

void read_command(Command* cmd) {
    fgets(cmd->command, BUFFER_SIZE, stdin);
    cmd->command[strcspn(cmd->command, "\n")] = '\0';  // Remove trailing newline

    if (strcmp(cmd->command, ".exit") == 0) {
        exit(EXIT_SUCCESS);
    }
}

int main(int argc, char* argv[]) {
    printf("SimpleDB version %s starting...\n",SIMPLE_DB_VERSION);
    // initializing h_table with default size at 8
    h_init(&db_data.db,32);
    while (true) {
        print_prompt();
        Command* cmd = malloc(sizeof(Command));
        read_command(cmd);

        // handling input commands and prepare for execution
        Statement statement;
        switch (prepare_statement(cmd, &statement)) {
          case (PREPARE_SUCCESS):
            break;
          case (PREPARE_UNRECOGNIZED_STATEMENT):
            printf("Unknown command: %s\n", cmd->command);
            continue;
          case (PREPARE_SYNTAX_ERROR):
            printf("Please check manual Sytax error occured\n");
        }

        execute_statement(&statement);
    }

    return 0;
}
