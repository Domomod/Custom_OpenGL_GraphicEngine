//
// Created by dominik on 21.03.19.
//

#ifndef GAMEENGINE_STORAGEMANAGERTEMPLATE_H
#define GAMEENGINE_STORAGEMANAGERTEMPLATE_H

#include <map>
#include <memory>

template <class Type>
class StorageManagerTemplate {
    public:
        void addObject(const std::string& id,const std::shared_ptr<Type>& object){
            nameToObjectMap.emplace(id, object);
        }

        /*I expect this method to be only called at initializing objects which will save the pointer to Mesh for themselves,
         * that's why I decided to use a string as a unique Id of a Mesh.
         * */
        std::shared_ptr<Type> getObject(const std::string& id) const {
            return  nameToObjectMap.at(id);
        }
    private:
        std::map<std::string, std::shared_ptr<Type>> nameToObjectMap;
};


#endif //GAMEENGINE_STORAGEMANAGERTEMPLATE_H
