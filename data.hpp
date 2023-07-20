#ifndef YAML_DATA_H
#define YAML_DATA_H

namespace YamlData
{

struct yaml_os_data
{
    uint8        type;
    bool         has_second_stage;

    /* General OS information. */
    puint8       OS_name;
    puint8       OS_version;
    uint8        FS_type;

    /* Disk image name. */
    puint8       disk_name;

    /* Does the user want FAMP to auto format the disk image? */
    bool         auto_format;

    /* Binary folder. */
    puint8       bin_folder;

    /* Kernel info. */
    puint8       kernel_filename;
    puint8       kernel_bin_filename;
    puint8       kernel_bin_o_filename;
};

enum class data_types
{
    Chr,
    Hex,
    Dec,
    Str
};

struct data
{
    puint8        user_defined;
    data_types    data_type;
    puint16       vdata;

    struct data   *next;
    struct data   *previous;

    data()
    {
        user_defined = nullptr;
        vdata = nullptr;
        next = nullptr;
        previous = nullptr;
    }

    template<typename T>
        requires std::is_same<T, struct data>::value
    void delete_instance(T *instance)
    {
        if(instance)
            delete instance;
        instance = nullptr;
    }

    ~data()
    {
        if(user_defined) delete user_defined;
        user_defined = nullptr;

        if(vdata) delete vdata;
        vdata = nullptr;
    }
};

static struct data *yaml_file_data = nullptr;
static struct data *all_yaml_data = nullptr;
static usize yaml_file_data_size = 0;

#define _next yaml_file_data = yaml_file_data->next;
#define _back yaml_file_data = yaml_file_data->previous;

class yaml_data
{
public:
    yaml_data()
    {
        yaml_file_data = new struct data;
        all_yaml_data = yaml_file_data;
    }

    void add_yaml_data(puint8 user_def_name, puint16 data, data_types type)
    {
        struct data previous = *yaml_file_data;
        yaml_file_data->next = new struct data;

        memcpy(yaml_file_data->user_defined, user_def_name, strlen((cpint8) user_def_name));

        //yaml_file_data->data_type = type;
        
    }

    template<typename T>
        requires std::is_same<T, struct data>::value
            || std::is_same<T, yaml_data>::value
    void delete_instance(T *instance)
    {
        if(instance)
            delete instance;
        instance = nullptr;
    }

    ~yaml_data()
    {
        yaml_file_data = all_yaml_data;

        while(yaml_file_data->next)
        {
            yaml_file_data->delete_instance<struct data> (yaml_file_data);
            yaml_file_data = yaml_file_data->next;
            delete yaml_file_data->previous;
        }

        if(yaml_file_data)
            yaml_file_data->delete_instance<struct data> (yaml_file_data);
        yaml_file_data = nullptr;
        all_yaml_data = nullptr;
    }
};
    
}

#endif
