#pragma once
#include "dbtypes.h"
#include <QObject>
#include <memory>


namespace db {


class ConnectionManager {
public:
    static ConnectionManager& instance();

    ConnectionManager(const ConnectionManager&) = delete;
    ConnectionManager& operator=(const ConnectionManager&) = delete;
    ~ConnectionManager();

    bool isValid() const;
private:
    class CManagerPrivate;
    std::unique_ptr<CManagerPrivate> m_private;
    ConnectionManager();
};

}
