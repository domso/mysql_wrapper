#include "src/connection.h"

namespace database {
    namespace mysql {

        connection::connection() : m_connection(nullptr) {

        }

        connection::~connection() {
            if (m_connection != nullptr) {
                mysql_close(m_connection);
            }
        }

        bool connection::open(const user_data_t& user, const server_adress_t& server) {
            // there might be an open connection
            if (m_connection != nullptr) {
                return false;
            }

            mysql_init(&m_databaseLink);

            // mysql_real_connect exspects nullptr for an disabled parameter
            const char* socket = nullptr;

            if (server.socketName != "") {
                socket = server.socketName.c_str();
            }

            m_connection = mysql_real_connect(&m_databaseLink, server.host.c_str(), user.username.c_str(), user.password.c_str(), server.dbName.c_str(), server.port, socket , server.flags);

            return m_connection != nullptr;

        }

        void connection::close() {
            mysql_close(m_connection);
            m_connection = nullptr;
        }

        MYSQL* connection::getNativeHandler() {
            return m_connection;
        }

        std::string connection::getErrorMsg()  {
            std::string result(mysql_error(&m_databaseLink));
            return result;
        }

    }
}


