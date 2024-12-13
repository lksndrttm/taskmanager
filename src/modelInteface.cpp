#include "modelInteface.h"
#include "qtsqlitemodel.h"
#include <QDebug>

IModelPtr IModel::create(const string name)
{
	IModelPtr modelP;
    if (name == "sqlite") {
        //modelP = std::make_shared<SQLiteModel>();
        throw std::invalid_argument("default sqlite model has been depricate. use qtsqlite");
	}
    else if(name == "qtsqlite"){
        modelP = std::make_shared<QtSQLiteModel>();
    }
	else {
		throw std::invalid_argument("no has model with name " + name);
	}
	return modelP;
}

