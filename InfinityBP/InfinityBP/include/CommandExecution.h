#ifndef COMMANDS_EXECUTION_H_
#define COMMANDS_EXECUTION_H_

#define COMM_SUCCESS 0xAC // note that success can ALSO double as meaning acknowledged, for those tasks that don't wait on completion to reply
#define COMM_TASK_FAILED 0xAD

void init_communication();
void execute_command();

#endif // COMMANDS_EXECUTION_H_