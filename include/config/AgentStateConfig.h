#ifndef INCLUDE_CONFIG_AGENTSTATECONFIG_H_
#define INCLUDE_CONFIG_AGENTSTATECONFIG_H_

#include <string>

/**
* This class holds the common components for a visualisation
*/
struct AgentStateConfig {
    friend class AgentVis;
    friend class AgentStateVis;
    AgentStateConfig();
    ~AgentStateConfig();
    AgentStateConfig(const AgentStateConfig &other);
    AgentStateConfig &operator=(const AgentStateConfig &other);

    const char *model_path = nullptr;
    float model_scale[3];

 private:
    static void setString(const char ** target, const std::string &src) {
        if (*target)
            free(const_cast<char*>(*target));
        char *t = static_cast<char*>(malloc(src.size() + 1));
        snprintf(t, src.size() + 1, "%s", src.c_str());
        *target = t;
    }
};

/**
 * This struct mirrors AgentStateConfig, however each variable has a single bit
 * This is used by states to flag whether they have had their value set
 */
struct AgentStateConfigFlags {
    AgentStateConfigFlags() {
        memset(this, 0, sizeof(AgentStateConfigFlags));
    }
// This code is not used on the device
// However if nvcc sees it, it reports an error as the device does not support bitfields inside lists??
// >C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.25.28610\include\list(288): error :
// "Bitfields and field types containing bitfields are not supported in packed structures and unions for device compilation!"
// So we wrap in ifndef to stop nvcc seeing it, potentially harmful that nvcc will see it's size incorrectly but it seems to work
// Other solution would be to point to this structure and ensure the bitfield is never included by any .cu files
#ifndef __CUDACC__
    unsigned int model_path : 1;
    unsigned int model_scale : 1;
#endif  // __CUDACC__
};

#endif  // INCLUDE_CONFIG_AGENTSTATECONFIG_H_
