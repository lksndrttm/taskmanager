#include "listcontroller.h"

IModelPtr ListController::model = IModel::create("qtsqlite");



list<std::string> ListController::getAllListsNames()
{
    return  model->getAllTargetListsNames();
}

ITargetListPtr ListController::getTargetListByName(std::string name)
{
    auto listSettings =  model->getTargetListSetting(name);
    if(listSettings.empty())
    {
        std::cout << "listNotFound\n";
        return nullptr;
    }
    return ITargetList::create(listSettings);
}

ITargetListPtr ListController::createList(map<std::string, std::string> listSetting)
{
    auto names = getAllListsNames();
    auto it = std::find_if(names.begin(),names.end(),[&listSetting](string name){
            return listSetting.at("name") == name;});
    if(it != names.end()){
        std::cout << "List not create. Name must be unique. [" << listSetting.at("name") << "]\n";
        return nullptr;
    }
    model->createList(listSetting);
    return ITargetList::create(listSetting);
}

void ListController::deleteList(std::string name)
{
    model->deleteTargetList(name);
}

void ListController::clearList(std::string name)
{
    model->clearList(name);
}
