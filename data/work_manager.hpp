#ifndef WORK_MANAGER_HPP
#define WORK_MANAGER_HPP

#include <string>
#include <list>

class tag_collection;
class tag_deactivator;

class work_manager {
    private:
        std::list<std::pair<std::string, bool>> msg;
        tag_collection &t_col;

        void handle_work_delegation(tag_deactivator &t_deact);
    public:
        work_manager(tag_collection &t_co);
        void store_msg(std::pair<std::string, bool> mesg);
        void handle_msg(tag_deactivator &t_deact);
};

#endif