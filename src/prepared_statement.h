#ifndef general_database_mysql_prepared_statements_h
#define general_database_mysql_prepared_statements_h

#include <vector>
#include <mysql.h>
#include <errmsg.h>
#include "src/connection.h"

namespace database {
    namespace mysql {
        //______________________________________________________________________________________________________
        //
        // abstract basic query
        //______________________________________________________________________________________________________
        class prepared_statement {
        public:
            prepared_statement();
            ~prepared_statement();
            //______________________________________________________________________________________________________
            // Description:
            // - connects the query to an open connection
            // - calls init() afterwards
            // Parameter:
            // - conn: a valid connection
            // Return:
            // - true  | on success
            // - false | on any error
            //______________________________________________________________________________________________________
            bool prepare(connection& conn);
            //______________________________________________________________________________________________________
            //
            // Description:
            // - executes the query
            // - the query and all necessary parameters/results have to be set first
            // Return:
            // - true  | on success
            // - false | on any error
            //______________________________________________________________________________________________________
            bool execute();
            //______________________________________________________________________________________________________
            //
            // Description:
            // - stores the next result-row into the result-columns
            // Return:
            // - true  | on success
            // - false | on any error
            //______________________________________________________________________________________________________
            bool fetchRow();
            //______________________________________________________________________________________________________
            //
            // Description:
            // - returns the length of valid bytes in the result-column specified by the paramID
            // Parameter:
            // - paramID: the ascending column number (starting with 0)
            // Return:
            // - number of valid bytes in the result-column
            //______________________________________________________________________________________________________
            long unsigned int getParamResultLength(const size_t paramID);
            //______________________________________________________________________________________________________
            //
            // Description:
            // - returns the current error message from the internal MySQL-Connector (--> mysql_stmt_error())
            // Return:
            // - new String containing the error message
            //______________________________________________________________________________________________________
            std::string getErrorMsg();
        protected:
            //______________________________________________________________________________________________________
            //
            // Description:
            // - initializes the SQL-statements
            // Return:
            // - true  | on success
            // - false | on any error
            //______________________________________________________________________________________________________
            virtual bool init() = 0;
            //
            // holds the information for a single columns inside a single row from a load query
            //______________________________________________________________________________________________________
            struct result_param_t {
                long unsigned int length;
                my_bool isNULL = false;
                my_bool useError = false;
            };
            //______________________________________________________________________________________________________
            //
            // Description:
            // - sets n paramT-instances as a single parameter for queries
            // Parameters:
            // - paramID: the ascending parameter (starting with 0)
            // - mysqlType: the SQL-Type of the column
            // - input: pointer to a valid instance of paramT
            // - n: number of paramT-instances at input
            // - isNULL: see MYSQL_BIND.is_null
            // - isUnsigned: see MYSQL_BIND.is_unsigned
            // - useError: see MYSQL_BIND.error
            // Return:
            // - true  | on success
            // - false | on any error
            //______________________________________________________________________________________________________
            template <typename paramT>
            bool setParam(const unsigned int paramID, enum enum_field_types mysqlType, const paramT* input, const int n = 1,  my_bool isNULL = false, my_bool isUnsigned = false, my_bool useError = false) {
                m_isBinded = false;

                if (paramID < m_params.size()) {
                    m_paramInfo[paramID].isNULL = isNULL;
                    m_paramInfo[paramID].useError = useError;
                    m_paramInfo[paramID].length = n * sizeof(paramT);

                    m_params[paramID].buffer_type = mysqlType;
                    m_params[paramID].buffer = (char*) input;
                    m_params[paramID].buffer_length = n * sizeof(paramT);
                    m_params[paramID].length = &(m_paramInfo[paramID].length);
                    m_params[paramID].is_null = &(m_paramInfo[paramID].isNULL);
                    m_params[paramID].is_unsigned = isUnsigned;
                    m_params[paramID].error = &(m_paramInfo[paramID].useError);

                    return true;
                }

                return false;
            }
            //______________________________________________________________________________________________________
            //
            // Description:
            // - sets n paramT-instances as a single result-column buffer for queries
            // Parameters:
            // - paramID: the ascending column number (starting with 0)
            // - mysqlType: the SQL-Type of the column
            // - input: pointer to a valid instance of paramT
            // - n: number of paramT-instances at input
            // - isNULL: see MYSQL_BIND.is_null
            // - isUnsigned: see MYSQL_BIND.is_unsigned
            // - useError: see MYSQL_BIND.error
            // Return:
            // - true  | on success
            // - false | on any error
            //______________________________________________________________________________________________________
            template <typename paramT>
            bool setResult(const unsigned int paramID, enum enum_field_types mysqlType, const paramT* input, const int n = 1,  my_bool isNULL = false, my_bool isUnsigned = false, my_bool useError = false) {
                m_isBinded = false;

                if (paramID < m_results.size()) {
                    m_resultInfo[paramID].isNULL = isNULL;
                    m_resultInfo[paramID].useError = useError;

                    m_results[paramID].buffer_type = mysqlType;
                    m_results[paramID].buffer = (char*) input;
                    m_results[paramID].buffer_length = n * sizeof(paramT);
                    m_results[paramID].length = &(m_resultInfo[paramID].length);
                    m_results[paramID].is_null = &(m_resultInfo[paramID].isNULL);
                    m_results[paramID].is_unsigned = isUnsigned;
                    m_results[paramID].error = &(m_resultInfo[paramID].useError);

                    return true;
                }

                return false;
            }
            //______________________________________________________________________________________________________
            //
            // Description:
            // - sets a new query
            // Parameter:
            // - query: a valid MySQL-query-string
            // Return:
            // - true  | on success
            // - false | on any error
            //______________________________________________________________________________________________________
            bool setQuery(const std::string query);
            //______________________________________________________________________________________________________
            //
            // Description:
            // - binds the parameters to the prepared statement
            // Return:
            // - true  | on success
            // - false | on any error
            //______________________________________________________________________________________________________
            bool bindParam();
            //______________________________________________________________________________________________________
            //
            // Description:
            // - binds the result-columns to the prepared statement
            // Return:
            // - true  | on success
            // - false | on any error
            //______________________________________________________________________________________________________
            bool bindResult();

            // is the current query binded?
            bool m_isBinded;
            // internal representation of the prepared statement
            MYSQL_STMT* m_statement;
            // container holding all parameters/result-columns
            std::vector<MYSQL_BIND> m_params;
            std::vector<MYSQL_BIND> m_results;
            // container holding all parameter-informations
            std::vector<result_param_t> m_paramInfo;
            // container holding all result-informations
            std::vector<result_param_t> m_resultInfo;
        };
    }
}

#endif



