#ifndef DBTYPES_H
#define DBTYPES_H


namespace DBTypes {


enum class DBResult{
    OK,
    FAIL
};

enum class DBState{
    OK,
    NO_DRIVER_ERROR,
    WORKSPACE_ERROR,
    TABLES_ERROR,
    OPENNING_ERROR
};

}
#endif // DBTYPES_H
