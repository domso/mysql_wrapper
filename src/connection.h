#ifndef general_database_mysql_connection_h
#define general_database_mysql_connection_h

#include <mysql.h>
#include "stdio.h"
#include <string>

namespace database {
    namespace mysql {
        //______________________________________________________________________________________________________
        //
        // MySQL-Connector
        //______________________________________________________________________________________________________
        class connection {
        public:
            //______________________________________________________________________________________________________
            //
            // user-information for the login
            //______________________________________________________________________________________________________
            struct user_data_t {
                std::string username;
                std::string password;
            };
            //______________________________________________________________________________________________________
            //
            // host-information
            //______________________________________________________________________________________________________
            struct server_adress_t {
                std::string dbName;
                std::string host;
                std::string socketName;
                long unsigned int flags = 0;
                unsigned int port = 0;
            };

            connection();
            ~connection();
            //______________________________________________________________________________________________________
            //
            // Description:
            // - opens a new connection to the database specified by the host-information in the server parameter
            // - for authentication the user parameter is used
            // - if the datastructure already holds an open connection, no action will be performed
            // Parameters:
            // - user: the user-information
            // - server: the host-information
            // Return:
            // - true  | on success
            // - false | on any error
            //______________________________________________________________________________________________________
            bool open(const user_data_t& user, const server_adress_t& server);
            //______________________________________________________________________________________________________
            //
            // Description:
            // - closes any open connection
            //______________________________________________________________________________________________________
            void close();
            //______________________________________________________________________________________________________
            //
            // Description:
            // - returns the current internal connection representation
            // Return:
            // - pointer to a valid instance of MYSQL | on an open connection
            // - nullptr                              | else
            //______________________________________________________________________________________________________
            MYSQL* getNativeHandler();
            //______________________________________________________________________________________________________
            //
            // Description:
            // - returns the current error message from the internal MySQL-Connector (--> mysql_error())
            // Return:
            // - new String containing the error message
            //______________________________________________________________________________________________________
            std::string getErrorMsg();
        private:
            // the actual open connection
            MYSQL* m_connection;
            // the link-object for mysql_real_connect()
            MYSQL m_databaseLink;
        };
    }
}

#endif


