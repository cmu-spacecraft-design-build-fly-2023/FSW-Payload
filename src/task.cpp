#include "spdlog/spdlog.h"

#include <stdexcept>
#include "task.hpp"


Task::Task(int task_id, CommandFunction func, std::vector<uint8_t>& data, Payload* payload, int priority, std::string name)
: 
task_id(task_id),
priority(priority),
data(data),
payload(payload),
func(func),
name(name)
{
    created_at = std::chrono::system_clock::now();

    // TODO check task_id mapping again
}

void Task::Execute()
{
    SPDLOG_INFO("Executing task [{}][{}]", task_id, name);

    for (int attempts = 0; attempts < MAX_ATTEMPTS; attempts++)
    {
        try
        {
            func(payload, data);
            break;
        }
        catch (const std::exception& e)
        {
            
            SPDLOG_ERROR("Task execution failed (attempts {}): {}", attempts, e.what());
        }
    }

}


int Task::GetPriority() const {
    return priority;
}

int Task::GetID() const {
    return task_id;
}

size_t Task::GetDataSize() const {
    return data.size();
}


std::chrono::system_clock::time_point Task::GetCreationTime() const {
    return created_at;
}